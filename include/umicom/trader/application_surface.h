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

typedef struct UmiTraderApplicationSurface UmiTraderApplicationSurface;

UmiStatus umi_trader_application_surface_create(
    UmiTraderApplicationSurface **out_surface);
UmiStatus umi_trader_application_surface_create_for_audience(
    UmiApplicationComponentRecipeAudience audience,
    UmiTraderApplicationSurface **out_surface);
void umi_trader_application_surface_destroy(
    UmiTraderApplicationSurface *surface);
UmiStatus umi_trader_application_surface_refresh(
    UmiTraderApplicationSurface *surface);
UmiStatus umi_trader_application_surface_activate(
    UmiTraderApplicationSurface *surface,
    const char *component_id);
UmiStatus umi_trader_application_surface_deactivate(
    UmiTraderApplicationSurface *surface,
    const char *component_id);
UmiStatus umi_trader_application_surface_focus(
    UmiTraderApplicationSurface *surface,
    const char *component_id);
UmiStatus umi_trader_application_surface_command(
    UmiTraderApplicationSurface *surface,
    const char *component_id,
    const char *command_id);
UmiStatus umi_trader_application_surface_context_changed(
    UmiTraderApplicationSurface *surface,
    const char *component_id,
    const char *context_value);
UmiStatus umi_trader_application_surface_advance(
    UmiTraderApplicationSurface *surface,
    uint32_t elapsed_seconds);
UmiStatus umi_trader_application_surface_set_background(
    UmiTraderApplicationSurface *surface,
    int background);
UmiStatus umi_trader_application_surface_snapshot(
    const UmiTraderApplicationSurface *surface,
    UmiApplicationPresentationSurfaceSnapshot *out_snapshot);
UmiApplicationPresentationSurfaceRuntime *
umi_trader_application_surface_runtime(UmiTraderApplicationSurface *surface);
const UmiApplicationPresentationSurfaceRuntime *
umi_trader_application_surface_runtime_const(
    const UmiTraderApplicationSurface *surface);

#ifdef __cplusplus
}
#endif

#endif
