/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: include/umicom/trader/capability_centre.h
 *
 * PURPOSE:
 *   Project Framework-owned Trader panels, layouts, feature maturity and engine
 *   readiness into a thin capability view without duplicating trading logic.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_TRADER_CAPABILITY_CENTRE_H
#define UMICOM_TRADER_CAPABILITY_CENTRE_H

#include "umicom/umicom.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Snapshot counts are calculated from the canonical Framework experience. */
typedef struct UmiTraderCapabilityCentreSnapshot {
    size_t panel_count;
    size_t layout_count;
    size_t feature_count;
    size_t planned_count;
    size_t foundation_count;
    size_t implemented_count;
    size_t verified_count;
    bool engine_requirements_ready;
} UmiTraderCapabilityCentreSnapshot;

/* Count capability maturity and validate the shared trading engine baseline. */
UmiStatus umi_trader_capability_centre_snapshot(
    UmiTraderCapabilityCentreSnapshot *out_snapshot,
    UmiEngineRequirementReport *out_engine_report);

/* Build a movable toolkit-neutral capability panel from Framework definitions. */
UmiStatus umi_trader_capability_centre_view_create(
    const char *view_id,
    UmiUiViewModel **out_view);

#ifdef __cplusplus
}
#endif

#endif
