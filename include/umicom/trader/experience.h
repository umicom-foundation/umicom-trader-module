/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: include/umicom/trader/experience.h
 *
 * PURPOSE:
 *   Expose the product's canonical Framework-owned application experience.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_TRADER_EXPERIENCE_H
#define UMICOM_TRADER_EXPERIENCE_H

#include "umicom/application/experience.h"
#include "umicom/application/experience_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Provide the trader experience operation used by this module and its client applications.
 */
const UmiApplicationExperienceDefinition *umi_trader_experience(void);
/**
 * Provide the trader experience status operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_experience_status(
    UmiApplicationExperienceStatus *out_status);

#ifdef __cplusplus
}
#endif

#endif
