/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: include/umicom/trader/productisation_contribution.h
 *
 * PURPOSE:
 *   Declare this thin module's adoption of Framework-owned application
 *   experience, components, layouts and productisation evidence.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_TRADER_PRODUCTISATION_CONTRIBUTION_H
#define UMICOM_TRADER_PRODUCTISATION_CONTRIBUTION_H

#include "umicom/application/productisation/session.h"

#ifdef __cplusplus
extern "C" {
#endif

const UmiProductApplicationAdoption *
umi_trader_productisation_contribution(void);
UmiStatus umi_trader_productisation_snapshot(
    UmiProductApplicationAdoptionSnapshot *out_snapshot);
UmiStatus umi_trader_product_session_init(
    UmiProductApplicationSession *out_session);

#ifdef __cplusplus
}
#endif

#endif
