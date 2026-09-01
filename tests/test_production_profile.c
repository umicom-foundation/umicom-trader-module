/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: tests/test_production_profile.c
 *
 * PURPOSE:
 *   Verify the test production profile behavior for
 *   Umicom Trader Module.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
/* Umicom Trader production control-plane test | Sammy Hegab | Umicom Foundation | MIT */
#include "umicom/test_runtime/check.h"
#include "umicom/trader/production_profile.h"
int main(void) {
    UmiTraderProductionProfile profile;
    UMI_TEST_REQUIRE(umi_trader_production_profile_build(&profile) == UMI_STATUS_OK);
    UMI_TEST_REQUIRE(profile.binding.experience != NULL);
    UMI_TEST_REQUIRE(profile.binding.adoption_snapshot.acceptance_ready);
    return 0;
}

