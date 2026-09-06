/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: applications/trader/src/console/main.c
 *
 * PURPOSE:
 *   Provide a native verification frontend for the Framework-composed Trader workbench and running trading layout.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include <stdio.h>

#include "umicom/application/runtime/readiness.h"
#include "umicom/application/experience_catalogue.h"
#include "umicom/application/runtime/command_surface.h"
#include "umicom/trader/runtime.h"
#include "umicom/trader/workbench_profile.h"

/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
int main(void)
{
    UmiTraderWorkbenchProfile *profile = NULL;
    UmiTraderWorkbenchProfileSnapshot snapshot;
    UmiUiWorkspaceLayout layout;
    UmiApplicationLaunchReadiness launch_readiness;
    UmiApplicationLaunchReadinessSummary portfolio_readiness;
    UmiApplicationCommandSurface command_surface;
    UmiApplicationCommandQuery command_query = {0};
    UmiApplicationCommandQueryResult command_result;
    UmiStatus status;

    status = umi_trader_workbench_profile_create(&profile);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        (void)fprintf(stderr,
                      "Unable to create Trader workbench profile: %d\n",
                      (int)status);
        return 1;
    }

    status = umi_trader_workbench_profile_snapshot(profile, &snapshot);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        (void)fprintf(stderr,
                      "Unable to read Trader workbench profile: %d\n",
                      (int)status);
        umi_trader_workbench_profile_destroy(profile);
        return 1;
    }

    status = umi_trader_runtime_layout_default(&layout);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        (void)fprintf(stderr,
                      "Unable to materialise Trader layout: %d\n",
                      (int)status);
        umi_trader_workbench_profile_destroy(profile);
        return 1;
    }

    (void)printf("Umicom Trader Module\n");
    (void)printf("Profile: %s\n", snapshot.profile_id);
    (void)printf("Groups: %zu\n", snapshot.group_count);
    (void)printf("Endpoints: %zu\n", snapshot.endpoint_count);
    (void)printf("Trading group: %s\n", snapshot.trading_group_id);
    (void)printf("Operations group: %s\n", snapshot.operations_group_id);
    (void)printf("Layout: %s\n", layout.name);
    (void)printf("Layout windows: %zu\n", layout.window_count);

    /* Report the same Framework launch gate used by Umicom Desk. */
    status = umi_application_launch_readiness_check(
        "org.umicom.trader", &launch_readiness);
    if (status == UMI_STATUS_OK) {
        (void)printf("Launch readiness: %s (%u%%) - %s\n",
                     umi_application_launch_readiness_state_text(
                         launch_readiness.state),
                     launch_readiness.feature_readiness_percent,
                     launch_readiness.reason);
    }
    /* Report the same portfolio health summary that Umicom Desk uses for product selection. */
    status = umi_application_launch_readiness_summary(&portfolio_readiness);
    if (status == UMI_STATUS_OK) {
        (void)printf("Portfolio readiness: %zu/%zu launchable, %zu blocked, %u%% average\n",
                     portfolio_readiness.ready_count,
                     portfolio_readiness.application_count,
                     portfolio_readiness.blocked_count,
                     portfolio_readiness.average_feature_readiness_percent);
    }
    /* Use the shared command palette query to count searchable Trader layout commands. */
    command_query.structure_size = sizeof(command_query);
    command_query.text = "layout";
    if (umi_application_command_surface_build(
            umi_application_experience_catalogue_find("org.umicom.trader"),
            &command_surface) == UMI_STATUS_OK &&
        umi_application_command_surface_query(
            &command_surface, &command_query, &command_result) == UMI_STATUS_OK) {
        (void)printf("Palette layout commands: %zu\n", command_result.match_count);
    }

    umi_trader_workbench_profile_destroy(profile);
    return 0;
}
