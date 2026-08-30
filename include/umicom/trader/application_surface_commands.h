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

UmiStatus umi_trader_application_surface_open_chart(
    UmiTraderApplicationSurface *surface);
UmiStatus umi_trader_application_surface_open_order_ticket(
    UmiTraderApplicationSurface *surface);
UmiStatus umi_trader_application_surface_open_risk(
    UmiTraderApplicationSurface *surface);
UmiStatus umi_trader_application_surface_prepare_order(
    UmiTraderApplicationSurface *surface);
UmiStatus umi_trader_application_surface_refresh_market_data(
    UmiTraderApplicationSurface *surface);

#ifdef __cplusplus
}
#endif

#endif
