/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: src/workbench_profile.c
 *
 * PURPOSE:
 *   Compose the Trader workbench entirely from Framework-owned groups,
 *   endpoints and context-source definitions.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include "umicom/trader/workbench_profile.h"

#include <stdlib.h>
#include <string.h>

struct UmiTraderWorkbenchProfile {
    UmiWorkbenchSelectionProviderTradingWorkbench shared;
    uint64_t revision;
};

/* Provide the copy text operation used by this module and its client applications. */
static UmiStatus copy_text(char *destination,
                           size_t capacity,
                           const char *source)
{
    return umi_workbench_selection_provider_copy_text(
        destination,
        capacity,
        source != NULL ? source : "");
}

/*
 * Initialise trader workbench profile from caller-provided values so later operations
 * receive a known state.
 */
UmiStatus umi_trader_workbench_profile_create(
    UmiTraderWorkbenchProfile **out_profile)
{
    UmiTraderWorkbenchProfile *profile;
    UmiStatus status;

    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (out_profile == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }

    *out_profile = NULL;
    profile = (UmiTraderWorkbenchProfile *)calloc(1U, sizeof(*profile));
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (profile == NULL) {
        return UMI_STATUS_OUT_OF_MEMORY;
    }

    /*
     * The Framework is the single source of truth for panel semantics.  Trader
     * only selects the Trader half of the shared Trader/TMS workbench profile.
     */
    status = umi_workbench_selection_provider_trading_workbench_build(
        &profile->shared);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status == UMI_STATUS_OK) {
        status = umi_workbench_selection_provider_trading_workbench_validate(
            &profile->shared);
    }
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        free(profile);
        return status;
    }

    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(profile->shared.trader.application_id,
               "org.umicom.trader") != 0) {
        free(profile);
        return UMI_STATUS_INVALID_STATE;
    }

    profile->revision = 1U;
    *out_profile = profile;
    return UMI_STATUS_OK;
}

/*
 * Release or reset state held by trader workbench profile so the same storage can be
 * reused safely.
 */
void umi_trader_workbench_profile_destroy(
    UmiTraderWorkbenchProfile *profile)
{
    free(profile);
}

/*
 * Provide the trader workbench profile snapshot operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_workbench_profile_snapshot(
    const UmiTraderWorkbenchProfile *profile,
    UmiTraderWorkbenchProfileSnapshot *out_snapshot)
{
    UmiStatus status;

    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (profile == NULL || out_snapshot == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }

    memset(out_snapshot, 0, sizeof(*out_snapshot));
    out_snapshot->struct_size = (uint32_t)sizeof(*out_snapshot);
    out_snapshot->api_version = UMI_TRADER_WORKBENCH_PROFILE_API_VERSION;

    status = copy_text(
        out_snapshot->application_id,
        sizeof(out_snapshot->application_id),
        profile->shared.trader.application_id);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) return status;

    status = copy_text(
        out_snapshot->profile_id,
        sizeof(out_snapshot->profile_id),
        profile->shared.trader.profile_id);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) return status;

    status = copy_text(
        out_snapshot->title,
        sizeof(out_snapshot->title),
        profile->shared.trader.title);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) return status;

    status = copy_text(
        out_snapshot->trading_group_id,
        sizeof(out_snapshot->trading_group_id),
        profile->shared.trading_group_id);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) return status;

    status = copy_text(
        out_snapshot->operations_group_id,
        sizeof(out_snapshot->operations_group_id),
        profile->shared.operations_group_id);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) return status;

    out_snapshot->group_count = profile->shared.trader.group_count;
    out_snapshot->endpoint_count = profile->shared.trader.endpoint_count;
    out_snapshot->revision =
        profile->revision + profile->shared.trader.revision;

    return UMI_STATUS_OK;
}

/*
 * Provide the trader workbench profile context host operation used by this module and its
 * client applications.
 */
const UmiWorkbenchContextHostProfile *
umi_trader_workbench_profile_context_host(
    const UmiTraderWorkbenchProfile *profile)
{
    return profile != NULL ? &profile->shared.trader : NULL;
}

/*
 * Provide the trader workbench profile context sources operation used by this module and
 * its client applications.
 */
const UmiWorkbenchContextSourceTradingProfile *
umi_trader_workbench_profile_context_sources(
    const UmiTraderWorkbenchProfile *profile)
{
    return profile != NULL ? &profile->shared.sources : NULL;
}
