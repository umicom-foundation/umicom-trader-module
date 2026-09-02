/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: tests/test_test_readiness_contribution.c
 * PURPOSE: Verify Trader uses the all-module Framework validation profile.
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include <assert.h>
#include <string.h>

#include "umicom/trader/test_readiness_contribution.h"

/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
int main(void)
{
    const UmiTraderTestReadinessContribution *item =
        umi_trader_test_readiness_contribution();
    assert(item != NULL);
    assert(strcmp(item->product_id, "trader") == 0);
    assert(strcmp(item->profile->preset, "windows-ucrt64-all-debug") == 0);
    assert(item->profile->requires_all_modules);
    return 0;
}
