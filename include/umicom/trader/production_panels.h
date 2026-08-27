/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: include/umicom/trader/production_panels.h
 *
 * PURPOSE:
 *   Project the Framework-owned application production control plane into this
 *   thin product without duplicating layout, readiness or acceptance logic.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_TRADER_PRODUCTION_PANELS_H
#define UMICOM_TRADER_PRODUCTION_PANELS_H
#include "umicom/trader/production_profile.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct UmiTraderProductionPanelContribution {
    const UmiApplicationProductionPanelBinding *binding;
    const char *workspace_area;
    int risk_sensitive;
    int read_only;
} UmiTraderProductionPanelContribution;
typedef struct UmiTraderProductionPanelCatalogue {
    UmiTraderProductionPanelContribution
        entries[UMI_APPLICATION_PRODUCTION_MAX_PANELS];
    size_t count;
    size_t risk_sensitive_count;
    size_t read_only_count;
} UmiTraderProductionPanelCatalogue;
UmiStatus umi_trader_production_panels_build(
    const UmiApplicationProductionRuntime *runtime,
    UmiTraderProductionPanelCatalogue *out_catalogue);
#ifdef __cplusplus
}
#endif
#endif

