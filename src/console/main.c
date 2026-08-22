/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: src/console/main.c
 *
 * PURPOSE:
 *   Provide a tiny native verification frontend for the Framework-composed
 *   Trader workbench.  No trading or execution logic lives in this executable.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/

#include <stdio.h>

#include "umicom/trader/workbench_profile.h"

int main(void)
{
    UmiTraderWorkbenchProfile *profile = NULL;
    UmiTraderWorkbenchProfileSnapshot snapshot;
    UmiStatus status;

    status = umi_trader_workbench_profile_create(&profile);
    if (status != UMI_STATUS_OK) {
        (void)fprintf(stderr,
                      "Unable to create Trader workbench profile: %d\n",
                      (int)status);
        return 1;
    }

    status = umi_trader_workbench_profile_snapshot(profile, &snapshot);
    if (status != UMI_STATUS_OK) {
        (void)fprintf(stderr,
                      "Unable to read Trader workbench profile: %d\n",
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

    umi_trader_workbench_profile_destroy(profile);
    return 0;
}
