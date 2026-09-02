/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: src/production_commands.c
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
#include "umicom/trader/production_commands.h"
#include <string.h>
/*
 * Provide the trader production commands build operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_production_commands_build(
    UmiTraderProductionCommandCatalogue *out_catalogue)
{
    size_t index;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (out_catalogue == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    (void)memset(out_catalogue, 0, sizeof(*out_catalogue));
    /* Visit each bounded item once so every record receives the same rule. */
    for (index = 0U;
         index < umi_application_production_control_command_count();
         ++index) {
        const UmiApplicationProductionControlCommand *command =
            umi_application_production_control_command_at(index);
        UmiTraderProductionCommandContribution *entry;
        /* Use the shared build helper when it is available from the parent composition. */
        if (command == NULL || out_catalogue->count >= 32U)
            return UMI_STATUS_CAPACITY_EXCEEDED;
        entry = &out_catalogue->entries[out_catalogue->count];
        entry->command = command;
        entry->menu_path = strcmp(command->category, "Quality") == 0
            ? "Trading/Controls/Acceptance"
            : "Trading/Workspace/Production";
        entry->allowed_during_live_session = !command->mutates_state;
        out_catalogue->live_safe_count +=
            (size_t)entry->allowed_during_live_session;
        out_catalogue->count += 1U;
    }
    return UMI_STATUS_OK;
}

