/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: include/umicom/trader/application_surface.h
 *
 * PURPOSE:
 *   Compose Trader's standard workspace from Framework-owned presentation
 *   components while keeping market and order logic in Framework services.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_TRADER_APPLICATION_SURFACE_H
#define UMICOM_TRADER_APPLICATION_SURFACE_H

#include "umicom/application/presentation/presentation.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UMI_TRADER_APPLICATION_ID "org.umicom.trader"
#define UMI_TRADER_STANDARD_RECIPE_ID "org.umicom.workspace.trader.standard"

/**
 * Represent the trader application surface data shared with callers of this public
 * contract.
 */
typedef struct UmiTraderApplicationSurface UmiTraderApplicationSurface;

/**
 * Initialise trader application surface from caller-provided values so later operations
 * receive a known state.
 */
UmiStatus umi_trader_application_surface_create(
    UmiTraderApplicationSurface **out_surface);
/**
 * Provide the trader application surface create for audience operation used by this module
 * and its client applications.
 */
UmiStatus umi_trader_application_surface_create_for_audience(
    UmiApplicationComponentRecipeAudience audience,
    UmiTraderApplicationSurface **out_surface);
/**
 * Release or reset state held by trader application surface so the same storage can be
 * reused safely.
 */
void umi_trader_application_surface_destroy(
    UmiTraderApplicationSurface *surface);
/**
 * Provide the trader application surface refresh operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_application_surface_refresh(
    UmiTraderApplicationSurface *surface);
/**
 * Provide the trader application surface activate operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_application_surface_activate(
    UmiTraderApplicationSurface *surface,
    const char *component_id);
/**
 * Provide the trader application surface deactivate operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_application_surface_deactivate(
    UmiTraderApplicationSurface *surface,
    const char *component_id);
/**
 * Provide the trader application surface focus operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_application_surface_focus(
    UmiTraderApplicationSurface *surface,
    const char *component_id);
/**
 * Provide the trader application surface command operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_application_surface_command(
    UmiTraderApplicationSurface *surface,
    const char *component_id,
    const char *command_id);
/**
 * Provide the trader application surface context changed operation used by this module and
 * its client applications.
 */
UmiStatus umi_trader_application_surface_context_changed(
    UmiTraderApplicationSurface *surface,
    const char *component_id,
    const char *context_value);
/**
 * Provide the trader application surface advance operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_application_surface_advance(
    UmiTraderApplicationSurface *surface,
    uint32_t elapsed_seconds);
/**
 * Provide the trader application surface set background operation used by this module and
 * its client applications.
 */
UmiStatus umi_trader_application_surface_set_background(
    UmiTraderApplicationSurface *surface,
    int background);
/**
 * Provide the trader application surface snapshot operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_application_surface_snapshot(
    const UmiTraderApplicationSurface *surface,
    UmiApplicationPresentationSurfaceSnapshot *out_snapshot);
/**
 * Provide the trader application surface runtime operation used by this module and its
 * client applications.
 */
UmiApplicationPresentationSurfaceRuntime *
umi_trader_application_surface_runtime(UmiTraderApplicationSurface *surface);
/**
 * Provide the trader application surface runtime const operation used by this module and
 * its client applications.
 */
const UmiApplicationPresentationSurfaceRuntime *
umi_trader_application_surface_runtime_const(
    const UmiTraderApplicationSurface *surface);

#ifdef __cplusplus
}
#endif

#endif
