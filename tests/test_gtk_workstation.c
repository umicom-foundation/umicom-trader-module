/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: applications/trader/tests/test_gtk_workstation.c
 *
 * PURPOSE:
 *   Smoke-test the real Framework-rendered Trader GTK4 workstation and its
 *   canonical Trading, Research and Strategy Development layout switching.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "umicom/trader/gtk_workstation.h"

int main(void)
{
    UmiTraderGtkWorkstation *workstation = NULL;
    UmiApplicationSuiteGtk4WorkstationSnapshot snapshot;
    UmiStatus status;

    if (!gtk_init_check()) {
        (void)printf("GTK4 display unavailable; Trader workstation smoke test skipped.\n");
        return 0;
    }

    status = umi_trader_gtk_workstation_create(&workstation);
    assert(status == UMI_STATUS_OK);
    assert(workstation != NULL);
    assert(umi_trader_gtk_workstation_widget(workstation) != NULL);

    snapshot = umi_trader_gtk_workstation_snapshot(workstation);
    assert(strcmp(snapshot.application_id, "org.umicom.trader") == 0);
    assert(strcmp(snapshot.active_layout_id, "trading") == 0);
    assert(snapshot.layout_count == 3U);
    assert(snapshot.rendered_panel_count == 8U);

    status = umi_trader_gtk_workstation_select_layout(workstation, "research");
    assert(status == UMI_STATUS_OK);
    snapshot = umi_trader_gtk_workstation_snapshot(workstation);
    assert(strcmp(snapshot.active_layout_id, "research") == 0);
    assert(snapshot.rendered_panel_count == 6U);
    assert(snapshot.placeholder_count > 0U);

    status = umi_trader_gtk_workstation_select_layout(
        workstation, "strategy-development");
    assert(status == UMI_STATUS_OK);
    snapshot = umi_trader_gtk_workstation_snapshot(workstation);
    assert(strcmp(snapshot.active_layout_id, "strategy-development") == 0);
    assert(snapshot.rendered_panel_count == 6U);

    umi_trader_gtk_workstation_destroy(workstation);
    return 0;
}
