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

#include "umicom/ui/workspace_library_exchange.h"
#include <stddef.h>

#include <stdint.h>

#include "umicom/ui/workspace_library.h"
#include "umicom/ui/workspace_library_checkpoint.h"
#include <gtk/gtk.h>

#include "umicom/application/suite_layout/gtk4_workstation.h"
#include "umicom/trader/application_surface.h"
#include "umicom/trader/application_surface_policy.h"
#include "umicom/trader/runtime.h"
#include "umicom/trading_ui/gtk4/trading_suite_workstation.h"

#include "umicom/trading/order_csv.h"
#include "umicom/trading/session_report.h"

#include "umicom/chart/drawing_history.h"

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
/** Bind the existing shared product identity to a native window before its
 * first realization. The Framework owns titlebar composition and lifetime;
 * no application catalogue or appearance state is duplicated. */
UmiStatus umi_trader_gtk_workstation_bind_window(
    UmiTraderGtkWorkstation *workstation, GtkWindow *window);
/** Explicitly enable shared user-local SQLite layout checkpoints.
 * Native launchers opt in after construction; constructors do no checkpoint I/O.
 * A failed restore leaves the current layout visible and reports its error. */
UmiStatus umi_trader_gtk_workstation_enable_checkpoint_storage(
    UmiTraderGtkWorkstation *workstation, int restore_saved);

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
/** Apply several panel edits together and leave the layout unchanged on failure. */
UmiStatus umi_trader_gtk_workstation_apply_panel_batch(
    UmiTraderGtkWorkstation *workstation,
    const UmiUiWorkspacePanelSettings *settings,
    size_t setting_count);
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


/** Copy the current ordered layout list on the GTK owning thread. No live
 * pointers escape; a failed read leaves output unchanged. This performs no I/O. */
UmiStatus umi_trader_gtk_workstation_library_snapshot(
    UmiTraderGtkWorkstation *workstation, UmiUiWorkspaceLibrarySnapshot *out_snapshot);

/** Apply a revision-checked library action through the same staged native
 * publication path as Layout Library. Product scope and edit gates remain in
 * Framework. A move preserves active panels and does not execute trades, save
 * documents or persist the library. Use explicit Save library for persistence.
 * Inputs are borrowed for this synchronous owner-thread call only. */
UmiStatus umi_trader_gtk_workstation_library_apply(
    UmiTraderGtkWorkstation *workstation, const UmiUiWorkspaceLibraryRequest *request);

/** Bind an existing borrowed Data Server for explicit layout/library saves.
 * The server outlives the workstation or is unbound with NULL. This probes saved
 * evidence but never restores automatically, opens a path or enables trading.
 * Memory backends remain visibly non-durable. Active edits return BUSY. */
UmiStatus umi_trader_gtk_workstation_bind_checkpoint_storage(
    UmiTraderGtkWorkstation *workstation, UmiDataServer *server);

/** Read the saved library into owned preview data without changing the live
 * layout, document/trading state, storage or cached Save revision. Call on the
 * GTK owner thread. Failure leaves output unchanged. Later Restore rereads the
 * store; this preview is not a reservation. The connected server is borrowed. */
UmiStatus umi_trader_gtk_workstation_library_preview(
    UmiTraderGtkWorkstation *workstation, UmiUiWorkspaceLibraryPreview *out_preview);

/* Product entry points delegate order browsing to Framework. Search does not
 * modify the ticket or portfolio. Reviews are owned copies; access on the GTK
 * owner thread. Failure leaves output unchanged. */
UmiStatus umi_trader_gtk_workstation_set_order_query(UmiTraderGtkWorkstation *workstation,
    const UmiTradingOrderQuery *query);
UmiStatus umi_trader_gtk_workstation_review_order(UmiTraderGtkWorkstation *workstation,
    const char *client_order_id, UmiTradingOrderReview *out_review);

/* Use Framework-owned profile-specific layout persistence after local sign-in. */
UmiStatus UmiTraderGtkEnableProfileStorage(UmiTraderGtkWorkstation *workstation, const char *profile, int restore_saved);

/* Capture the applied order query through Framework. Caller destroys the
 * returned document. No broker request or selection change occurs. */
UmiStatus UmiTraderGtkExportOrdersCsv(UmiTraderGtkWorkstation *workstation,
    UmiCsvDocument **outDocument);

/* Chart persistence delegates to Framework. The server is borrowed until
 * unbound or the workstation closes. These synchronous GTK-owner-thread calls
 * never submit orders. Preview returns the identity required by Restore. */
UmiStatus UmiTraderGtkBindChartStorage(UmiTraderGtkWorkstation *workstation,
    UmiDataServer *server, const char *scope);
UmiStatus UmiTraderGtkSaveChart(UmiTraderGtkWorkstation *workstation,
    const char *instrumentId, uint64_t savedAtMs, UmiChartCheckpointReport *outReport);
/* Thin product access to per-instrument chart timeframes. No orders are sent.
 * Supported fixed durations are source=0, 1m, 5m, 15m, 1h, 4h and 1d UTC. */
UmiStatus UmiTraderGtkSetChartTimeframe(UmiTraderGtkWorkstation *workstation,
    const char *instrumentId, uint32_t intervalMs);
UmiStatus UmiTraderGtkGetChartTimeframe(UmiTraderGtkWorkstation *workstation,
    const char *instrumentId, uint32_t *outIntervalMs);
UmiStatus UmiTraderGtkPreviewChart(UmiTraderGtkWorkstation *workstation,
    const char *instrumentId, UmiTradingChartPreview *outPreview);
UmiStatus UmiTraderGtkRestoreChart(UmiTraderGtkWorkstation *workstation,
    const char *instrumentId, uint64_t previewId);

/* Copy session evidence through the canonical Framework owner. The report is
 * independent of the workstation lifetime and must be destroyed by its caller.
 * The instrument filter does not change chart or order selection. */
UmiStatus UmiTraderGtkCaptureSessionReport(UmiTraderGtkWorkstation *workstation,
    const char *instrumentFilter, UmiTradingSessionReport **outReport);

/* Drawing-only history delegates to Framework; revision must come from the
 * displayed snapshot. Restore starts a new history; save retains the current one. */
UmiStatus UmiTraderGtkDrawingHistory(UmiTraderGtkWorkstation *workstation, UmiChartDrawingHistorySnapshot *out);
UmiStatus UmiTraderGtkUndoDrawing(UmiTraderGtkWorkstation *workstation, const char *instrument, uint64_t expectedRevision);
UmiStatus UmiTraderGtkRedoDrawing(UmiTraderGtkWorkstation *workstation, const char *instrument, uint64_t expectedRevision);

/* Portable library exchange uses the same owner and scope as the native
 * Layout Library. Review owns copied bytes; apply refuses a stale workspace.
 * Calls run on the GTK owner thread. They never save storage or place orders.
 * The caller destroys the review after apply or cancellation. */
UmiStatus umi_trader_gtk_workstation_library_export(
    UmiTraderGtkWorkstation *workstation, char *bytes, size_t capacity, size_t *out_size);
UmiStatus umi_trader_gtk_workstation_library_import_review(
    UmiTraderGtkWorkstation *workstation, const void *bytes, size_t size, UmiUiWorkspaceLibraryImport **out_review);
UmiStatus umi_trader_gtk_workstation_library_import_apply(
    UmiTraderGtkWorkstation *workstation, const UmiUiWorkspaceLibraryImport *review);

/* Layout recovery stays with Framework. This owner-thread call neither
 * saves product data nor runs a trade, payment or project command. */
UmiStatus umi_trader_gtk_workstation_library_history_read(UmiTraderGtkWorkstation *workstation,
    UmiUiWorkspaceLibraryHistoryState *out_state);
/* Layout recovery stays with Framework. This owner-thread call neither
 * saves product data nor runs a trade, payment or project command. */
UmiStatus umi_trader_gtk_workstation_library_history_navigate(UmiTraderGtkWorkstation *workstation,
    UmiUiWorkspaceLibraryHistoryDirection direction, uint64_t expected_revision);

#ifdef __cplusplus
}
#endif
#endif
