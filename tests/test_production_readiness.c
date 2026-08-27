/* Umicom Trader production control-plane test | Sammy Hegab | Umicom Foundation | MIT */
#include <assert.h>
#include <stdlib.h>
#include "umicom/trader/production_readiness.h"
static int available(const char *id, void *context) {
    (void)context; return id != NULL && id[0] != '\0';
}
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

