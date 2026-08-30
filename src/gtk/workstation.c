/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: applications/trader/src/gtk/workstation.c
 *
 * PURPOSE:
 *   Keep Trader a thin product composition: create the canonical trading
 *   workspace and delegate all GTK4 interaction, simulation, panel rendering
 *   and suite-layout behaviour to Umicom Framework.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/
#include "umicom/trader/gtk_workstation.h"

#include <stdlib.h>

struct UmiTraderGtkWorkstation {
    UmiTradingWorkspace *trading;
    UmiGtk4TradingSuiteWorkstation *framework_workstation;
    UmiTraderApplicationSurface *application_surface;
};

UmiStatus umi_trader_gtk_workstation_create(
    UmiTraderGtkWorkstation **out_workstation)
{
    UmiTraderGtkWorkstation *workstation;
    UmiTradingWorkspaceConfig trading_config;
    UmiGtk4TradingSuiteWorkstationConfig framework_config;
    UmiStatus status;
    if (out_workstation == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    *out_workstation = NULL;
    workstation = (UmiTraderGtkWorkstation *)calloc(1U, sizeof(*workstation));
    if (workstation == NULL) return UMI_STATUS_OUT_OF_MEMORY;

    /* Framework defaults are simulation-only, with no broker transport and no
     * live arming. The GUI therefore starts in a safe runnable state. */
    trading_config = umi_trading_workspace_config_default();
    status = umi_trading_workspace_create(
        &trading_config, &workstation->trading);
    if (status != UMI_STATUS_OK) goto fail;

    /* The Framework-owned trading suite seeds the safe simulation market,
     * supplies guarded actions and renders the canonical product layouts. */
    framework_config = umi_gtk4_trading_suite_workstation_config_default(
        workstation->trading);
    status = umi_gtk4_trading_suite_workstation_create(
        &framework_config, &workstation->framework_workstation);
    if (status != UMI_STATUS_OK) goto fail;

    /* Keep the canonical component/presentation session beside the existing
     * Framework GTK4 trading workstation. This is additive: native trading
     * widgets and simulation state remain owned by their established paths. */
    status = umi_trader_application_surface_create(
        &workstation->application_surface);
    if (status != UMI_STATUS_OK) goto fail;

    *out_workstation = workstation;
    return UMI_STATUS_OK;

fail:
    umi_trader_gtk_workstation_destroy(workstation);
    return status;
}

void umi_trader_gtk_workstation_destroy(
    UmiTraderGtkWorkstation *workstation)
{
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

GtkWidget *umi_trader_gtk_workstation_widget(
    UmiTraderGtkWorkstation *workstation)
{
    return workstation != NULL
        ? umi_gtk4_trading_suite_workstation_widget(
            workstation->framework_workstation)
        : NULL;
}

UmiStatus umi_trader_gtk_workstation_select_layout(
    UmiTraderGtkWorkstation *workstation,
    const char *layout_id)
{
    if (workstation == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    return umi_gtk4_trading_suite_workstation_select_layout(
        workstation->framework_workstation, layout_id);
}

UmiApplicationSuiteGtk4WorkstationSnapshot
umi_trader_gtk_workstation_snapshot(
    const UmiTraderGtkWorkstation *workstation)
{
    UmiGtk4TradingSuiteWorkstationSnapshot snapshot;
    if (workstation == NULL || workstation->framework_workstation == NULL)
        return (UmiApplicationSuiteGtk4WorkstationSnapshot){0};
    snapshot = umi_gtk4_trading_suite_workstation_snapshot(
        workstation->framework_workstation);
    return snapshot.layout;
}

UmiStatus umi_trader_gtk_workstation_trading_snapshot(
    UmiTraderGtkWorkstation *workstation,
    UmiTradingWorkspaceSnapshot *out_snapshot)
{
    if (workstation == NULL || workstation->trading == NULL ||
        out_snapshot == NULL)
        return UMI_STATUS_INVALID_ARGUMENT;
    return umi_trading_workspace_snapshot(workstation->trading, out_snapshot);
}

UmiStatus umi_trader_gtk_workstation_application_surface_snapshot(
    const UmiTraderGtkWorkstation *workstation,
    UmiApplicationPresentationSurfaceSnapshot *out_snapshot)
{
    if (workstation == NULL || workstation->application_surface == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    return umi_trader_application_surface_snapshot(
        workstation->application_surface, out_snapshot);
}

UmiStatus umi_trader_gtk_workstation_advance(
    UmiTraderGtkWorkstation *workstation,
    uint32_t elapsed_seconds)
{
    if (workstation == NULL || workstation->application_surface == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    return umi_trader_application_surface_policy_advance(
        workstation->application_surface, elapsed_seconds);
}

UmiStatus umi_trader_gtk_workstation_set_background(
    UmiTraderGtkWorkstation *workstation,
    int background)
{
    if (workstation == NULL || workstation->application_surface == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    return umi_trader_application_surface_policy_set_background(
        workstation->application_surface, background);
}

UmiStatus umi_trader_gtk_workstation_context_changed(
    UmiTraderGtkWorkstation *workstation,
    const char *component_id,
    const char *context_value)
{
    if (workstation == NULL || workstation->application_surface == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    return umi_trader_application_surface_policy_context_changed(
        workstation->application_surface, component_id, context_value);
}

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
