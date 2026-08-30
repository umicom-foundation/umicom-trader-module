/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: src/application_surface.c
 *
 * PURPOSE:
 *   Own Trader's thin composition over the shared Framework application
 *   presentation runtime and headless verification host.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/trader/application_surface.h"

#include <stdlib.h>

#include "umicom/trader/application_surface_controllers.h"

struct UmiTraderApplicationSurface {
    UmiApplicationPresentationSurfaceRuntime runtime;
    UmiApplicationPresentationHeadlessSurfaceHost headless;
};

UmiStatus umi_trader_application_surface_create(
    UmiTraderApplicationSurface **out_surface)
{
    UmiTraderApplicationSurface *surface;
    UmiStatus status;
    if (out_surface == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    *out_surface = NULL;
    surface = calloc(1U, sizeof(*surface));
    if (surface == NULL) return UMI_STATUS_OUT_OF_MEMORY;
    status = umi_application_presentation_surface_runtime_init(
        UMI_TRADER_STANDARD_RECIPE_ID, &surface->runtime);
    if (status == UMI_STATUS_OK) {
        umi_application_presentation_headless_surface_host_init(
            &surface->headless);
        status = umi_application_presentation_surface_runtime_bind_host(
            &surface->runtime,
            umi_application_presentation_headless_surface_host_interface(
                &surface->headless));
    }
    if (status == UMI_STATUS_OK) {
        status = umi_trader_application_surface_controllers_register(
            &surface->runtime, surface);
    }
    if (status == UMI_STATUS_OK) {
        status = umi_application_presentation_surface_runtime_start(
            &surface->runtime);
    }
    if (status != UMI_STATUS_OK) {
        umi_trader_application_surface_destroy(surface);
        return status;
    }
    *out_surface = surface;
    return UMI_STATUS_OK;
}

void umi_trader_application_surface_destroy(
    UmiTraderApplicationSurface *surface)
{
    if (surface == NULL) return;
    (void)umi_application_presentation_surface_runtime_stop(&surface->runtime);
    free(surface);
}

UmiStatus umi_trader_application_surface_refresh(
    UmiTraderApplicationSurface *surface)
{
    return surface != NULL
        ? umi_application_presentation_surface_runtime_refresh_all(
              &surface->runtime)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_trader_application_surface_focus(
    UmiTraderApplicationSurface *surface,
    const char *component_id)
{
    return surface != NULL
        ? umi_application_presentation_surface_runtime_focus(
              &surface->runtime, component_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_trader_application_surface_command(
    UmiTraderApplicationSurface *surface,
    const char *component_id,
    const char *command_id)
{
    return surface != NULL
        ? umi_application_presentation_surface_runtime_command(
              &surface->runtime, component_id, command_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_trader_application_surface_snapshot(
    const UmiTraderApplicationSurface *surface,
    UmiApplicationPresentationSurfaceSnapshot *out_snapshot)
{
    return surface != NULL
        ? umi_application_presentation_surface_runtime_snapshot(
              &surface->runtime, out_snapshot)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiApplicationPresentationSurfaceRuntime *
umi_trader_application_surface_runtime(UmiTraderApplicationSurface *surface)
{
    return surface != NULL ? &surface->runtime : NULL;
}

const UmiApplicationPresentationSurfaceRuntime *
umi_trader_application_surface_runtime_const(
    const UmiTraderApplicationSurface *surface)
{
    return surface != NULL ? &surface->runtime : NULL;
}
