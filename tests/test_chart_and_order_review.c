/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: tests/test_chart_and_order_review.c
 * PURPOSE: Verify Trader composes the shared chart tools and canonical order query.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/trader/gtk_workstation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #x); exit(1); } } while (0)
static GtkWidget *Find(GtkWidget *root, const char *id)
{
    const char *tag = g_object_get_data(G_OBJECT(root), "umicom-automation-id");
    if (tag != NULL && strcmp(tag, id) == 0) return root;
    for (GtkWidget *child = gtk_widget_get_first_child(root); child != NULL; child = gtk_widget_get_next_sibling(child)) {
        GtkWidget *match = Find(child, id); if (match != NULL) return match;
    }
    return NULL;
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
    free(review); umi_trader_gtk_workstation_destroy(workstation); return 0;
}
