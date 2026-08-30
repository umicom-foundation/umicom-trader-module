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
#include <assert.h>
#include "umicom/trader/production_profile.h"
int main(void) {
    UmiTraderProductionProfile profile;
    assert(umi_trader_production_profile_build(&profile) == UMI_STATUS_OK);
    assert(profile.binding.experience != NULL);
    assert(profile.binding.adoption_snapshot.acceptance_ready);
    return 0;
}

