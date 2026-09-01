/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: applications/trader/tests/test_gtk_workstation.c
 *
 * PURPOSE:
 *   Smoke-test the real Framework-rendered Trader GTK4 workstation and its
 *   canonical professional trading layout switching and panel customisation.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "umicom/trader/gtk_workstation.h"

int main(void)
{
    UmiTraderGtkWorkstation *workstation = NULL;
    UmiApplicationSuiteGtk4WorkstationSnapshot snapshot =
        umi_trader_gtk_workstation_snapshot(NULL);
    UmiTradingWorkspaceSnapshot trading;
    UmiUiWorkspaceImportReport import_report;
    char saved_layout[UMI_UI_LAYOUT_ENCODED_CAPACITY];
    UmiStatus status;

    assert(snapshot.application_id[0] == '\0');
    assert(snapshot.active_layout_id[0] == '\0');
    assert(snapshot.layout_count == 0U);

    if (!gtk_init_check()) {
        (void)printf("GTK4 display unavailable; Trader workstation smoke test skipped.\n");
        return 0;
    }

    status = umi_trader_gtk_workstation_create(&workstation);
    assert(status == UMI_STATUS_OK);
    assert(workstation != NULL);
    assert(umi_trader_gtk_workstation_widget(workstation) != NULL);

    assert(umi_trader_gtk_workstation_trading_snapshot(
               workstation, &trading) == UMI_STATUS_OK);
    assert(trading.environment == UMI_TRADING_SIMULATION);
    assert(trading.watchlist_count ==
           UMI_TRADING_SIMULATION_MARKET_DEFAULT_INSTRUMENTS);
    assert(trading.visible_instrument_count == trading.watchlist_count);
    assert(trading.market_data_ready);
    assert(trading.has_selected_instrument);

    snapshot = umi_trader_gtk_workstation_snapshot(workstation);
    assert(strcmp(snapshot.application_id, "org.umicom.trader") == 0);
    assert(strcmp(snapshot.active_layout_id, "trading") == 0);
    assert(snapshot.layout_count == 6U);
    assert(snapshot.rendered_panel_count == 8U);
    assert(snapshot.context_group_count >= 3U);
    assert(!snapshot.has_saved_layout);

    /* Trader delegates recovery and portable persistence to the Framework
     * workstation instead of owning a separate trading layout format. */
    assert(umi_trader_gtk_workstation_save_checkpoint(
               workstation, UINT64_C(2000)) == UMI_STATUS_OK);
    snapshot = umi_trader_gtk_workstation_snapshot(workstation);
    assert(snapshot.has_saved_layout);
    assert(snapshot.saved_layout_at_ns == UINT64_C(2000));
    assert(umi_trader_gtk_workstation_export_layout(
               workstation,
               UINT64_C(2100),
               saved_layout,
               sizeof(saved_layout)) == UMI_STATUS_OK);
    assert(strstr(saved_layout, "UMILAYOUT3") != NULL);

    /* Trader remains thin: panel movement, floating and context policy are
     * delegated to the shared Framework workstation. */
    assert(umi_trader_gtk_workstation_begin_layout_edit(workstation) ==
           UMI_STATUS_OK);
    {
        UmiUiWorkspacePanelSettings settings =
            umi_ui_workspace_panel_settings_default("watchlist");
        char second_chart[UMI_UI_WORKSPACE_LAYOUT_ID_CAPACITY];

        settings.placement_id = "right";
        settings.stack_id = "market-tools";
        settings.context_group_id = "trading.red";
        assert(umi_trader_gtk_workstation_apply_panel_settings(
                   workstation, &settings) == UMI_STATUS_OK);
        /* Trader requests another analytical panel through the thin wrapper;
         * Framework owns capability checks and collision-free instance IDs. */
        assert(umi_trader_gtk_workstation_open_window(
                   workstation,
                   "chart",
                   "centre",
                   0,
                   UINT64_C(1000),
                   second_chart,
                   sizeof(second_chart)) == UMI_STATUS_OK);
        assert(strcmp(second_chart, "chart-2") == 0);
    }
    assert(umi_trader_gtk_workstation_cancel_layout_edit(workstation) ==
           UMI_STATUS_OK);
    assert(umi_trader_gtk_workstation_import_layout(
               workstation,
               saved_layout,
               1,
               &import_report) == UMI_STATUS_OK);
    assert(import_report.replaced);
    assert(strcmp(import_report.layout_id, "trading") == 0);
    assert(umi_trader_gtk_workstation_restore_checkpoint(workstation) ==
           UMI_STATUS_OK);

    status = umi_trader_gtk_workstation_select_layout(workstation, "research");
    assert(status == UMI_STATUS_OK);
    snapshot = umi_trader_gtk_workstation_snapshot(workstation);
    assert(strcmp(snapshot.active_layout_id, "research") == 0);
    assert(snapshot.rendered_panel_count == 6U);
    /* Scanner, predictive research, news and context inspection now render
     * through Framework view models instead of placeholder panels. */
    assert(snapshot.placeholder_count == 0U);

    status = umi_trader_gtk_workstation_select_layout(
        workstation, "market-analysis");
    assert(status == UMI_STATUS_OK);
    snapshot = umi_trader_gtk_workstation_snapshot(workstation);
    assert(strcmp(snapshot.active_layout_id, "market-analysis") == 0);
    assert(snapshot.rendered_panel_count == 7U);
    assert(snapshot.placeholder_count == 0U);

    status = umi_trader_gtk_workstation_select_layout(
        workstation, "strategy-development");
    assert(status == UMI_STATUS_OK);
    snapshot = umi_trader_gtk_workstation_snapshot(workstation);
    assert(strcmp(snapshot.active_layout_id, "strategy-development") == 0);
    assert(snapshot.rendered_panel_count == 6U);
    assert(snapshot.placeholder_count == 0U);

    umi_trader_gtk_workstation_destroy(workstation);
    return 0;
}
