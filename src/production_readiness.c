/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: src/production_readiness.c
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
#include "umicom/trader/production_readiness.h"

#include <stdio.h>
#include <string.h>

/* Provide the feature at least operation used by this module and its client applications. */
static int feature_at_least(
    const UmiApplicationProductionRuntime *runtime, const char *feature_id,
    UmiExperienceFeatureState state)
{
    const UmiApplicationProductionFeatureBinding *binding =
        umi_application_production_feature_bindings_find(
            &runtime->features, feature_id);
    return binding != NULL && binding->feature->state >= state;
}

/*
 * Provide the trader production readiness build operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_production_readiness_build(
    const UmiApplicationProductionRuntime *runtime,
    UmiTraderProductionReadiness *out_readiness)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (runtime == NULL || out_readiness == NULL || !runtime->initialised ||
        strcmp(runtime->binding.experience->application_id,
               "org.umicom.trader") != 0)
        return UMI_STATUS_INVALID_ARGUMENT;
    (void)memset(out_readiness, 0, sizeof(*out_readiness));
    out_readiness->state = runtime->acceptance.state;
    out_readiness->market_data_ready = feature_at_least(
        runtime, "trader.market-data", UMI_EXPERIENCE_FEATURE_IMPLEMENTED);
    out_readiness->oms_ready = feature_at_least(
        runtime, "trader.oms", UMI_EXPERIENCE_FEATURE_IMPLEMENTED);
    out_readiness->risk_ready = feature_at_least(
        runtime, "trader.risk", UMI_EXPERIENCE_FEATURE_IMPLEMENTED);
    out_readiness->paper_adapter_ready = feature_at_least(
        runtime, "trader.paper-adapter", UMI_EXPERIENCE_FEATURE_IMPLEMENTED);
    out_readiness->paper_execution_ready =
        runtime->acceptance.accepted && out_readiness->market_data_ready &&
        out_readiness->oms_ready && out_readiness->risk_ready &&
        out_readiness->paper_adapter_ready;
    out_readiness->live_execution_verified = feature_at_least(
        runtime, "trader.live", UMI_EXPERIENCE_FEATURE_VERIFIED);
    out_readiness->live_execution_ready =
        out_readiness->paper_execution_ready &&
        out_readiness->live_execution_verified;
    out_readiness->open_adapter_feature_count =
        runtime->features.external_adapter_work_count;
    out_readiness->readiness_percent = runtime->readiness.combined_percent;
    return UMI_STATUS_OK;
}

/*
 * Keep Trader thin by forwarding professional workstation inspection to the
 * Framework service that owns the calculations and readiness contracts.
 */
UmiStatus umi_trader_production_workstation_snapshot(
    UmiTradingWorkspace *workspace,
    UmiTradingProfessionalWorkstationSnapshot *out_snapshot)
{
    return umi_trading_professional_workstation_snapshot(
        workspace, out_snapshot);
}


/*
 * Keep Trader thin by forwarding strategy/backtest/replay readiness to the
 * Framework research service. The product adds no local research engine.
 */
UmiStatus umi_trader_strategy_research_snapshot(
    UmiTradingWorkspace *workspace,
    UmiStrategyResearchWorkspaceSnapshot *out_snapshot)
{
    return umi_strategy_research_workspace_snapshot(
        workspace, out_snapshot);
}


/*
 * Trader's product defaults identify the canonical paper runtime and the IBKR
 * adapter boundary. The IBKR configuration stays paper-only until the user
 * explicitly changes and approves a live profile through governed controls.
 */
UmiStatus umi_trader_broker_connectivity_defaults(
    UmiBrokerProviderRegistry *providers,
    UmiIbkrAdapterConfig *ibkr)
{
    UmiBrokerProviderDescriptor paper = {0};
    UmiBrokerProviderDescriptor interactiveBrokers = {0};
    UmiStatus status;

    if (providers == NULL || ibkr == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }

    umi_broker_provider_registry_init(providers);
    umi_ibkr_adapter_config_init(ibkr);

    (void)snprintf(paper.id, sizeof(paper.id), "%s", "paper");
    (void)snprintf(
        paper.displayName, sizeof(paper.displayName),
        "%s", "Umicom Paper Broker");
    paper.paperSupported = 1;
    paper.accountSupported = 1;
    paper.orderSupported = 1;
    paper.executionSupported = 1;
    status = umi_broker_provider_registry_register(providers, &paper);

    if (status == UMI_STATUS_OK) {
        (void)snprintf(
            interactiveBrokers.id,
            sizeof(interactiveBrokers.id),
            "%s",
            "ibkr");
        (void)snprintf(
            interactiveBrokers.displayName,
            sizeof(interactiveBrokers.displayName),
            "%s",
            "Interactive Brokers");
        interactiveBrokers.paperSupported = 1;
        interactiveBrokers.liveSupported = 1;
        interactiveBrokers.accountSupported = 1;
        interactiveBrokers.orderSupported = 1;
        interactiveBrokers.executionSupported = 1;
        status = umi_broker_provider_registry_register(
            providers, &interactiveBrokers);
    }
    return status;
}

/* Keep Trader's application boundary thin over Framework-owned connectivity. */
UmiStatus umi_trader_broker_connectivity_snapshot(
    const UmiBrokerProviderRegistry *providers,
    const UmiBrokerSessionSupervisor *session,
    const UmiBrokerAccountStore *accounts,
    const UmiBrokerOrderJournal *orders,
    const UmiBrokerExecutionReconciler *executions,
    const UmiBrokerPositionStore *positions,
    const UmiBrokerAuditJournal *audit,
    const UmiIbkrAdapterConfig *ibkr,
    int live_approved,
    UmiBrokerConnectivityPlatformSnapshot *out_snapshot)
{
    return umi_broker_connectivity_platform_snapshot(
        providers,
        session,
        accounts,
        orders,
        executions,
        positions,
        audit,
        ibkr,
        live_approved,
        out_snapshot);
}
