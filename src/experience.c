/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: src/experience.c
 *
 * PURPOSE:
 *   Bind the existing workbench composition to the canonical Framework application experience.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/trader/experience.h"

#include "umicom/application/experience_catalogue.h"

const UmiApplicationExperienceDefinition *umi_trader_experience(void)
{
    return umi_application_experience_catalogue_find("org.umicom.trader");
}

UmiStatus umi_trader_experience_status(
    UmiApplicationExperienceStatus *out_status)
{
    const UmiApplicationExperienceDefinition *definition =
        umi_trader_experience();
    if (definition == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_experience_status(definition, out_status);
}
