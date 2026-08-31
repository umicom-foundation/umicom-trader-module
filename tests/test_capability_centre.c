/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: tests/test_capability_centre.c
 *
 * PURPOSE:
 *   Verify Trader capability truth comes from Framework experience and engines.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include <assert.h>

#include "umicom/trader/capability_centre.h"

/* Confirm new windows and unfinished work are both visible to the thin product. */
int main(void)
{
    UmiTraderCapabilityCentreSnapshot snapshot;
    UmiEngineRequirementReport engine_report;
    UmiUiViewModel *view = NULL;

    /* Trading baseline requires operational chart and matching engine services. */
    assert(umi_trader_capability_centre_snapshot(
        &snapshot, &engine_report) == UMI_STATUS_OK);
    assert(snapshot.panel_count >= 20U);
    assert(snapshot.layout_count >= 6U);
    assert(snapshot.planned_count > 0U);
    assert(snapshot.foundation_count > 0U);
    assert(snapshot.engine_requirements_ready);

    /* View construction proves a frontend can render the canonical capability list. */
    assert(umi_trader_capability_centre_view_create(
        "trader.capabilities", &view) == UMI_STATUS_OK);
    assert(view != NULL);
    umi_ui_view_model_destroy(view);
    return 0;
}
