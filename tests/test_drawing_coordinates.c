/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: tests/test_drawing_coordinates.c
 * PURPOSE: Verify precise drawing editing, persistence and retained controls in Trader.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/
#include "umicom/trader/gtk_workstation.h"
#include "umicom/chart/drawing_appearance.h"
#include "umicom/trading_ui/gtk4/interactive_chart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #x); exit(1); } } while (0)
#define OK(x) CHECK((x) == UMI_STATUS_OK)
static GtkWidget *Find(GtkWidget *root, const char *id)
{
    const char *tag = g_object_get_data(G_OBJECT(root), "umicom-automation-id");
    if (tag != NULL && strcmp(tag, id) == 0) return root;
    for (GtkWidget *child = gtk_widget_get_first_child(root); child != NULL; child = gtk_widget_get_next_sibling(child)) {
        GtkWidget *found = Find(child, id); if (found != NULL) return found;
    }
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 2) return 2;
    const char *name = argv[1];
    const char *cases[] = {"save", "restore", "invalid", "retained"};
    int known = 0;
    for (size_t index = 0U; index < sizeof cases / sizeof cases[0]; ++index) if (strcmp(name, cases[index]) == 0) known = 1;
    if (!known) return 2;
    if (!gtk_init_check()) return 77;
    UmiTraderGtkWorkstation *workstation = NULL; OK(umi_trader_gtk_workstation_create(&workstation));
    GtkWidget *root = umi_trader_gtk_workstation_widget(workstation);
    UmiTradingWorkspaceSnapshot before, after; OK(umi_trader_gtk_workstation_trading_snapshot(workstation, &before));
    UmiDataServer *server = NULL; OK(umi_data_server_create_memory(&server));
    OK(UmiTraderGtkBindChartStorage(workstation, server, "coordinates.product"));
    UmiChartDrawingSnapshot drawing;
    OK(UmiChartDrawingInitialize("product.range", before.selected_instrument_id, UMI_CHART_DRAWING_RANGE,
        (UmiChartPoint){1000,1.1}, (UmiChartPoint){2000,1.2}, &drawing));
    UmiChartNavigation navigation = {0}; UmiChartDocument *document = NULL;
    OK(UmiChartDocumentCreate(before.selected_instrument_id, &navigation, &drawing, 1, 0, &document));
    UmiChartCheckpointReport report; UmiTradingChartPreview preview;
    OK(UmiChartCheckpointSave(server, "coordinates.product", document, 0, 1000, &report));
    UmiChartDocumentDestroy(document);
    OK(UmiTraderGtkPreviewChart(workstation, before.selected_instrument_id, &preview));
    OK(UmiTraderGtkRestoreChart(workstation, before.selected_instrument_id, preview.preview_id));
    UmiGtk4TradingInteractiveChartRefresh(Find(root, "trading.chart.panel"));

    GtkWidget *load = Find(root, "trading.chart.coordinates-load"), *apply = Find(root, "trading.chart.coordinates-apply");
    GtkWidget *price = Find(root, "trading.chart.coordinates-price-first"), *timestamp = Find(root, "trading.chart.coordinates-time-first");
    CHECK(GTK_IS_BUTTON(load) && GTK_IS_BUTTON(apply) && GTK_IS_ENTRY(price) && GTK_IS_ENTRY(timestamp));
    g_signal_emit_by_name(load, "clicked");
    gtk_editable_set_text(GTK_EDITABLE(price), strcmp(name, "invalid") == 0 ? "nan" : "1.125");
    gtk_editable_set_text(GTK_EDITABLE(timestamp), "1500"); g_signal_emit_by_name(apply, "clicked");
    OK(UmiTraderGtkSaveChart(workstation, before.selected_instrument_id, 2000, &report));
    OK(UmiChartCheckpointLoad(server, "coordinates.product", before.selected_instrument_id, &document, &report));
    OK(UmiChartDocumentDrawingAt(document, 0U, &drawing)); UmiChartDocumentDestroy(document);
    CHECK(drawing.time1 == (strcmp(name, "invalid") == 0 ? 1000 : 1500));
    CHECK(drawing.value1 == (strcmp(name, "invalid") == 0 ? 1.1 : 1.125));
    CHECK(drawing.time2 == 2000 && drawing.value2 == 1.2);
    if (strcmp(name, "restore") == 0) {
        gtk_editable_set_text(GTK_EDITABLE(price), "1.15"); g_signal_emit_by_name(apply, "clicked");
        OK(UmiTraderGtkPreviewChart(workstation, before.selected_instrument_id, &preview));
        OK(UmiTraderGtkRestoreChart(workstation, before.selected_instrument_id, preview.preview_id));
        UmiGtk4TradingInteractiveChartRefresh(Find(root, "trading.chart.panel"));
        g_signal_emit_by_name(load, "clicked");
        CHECK(strcmp(gtk_editable_get_text(GTK_EDITABLE(price)), "1.125") == 0);
        CHECK(strcmp(gtk_editable_get_text(GTK_EDITABLE(timestamp)), "1500") == 0);
    }
    OK(umi_trader_gtk_workstation_trading_snapshot(workstation, &after));
    CHECK(after.order_count == before.order_count && after.environment == before.environment && after.live_armed == before.live_armed);
    CHECK(memcmp(&after.draft_order, &before.draft_order, sizeof before.draft_order) == 0);
    if (strcmp(name, "retained") == 0) { g_object_ref(load); g_object_ref(apply); }
    OK(UmiTraderGtkBindChartStorage(workstation, NULL, NULL));
    umi_trader_gtk_workstation_destroy(workstation); umi_data_server_destroy(server);
    if (strcmp(name, "retained") == 0) {
        g_signal_emit_by_name(load, "clicked"); g_signal_emit_by_name(apply, "clicked"); g_object_unref(load); g_object_unref(apply);
    }
    return 0;
}
