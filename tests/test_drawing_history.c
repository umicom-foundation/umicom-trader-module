/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: tests/test_drawing_history.c
 * PURPOSE: Verify actual Trader facade and native drawing history with saved chart evidence.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/ui/gtk4/automation.h"
#include "umicom/trader/gtk_workstation.h"
#include "umicom/chart/drawing_tools.h"
#include "umicom/trading_ui/gtk4/interactive_chart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"%s:%d: %s\n",__FILE__,__LINE__,#x); exit(1); } } while (0)
#define OK(x) CHECK((x)==UMI_STATUS_OK)
/* The rendered-child walk omitted controls owned by collapsed expanders. The shared bounded logical-tree lookup replaces it; retain the earlier traversal for review. */
#if 0
/* Rendered-child traversal missed controls owned by collapsed inspectors. The Framework logical lookup preserves those controls and rejects ambiguous identities. The original fixture traversal is retained for review. The previous implementation is retained for engineering review. */
#if 0
static GtkWidget *Find(GtkWidget *root,const char *id)
{
    const char *tag=g_object_get_data(G_OBJECT(root),"umicom-automation-id");
    if (tag!=NULL && strcmp(tag,id)==0) return root;
    for (GtkWidget *child=gtk_widget_get_first_child(root);child!=NULL;child=gtk_widget_get_next_sibling(child)) {
        GtkWidget *found=Find(child,id); if (found!=NULL) return found;
    }
    return NULL;
}
#endif
static GtkWidget *Find(GtkWidget *root,const char *id)
{
    /* Inspect logical children without expanding or activating a panel. */
    return umi_gtk4_automation_find_tagged_widget(root, id);
}
#endif
/* Use the Framework logical tree so a collapsed panel can be inspected
 * without changing the user's layout or overlooking an ambiguous identifier. */
static GtkWidget *Find(GtkWidget *root, const char *id)
{
    return umi_gtk4_automation_find_tagged_widget(root, id);
}
int main(int argc,char **argv)
{
    CHECK(argc==2); if (!gtk_init_check()) return 77;
    const char *name=argv[1]; UmiTraderGtkWorkstation *product=NULL; OK(umi_trader_gtk_workstation_create(&product));
    GtkWidget *root=umi_trader_gtk_workstation_widget(product), *chart=Find(root,"trading.chart.panel");
    UmiTradingWorkspaceSnapshot before,after; OK(umi_trader_gtk_workstation_trading_snapshot(product,&before));
    const char *pane=before.selected_instrument_id;
    UmiDataServer *server=NULL; OK(umi_data_server_create_memory(&server)); OK(UmiTraderGtkBindChartStorage(product,server,"history.product"));
    UmiChartDrawingSnapshot drawing; OK(UmiChartDrawingInitialize("product.range",pane,UMI_CHART_DRAWING_RANGE,(UmiChartPoint){1000,1.1},(UmiChartPoint){2000,1.2},&drawing));
    UmiChartNavigation navigation={0}; UmiChartDocument *document=NULL;
    OK(UmiChartDocumentCreate(pane,&navigation,&drawing,1,0,&document));
    UmiChartCheckpointReport report; UmiTradingChartPreview preview;
    OK(UmiChartCheckpointSave(server,"history.product",document,0,1000,&report)); UmiChartDocumentDestroy(document);
    OK(UmiTraderGtkPreviewChart(product,pane,&preview)); OK(UmiTraderGtkRestoreChart(product,pane,preview.preview_id));
    UmiGtk4TradingInteractiveChartRefresh(chart);
    GtkWidget *undo=Find(root,"trading.chart.undo-drawing"), *redo=Find(root,"trading.chart.redo-drawing");
    GtkWidget *load=Find(root,"trading.chart.appearance-load"), *apply=Find(root,"trading.chart.appearance-apply");
    CHECK(GTK_IS_BUTTON(undo) && GTK_IS_BUTTON(redo) && GTK_IS_BUTTON(load) && GTK_IS_BUTTON(apply));
    g_signal_emit_by_name(load,"clicked"); gtk_editable_set_text(GTK_EDITABLE(Find(root,"trading.chart.appearance-color")),"#123456");
    g_signal_emit_by_name(apply,"clicked");
    UmiChartDrawingHistorySnapshot history; OK(UmiTraderGtkDrawingHistory(product,&history)); CHECK(history.undo_count==1);
    if (strcmp(name,"facade")==0) {
        CHECK(UmiTraderGtkUndoDrawing(product,pane,history.revision-1)==UMI_STATUS_BUSY);
        OK(UmiTraderGtkUndoDrawing(product,pane,history.revision));
    } else if (strcmp(name,"native")==0 || strcmp(name,"restore")==0 || strcmp(name,"retained")==0) g_signal_emit_by_name(undo,"clicked");
    else return 2;
    OK(UmiTraderGtkSaveChart(product,pane,2000,&report));
    OK(UmiChartCheckpointLoad(server,"history.product",pane,&document,&report)); OK(UmiChartDocumentDrawingAt(document,0,&drawing)); UmiChartDocumentDestroy(document);
    CHECK(drawing.style[0]=='\0' && drawing.time1==1000 && drawing.value2==1.2);
    OK(UmiTraderGtkDrawingHistory(product,&history)); CHECK(history.redo_count==1);
    if (strcmp(name,"restore")==0) {
        OK(UmiTraderGtkPreviewChart(product,pane,&preview)); OK(UmiTraderGtkRestoreChart(product,pane,preview.preview_id));
        OK(UmiTraderGtkDrawingHistory(product,&history)); CHECK(!history.redo_count && !history.undo_count);
    } else {
        if (strcmp(name,"facade")==0) OK(UmiTraderGtkRedoDrawing(product,pane,history.revision));
        else { UmiGtk4TradingInteractiveChartRefresh(chart); g_signal_emit_by_name(redo,"clicked"); }
        OK(UmiTraderGtkSaveChart(product,pane,3000,&report));
        OK(UmiChartCheckpointLoad(server,"history.product",pane,&document,&report)); OK(UmiChartDocumentDrawingAt(document,0,&drawing)); UmiChartDocumentDestroy(document);
        CHECK(strncmp(drawing.style,"umi-drawing:1:123456:",21)==0);
    }
    OK(umi_trader_gtk_workstation_trading_snapshot(product,&after));
    CHECK(after.order_count==before.order_count && after.live_armed==before.live_armed && after.environment==before.environment);
    CHECK(memcmp(&after.draft_order,&before.draft_order,sizeof before.draft_order)==0);
    if (strcmp(name,"retained")==0) { g_object_ref(undo); g_object_ref(redo); }
    OK(UmiTraderGtkBindChartStorage(product,NULL,NULL)); umi_trader_gtk_workstation_destroy(product); umi_data_server_destroy(server);
    if (strcmp(name,"retained")==0) { g_signal_emit_by_name(undo,"clicked"); g_signal_emit_by_name(redo,"clicked"); g_object_unref(undo); g_object_unref(redo); }
    return 0;
}
