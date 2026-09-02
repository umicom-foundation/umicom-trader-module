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

/**
 * Represent the trader gtk workstation data shared with callers of this public contract.
 */
typedef struct UmiTraderGtkWorkstation UmiTraderGtkWorkstation;

/** Create Trader's thin GTK adapter over the shared suite workstation. */
UmiStatus umi_trader_gtk_workstation_create(
    UmiTraderGtkWorkstation **out_workstation);
/** Release the adapter and every Framework service it owns. */
void umi_trader_gtk_workstation_destroy(
    UmiTraderGtkWorkstation *workstation);
/** Borrow the root widget so an application window can present it. */
GtkWidget *umi_trader_gtk_workstation_widget(
    UmiTraderGtkWorkstation *workstation);
/** Select and render one canonical Trader layout by identifier. */
UmiStatus umi_trader_gtk_workstation_select_layout(
    UmiTraderGtkWorkstation *workstation,
    const char *layout_id);
/** Select a Framework appearance such as dark, light, retro or neo. */
UmiStatus umi_trader_gtk_workstation_select_appearance(
    UmiTraderGtkWorkstation *workstation,
    const char *profile_id);
/** Apply user-defined fonts, scale, density and semantic colours. */
UmiStatus umi_trader_gtk_workstation_apply_custom_appearance(
    UmiTraderGtkWorkstation *workstation,
    const UmiUiAppearanceProfile *profile);
/** Copy the active appearance without exposing Framework-owned widgets. */
UmiStatus umi_trader_gtk_workstation_active_appearance(
    const UmiTraderGtkWorkstation *workstation,
    UmiUiAppearanceProfile *out_profile);
/** Begin a reversible layout customisation transaction. */
UmiStatus umi_trader_gtk_workstation_begin_layout_edit(
    UmiTraderGtkWorkstation *workstation);
/** Accept the current layout edits and make them the active arrangement. */
UmiStatus umi_trader_gtk_workstation_commit_layout_edit(
    UmiTraderGtkWorkstation *workstation);
/** Discard uncommitted edits and restore the previous arrangement. */
UmiStatus umi_trader_gtk_workstation_cancel_layout_edit(
    UmiTraderGtkWorkstation *workstation);
/** Encode the active layout through portable Framework persistence. */
UmiStatus umi_trader_gtk_workstation_export_layout(
    const UmiTraderGtkWorkstation *workstation,
    uint64_t saved_at_ns,
    char *out_text,
    size_t capacity);
/** Decode, validate and optionally activate a portable Framework layout. */
UmiStatus umi_trader_gtk_workstation_import_layout(
    UmiTraderGtkWorkstation *workstation,
    const char *text,
    int activate,
    UmiUiWorkspaceImportReport *out_report);
/** Save a recovery checkpoint without duplicating persistence in Trader. */
UmiStatus umi_trader_gtk_workstation_save_checkpoint(
    UmiTraderGtkWorkstation *workstation,
    uint64_t saved_at_ns);
/** Restore the latest valid recovery checkpoint. */
UmiStatus umi_trader_gtk_workstation_restore_checkpoint(
    UmiTraderGtkWorkstation *workstation);
/** Open another reusable tool window and return its collision-free ID. */
UmiStatus umi_trader_gtk_workstation_open_window(
    UmiTraderGtkWorkstation *workstation,
    const char *tool_id,
    const char *region_id,
    int floating,
    uint64_t opened_at_ms,
    char *out_window_id,
    size_t out_window_id_capacity);
/** Move, dock, float or resize an existing workspace window. */
UmiStatus umi_trader_gtk_workstation_move_window(
    UmiTraderGtkWorkstation *workstation,
    const char *window_id,
    const char *region_id,
    double x,
    double y,
    double width,
    double height);
/** Close a closable window while preserving its component definition. */
UmiStatus umi_trader_gtk_workstation_close_window(
    UmiTraderGtkWorkstation *workstation,
    const char *window_id);
/** Apply Framework-owned placement, stack, context and visibility settings. */
UmiStatus umi_trader_gtk_workstation_apply_panel_settings(
    UmiTraderGtkWorkstation *workstation,
    const UmiUiWorkspacePanelSettings *settings);
/** Return a value snapshot of layout and rendering state. */
UmiApplicationSuiteGtk4WorkstationSnapshot
umi_trader_gtk_workstation_snapshot(
    const UmiTraderGtkWorkstation *workstation);
/** Copy current trading data, selection and execution state. */
UmiStatus umi_trader_gtk_workstation_trading_snapshot(
    UmiTraderGtkWorkstation *workstation,
    UmiTradingWorkspaceSnapshot *out_snapshot);
/** Copy application lifecycle and presentation-surface state. */
UmiStatus umi_trader_gtk_workstation_application_surface_snapshot(
    const UmiTraderGtkWorkstation *workstation,
    UmiApplicationPresentationSurfaceSnapshot *out_snapshot);
/** Advance deterministic simulation and maintenance clocks. */
UmiStatus umi_trader_gtk_workstation_advance(
    UmiTraderGtkWorkstation *workstation,
    uint32_t elapsed_seconds);
/** Tell the surface policy whether the application is in the background. */
UmiStatus umi_trader_gtk_workstation_set_background(
    UmiTraderGtkWorkstation *workstation,
    int background);
/** Publish a component context change to linked windows. */
UmiStatus umi_trader_gtk_workstation_context_changed(
    UmiTraderGtkWorkstation *workstation,
    const char *component_id,
    const char *context_value);
/** Return non-zero when changed state should receive a recovery checkpoint. */
int umi_trader_gtk_workstation_checkpoint_due(
    const UmiTraderGtkWorkstation *workstation,
    uint32_t elapsed_since_checkpoint_seconds,
    int changed);

#ifdef __cplusplus
}
#endif
#endif
