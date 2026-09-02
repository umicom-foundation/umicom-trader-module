/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: applications/trader/src/gtk/workstation.c
 *
 * PURPOSE:
 *   Keep Trader a thin product composition: create the canonical trading
 *   workspace and delegate all GTK4 interaction, simulation, panel rendering
 *   and suite-layout behaviour to Umicom Framework.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/trader/gtk_workstation.h"

#include <stdlib.h>

struct UmiTraderGtkWorkstation {
    UmiTradingWorkspace *trading;
    UmiGtk4TradingSuiteWorkstation *framework_workstation;
    UmiTraderApplicationSurface *application_surface;
};

/*
 * Initialise trader gtk workstation from caller-provided values so later operations
 * receive a known state.
 */
UmiStatus umi_trader_gtk_workstation_create(
    UmiTraderGtkWorkstation **out_workstation)
{
    UmiTraderGtkWorkstation *workstation;
    UmiTradingWorkspaceConfig trading_config;
    UmiGtk4TradingSuiteWorkstationConfig framework_config;
    UmiStatus status;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (out_workstation == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    *out_workstation = NULL;
    workstation = (UmiTraderGtkWorkstation *)calloc(1U, sizeof(*workstation));
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) return UMI_STATUS_OUT_OF_MEMORY;

    /* Framework defaults are simulation-only, with no broker transport and no
     * live arming. The GUI therefore starts in a safe runnable state. */
    trading_config = umi_trading_workspace_config_default();
    status = umi_trading_workspace_create(
        &trading_config, &workstation->trading);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) goto fail;

    /* The Framework-owned trading suite seeds the safe simulation market,
     * supplies guarded actions and renders the canonical product layouts. */
    framework_config = umi_gtk4_trading_suite_workstation_config_default(
        workstation->trading);
    status = umi_gtk4_trading_suite_workstation_create(
        &framework_config, &workstation->framework_workstation);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) goto fail;

    /* Keep the canonical component/presentation session beside the existing
     * Framework GTK4 trading workstation. This is additive: native trading
     * widgets and simulation state remain owned by their established paths. */
    status = umi_trader_application_surface_create(
        &workstation->application_surface);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) goto fail;

    *out_workstation = workstation;
    return UMI_STATUS_OK;

fail:
    umi_trader_gtk_workstation_destroy(workstation);
    return status;
}

/*
 * Release or reset state held by trader gtk workstation so the same storage can be reused
 * safely.
 */
void umi_trader_gtk_workstation_destroy(
    UmiTraderGtkWorkstation *workstation)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) return;
    umi_trader_application_surface_destroy(workstation->application_surface);
    workstation->application_surface = NULL;
    umi_gtk4_trading_suite_workstation_destroy(
        workstation->framework_workstation);
    workstation->framework_workstation = NULL;
    umi_trading_workspace_destroy(workstation->trading);
    workstation->trading = NULL;
    free(workstation);
}

/*
 * Provide the trader gtk workstation widget operation used by this module and its client
 * applications.
 */
GtkWidget *umi_trader_gtk_workstation_widget(
    UmiTraderGtkWorkstation *workstation)
{
    return workstation != NULL
        ? umi_gtk4_trading_suite_workstation_widget(
            workstation->framework_workstation)
        : NULL;
}

/*
 * Provide the trader gtk workstation select layout operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_gtk_workstation_select_layout(
    UmiTraderGtkWorkstation *workstation,
    const char *layout_id)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    return umi_gtk4_trading_suite_workstation_select_layout(
        workstation->framework_workstation, layout_id);
}

/* Trader forwards presentation choices and keeps product code free of CSS. */
UmiStatus umi_trader_gtk_workstation_select_appearance(
    UmiTraderGtkWorkstation *workstation,
    const char *profile_id)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL || profile_id == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    return umi_gtk4_trading_suite_workstation_select_appearance(
        workstation->framework_workstation, profile_id);
}

/* Custom visual preferences use the same Framework validation as the menu. */
UmiStatus umi_trader_gtk_workstation_apply_custom_appearance(
    UmiTraderGtkWorkstation *workstation,
    const UmiUiAppearanceProfile *profile)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL || profile == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    return umi_gtk4_trading_suite_workstation_apply_custom_appearance(
        workstation->framework_workstation, profile);
}

/* Return a copied value suitable for settings pages and tests. */
UmiStatus umi_trader_gtk_workstation_active_appearance(
    const UmiTraderGtkWorkstation *workstation,
    UmiUiAppearanceProfile *out_profile)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL || out_profile == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    return umi_gtk4_trading_suite_workstation_active_appearance(
        workstation->framework_workstation, out_profile);
}

/*
 * Provide the trader gtk workstation snapshot operation used by this module and its client
 * applications.
 */
UmiApplicationSuiteGtk4WorkstationSnapshot
umi_trader_gtk_workstation_snapshot(
    const UmiTraderGtkWorkstation *workstation)
{
    UmiGtk4TradingSuiteWorkstationSnapshot snapshot;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL || workstation->framework_workstation == NULL)
        return (UmiApplicationSuiteGtk4WorkstationSnapshot){0};
    snapshot = umi_gtk4_trading_suite_workstation_snapshot(
        workstation->framework_workstation);
    return snapshot.layout;
}

/*
 * Provide the trader gtk workstation trading snapshot operation used by this module and
 * its client applications.
 */
UmiStatus umi_trader_gtk_workstation_trading_snapshot(
    UmiTraderGtkWorkstation *workstation,
    UmiTradingWorkspaceSnapshot *out_snapshot)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL || workstation->trading == NULL ||
        out_snapshot == NULL)
        return UMI_STATUS_INVALID_ARGUMENT;
    return umi_trading_workspace_snapshot(workstation->trading, out_snapshot);
}

/*
 * Provide the trader gtk workstation begin layout edit operation used by this module and
 * its client applications.
 */
UmiStatus umi_trader_gtk_workstation_begin_layout_edit(
    UmiTraderGtkWorkstation *workstation)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    return umi_gtk4_trading_suite_workstation_begin_layout_edit(
        workstation->framework_workstation);
}

/*
 * Provide the trader gtk workstation commit layout edit operation used by this module and
 * its client applications.
 */
UmiStatus umi_trader_gtk_workstation_commit_layout_edit(
    UmiTraderGtkWorkstation *workstation)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    return umi_gtk4_trading_suite_workstation_commit_layout_edit(
        workstation->framework_workstation);
}

/*
 * Provide the trader gtk workstation cancel layout edit operation used by this module and
 * its client applications.
 */
UmiStatus umi_trader_gtk_workstation_cancel_layout_edit(
    UmiTraderGtkWorkstation *workstation)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    return umi_gtk4_trading_suite_workstation_cancel_layout_edit(
        workstation->framework_workstation);
}

/* Trader forwards portable export to its Framework trading composition. */
UmiStatus umi_trader_gtk_workstation_export_layout(
    const UmiTraderGtkWorkstation *workstation,
    uint64_t saved_at_ns,
    char *out_text,
    size_t capacity)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    return umi_gtk4_trading_suite_workstation_export_layout(
        workstation->framework_workstation,
        saved_at_ns,
        out_text,
        capacity);
}

/* Trader forwards portable import and receives the Framework result report. */
UmiStatus umi_trader_gtk_workstation_import_layout(
    UmiTraderGtkWorkstation *workstation,
    const char *text,
    int activate,
    UmiUiWorkspaceImportReport *out_report)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    return umi_gtk4_trading_suite_workstation_import_layout(
        workstation->framework_workstation,
        text,
        activate,
        out_report);
}

/* Trader forwards checkpoint save so its thin shell owns no layout buffer. */
UmiStatus umi_trader_gtk_workstation_save_checkpoint(
    UmiTraderGtkWorkstation *workstation,
    uint64_t saved_at_ns)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    return umi_gtk4_trading_suite_workstation_save_checkpoint(
        workstation->framework_workstation, saved_at_ns);
}

/* Trader forwards checkpoint restore to the shared suite workstation. */
UmiStatus umi_trader_gtk_workstation_restore_checkpoint(
    UmiTraderGtkWorkstation *workstation)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    return umi_gtk4_trading_suite_workstation_restore_checkpoint(
        workstation->framework_workstation);
}

/*
 * Provide the trader gtk workstation open window operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_gtk_workstation_open_window(
    UmiTraderGtkWorkstation *workstation,
    const char *tool_id,
    const char *region_id,
    int floating,
    uint64_t opened_at_ms,
    char *out_window_id,
    size_t out_window_id_capacity)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    return umi_gtk4_trading_suite_workstation_open_window(
        workstation->framework_workstation,
        tool_id,
        region_id,
        floating,
        opened_at_ms,
        out_window_id,
        out_window_id_capacity);
}

/*
 * Provide the trader gtk workstation move window operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_gtk_workstation_move_window(
    UmiTraderGtkWorkstation *workstation,
    const char *window_id,
    const char *region_id,
    double x,
    double y,
    double width,
    double height)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    return umi_gtk4_trading_suite_workstation_move_window(
        workstation->framework_workstation,
        window_id,
        region_id,
        x,
        y,
        width,
        height);
}

/*
 * Provide the trader gtk workstation close window operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_gtk_workstation_close_window(
    UmiTraderGtkWorkstation *workstation,
    const char *window_id)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    return umi_gtk4_trading_suite_workstation_close_window(
        workstation->framework_workstation, window_id);
}

/* Forward panel settings from the Trader shell to its Framework workstation. */
UmiStatus umi_trader_gtk_workstation_apply_panel_settings(
    UmiTraderGtkWorkstation *workstation,
    const UmiUiWorkspacePanelSettings *settings)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL || settings == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    /* Trader owns no docking policy; the shared workstation applies the same
     * atomic request used by every other Umicom application. */
    return umi_gtk4_trading_suite_workstation_apply_panel_settings(
        workstation->framework_workstation, settings);
}

/*
 * Provide the trader gtk workstation application surface snapshot operation used by this
 * module and its client applications.
 */
UmiStatus umi_trader_gtk_workstation_application_surface_snapshot(
    const UmiTraderGtkWorkstation *workstation,
    UmiApplicationPresentationSurfaceSnapshot *out_snapshot)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL || workstation->application_surface == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    return umi_trader_application_surface_snapshot(
        workstation->application_surface, out_snapshot);
}

/*
 * Provide the trader gtk workstation advance operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_gtk_workstation_advance(
    UmiTraderGtkWorkstation *workstation,
    uint32_t elapsed_seconds)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL || workstation->application_surface == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    return umi_trader_application_surface_policy_advance(
        workstation->application_surface, elapsed_seconds);
}

/*
 * Provide the trader gtk workstation set background operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_gtk_workstation_set_background(
    UmiTraderGtkWorkstation *workstation,
    int background)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL || workstation->application_surface == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    return umi_trader_application_surface_policy_set_background(
        workstation->application_surface, background);
}

/*
 * Provide the trader gtk workstation context changed operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_gtk_workstation_context_changed(
    UmiTraderGtkWorkstation *workstation,
    const char *component_id,
    const char *context_value)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL || workstation->application_surface == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    return umi_trader_application_surface_policy_context_changed(
        workstation->application_surface, component_id, context_value);
}

/*
 * Provide the trader gtk workstation checkpoint due operation used by this module and its
 * client applications.
 */
int umi_trader_gtk_workstation_checkpoint_due(
    const UmiTraderGtkWorkstation *workstation,
    uint32_t elapsed_since_checkpoint_seconds,
    int changed)
{
    return workstation != NULL && workstation->application_surface != NULL
        ? umi_trader_application_surface_policy_checkpoint_due(
              workstation->application_surface,
              elapsed_since_checkpoint_seconds, changed)
        : 0;
}
