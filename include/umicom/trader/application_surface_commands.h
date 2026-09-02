/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: include/umicom/trader/application_surface_commands.h
 *
 * PURPOSE:
 *   Provide safe navigation and preparation commands for Trader's canonical
 *   application components without directly submitting a live order.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_TRADER_APPLICATION_SURFACE_COMMANDS_H
#define UMICOM_TRADER_APPLICATION_SURFACE_COMMANDS_H

#include "umicom/trader/application_surface.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Provide the trader application surface open chart operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_application_surface_open_chart(
    UmiTraderApplicationSurface *surface);
/**
 * Provide the trader application surface open order ticket operation used by this module
 * and its client applications.
 */
UmiStatus umi_trader_application_surface_open_order_ticket(
    UmiTraderApplicationSurface *surface);
/**
 * Provide the trader application surface open risk operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_application_surface_open_risk(
    UmiTraderApplicationSurface *surface);
/**
 * Provide the trader application surface prepare order operation used by this module and
 * its client applications.
 */
UmiStatus umi_trader_application_surface_prepare_order(
    UmiTraderApplicationSurface *surface);
/**
 * Provide the trader application surface refresh market data operation used by this module
 * and its client applications.
 */
UmiStatus umi_trader_application_surface_refresh_market_data(
    UmiTraderApplicationSurface *surface);

#ifdef __cplusplus
}
#endif

#endif
