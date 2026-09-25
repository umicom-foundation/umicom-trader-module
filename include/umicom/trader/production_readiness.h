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
#include "umicom/trading_workstation/service.h"
#include "umicom/strategy_research/service.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Represent the trader production readiness data shared with callers of this public
 * contract.
 */
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

/**
 * Copy the professional workstation state calculated by Framework.
 *
 * Trader does not calculate spreads, depth, execution risk, P&L, scanner or
 * linked-context readiness locally; this function is a thin product boundary.
 */
UmiStatus umi_trader_production_workstation_snapshot(
    UmiTradingWorkspace *workspace,
    UmiTradingProfessionalWorkstationSnapshot *out_snapshot);



/**
 * Copy Framework-owned strategy/replay research readiness for Trader.
 *
 * This remains research-only: backtest and replay evidence never arm live
 * execution or bypass the existing order/risk controls.
 */
UmiStatus umi_trader_strategy_research_snapshot(
    UmiTradingWorkspace *workspace,
    UmiStrategyResearchWorkspaceSnapshot *out_snapshot);

#ifdef __cplusplus
}
#endif
#endif
