/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: src/test_readiness_contribution.c
 * PURPOSE: Implement thin placement over Framework readiness contracts.
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/
#include "umicom/trader/test_readiness_contribution.h"

const UmiTraderTestReadinessContribution *
umi_trader_test_readiness_contribution(void)
{
    static UmiTraderTestReadinessContribution contribution;
    contribution.product_id = "trader";
    contribution.view_id = "trader.testing.build-readiness";
    contribution.region = "bottom-panel";
    contribution.profile =
        umi_test_platform_product_validation_profile_find("trader");
    contribution.command_count =
        umi_test_platform_build_readiness_command_count();
    return contribution.profile != NULL ? &contribution : NULL;
}
