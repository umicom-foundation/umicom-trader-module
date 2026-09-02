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

/*
 * Provide the trader application surface open chart operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_application_surface_open_chart(
    UmiTraderApplicationSurface *surface)
{
    return umi_trader_application_surface_focus(
        surface, "umicom.trading.chart");
}

/*
 * Provide the trader application surface open order ticket operation used by this module
 * and its client applications.
 */
UmiStatus umi_trader_application_surface_open_order_ticket(
    UmiTraderApplicationSurface *surface)
{
    return umi_trader_application_surface_focus(
        surface, "umicom.trading.order-ticket");
}

/*
 * Provide the trader application surface open risk operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_application_surface_open_risk(
    UmiTraderApplicationSurface *surface)
{
    return umi_trader_application_surface_focus(
        surface, "umicom.trading.risk");
}

/*
 * Provide the trader application surface prepare order operation used by this module and
 * its client applications.
 */
UmiStatus umi_trader_application_surface_prepare_order(
    UmiTraderApplicationSurface *surface)
{
    return umi_trader_application_surface_command(
        surface, "umicom.trading.order-ticket", "trader.order.prepare-simulation");
}

/*
 * Provide the trader application surface refresh market data operation used by this module
 * and its client applications.
 */
UmiStatus umi_trader_application_surface_refresh_market_data(
    UmiTraderApplicationSurface *surface)
{
    UmiApplicationPresentationSurfaceRuntime *runtime =
        umi_trader_application_surface_runtime(surface);
    UmiStatus status;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (runtime == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    status = umi_application_presentation_surface_runtime_refresh(
        runtime, "umicom.trading.chart");
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status == UMI_STATUS_OK) {
        status = umi_application_presentation_surface_runtime_refresh(
            runtime, "umicom.trading.watchlist");
    }
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status == UMI_STATUS_OK) {
        status = umi_application_presentation_surface_runtime_refresh(
            runtime, "umicom.trading.market-depth");
    }
    return status;
}
