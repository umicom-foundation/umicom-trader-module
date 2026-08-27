/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: tests/test_framework_views.c
 *
 * PURPOSE:
 *   Verify Trader composes existing Framework trading views instead of duplicating them.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/
#include <assert.h>
#include <stddef.h>

#include "umicom/trader/runtime.h"

int main(void)
{
    UmiTradingWorkspaceConfig config = umi_trading_workspace_config_default();
    UmiTradingWorkspace *workspace = NULL;
    UmiUiViewModel *view = NULL;

    assert(umi_trading_workspace_create(&config, &workspace) == UMI_STATUS_OK);
    static const char *const panel_ids[] = {
        "watchlist", "chart", "depth", "order-entry", "blotter",
        "positions", "risk", "account", "scanner", "predictive-lab",
        "news", "context-inspector", "strategy", "replay", "output"
    };
    size_t index;
    for (index = 0U; index < sizeof(panel_ids) / sizeof(panel_ids[0]); ++index) {
        assert(umi_trader_runtime_create_panel_view(
                   panel_ids[index], workspace, &view) == UMI_STATUS_OK);
        assert(view != NULL);
        umi_ui_view_model_destroy(view);
        view = NULL;
    }
    assert(umi_trader_runtime_create_panel_view(
               "unknown", workspace, &view) == UMI_STATUS_NOT_IMPLEMENTED);
    umi_trading_workspace_destroy(workspace);
    return 0;
}
