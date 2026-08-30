/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: tests/test_application_surface_controllers.c
 *
 * PURPOSE:
 *   Verify Trader contributes state controllers for every component selected
 *   by its standard Framework presentation recipe.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include <assert.h>

#include "umicom/trader/application_surface_controllers.h"

int main(void)
{
    UmiApplicationPresentationSurfaceRuntime runtime;
    assert(umi_application_presentation_surface_runtime_init(
               "org.umicom.workspace.trader.standard", &runtime) ==
           UMI_STATUS_OK);
    assert(umi_trader_application_surface_controllers_register(
               &runtime, NULL) == UMI_STATUS_OK);
    assert(runtime.controllers.count == runtime.session.item_count);
    return 0;
}
