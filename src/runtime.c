/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: src/runtime.c
 *
 * PURPOSE:
 *   Initialise the product through canonical Framework application runtime contracts.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/trader/runtime.h"

#include <string.h>

#include "umicom/application/experience_catalogue.h"
#include "umicom/trading_ui/trading_ui.h"

const UmiApplicationExperienceDefinition *umi_trader_runtime_experience(void)
{
    return umi_application_experience_catalogue_find("org.umicom.trader");
}

UmiStatus umi_trader_runtime_init(UmiApplicationWorkspaceRuntime *out_runtime)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_trader_runtime_experience();
    if (experience == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_workspace_runtime_init(experience, out_runtime);
}

UmiStatus umi_trader_runtime_health(
    UmiApplicationCapabilityProbe probe,
    void *user_data,
    UmiApplicationRuntimeHealth *out_health)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_trader_runtime_experience();
    if (experience == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_runtime_health_evaluate(
        experience, probe, user_data, out_health);
}

/* Compose only existing Framework trading factories for product panel identifiers. */
UmiStatus umi_trader_runtime_create_panel_view(
    const char *panel_id,
    UmiTradingWorkspace *workspace,
    UmiUiViewModel **out_view)
{
    if (panel_id == NULL || workspace == NULL || out_view == NULL)
        return UMI_STATUS_INVALID_ARGUMENT;

    /* Map only panels that already have canonical Framework trading factories.
     * Planned research/news/scanner views remain unimplemented instead of being duplicated here. */
    if (strcmp(panel_id, "watchlist") == 0)
        return umi_trading_ui_watchlist_view_create(panel_id, workspace, out_view);
    if (strcmp(panel_id, "chart") == 0)
        return umi_trading_ui_chart_view_create(panel_id, workspace, out_view);
    if (strcmp(panel_id, "depth") == 0)
        return umi_trading_ui_depth_view_create(panel_id, workspace, out_view);
    if (strcmp(panel_id, "order-entry") == 0)
        return umi_trading_ui_order_ticket_view_create(panel_id, workspace, out_view);
    if (strcmp(panel_id, "blotter") == 0)
        return umi_trading_ui_orders_view_create(panel_id, workspace, out_view);
    if (strcmp(panel_id, "positions") == 0 || strcmp(panel_id, "risk") == 0)
        return umi_trading_ui_portfolio_risk_view_create(panel_id, workspace, out_view);
    if (strcmp(panel_id, "account") == 0)
        return umi_trading_ui_dashboard_view_create(panel_id, workspace, out_view);
    return UMI_STATUS_NOT_IMPLEMENTED;
}
