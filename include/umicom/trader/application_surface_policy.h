/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: include/umicom/trader/application_surface_policy.h
 *
 * PURPOSE:
 *   Give Trader a product-facing view of shared refresh, background, context,
 *   checkpoint and guarded-command behavior without duplicating Framework.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_TRADER_APPLICATION_SURFACE_POLICY_H
#define UMICOM_TRADER_APPLICATION_SURFACE_POLICY_H

#include <stddef.h>
#include <stdint.h>

#include "umicom/trader/application_surface.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Represent the trader application surface policy snapshot data shared with callers of
 * this public contract.
 */
typedef struct UmiTraderApplicationSurfacePolicySnapshot {
    const char *recipe_id;
    const char *startup_policy;
    const char *checkpoint_policy;
    const char *focus_policy;
    const char *background_policy;
    uint32_t checkpoint_interval_seconds;
    size_t scheduled_refresh_panels;
    size_t streaming_panels;
    size_t guarded_command_panels;
    size_t context_enabled_panels;
    int restore_focus;
    int share_context;
    int allow_background_commands;
} UmiTraderApplicationSurfacePolicySnapshot;

/**
 * Provide the trader application surface policy snapshot operation used by this module and
 * its client applications.
 */
UmiStatus umi_trader_application_surface_policy_snapshot(
    const UmiTraderApplicationSurface *surface,
    UmiTraderApplicationSurfacePolicySnapshot *out_snapshot);
/**
 * Provide the trader application surface policy advance operation used by this module and
 * its client applications.
 */
UmiStatus umi_trader_application_surface_policy_advance(
    UmiTraderApplicationSurface *surface,
    uint32_t elapsed_seconds);
/**
 * Provide the trader application surface policy set background operation used by this
 * module and its client applications.
 */
UmiStatus umi_trader_application_surface_policy_set_background(
    UmiTraderApplicationSurface *surface,
    int background);
/**
 * Provide the trader application surface policy context changed operation used by this
 * module and its client applications.
 */
UmiStatus umi_trader_application_surface_policy_context_changed(
    UmiTraderApplicationSurface *surface,
    const char *component_id,
    const char *context_value);
/**
 * Provide the trader application surface policy checkpoint due operation used by this
 * module and its client applications.
 */
int umi_trader_application_surface_policy_checkpoint_due(
    const UmiTraderApplicationSurface *surface,
    uint32_t elapsed_since_checkpoint_seconds,
    int changed);

#ifdef __cplusplus
}
#endif

#endif
