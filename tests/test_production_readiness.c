/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: tests/test_production_readiness.c
 *
 * PURPOSE:
 *   Verify production readiness and the thin Framework professional-workstation
 *   projection used by Umicom Trader.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

/*
 * The previous production-readiness test is retained for review. The active
 * test below extends it with the new Framework professional-workstation
 * snapshot and does not remove its existing acceptance assertions.
 */
#if 0
/* Umicom Trader production control-plane test | Sammy Hegab | Umicom Foundation | MIT */
#include <assert.h>
#include <stdlib.h>
#include "umicom/trader/production_readiness.h"
/*
 * Exercise available and return a clear result when the behaviour no longer matches its
 * contract.
 */
static int available(const char *id, void *context) {
    (void)context; return id != NULL && id[0] != '\0';
}
/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
int main(void) {
    UmiApplicationProductionRuntime *runtime = calloc(1U, sizeof(*runtime));
    UmiTraderProductionReadiness readiness;
    assert(runtime != NULL);
    assert(umi_application_production_runtime_init(umi_trader_productisation_contribution(), available, NULL, runtime) == UMI_STATUS_OK);
    assert(umi_trader_production_readiness_build(runtime, &readiness) == UMI_STATUS_OK);
    assert(readiness.market_data_ready && readiness.oms_ready && readiness.risk_ready);
    assert(!readiness.live_execution_ready);
    free(runtime); return 0;
}
#endif
#include <assert.h>
#include <stdlib.h>

#include "umicom/trader/production_readiness.h"

/*
 * Exercise available and return a clear result when the behaviour no longer matches its
 * contract.
 */
static int available(const char *id, void *context)
{
    (void)context;
    return id != NULL && id[0] != '\0';
}

/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
int main(void)
{
    UmiApplicationProductionRuntime *runtime =
        calloc(1U, sizeof(*runtime));
    UmiTraderProductionReadiness readiness;
    UmiTradingWorkspaceConfig config;
    UmiTradingWorkspace *workspace = NULL;
    UmiTradingProfessionalWorkstationSnapshot workstation;
    UmiStrategyResearchWorkspaceSnapshot research;

    assert(runtime != NULL);
    assert(umi_application_production_runtime_init(
               umi_trader_productisation_contribution(),
               available,
               NULL,
               runtime) == UMI_STATUS_OK);
    assert(umi_trader_production_readiness_build(
               runtime,
               &readiness) == UMI_STATUS_OK);
    assert(readiness.market_data_ready &&
           readiness.oms_ready &&
           readiness.risk_ready);
    assert(!readiness.live_execution_ready);

    config = umi_trading_workspace_config_default();
    assert(umi_trading_workspace_create(&config, &workspace) ==
           UMI_STATUS_OK);
    assert(umi_trader_production_workstation_snapshot(
               workspace,
               &workstation) == UMI_STATUS_OK);
    assert(workstation.capabilityCount == 10U);
    assert(workstation.revision > 0U);

    assert(umi_trader_strategy_research_snapshot(
               workspace, &research) == UMI_STATUS_OK);
    assert(research.revision > 0U);
    assert(research.marketDataReady);

    umi_trading_workspace_destroy(workspace);
    free(runtime);
    return 0;
}
