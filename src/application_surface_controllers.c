/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: src/application_surface_controllers.c
 *
 * PURPOSE:
 *   Describe safe simulation-ready state for Trader panels and clearly label
 *   commands as preparation events rather than live trade execution.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/trader/application_surface_controllers.h"

#include <stdio.h>
#include <string.h>

/* Provide the panel guidance operation used by this module and its client applications. */
static const char *panel_guidance(const char *component_id)
{
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.trading.chart") == 0)
        return "Chart is ready for Framework market-series data.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.trading.watchlist") == 0)
        return "Watchlist is ready for simulated instruments.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.trading.market-depth") == 0)
        return "Market depth is ready for simulated order-book updates.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.trading.order-ticket") == 0)
        return "Order ticket is in safe preparation mode.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.trading.orders") == 0)
        return "No simulated orders have been submitted.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.trading.executions") == 0)
        return "No simulated executions have been received.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.trading.portfolio") == 0)
        return "Portfolio is ready for simulated positions.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.trading.risk") == 0)
        return "Risk panel is ready for Framework exposure calculations.";
    return "Strategy workspace is ready for simulation and replay.";
}

/* Provide the trader controller operation used by this module and its client applications. */
static UmiStatus trader_controller(
    void *context,
    const UmiApplicationPresentationPanelPlacement *placement,
    UmiApplicationPresentationSurfaceEvent event,
    const char *payload,
    UmiApplicationPresentationSurfaceUpdate *out_update)
{
    const char *component_id;
    (void)context;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (placement == NULL || placement->panel == NULL || out_update == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    component_id = placement->panel->component_id;
    (void)memset(out_update, 0, sizeof(*out_update));
    /* Apply this branch only when its contract condition is satisfied. */
    if (event == UMI_APPLICATION_PRESENTATION_EVENT_UNMOUNT ||
        event == UMI_APPLICATION_PRESENTATION_EVENT_DEACTIVATE) {
        out_update->state = UMI_APPLICATION_PRESENTATION_STATE_DORMANT;
        return UMI_STATUS_OK;
    }
    /* Apply this branch only when its contract condition is satisfied. */
    if (event == UMI_APPLICATION_PRESENTATION_EVENT_COMMAND) {
        out_update->state = UMI_APPLICATION_PRESENTATION_STATE_READY;
        (void)snprintf(out_update->message, sizeof(out_update->message),
                       "Prepared simulation command: %s",
                       payload != NULL ? payload : "");
        (void)snprintf(out_update->badge, sizeof(out_update->badge), "SIM");
        return UMI_STATUS_OK;
    }
    out_update->state = UMI_APPLICATION_PRESENTATION_STATE_READY;
    (void)snprintf(out_update->message, sizeof(out_update->message), "%s",
                   panel_guidance(component_id));
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.trading.orders") == 0 ||
        strcmp(component_id, "umicom.trading.executions") == 0) {
        out_update->state = UMI_APPLICATION_PRESENTATION_STATE_EMPTY;
        (void)snprintf(out_update->badge, sizeof(out_update->badge), "0");
    }
    return UMI_STATUS_OK;
}

/*
 * Add trader application surface controllers only after its inputs and available capacity
 * have been checked.
 */
UmiStatus umi_trader_application_surface_controllers_register(
    UmiApplicationPresentationSurfaceRuntime *runtime,
    void *context)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (runtime == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    return umi_application_presentation_surface_runtime_register_controller_for_all(
        runtime, trader_controller, context);
}
