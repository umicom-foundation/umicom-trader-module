/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: tests/test_chart_timeframes.c
 * PURPOSE: Qualify timeframes through the real Trader controls, facade and saved chart.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/trader/gtk_workstation.h"
#include "umicom/trading_ui/gtk4/interactive_chart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"%d: %s\n",__LINE__,#x);exit(1);}}while(0)
#define OK(x) CHECK((x)==UMI_STATUS_OK)
static GtkWidget *Find(GtkWidget *root,const char *id)
{
    const char *tag=g_object_get_data(G_OBJECT(root),"umicom-automation-id");if(tag!=NULL && strcmp(tag,id)==0)return root;
    for(GtkWidget *child=gtk_widget_get_first_child(root);child!=NULL;child=gtk_widget_get_next_sibling(child)){
        GtkWidget *found=Find(child,id);if(found!=NULL)return found;
    }return NULL;
}
int main(int argc,char **argv)
{
    CHECK(argc==2);if(!gtk_init_check())return 77;const char *name=argv[1];
    UmiTraderGtkWorkstation *workstation=NULL;OK(umi_trader_gtk_workstation_create(&workstation));
    GtkWidget *root=umi_trader_gtk_workstation_widget(workstation),*selector=Find(root,"trading.chart.timeframe");CHECK(GTK_IS_DROP_DOWN(selector));
    UmiTradingWorkspaceSnapshot before,after;OK(umi_trader_gtk_workstation_trading_snapshot(workstation,&before));const char *id=before.selected_instrument_id;
    UmiDataServer *server=NULL;OK(umi_data_server_create_memory(&server));OK(UmiTraderGtkBindChartStorage(workstation,server,"product.timeframe"));
    gtk_drop_down_set_selected(GTK_DROP_DOWN(selector),2U);uint32_t interval=0U;
    OK(UmiTraderGtkGetChartTimeframe(workstation,id,&interval));CHECK(interval==300000U);
    UmiChartCheckpointReport report;OK(UmiTraderGtkSaveChart(workstation,id,1000U,&report));
    UmiChartDocument *saved=NULL;OK(UmiChartCheckpointLoad(server,"product.timeframe",id,&saved,&report));UmiChartDocumentSummary summary;
    OK(UmiChartDocumentGetSummary(saved,&summary));CHECK(summary.navigation.interval_ms==300000U);UmiChartDocumentDestroy(saved);
    if(strcmp(name,"restore")==0){
        OK(UmiTraderGtkSetChartTimeframe(workstation,id,0U));UmiTradingChartPreview preview;
        OK(UmiTraderGtkPreviewChart(workstation,id,&preview));CHECK(preview.current.navigation.interval_ms==0U && preview.saved.navigation.interval_ms==300000U);
        OK(UmiTraderGtkRestoreChart(workstation,id,preview.preview_id));UmiGtk4TradingInteractiveChartRefresh(Find(root,"trading.chart.panel"));
        CHECK(gtk_drop_down_get_selected(GTK_DROP_DOWN(selector))==2U);
    }else if(strcmp(name,"invalid")==0){
        CHECK(UmiTraderGtkSetChartTimeframe(workstation,id,7U)==UMI_STATUS_INVALID_ARGUMENT);
        interval=123U;CHECK(UmiTraderGtkGetChartTimeframe(workstation,"missing.instrument",&interval)!=UMI_STATUS_OK && interval==123U);
        OK(UmiTraderGtkGetChartTimeframe(workstation,id,&interval));CHECK(interval==300000U);
    }else CHECK(strcmp(name,"save")==0 || strcmp(name,"retained")==0);
    OK(umi_trader_gtk_workstation_trading_snapshot(workstation,&after));
    CHECK(after.order_count==before.order_count && after.environment==before.environment && after.live_armed==before.live_armed);
    CHECK(memcmp(&after.draft_order,&before.draft_order,sizeof before.draft_order)==0);
    if(strcmp(name,"retained")==0)g_object_ref(selector);
    OK(UmiTraderGtkBindChartStorage(workstation,NULL,NULL));umi_trader_gtk_workstation_destroy(workstation);umi_data_server_destroy(server);
    if(strcmp(name,"retained")==0){gtk_drop_down_set_selected(GTK_DROP_DOWN(selector),4U);g_object_unref(selector);}
    return 0;
}
