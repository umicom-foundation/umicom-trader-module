/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: include/umicom/trader/test_readiness_contribution.h
 * PURPOSE: Place Framework test-readiness commands and panel inside Trader.
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_TRADER_TEST_READINESS_CONTRIBUTION_H
#define UMICOM_TRADER_TEST_READINESS_CONTRIBUTION_H

#include <stddef.h>

#include "umicom/test_platform/build_readiness/build_readiness.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UmiTraderTestReadinessContribution {
    const char *product_id;
    const char *view_id;
    const char *region;
    const UmiTestPlatformProductValidationProfile *profile;
    size_t command_count;
} UmiTraderTestReadinessContribution;

const UmiTraderTestReadinessContribution *
umi_trader_test_readiness_contribution(void);

#ifdef __cplusplus
}
#endif
#endif
