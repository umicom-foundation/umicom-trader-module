/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: tests/test_application_surface_commands.c
 *
 * PURPOSE:
 *   Verify Trader navigation and order-preparation commands update the shared
 *   surface session without submitting a live order.
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

#include "umicom/trader/application_surface_commands.h"

int main(void)
{
    UmiTraderApplicationSurface *surface = NULL;
    UmiApplicationPresentationSurfaceSnapshot snapshot;
    UmiApplicationPresentationSurfaceItem *ticket;
    assert(umi_trader_application_surface_create(&surface) == UMI_STATUS_OK);
    assert(umi_trader_application_surface_open_order_ticket(surface) ==
           UMI_STATUS_OK);
    assert(umi_trader_application_surface_prepare_order(surface) ==
           UMI_STATUS_OK);
    assert(umi_trader_application_surface_refresh_market_data(surface) ==
           UMI_STATUS_OK);
    assert(umi_trader_application_surface_snapshot(surface, &snapshot) ==
           UMI_STATUS_OK);
    assert(strcmp(snapshot.focused_component_id,
                  "umicom.trading.order-ticket") == 0);
    ticket = umi_application_presentation_surface_session_find(
        &umi_trader_application_surface_runtime(surface)->session,
        "umicom.trading.order-ticket");
    assert(ticket != NULL && strcmp(ticket->badge, "SIM") == 0);
    umi_trader_application_surface_destroy(surface);
    return 0;
}
