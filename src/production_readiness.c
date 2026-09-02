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

