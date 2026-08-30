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
    UmiApplicationPresentationProductSurface product;
};

UmiStatus umi_trader_application_surface_create(
    UmiTraderApplicationSurface **out_surface)
{
    return umi_trader_application_surface_create_for_audience(
        UMI_APPLICATION_COMPONENT_RECIPE_AUDIENCE_STANDARD, out_surface);
}

UmiStatus umi_trader_application_surface_create_for_audience(
    UmiApplicationComponentRecipeAudience audience,
    UmiTraderApplicationSurface **out_surface)
{
    UmiTraderApplicationSurface *surface;
    UmiStatus status;
    if (out_surface == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    *out_surface = NULL;
    surface = calloc(1U, sizeof(*surface));
    if (surface == NULL) return UMI_STATUS_OUT_OF_MEMORY;
    status = umi_application_presentation_product_surface_init_for_audience(
        UMI_TRADER_APPLICATION_ID,
        audience,
        umi_trader_application_surface_controllers_register,
        surface,
        &surface->product);
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
    umi_application_presentation_product_surface_dispose(&surface->product);
    free(surface);
}

UmiStatus umi_trader_application_surface_refresh(
    UmiTraderApplicationSurface *surface)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_refresh(
              &surface->product)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_trader_application_surface_activate(
    UmiTraderApplicationSurface *surface,
    const char *component_id)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_activate(
              &surface->product, component_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_trader_application_surface_deactivate(
    UmiTraderApplicationSurface *surface,
    const char *component_id)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_deactivate(
              &surface->product, component_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_trader_application_surface_focus(
    UmiTraderApplicationSurface *surface,
    const char *component_id)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_focus(
              &surface->product, component_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_trader_application_surface_command(
    UmiTraderApplicationSurface *surface,
    const char *component_id,
    const char *command_id)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_command(
              &surface->product, component_id, command_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_trader_application_surface_context_changed(
    UmiTraderApplicationSurface *surface,
    const char *component_id,
    const char *context_value)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_context_changed(
              &surface->product, component_id, context_value)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_trader_application_surface_advance(
    UmiTraderApplicationSurface *surface,
    uint32_t elapsed_seconds)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_advance(
              &surface->product, elapsed_seconds)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_trader_application_surface_set_background(
    UmiTraderApplicationSurface *surface,
    int background)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_set_background(
              &surface->product, background)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_trader_application_surface_snapshot(
    const UmiTraderApplicationSurface *surface,
    UmiApplicationPresentationSurfaceSnapshot *out_snapshot)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_snapshot(
              &surface->product, out_snapshot)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiApplicationPresentationSurfaceRuntime *
umi_trader_application_surface_runtime(UmiTraderApplicationSurface *surface)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_runtime(
              &surface->product)
        : NULL;
}

const UmiApplicationPresentationSurfaceRuntime *
umi_trader_application_surface_runtime_const(
    const UmiTraderApplicationSurface *surface)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_runtime_const(
              &surface->product)
        : NULL;
}
