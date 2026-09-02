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

    umi_trader_workbench_profile_destroy(profile);
    return 0;
}
