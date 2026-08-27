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
#include <string.h>

#include "umicom/ui/gtk4/workstation/view_model_panel.h"
#include "umicom/ui/gtk4/workstation/workspace_layout_host.h"

struct UmiTraderGtkWorkstation {
    UmiTradingWorkspace *trading;
	UmiGtk4TradingSuiteWorkstation *framework_workstation;
    UmiApplicationSuiteGtk4Workstation *suite;
    guint pending_refresh;
};

static gboolean refresh_layout_idle(gpointer data)
{
    UmiTraderGtkWorkstation *workstation =
        (UmiTraderGtkWorkstation *)data;
    UmiApplicationSuiteGtk4WorkstationSnapshot snapshot;
    if (workstation == NULL || workstation->suite == NULL) return G_SOURCE_REMOVE;
    workstation->pending_refresh = 0U;
    snapshot = umi_application_suite_gtk4_workstation_snapshot(
        workstation->suite);
    if (snapshot.active_layout_id[0] != '\0')
        (void)umi_application_suite_gtk4_workstation_select_layout(
            workstation->suite, snapshot.active_layout_id);
    return G_SOURCE_REMOVE;
}

static void schedule_refresh(UmiTraderGtkWorkstation *workstation)
{
    if (workstation == NULL || workstation->pending_refresh != 0U) return;
    workstation->pending_refresh = g_idle_add(refresh_layout_idle, workstation);
}

static UmiStatus trader_action_handler(const char *action_id, void *user_data)
{
    UmiTraderGtkWorkstation *workstation =
        (UmiTraderGtkWorkstation *)user_data;
    UmiStatus status;
    if (workstation == NULL || workstation->trading == NULL || action_id == NULL)
        return UMI_STATUS_INVALID_ARGUMENT;

    if (strcmp(action_id, "studio.action.trading.refresh") == 0) {
        status = umi_trading_workspace_refresh(workstation->trading);
    } else if (strcmp(action_id,
                      "studio.action.trading.engage-kill-switch") == 0) {
        umi_trading_workspace_engage_kill_switch(
            workstation->trading, "Requested from Umicom Trader workstation");
        status = UMI_STATUS_OK;
    } else if (strcmp(action_id,
                      "studio.action.trading.reset-kill-switch") == 0) {
        umi_trading_workspace_reset_kill_switch(workstation->trading);
        status = UMI_STATUS_OK;
    } else {
        /* Environment changes, instrument selection and any order-affecting
         * operations stay behind their established guarded product workflows.
         * This first native workstation must not invent bypass controls. */
        return UMI_STATUS_NOT_IMPLEMENTED;
    }
    if (status == UMI_STATUS_OK) schedule_refresh(workstation);
    return status;
}

static GtkWidget *trader_panel_factory(
    const UmiUiWorkspaceWindow *window,
    void *user_data)
{
    UmiTraderGtkWorkstation *workstation =
        (UmiTraderGtkWorkstation *)user_data;
    UmiUiViewModel *view = NULL;
    GtkWidget *widget;
    UmiStatus status;
    if (window == NULL || workstation == NULL || workstation->trading == NULL)
        return NULL;

    status = umi_trader_runtime_create_panel_view(
        window->tool_id, workstation->trading, &view);
    if (status == UMI_STATUS_NOT_IMPLEMENTED) {
        /* Returning NULL delegates missing/planned panel presentation to the
         * canonical Framework layout host, which also records placeholder
         * evidence in its snapshot. */
        return NULL;
    }
    if (status != UMI_STATUS_OK || view == NULL) return NULL;
    widget = umi_gtk4_view_model_panel_create(
        view, trader_action_handler, workstation);
    umi_ui_view_model_destroy(view);
    return widget;
}

UmiStatus umi_trader_gtk_workstation_create(
    UmiTraderGtkWorkstation **out_workstation)
{
    UmiTraderGtkWorkstation *workstation;
    UmiTradingWorkspaceConfig trading_config;
    UmiApplicationSuiteGtk4WorkstationConfig suite_config;
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

    /* Framework defaults intentionally provide a populated simulation market,
     * animated evidence and no live-environment access. Broker readiness and
     * explicit live arming remain separate capabilities for a later adapter. */

    (void)memset(&suite_config, 0, sizeof(suite_config));
    suite_config.application_id = "org.umicom.trader";
    suite_config.title = "Umicom Trader";
    suite_config.panel_factory = trader_panel_factory;
    suite_config.user_data = workstation;
    status = umi_application_suite_gtk4_workstation_create(
        &suite_config, &workstation->suite);
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
    if (workstation->pending_refresh != 0U) {
        g_source_remove(workstation->pending_refresh);
        workstation->pending_refresh = 0U;
    }
    umi_application_suite_gtk4_workstation_destroy(workstation->suite);
    workstation->suite = NULL;
    umi_trading_workspace_destroy(workstation->trading);
    workstation->trading = NULL;
    free(workstation);
}

GtkWidget *umi_trader_gtk_workstation_widget(
    UmiTraderGtkWorkstation *workstation)
{
    return workstation != NULL
        ? umi_application_suite_gtk4_workstation_widget(workstation->suite)
        : NULL;
}

UmiStatus umi_trader_gtk_workstation_select_layout(
    UmiTraderGtkWorkstation *workstation,
    const char *layout_id)
{
    if (workstation == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    return umi_application_suite_gtk4_workstation_select_layout(
        workstation->suite, layout_id);
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
