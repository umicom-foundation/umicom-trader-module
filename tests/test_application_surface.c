/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: tests/test_application_surface.c
 *
 * PURPOSE:
 *   Verify Trader starts all nine panels from its Framework-owned standard
 *   presentation recipe in safe simulation-oriented state.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include <assert.h>
#include <string.h>

#include "umicom/trader/application_surface.h"

/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
int main(void)
{
    UmiTraderApplicationSurface *surface = NULL;
    UmiApplicationPresentationSurfaceSnapshot snapshot;
    assert(umi_trader_application_surface_create(&surface) == UMI_STATUS_OK);
    assert(umi_trader_application_surface_snapshot(surface, &snapshot) ==
           UMI_STATUS_OK);
    assert(snapshot.panel_count == 9U);
    assert(snapshot.visible_count == 9U);
    assert(snapshot.attention_count == 0U);
    assert(snapshot.ready_count == 7U);
    umi_trader_application_surface_destroy(surface);

    surface = NULL;
    assert(umi_trader_application_surface_create_for_audience(
               UMI_APPLICATION_COMPONENT_RECIPE_AUDIENCE_FOCUS,
               &surface) == UMI_STATUS_OK);
    assert(umi_trader_application_surface_snapshot(surface, &snapshot) ==
           UMI_STATUS_OK);
    assert(strcmp(snapshot.recipe_id,
                  "org.umicom.workspace.trader.focus") == 0);
    assert(snapshot.panel_count > 0U);
    assert(snapshot.panel_count < 9U);
    umi_trader_application_surface_destroy(surface);

    surface = NULL;
    assert(umi_trader_application_surface_create_for_audience(
               UMI_APPLICATION_COMPONENT_RECIPE_AUDIENCE_LEARNING,
               &surface) == UMI_STATUS_OK);
    assert(umi_trader_application_surface_snapshot(surface, &snapshot) ==
           UMI_STATUS_OK);
    assert(strcmp(snapshot.recipe_id,
                  "org.umicom.workspace.trader.learning") == 0);
    umi_trader_application_surface_destroy(surface);
    return 0;
}
