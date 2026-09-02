/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: include/umicom/trader/application_surface_controllers.h
 *
 * PURPOSE:
 *   Register Trader presentation controllers for the reusable market, order,
 *   portfolio, risk and strategy panels selected by its standard recipe.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_TRADER_APPLICATION_SURFACE_CONTROLLERS_H
#define UMICOM_TRADER_APPLICATION_SURFACE_CONTROLLERS_H

#include "umicom/application/presentation/presentation.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Add trader application surface controllers only after its inputs and available capacity
 * have been checked.
 */
UmiStatus umi_trader_application_surface_controllers_register(
    UmiApplicationPresentationSurfaceRuntime *runtime,
    void *context);

#ifdef __cplusplus
}
#endif

#endif
