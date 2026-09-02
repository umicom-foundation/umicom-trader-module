/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: tests/test_application_surface_policy.c
 *
 * PURPOSE:
 *   Verify Trader adopts full-rate market refresh, guarded order commands,
 *   shared instrument context and frequent workspace checkpoints safely.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include <assert.h>
#include <string.h>

#include "umicom/trader/application_surface_policy.h"

/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
int main(void)
{
    UmiTraderApplicationSurface *surface = NULL;
    UmiTraderApplicationSurfacePolicySnapshot policy;
    assert(umi_trader_application_surface_create(&surface) == UMI_STATUS_OK);
    assert(umi_trader_application_surface_policy_snapshot(surface, &policy) ==
           UMI_STATUS_OK);
    assert(strcmp(policy.recipe_id, UMI_TRADER_STANDARD_RECIPE_ID) == 0);
    assert(strcmp(policy.checkpoint_policy, "periodic") == 0);
    assert(strcmp(policy.background_policy, "full") == 0);
    assert(policy.checkpoint_interval_seconds == 15U);
    assert(policy.streaming_panels > 0U);
    assert(policy.guarded_command_panels > 0U);
    assert(policy.share_context);
    assert(policy.allow_background_commands);
    assert(umi_trader_application_surface_policy_context_changed(
               surface, "umicom.trading.chart", "instrument:GBPUSD") ==
           UMI_STATUS_OK);
    assert(umi_trader_application_surface_policy_set_background(surface, 1) ==
           UMI_STATUS_OK);
    assert(umi_trader_application_surface_policy_advance(surface, 1U) ==
           UMI_STATUS_OK);
    assert(umi_trader_application_surface_command(
               surface, "umicom.trading.order-ticket", "order.prepare") ==
           UMI_STATUS_OK);
    assert(umi_trader_application_surface_policy_checkpoint_due(
        surface, 15U, 0));
    umi_trader_application_surface_destroy(surface);
    return 0;
}
