/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: src/experience.c
 *
 * PURPOSE:
 *   Bind the existing workbench composition to the canonical Framework application experience.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include "umicom/trader/experience.h"

#include "umicom/application/experience_catalogue.h"

/* Provide the trader experience operation used by this module and its client applications. */
const UmiApplicationExperienceDefinition *umi_trader_experience(void)
{
    return umi_application_experience_catalogue_find("org.umicom.trader");
}

/*
 * Provide the trader experience status operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_experience_status(
    UmiApplicationExperienceStatus *out_status)
{
    const UmiApplicationExperienceDefinition *definition =
        umi_trader_experience();
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (definition == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_experience_status(definition, out_status);
}
