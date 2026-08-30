/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: src/productisation_contribution.c
 *
 * PURPOSE:
 *   Bind product identity and executable evidence to canonical Framework
 *   application definitions without copying shared implementation.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/trader/productisation_contribution.h"

static const UmiProductApplicationAdoption ADOPTION = {
    sizeof(UmiProductApplicationAdoption),
    "trader",
    "org.umicom.trader",
    "Umicom Trader",
    "umicom-trader-console",
    UMI_PRODUCT_FRONTEND_FLAG_CONSOLE,
    1,
    1,
    1,
    1
};

const UmiProductApplicationAdoption *
umi_trader_productisation_contribution(void)
{
    return &ADOPTION;
}

UmiStatus umi_trader_productisation_snapshot(
    UmiProductApplicationAdoptionSnapshot *out_snapshot)
{
    return umi_product_application_adoption_snapshot(
        &ADOPTION, out_snapshot);
}
