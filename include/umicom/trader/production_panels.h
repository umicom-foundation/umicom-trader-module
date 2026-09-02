/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: include/umicom/trader/production_panels.h
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
#ifndef UMICOM_TRADER_PRODUCTION_PANELS_H
#define UMICOM_TRADER_PRODUCTION_PANELS_H
#include "umicom/trader/production_profile.h"
#ifdef __cplusplus
extern "C" {
#endif
/**
 * Represent the trader production panel contribution data shared with callers of this
 * public contract.
 */
typedef struct UmiTraderProductionPanelContribution {
    const UmiApplicationProductionPanelBinding *binding;
    const char *workspace_area;
    int risk_sensitive;
    int read_only;
} UmiTraderProductionPanelContribution;
/**
 * Represent the trader production panel catalogue data shared with callers of this public
 * contract.
 */
typedef struct UmiTraderProductionPanelCatalogue {
    UmiTraderProductionPanelContribution
        entries[UMI_APPLICATION_PRODUCTION_MAX_PANELS];
    size_t count;
    size_t risk_sensitive_count;
    size_t read_only_count;
} UmiTraderProductionPanelCatalogue;
/** Project runtime panel bindings with trading risk and read-only metadata. */
UmiStatus umi_trader_production_panels_build(
    const UmiApplicationProductionRuntime *runtime,
    UmiTraderProductionPanelCatalogue *out_catalogue);
#ifdef __cplusplus
}
#endif
#endif

