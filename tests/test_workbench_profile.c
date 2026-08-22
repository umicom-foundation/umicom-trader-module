/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: tests/test_workbench_profile.c
 *
 * PURPOSE:
 *   Verify Trader consumes the Framework-owned linked-workbench profile without
 *   duplicating panel or colour-group definitions in the application module.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/

#include <assert.h>
#include <string.h>

#include "umicom/trader/workbench_profile.h"

int main(void)
{
    UmiTraderWorkbenchProfile *profile = NULL;
    UmiTraderWorkbenchProfileSnapshot snapshot;
    const UmiWorkbenchContextHostProfile *host_profile;
    const UmiWorkbenchContextSourceTradingProfile *sources;

    assert(umi_trader_workbench_profile_create(&profile) == UMI_STATUS_OK);
    assert(profile != NULL);
    assert(umi_trader_workbench_profile_snapshot(
               profile, &snapshot) == UMI_STATUS_OK);

    assert(strcmp(snapshot.application_id, "org.umicom.trader") == 0);
    assert(strcmp(snapshot.profile_id, "trader.linked-workbench") == 0);
    assert(strcmp(snapshot.trading_group_id, "trading.red") == 0);
    assert(strcmp(snapshot.operations_group_id, "operations.green") == 0);
    assert(snapshot.group_count == 2U);
    assert(snapshot.endpoint_count == 7U);

    host_profile = umi_trader_workbench_profile_context_host(profile);
    sources = umi_trader_workbench_profile_context_sources(profile);
    assert(host_profile != NULL);
    assert(sources != NULL);
    assert(umi_workbench_context_host_profile_validate(
               host_profile) == UMI_STATUS_OK);
    assert(strcmp(sources->trader_application_id,
                  "org.umicom.trader") == 0);

    umi_trader_workbench_profile_destroy(profile);
    return 0;
}
