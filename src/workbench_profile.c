/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: src/workbench_profile.c
 *
 * PURPOSE:
 *   Compose the Trader workbench entirely from Framework-owned groups,
 *   endpoints and context-source definitions.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/trader/workbench_profile.h"

#include <stdlib.h>
#include <string.h>

struct UmiTraderWorkbenchProfile {
    UmiWorkbenchSelectionProviderTradingWorkbench shared;
    uint64_t revision;
};

static UmiStatus copy_text(char *destination,
                           size_t capacity,
                           const char *source)
{
    return umi_workbench_selection_provider_copy_text(
        destination,
        capacity,
        source != NULL ? source : "");
}

UmiStatus umi_trader_workbench_profile_create(
    UmiTraderWorkbenchProfile **out_profile)
{
    UmiTraderWorkbenchProfile *profile;
    UmiStatus status;

    if (out_profile == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }

    *out_profile = NULL;
    profile = (UmiTraderWorkbenchProfile *)calloc(1U, sizeof(*profile));
    if (profile == NULL) {
        return UMI_STATUS_OUT_OF_MEMORY;
    }

    /*
     * The Framework is the single source of truth for panel semantics.  Trader
     * only selects the Trader half of the shared Trader/TMS workbench profile.
     */
    status = umi_workbench_selection_provider_trading_workbench_build(
        &profile->shared);
    if (status == UMI_STATUS_OK) {
        status = umi_workbench_selection_provider_trading_workbench_validate(
            &profile->shared);
    }
    if (status != UMI_STATUS_OK) {
        free(profile);
        return status;
    }

    if (strcmp(profile->shared.trader.application_id,
               "org.umicom.trader") != 0) {
        free(profile);
        return UMI_STATUS_INVALID_STATE;
    }

    profile->revision = 1U;
    *out_profile = profile;
    return UMI_STATUS_OK;
}

void umi_trader_workbench_profile_destroy(
    UmiTraderWorkbenchProfile *profile)
{
    free(profile);
}

UmiStatus umi_trader_workbench_profile_snapshot(
    const UmiTraderWorkbenchProfile *profile,
    UmiTraderWorkbenchProfileSnapshot *out_snapshot)
{
    UmiStatus status;

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
    if (status != UMI_STATUS_OK) return status;

    status = copy_text(
        out_snapshot->profile_id,
        sizeof(out_snapshot->profile_id),
        profile->shared.trader.profile_id);
    if (status != UMI_STATUS_OK) return status;

    status = copy_text(
        out_snapshot->title,
        sizeof(out_snapshot->title),
        profile->shared.trader.title);
    if (status != UMI_STATUS_OK) return status;

    status = copy_text(
        out_snapshot->trading_group_id,
        sizeof(out_snapshot->trading_group_id),
        profile->shared.trading_group_id);
    if (status != UMI_STATUS_OK) return status;

    status = copy_text(
        out_snapshot->operations_group_id,
        sizeof(out_snapshot->operations_group_id),
        profile->shared.operations_group_id);
    if (status != UMI_STATUS_OK) return status;

    out_snapshot->group_count = profile->shared.trader.group_count;
    out_snapshot->endpoint_count = profile->shared.trader.endpoint_count;
    out_snapshot->revision =
        profile->revision + profile->shared.trader.revision;

    return UMI_STATUS_OK;
}

const UmiWorkbenchContextHostProfile *
umi_trader_workbench_profile_context_host(
    const UmiTraderWorkbenchProfile *profile)
{
    return profile != NULL ? &profile->shared.trader : NULL;
}

const UmiWorkbenchContextSourceTradingProfile *
umi_trader_workbench_profile_context_sources(
    const UmiTraderWorkbenchProfile *profile)
{
    return profile != NULL ? &profile->shared.sources : NULL;
}
