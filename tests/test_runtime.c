/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: tests/test_runtime.c
 *
 * PURPOSE:
 *   Verify the thin product starts a Framework-owned workspace runtime.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include <assert.h>

#include "umicom/trader/runtime.h"

int main(void)
{
    UmiApplicationWorkspaceRuntime runtime;
    assert(umi_trader_runtime_init(&runtime) == UMI_STATUS_OK);
    assert(runtime.session.experience != NULL);
    assert(runtime.session.active_panel_count > 0U);
    assert(runtime.commands.command_count > 0U);
    return 0;
}
