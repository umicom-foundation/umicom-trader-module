/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: src/production_panels.c
 *
 * PURPOSE:
 *   Project the Framework-owned application production control plane into this
 *   thin product without duplicating layout, readiness or acceptance logic.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/trader/production_panels.h"
#include <string.h>
/*
 * Provide the trader production panels build operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_production_panels_build(
    const UmiApplicationProductionRuntime *runtime,
    UmiTraderProductionPanelCatalogue *out_catalogue)
{
    size_t index;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (runtime == NULL || out_catalogue == NULL || !runtime->initialised ||
        strcmp(runtime->binding.experience->application_id,
               "org.umicom.trader") != 0)
        return UMI_STATUS_INVALID_ARGUMENT;
    (void)memset(out_catalogue, 0, sizeof(*out_catalogue));
    /* Visit each bounded item once so every record receives the same rule. */
    for (index = 0U; index < runtime->panels.count; ++index) {
        const UmiApplicationProductionPanelBinding *binding =
            &runtime->panels.entries[index];
        UmiTraderProductionPanelContribution *entry =
            &out_catalogue->entries[out_catalogue->count];
        entry->binding = binding;
        entry->workspace_area = binding->panel->default_region;
        entry->risk_sensitive =
            binding->required ||
            strstr(binding->panel->panel_id, "risk") != NULL ||
            strstr(binding->panel->panel_id, "order") != NULL;
        entry->read_only =
            (binding->panel->flags & UMI_EXPERIENCE_PANEL_READ_ONLY) != 0U;
        out_catalogue->risk_sensitive_count +=
            (size_t)entry->risk_sensitive;
        out_catalogue->read_only_count += (size_t)entry->read_only;
        out_catalogue->count += 1U;
    }
    return UMI_STATUS_OK;
}

