/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: include/umicom/trader/production_profile.h
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
#ifndef UMICOM_TRADER_PRODUCTION_PROFILE_H
#define UMICOM_TRADER_PRODUCTION_PROFILE_H
#include "umicom/application/production/production.h"
#include "umicom/trader/productisation_contribution.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct UmiTraderProductionProfile {
    UmiApplicationProductionBinding binding;
    const char *primary_workspace_id;
    const char *risk_context_group_id;
} UmiTraderProductionProfile;
UmiStatus umi_trader_production_profile_build(
    UmiTraderProductionProfile *out_profile);
#ifdef __cplusplus
}
#endif
#endif

