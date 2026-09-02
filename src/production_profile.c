/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: src/production_profile.c
 *
 * PURPOSE:
 *   Project the Framework-owned application production control plane into this
 *   thin product without duplicating layout, readiness or acceptance logic.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/trader/production_profile.h"
#include <string.h>

/* Build a small Trader-specific view over the canonical production binding. */
UmiStatus umi_trader_production_profile_build(
    UmiTraderProductionProfile *out_profile)
{
    UmiStatus status;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (out_profile == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    (void)memset(out_profile, 0, sizeof(*out_profile));
    status = umi_application_production_binding_init(
        umi_trader_productisation_contribution(), &out_profile->binding);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) return status;
    out_profile->primary_workspace_id = "trader.execution";
    out_profile->risk_context_group_id = "trader.instrument";
    return UMI_STATUS_OK;
}

