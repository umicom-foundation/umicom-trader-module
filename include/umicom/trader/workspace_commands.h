/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: include/umicom/trader/workspace_commands.h
 *
 * PURPOSE:
 *   Expose product-facing layout, panel and context commands implemented by the Framework runtime.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_TRADER_WORKSPACE_COMMANDS_H
#define UMICOM_TRADER_WORKSPACE_COMMANDS_H

#include "umicom/trader/runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Provide the trader workspace select layout operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_workspace_select_layout(
    UmiApplicationWorkspaceRuntime *runtime,
    const char *layout_id);
/**
 * Provide the trader workspace activate panel operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_workspace_activate_panel(
    UmiApplicationWorkspaceRuntime *runtime,
    const char *panel_id);
/**
 * Provide the trader workspace set context operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_workspace_set_context(
    UmiApplicationWorkspaceRuntime *runtime,
    const char *group_id,
    const char *value);
/**
 * Provide the trader workspace commands operation used by this module and its client
 * applications.
 */
const UmiApplicationCommandSurface *umi_trader_workspace_commands(
    const UmiApplicationWorkspaceRuntime *runtime);

#ifdef __cplusplus
}
#endif

#endif
