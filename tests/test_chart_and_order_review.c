/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: tests/test_chart_and_order_review.c
 * PURPOSE: Verify Trader composes the shared chart tools and canonical order query.
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
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #x); exit(1); } } while (0)
/* The rendered-child walk omitted controls owned by collapsed expanders. The shared bounded logical-tree lookup replaces it; retain the earlier traversal for review. */
#if 0
static GtkWidget *Find(GtkWidget *root, const char *id)
{
    const char *tag = g_object_get_data(G_OBJECT(root), "umicom-automation-id");
    if (tag != NULL && strcmp(tag, id) == 0) return root;
    for (GtkWidget *child = gtk_widget_get_first_child(root); child != NULL; child = gtk_widget_get_next_sibling(child)) {
        GtkWidget *match = Find(child, id); if (match != NULL) return match;
    }
    return NULL;
}
#endif
/* Use the Framework logical tree so a collapsed panel can be inspected
 * without changing the user's layout or overlooking an ambiguous identifier. */
static GtkWidget *Find(GtkWidget *root, const char *id)
{
    return umi_gtk4_automation_find_tagged_widget(root, id);
}
int main(void)
{
    if (!gtk_init_check()) return 77;
    UmiTraderGtkWorkstation *workstation = NULL;
    CHECK(umi_trader_gtk_workstation_create(&workstation) == UMI_STATUS_OK);
    GtkWidget *root = umi_trader_gtk_workstation_widget(workstation);
    const char *ids[] = {"trading.chart.canvas", "trading.chart.zoom-in", "trading.chart.zoom-out", "trading.chart.fit",
        "trading.chart.trend", "trading.chart.support", "trading.chart.resistance", "trading.chart.buy-limit", "trading.chart.sell-limit",
        "trading.chart.drawings", "trading.orders.search", "trading.orders.review"};
    for (size_t i = 0; i < sizeof ids / sizeof ids[0]; ++i) CHECK(Find(root, ids[i]) != NULL);
    UmiTradingWorkspaceSnapshot before, after;
    CHECK(umi_trader_gtk_workstation_trading_snapshot(workstation, &before) == UMI_STATUS_OK);
    g_signal_emit_by_name(Find(root, "trading.chart.zoom-in"), "clicked");
    g_signal_emit_by_name(Find(root, "trading.chart.fit"), "clicked");
    UmiTradingOrderQuery query = {UMI_TRADING_WORKSPACE_ORDERS_OPEN, "missing-order"};
    CHECK(umi_trader_gtk_workstation_set_order_query(workstation, &query) == UMI_STATUS_OK);
    /* Process the real controller notification. It must refresh order panels
     * without replacing the chart or cancelling a selected drawing tool. */
    GtkWidget *canvas = Find(root, "trading.chart.canvas");
    unsigned int iterations = 0;
    while (g_main_context_pending(NULL) && iterations++ < 256U) (void)g_main_context_iteration(NULL, FALSE);
    CHECK(iterations < 256U && Find(root, "trading.chart.canvas") == canvas);
    CHECK(strcmp(gtk_editable_get_text(GTK_EDITABLE(Find(root, "trading.orders.search"))), "missing-order") == 0);
    CHECK(umi_trader_gtk_workstation_trading_snapshot(workstation, &after) == UMI_STATUS_OK);
    CHECK(after.visible_order_count == 0 && after.order_count == before.order_count);
    CHECK(strcmp(after.selected_instrument_id, before.selected_instrument_id) == 0);
    CHECK(memcmp(&after.draft_order, &before.draft_order, sizeof after.draft_order) == 0);
    CHECK(after.environment == UMI_TRADING_SIMULATION && !after.live_armed);
    UmiTradingOrderReview *review = calloc(1, sizeof *review); CHECK(review != NULL);
    CHECK(umi_trader_gtk_workstation_review_order(workstation, "missing-order", review) == UMI_STATUS_NOT_FOUND);
    CHECK(Find(root, "trading.orders.copy-csv") != NULL);
    UmiCsvDocument *csv = NULL;
    CHECK(UmiTraderGtkExportOrdersCsv(workstation, &csv) == UMI_STATUS_OK);
    CHECK(UmiCsvDocumentRows(csv) == 2 && strstr(UmiCsvDocumentData(csv), "missing-order") != NULL);
    UmiTradingWorkspaceSnapshot exported;
    CHECK(umi_trader_gtk_workstation_trading_snapshot(workstation, &exported) == UMI_STATUS_OK);
    CHECK(exported.revision == after.revision && exported.order_count == after.order_count);
    UmiCsvDocumentDestroy(csv);
    /* Product acceptance traverses the same coordinator used by native chart
     * buttons, with a borrowed test store that cannot touch a real profile. */
    CHECK(Find(root, "trading.chart.save") != NULL && Find(root, "trading.chart.preview") != NULL);
    /* The review is deliberately collapsed until requested. Inspect its
     * public expander first; its details join GTK's child tree when opened. */
    GtkWidget *saved_review = Find(root, "trading.chart.saved-review");
    CHECK(GTK_IS_EXPANDER(saved_review) && !gtk_expander_get_expanded(GTK_EXPANDER(saved_review)));
    gtk_expander_set_expanded(GTK_EXPANDER(saved_review), TRUE);
    CHECK(Find(root, "trading.chart.restore") != NULL && Find(root, "trading.chart.saved-details") != NULL);
    GtkWidget *saved_details = Find(root, "trading.chart.saved-details");
    CHECK(GTK_IS_TEXT_VIEW(saved_details));
    gtk_expander_set_expanded(GTK_EXPANDER(saved_review), FALSE);
    UmiDataServer *chart_server = NULL;
    CHECK(umi_data_server_create_memory(&chart_server) == UMI_STATUS_OK);
    CHECK(UmiTraderGtkBindChartStorage(workstation, chart_server, "product.acceptance") == UMI_STATUS_OK);
    UmiChartCheckpointReport report; UmiTradingChartPreview preview;
    CHECK(UmiTraderGtkSaveChart(workstation, exported.selected_instrument_id, 1000U, &report) == UMI_STATUS_OK && !report.durable);
    /* Use the actual Preview button with this borrowed memory store. The
     * shared Framework action must reveal the existing review text in Trader. */
    g_signal_emit_by_name(Find(root, "trading.chart.preview"), "clicked");
    CHECK(gtk_expander_get_expanded(GTK_EXPANDER(saved_review)));
    CHECK(Find(root, "trading.chart.saved-details") == saved_details);
    GtkTextIter review_start, review_end;
    GtkTextBuffer *review_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(saved_details));
    gtk_text_buffer_get_bounds(review_buffer, &review_start, &review_end);
    char *review_text = gtk_text_buffer_get_text(review_buffer, &review_start, &review_end, TRUE);
    CHECK(review_text != NULL && strstr(review_text, exported.selected_instrument_id) != NULL &&
        strstr(review_text, "memory only") != NULL);
    g_free(review_text);
    g_signal_emit_by_name(Find(root, "trading.chart.zoom-in"), "clicked");
    CHECK(UmiTraderGtkPreviewChart(workstation, exported.selected_instrument_id, &preview) == UMI_STATUS_OK);
    CHECK(preview.saved.navigation.visible_bars == 0U && preview.current.navigation.visible_bars != 0U);
    CHECK(UmiTraderGtkRestoreChart(workstation, exported.selected_instrument_id, preview.preview_id) == UMI_STATUS_OK);
    CHECK(UmiTraderGtkPreviewChart(workstation, exported.selected_instrument_id, &preview) == UMI_STATUS_OK && preview.current.navigation.visible_bars == 0U);
    /* Import a genuine saved chart through the public product workflow, then
     * use real native object actions. No private product model is substituted. */
    const char *toolTags[]={"range","liquidity-zone","ray","move-drawing","lock-drawing","duplicate-drawing"};
    for(size_t i=0;i<6;++i){char tag[80];(void)snprintf(tag,sizeof(tag),"trading.chart.%s",toolTags[i]);CHECK(Find(root,tag)!=NULL);}
    UmiChartDrawingSnapshot toolDrawings[3];
    const UmiChartDrawingKind kinds[]={UMI_CHART_DRAWING_RANGE,UMI_CHART_DRAWING_LIQUIDITY_ZONE,UMI_CHART_DRAWING_RAY};
    for(size_t i=0;i<3;++i){char id[32];(void)snprintf(id,sizeof(id),"product.drawing.%zu",i);
        CHECK(UmiChartDrawingInitialize(id,exported.selected_instrument_id,kinds[i],(UmiChartPoint){1000,1.1},(UmiChartPoint){2000,1.2},&toolDrawings[i])==UMI_STATUS_OK);}
    UmiChartNavigation toolNavigation={0};UmiChartDocument *toolDocument=NULL;
    CHECK(UmiChartDocumentCreate(exported.selected_instrument_id,&toolNavigation,toolDrawings,3,0,&toolDocument)==UMI_STATUS_OK);
    CHECK(UmiChartCheckpointSave(chart_server,"product.acceptance",toolDocument,report.storage_revision,2000,&report)==UMI_STATUS_OK);
    UmiChartDocumentDestroy(toolDocument);toolDocument=NULL;
    CHECK(UmiTraderGtkPreviewChart(workstation,exported.selected_instrument_id,&preview)==UMI_STATUS_OK);
    CHECK(UmiTraderGtkRestoreChart(workstation,exported.selected_instrument_id,preview.preview_id)==UMI_STATUS_OK);
    UmiGtk4TradingInteractiveChartRefresh(Find(root,"trading.chart.panel"));
    GtkWidget *drawingList=Find(root,"trading.chart.drawings");CHECK(GTK_IS_DROP_DOWN(drawingList));
    CHECK(g_list_model_get_n_items(gtk_drop_down_get_model(GTK_DROP_DOWN(drawingList)))==3);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(drawingList),2);
    g_signal_emit_by_name(Find(root,"trading.chart.lock-drawing"),"clicked");
    g_signal_emit_by_name(Find(root,"trading.chart.duplicate-drawing"),"clicked");
    /* Visibility travels through the actual product control and saved chart.
     * The hidden copy remains present, with its original anchors and tool. */
    CHECK(Find(root,"trading.chart.hide-drawing")!=NULL);
    g_signal_emit_by_name(Find(root,"trading.chart.hide-drawing"),"clicked");
    CHECK(UmiTraderGtkSaveChart(workstation,exported.selected_instrument_id,3000,&report)==UMI_STATUS_OK);
    CHECK(UmiChartCheckpointLoad(chart_server,"product.acceptance",exported.selected_instrument_id,&toolDocument,&report)==UMI_STATUS_OK);
    UmiChartDocumentSummary toolSummary;CHECK(UmiChartDocumentGetSummary(toolDocument,&toolSummary)==UMI_STATUS_OK&&toolSummary.drawing_count==4);
    UmiChartDrawingSnapshot original,copy;CHECK(UmiChartDocumentDrawingAt(toolDocument,2,&original)==UMI_STATUS_OK);
    CHECK(UmiChartDocumentDrawingAt(toolDocument,3,&copy)==UMI_STATUS_OK);
    CHECK(original.locked&&!copy.locked&&strcmp(original.id,copy.id)!=0&&strcmp(copy.tool,"ray")==0);
    CHECK(original.visibility_flags==0U&&copy.visibility_flags==UMI_CHART_DRAWING_VISIBILITY_HIDDEN);
    CHECK(original.time1==copy.time1&&original.time2==copy.time2&&original.value1==copy.value1&&original.value2==copy.value2);
    UmiChartDocumentDestroy(toolDocument);
    CHECK(umi_trader_gtk_workstation_trading_snapshot(workstation,&after)==UMI_STATUS_OK);
    CHECK(after.order_count==exported.order_count&&after.environment==exported.environment&&after.live_armed==exported.live_armed);
    CHECK(memcmp(&after.draft_order,&exported.draft_order,sizeof(after.draft_order))==0);
    CHECK(UmiTraderGtkBindChartStorage(workstation, NULL, NULL) == UMI_STATUS_OK);
/* The product case now writes several chart revisions. Persistence retains a manifest and each drawing for both current and previous valid checkpoints. The previous implementation remains for engineering review. */
#if 0
    CHECK(umi_data_server_count(chart_server) == 1U); umi_data_server_destroy(chart_server);
#endif
    /* Current: one manifest and four drawings. Previous: one and three. */
    CHECK(umi_data_server_count(chart_server) == 9U); umi_data_server_destroy(chart_server);
    free(review); umi_trader_gtk_workstation_destroy(workstation); return 0;
}
