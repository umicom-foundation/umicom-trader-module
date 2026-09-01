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
#include "umicom/test_runtime/check.h"
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

    UMI_TEST_REQUIRE(snapshot.application_id[0] == '\0');
    UMI_TEST_REQUIRE(snapshot.active_layout_id[0] == '\0');
    UMI_TEST_REQUIRE(snapshot.layout_count == 0U);

    if (!gtk_init_check()) {
        (void)printf("GTK4 display unavailable; Trader workstation smoke test skipped.\n");
        return 0;
    }

    status = umi_trader_gtk_workstation_create(&workstation);
    UMI_TEST_REQUIRE(status == UMI_STATUS_OK);
    UMI_TEST_REQUIRE(workstation != NULL);
    UMI_TEST_REQUIRE(umi_trader_gtk_workstation_widget(workstation) != NULL);

    UMI_TEST_REQUIRE(umi_trader_gtk_workstation_trading_snapshot(
               workstation, &trading) == UMI_STATUS_OK);
    UMI_TEST_REQUIRE(trading.environment == UMI_TRADING_SIMULATION);
    UMI_TEST_REQUIRE(trading.watchlist_count ==
           UMI_TRADING_SIMULATION_MARKET_DEFAULT_INSTRUMENTS);
    UMI_TEST_REQUIRE(trading.visible_instrument_count == trading.watchlist_count);
    UMI_TEST_REQUIRE(trading.market_data_ready);
    UMI_TEST_REQUIRE(trading.has_selected_instrument);

    snapshot = umi_trader_gtk_workstation_snapshot(workstation);
    UMI_TEST_REQUIRE(strcmp(snapshot.application_id, "org.umicom.trader") == 0);
    UMI_TEST_REQUIRE(strcmp(snapshot.active_layout_id, "trading") == 0);
    UMI_TEST_REQUIRE(snapshot.layout_count == 6U);
    UMI_TEST_REQUIRE(snapshot.rendered_panel_count == 8U);
    UMI_TEST_REQUIRE(snapshot.context_group_count >= 3U);
    UMI_TEST_REQUIRE(!snapshot.has_saved_layout);

    /* Trader delegates recovery and portable persistence to the Framework
     * workstation instead of owning a separate trading layout format. */
    UMI_TEST_REQUIRE(umi_trader_gtk_workstation_save_checkpoint(
               workstation, UINT64_C(2000)) == UMI_STATUS_OK);
    snapshot = umi_trader_gtk_workstation_snapshot(workstation);
    UMI_TEST_REQUIRE(snapshot.has_saved_layout);
    UMI_TEST_REQUIRE(snapshot.saved_layout_at_ns == UINT64_C(2000));
    UMI_TEST_REQUIRE(umi_trader_gtk_workstation_export_layout(
               workstation,
               UINT64_C(2100),
               saved_layout,
               sizeof(saved_layout)) == UMI_STATUS_OK);
    UMI_TEST_REQUIRE(strstr(saved_layout, "UMILAYOUT3") != NULL);

    /* Trader remains thin: panel movement, floating and context policy are
     * delegated to the shared Framework workstation. */
    UMI_TEST_REQUIRE(umi_trader_gtk_workstation_begin_layout_edit(workstation) ==
           UMI_STATUS_OK);
    {
        UmiUiWorkspacePanelSettings settings =
            umi_ui_workspace_panel_settings_default("watchlist");
        char second_chart[UMI_UI_WORKSPACE_LAYOUT_ID_CAPACITY];

        settings.placement_id = "right";
        settings.stack_id = "market-tools";
        settings.context_group_id = "trading.red";
        UMI_TEST_REQUIRE(umi_trader_gtk_workstation_apply_panel_settings(
                   workstation, &settings) == UMI_STATUS_OK);
        /* Trader requests another analytical panel through the thin wrapper;
         * Framework owns capability checks and collision-free instance IDs. */
        UMI_TEST_REQUIRE(umi_trader_gtk_workstation_open_window(
                   workstation,
                   "chart",
                   "centre",
                   0,
                   UINT64_C(1000),
                   second_chart,
                   sizeof(second_chart)) == UMI_STATUS_OK);
        UMI_TEST_REQUIRE(strcmp(second_chart, "chart-2") == 0);
    }
    UMI_TEST_REQUIRE(umi_trader_gtk_workstation_cancel_layout_edit(workstation) ==
           UMI_STATUS_OK);
    UMI_TEST_REQUIRE(umi_trader_gtk_workstation_import_layout(
               workstation,
               saved_layout,
               1,
               &import_report) == UMI_STATUS_OK);
    UMI_TEST_REQUIRE(import_report.replaced);
    UMI_TEST_REQUIRE(strcmp(import_report.layout_id, "trading") == 0);
    UMI_TEST_REQUIRE(umi_trader_gtk_workstation_restore_checkpoint(workstation) ==
           UMI_STATUS_OK);

    status = umi_trader_gtk_workstation_select_layout(workstation, "research");
    UMI_TEST_REQUIRE(status == UMI_STATUS_OK);
    snapshot = umi_trader_gtk_workstation_snapshot(workstation);
    UMI_TEST_REQUIRE(strcmp(snapshot.active_layout_id, "research") == 0);
    UMI_TEST_REQUIRE(snapshot.rendered_panel_count == 6U);
    /* Scanner, predictive research, news and context inspection now render
     * through Framework view models instead of placeholder panels. */
    UMI_TEST_REQUIRE(snapshot.placeholder_count == 0U);

    status = umi_trader_gtk_workstation_select_layout(
        workstation, "market-analysis");
    UMI_TEST_REQUIRE(status == UMI_STATUS_OK);
    snapshot = umi_trader_gtk_workstation_snapshot(workstation);
    UMI_TEST_REQUIRE(strcmp(snapshot.active_layout_id, "market-analysis") == 0);
    UMI_TEST_REQUIRE(snapshot.rendered_panel_count == 7U);
    UMI_TEST_REQUIRE(snapshot.placeholder_count == 0U);

    status = umi_trader_gtk_workstation_select_layout(
        workstation, "strategy-development");
    UMI_TEST_REQUIRE(status == UMI_STATUS_OK);
    snapshot = umi_trader_gtk_workstation_snapshot(workstation);
    UMI_TEST_REQUIRE(strcmp(snapshot.active_layout_id, "strategy-development") == 0);
    UMI_TEST_REQUIRE(snapshot.rendered_panel_count == 6U);
    UMI_TEST_REQUIRE(snapshot.placeholder_count == 0U);

    umi_trader_gtk_workstation_destroy(workstation);
    return 0;
}
