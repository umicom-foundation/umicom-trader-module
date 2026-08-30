/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: src/application_surface_commands.c
 *
 * PURPOSE:
 *   Implement safe Trader navigation and preparation helpers over reusable
 *   Framework presentation runtime operations.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/trader/application_surface_commands.h"

UmiStatus umi_trader_application_surface_open_chart(
    UmiTraderApplicationSurface *surface)
{
    return umi_trader_application_surface_focus(
        surface, "umicom.trading.chart");
}

UmiStatus umi_trader_application_surface_open_order_ticket(
    UmiTraderApplicationSurface *surface)
{
    return umi_trader_application_surface_focus(
        surface, "umicom.trading.order-ticket");
}

UmiStatus umi_trader_application_surface_open_risk(
    UmiTraderApplicationSurface *surface)
{
    return umi_trader_application_surface_focus(
        surface, "umicom.trading.risk");
}

UmiStatus umi_trader_application_surface_prepare_order(
    UmiTraderApplicationSurface *surface)
{
    return umi_trader_application_surface_command(
        surface, "umicom.trading.order-ticket", "trader.order.prepare-simulation");
}

UmiStatus umi_trader_application_surface_refresh_market_data(
    UmiTraderApplicationSurface *surface)
{
    UmiApplicationPresentationSurfaceRuntime *runtime =
        umi_trader_application_surface_runtime(surface);
    UmiStatus status;
    if (runtime == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    status = umi_application_presentation_surface_runtime_refresh(
        runtime, "umicom.trading.chart");
    if (status == UMI_STATUS_OK) {
        status = umi_application_presentation_surface_runtime_refresh(
            runtime, "umicom.trading.watchlist");
    }
    if (status == UMI_STATUS_OK) {
        status = umi_application_presentation_surface_runtime_refresh(
            runtime, "umicom.trading.market-depth");
    }
    return status;
}
