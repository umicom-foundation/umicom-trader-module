/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: applications/trader/include/umicom/trader/gtk_workstation.h
 *
 * PURPOSE:
 *   Expose the thin Trader GTK4 product composition over the Framework-owned
 *   interactive trading suite workstation and canonical application layouts.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_TRADER_GTK_WORKSTATION_H
#define UMICOM_TRADER_GTK_WORKSTATION_H

#include <stdint.h>

#include <gtk/gtk.h>

#include "umicom/application/suite_layout/gtk4_workstation.h"
#include "umicom/trader/application_surface.h"
#include "umicom/trader/application_surface_policy.h"
#include "umicom/trader/runtime.h"
#include "umicom/trading_ui/gtk4/trading_suite_workstation.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UmiTraderGtkWorkstation UmiTraderGtkWorkstation;

UmiStatus umi_trader_gtk_workstation_create(
    UmiTraderGtkWorkstation **out_workstation);
void umi_trader_gtk_workstation_destroy(
    UmiTraderGtkWorkstation *workstation);
GtkWidget *umi_trader_gtk_workstation_widget(
    UmiTraderGtkWorkstation *workstation);
UmiStatus umi_trader_gtk_workstation_select_layout(
    UmiTraderGtkWorkstation *workstation,
    const char *layout_id);
UmiStatus umi_trader_gtk_workstation_begin_layout_edit(
    UmiTraderGtkWorkstation *workstation);
UmiStatus umi_trader_gtk_workstation_commit_layout_edit(
    UmiTraderGtkWorkstation *workstation);
UmiStatus umi_trader_gtk_workstation_cancel_layout_edit(
    UmiTraderGtkWorkstation *workstation);
UmiStatus umi_trader_gtk_workstation_open_window(
    UmiTraderGtkWorkstation *workstation,
    const char *tool_id,
    const char *region_id,
    int floating,
    uint64_t opened_at_ms,
    char *out_window_id,
    size_t out_window_id_capacity);
UmiStatus umi_trader_gtk_workstation_move_window(
    UmiTraderGtkWorkstation *workstation,
    const char *window_id,
    const char *region_id,
    double x,
    double y,
    double width,
    double height);
UmiStatus umi_trader_gtk_workstation_close_window(
    UmiTraderGtkWorkstation *workstation,
    const char *window_id);
UmiApplicationSuiteGtk4WorkstationSnapshot
umi_trader_gtk_workstation_snapshot(
    const UmiTraderGtkWorkstation *workstation);
UmiStatus umi_trader_gtk_workstation_trading_snapshot(
    UmiTraderGtkWorkstation *workstation,
    UmiTradingWorkspaceSnapshot *out_snapshot);
UmiStatus umi_trader_gtk_workstation_application_surface_snapshot(
    const UmiTraderGtkWorkstation *workstation,
    UmiApplicationPresentationSurfaceSnapshot *out_snapshot);
UmiStatus umi_trader_gtk_workstation_advance(
    UmiTraderGtkWorkstation *workstation,
    uint32_t elapsed_seconds);
UmiStatus umi_trader_gtk_workstation_set_background(
    UmiTraderGtkWorkstation *workstation,
    int background);
UmiStatus umi_trader_gtk_workstation_context_changed(
    UmiTraderGtkWorkstation *workstation,
    const char *component_id,
    const char *context_value);
int umi_trader_gtk_workstation_checkpoint_due(
    const UmiTraderGtkWorkstation *workstation,
    uint32_t elapsed_since_checkpoint_seconds,
    int changed);

#ifdef __cplusplus
}
#endif
#endif
