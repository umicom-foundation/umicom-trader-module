/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: include/umicom/trader/experience_acceptance.h
 *
 * PURPOSE:
 *   Project Framework-owned executable journey evidence into this thin product.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#ifndef TRADER_EXPERIENCE_ACCEPTANCE_H
#define TRADER_EXPERIENCE_ACCEPTANCE_H
#include "umicom/application/journey/journey_platform.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct UmiTraderExperienceAcceptance {
    UmiApplicationJourneyCatalogue catalogue;
    UmiApplicationJourneyAudit audit;
    UmiApplicationJourneyReport report;
    int product_ready;
} UmiTraderExperienceAcceptance;
UmiStatus umi_trader_experience_acceptance_build(
    const UmiApplicationJourneyEvidenceRegistry *evidence,
    UmiTraderExperienceAcceptance *out_acceptance);
#ifdef __cplusplus
}
#endif
#endif

