/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: tests/test_candle_measurements.c
 * PURPOSE: Verify Trader exposes shared candle measurements without changing order state.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/trader/gtk_workstation.h"
#include "umicom/ui/gtk4/automation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(value)                                                                               \
  do {                                                                                             \
    if (!(value)) {                                                                                \
      fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #value);                                  \
      exit(1);                                                                                     \
    }                                                                                              \
  } while (0)

static GtkWidget *Find(GtkWidget *root, const char *tag) {
  /* Logical lookup includes the controls owned by a collapsed inspector. */
  GtkWidget *widget = umi_gtk4_automation_find_tagged_widget(root, tag);
  CHECK(widget != NULL);
  return widget;
}
int main(int argc, char **argv) {
  CHECK(argc == 2);
  const char *mode = argv[1];
  CHECK(!strcmp(mode, "visible") || !strcmp(mode, "clear") || !strcmp(mode, "timeframe") ||
        !strcmp(mode, "retained"));
  if (!gtk_init_check())
    return 77;
  UmiTraderGtkWorkstation *workstation = NULL;
  CHECK(umi_trader_gtk_workstation_create(&workstation) == UMI_STATUS_OK);
  GtkWidget *root = umi_trader_gtk_workstation_widget(workstation);
  GtkWidget *measure = Find(root, "trading.chart.measurement-visible");
  GtkWidget *label = Find(root, "trading.chart.measurement-result");
  GtkWidget *tool = Find(root, "trading.chart.measure");
  CHECK(GTK_IS_LABEL(Find(root, "trading.chart.candle-inspection")));
  CHECK(GTK_IS_BUTTON(Find(root, "trading.chart.measurement-copy")));
  UmiTradingWorkspaceSnapshot before, after;
  CHECK(umi_trader_gtk_workstation_trading_snapshot(workstation, &before) == UMI_STATUS_OK);
  /* Use the real product control on its seeded simulation observations.
   * No private workspace or alternate chart implementation is substituted. */
  g_signal_emit_by_name(measure, "clicked");
  const char *result = gtk_label_get_text(GTK_LABEL(label));
  CHECK(strstr(result, "Captured ") && strstr(result, before.selected_instrument_id));
  CHECK(strstr(result, "Opening-time span") && strstr(result, "provider units"));
  if (!strcmp(mode, "clear")) {
    g_signal_emit_by_name(Find(root, "trading.chart.measurement-clear"), "clicked");
    CHECK(strstr(gtk_label_get_text(GTK_LABEL(label)), "Captured ") == NULL);
  } else if (!strcmp(mode, "timeframe")) {
    gtk_drop_down_set_selected(GTK_DROP_DOWN(Find(root, "trading.chart.timeframe")), 1U);
    CHECK(strstr(gtk_label_get_text(GTK_LABEL(label)), "Captured ") == NULL);
  }
  CHECK(umi_trader_gtk_workstation_trading_snapshot(workstation, &after) == UMI_STATUS_OK);
  CHECK(after.order_count == before.order_count && after.environment == before.environment);
  CHECK(after.live_armed == before.live_armed);
  CHECK(strcmp(after.selected_instrument_id, before.selected_instrument_id) == 0);
  CHECK(memcmp(&after.draft_order, &before.draft_order, sizeof after.draft_order) == 0);
  char *previous = NULL;
  if (!strcmp(mode, "retained")) {
    /* Third-party hosts may keep controls after closing a product. Their
     * signal connections must no longer reach the destroyed trading model. */
    g_object_ref(measure);
    g_object_ref(label);
    g_object_ref(tool);
    previous = g_strdup(gtk_label_get_text(GTK_LABEL(label)));
  }
  umi_trader_gtk_workstation_destroy(workstation);
  if (previous != NULL) {
    g_signal_emit_by_name(measure, "clicked");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tool), TRUE);
    CHECK(strcmp(previous, gtk_label_get_text(GTK_LABEL(label))) == 0);
    g_free(previous);
    g_object_unref(measure);
    g_object_unref(label);
    g_object_unref(tool);
  }
  return 0;
}
