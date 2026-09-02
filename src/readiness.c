/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: src/readiness.c
 *
 * PURPOSE:
 *   Project the canonical Framework feature backlog without product-local roadmap duplication.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include "umicom/trader/readiness.h"

#include "umicom/trader/runtime.h"
#include "umicom/application/experience_plan.h"

/*
 * Provide the trader readiness report operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_readiness_report(UmiApplicationReadinessReport *out_report)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_trader_runtime_experience();
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (experience == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_readiness_report(experience, out_report);
}

/*
 * Provide the trader readiness next feature operation used by this module and its client
 * applications.
 */
const UmiExperienceFeatureDefinition *umi_trader_readiness_next_feature(void)
{
    return umi_application_experience_next_feature(
        umi_trader_runtime_experience());
}
