/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: tests/test_session_review.c
 * PURPOSE: Exercise the real Trader layout, Framework capture and detached native report controls.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/


#include "umicom/trader/gtk_workstation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"%s:%d: %s\n",__FILE__,__LINE__,#x);exit(1); } } while(0)
#define OK(x) CHECK((x)==UMI_STATUS_OK)
static GtkWidget *Find(GtkWidget *root,const char *id)
{
    const char *tag=g_object_get_data(G_OBJECT(root),"umicom-automation-id");
    if(tag!=NULL&&strcmp(tag,id)==0)return root;
    for(GtkWidget *child=gtk_widget_get_first_child(root);child!=NULL;child=gtk_widget_get_next_sibling(child)){
        GtkWidget *found=Find(child,id);if(found!=NULL)return found;
    }
    return NULL;
}
int main(void)
{
    if(!gtk_init_check())return 77;
    UmiTraderGtkWorkstation *workstation=NULL;OK(umi_trader_gtk_workstation_create(&workstation));
    OK(umi_trader_gtk_workstation_select_layout(workstation,"strategy-analysis"));
    GtkWidget *root=umi_trader_gtk_workstation_widget(workstation);
    GtkWidget *reportWidget=Find(root,"trading.session.report"),*refresh=Find(root,"trading.session.refresh"),*copy=Find(root,"trading.session.copy");
    CHECK(reportWidget&&refresh&&copy);g_object_ref(reportWidget);g_object_ref(refresh);g_object_ref(copy);
    UmiTradingWorkspaceSnapshot before,after;OK(umi_trader_gtk_workstation_trading_snapshot(workstation,&before));
    UmiTradingSessionReport *report=NULL;OK(UmiTraderGtkCaptureSessionReport(workstation,"",&report));
    UmiTradingSessionSummary summary;OK(UmiTradingSessionReportSummary(report,&summary));
    CHECK(summary.retainedOrders==before.order_count&&summary.retainedExecutions==before.execution_count&&summary.issues==0);
    g_signal_emit_by_name(refresh,"clicked");
    OK(umi_trader_gtk_workstation_trading_snapshot(workstation,&after));
    CHECK(before.revision==after.revision&&before.order_count==after.order_count&&before.environment==after.environment&&!after.live_armed);
    CHECK(strcmp(before.selected_instrument_id,after.selected_instrument_id)==0&&memcmp(&before.draft_order,&after.draft_order,sizeof(before.draft_order))==0);
    umi_trader_gtk_workstation_destroy(workstation);
    CHECK(!gtk_widget_get_sensitive(refresh)&&!gtk_widget_get_sensitive(copy));
    g_signal_emit_by_name(refresh,"clicked");g_signal_emit_by_name(copy,"clicked");
    UmiCsvDocument *csv=NULL;OK(UmiTradingSessionReportExportCsv(report,&csv));CHECK(strstr(UmiCsvDocumentData(csv),"LOCAL RETAINED EVIDENCE")!=NULL);
    UmiCsvDocumentDestroy(csv);UmiTradingSessionReportDestroy(report);
    g_object_unref(copy);g_object_unref(refresh);g_object_unref(reportWidget);return 0;
}
