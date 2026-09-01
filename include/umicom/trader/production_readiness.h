/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: include/umicom/trader/production_readiness.h
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
#ifndef UMICOM_TRADER_PRODUCTION_READINESS_H
#define UMICOM_TRADER_PRODUCTION_READINESS_H
#include "umicom/trader/production_profile.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct UmiTraderProductionReadiness {
    UmiApplicationProductionState state;
    int market_data_ready;
    int oms_ready;
    int risk_ready;
    int paper_adapter_ready;
    int paper_execution_ready;
    int live_execution_verified;
    int live_execution_ready;
    size_t open_adapter_feature_count;
    unsigned readiness_percent;
} UmiTraderProductionReadiness;
/** Summarise market, order, risk and execution readiness from Framework. */
UmiStatus umi_trader_production_readiness_build(
    const UmiApplicationProductionRuntime *runtime,
    UmiTraderProductionReadiness *out_readiness);
#ifdef __cplusplus
}
#endif
#endif

