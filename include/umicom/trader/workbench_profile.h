/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: include/umicom/trader/workbench_profile.h
 *
 * PURPOSE:
 *   Expose Trader's thin application composition over the Framework-owned
 *   linked-workbench trading profile.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_TRADER_WORKBENCH_PROFILE_H
#define UMICOM_TRADER_WORKBENCH_PROFILE_H

#include <stddef.h>
#include <stdint.h>

#include "umicom/workbench_selection_provider/trading_workbench.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UMI_TRADER_WORKBENCH_PROFILE_API_VERSION 1U

typedef struct UmiTraderWorkbenchProfile UmiTraderWorkbenchProfile;

typedef struct UmiTraderWorkbenchProfileSnapshot {
    uint32_t struct_size;
    uint32_t api_version;
    char application_id[UMI_WORKBENCH_CONTEXT_HOST_ID_CAPACITY];
    char profile_id[UMI_WORKBENCH_CONTEXT_HOST_ID_CAPACITY];
    char title[UMI_WORKBENCH_CONTEXT_HOST_TITLE_CAPACITY];
    char trading_group_id[UMI_WORKBENCH_CONTEXT_HOST_ID_CAPACITY];
    char operations_group_id[UMI_WORKBENCH_CONTEXT_HOST_ID_CAPACITY];
    size_t group_count;
    size_t endpoint_count;
    uint64_t revision;
} UmiTraderWorkbenchProfileSnapshot;

UmiStatus umi_trader_workbench_profile_create(
    UmiTraderWorkbenchProfile **out_profile);
void umi_trader_workbench_profile_destroy(
    UmiTraderWorkbenchProfile *profile);
UmiStatus umi_trader_workbench_profile_snapshot(
    const UmiTraderWorkbenchProfile *profile,
    UmiTraderWorkbenchProfileSnapshot *out_snapshot);

/*
 * These accessors return Framework-owned profile data held by this composition.
 * Ownership stays with UmiTraderWorkbenchProfile.
 */
const UmiWorkbenchContextHostProfile *
umi_trader_workbench_profile_context_host(
    const UmiTraderWorkbenchProfile *profile);
const UmiWorkbenchContextSourceTradingProfile *
umi_trader_workbench_profile_context_sources(
    const UmiTraderWorkbenchProfile *profile);

#ifdef __cplusplus
}
#endif
#endif
