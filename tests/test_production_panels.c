/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: tests/test_production_panels.c
 *
 * PURPOSE:
 *   Verify the test production panels behavior for
 *   Umicom Trader Module.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
/* Umicom Trader production control-plane test | Sammy Hegab | Umicom Foundation | MIT */
#include <assert.h>
#include <stdlib.h>
#include "umicom/trader/production_panels.h"
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
    UmiTraderProductionPanelCatalogue panels;
    assert(runtime != NULL);
    assert(umi_application_production_runtime_init(umi_trader_productisation_contribution(), available, NULL, runtime) == UMI_STATUS_OK);
    assert(umi_trader_production_panels_build(runtime, &panels) == UMI_STATUS_OK);
    assert(panels.count == runtime->panels.count);
    assert(panels.risk_sensitive_count > 0U);
    free(runtime); return 0;
}

