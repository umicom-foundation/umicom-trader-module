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

#include "umicom/trader/application_surface.h"

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
    return 0;
}
