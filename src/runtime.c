/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: applications/trader/src/runtime.c
 *
 * PURPOSE:
 *   Initialise Trader through canonical Framework application, layout and trading-view runtime contracts.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include "umicom/trader/runtime.h"

#include <string.h>

#include "umicom/application/experience_catalogue.h"
#include "umicom/trading_ui/trading_ui.h"

/*
 * Provide the trader runtime experience operation used by this module and its client
 * applications.
 */
const UmiApplicationExperienceDefinition *umi_trader_runtime_experience(void)
{
    return umi_application_experience_catalogue_find("org.umicom.trader");
}

/*
 * Initialise trader runtime from caller-provided values so later operations receive a
 * known state.
 */
UmiStatus umi_trader_runtime_init(UmiApplicationWorkspaceRuntime *out_runtime)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_trader_runtime_experience();
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (experience == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_workspace_runtime_init(experience, out_runtime);
}

/*
 * Provide the trader runtime layout default operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_runtime_layout_default(
    UmiUiWorkspaceLayout *out_layout)
{
    return umi_application_suite_layout_project_default(
        "org.umicom.trader", out_layout);
}

/*
 * Provide the trader runtime layout select operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_runtime_layout_select(
    const char *layout_id,
    UmiUiWorkspaceLayout *out_layout)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_trader_runtime_experience();
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (layout_id == NULL || out_layout == NULL)
        return UMI_STATUS_INVALID_ARGUMENT;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (experience == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_suite_layout_project(experience, layout_id, out_layout);
}

/*
 * Provide the trader runtime health operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_runtime_health(
    UmiApplicationCapabilityProbe probe,
    void *user_data,
    UmiApplicationRuntimeHealth *out_health)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_trader_runtime_experience();
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
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
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (panel_id == NULL || workspace == NULL || out_view == NULL)
        return UMI_STATUS_INVALID_ARGUMENT;

    /* Map only panels that have canonical Framework trading factories. Trader
     * contributes identifiers and never duplicates their data projections. */
    if (strcmp(panel_id, "watchlist") == 0)
        return umi_trading_ui_watchlist_view_create(panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "chart") == 0)
        return umi_trading_ui_chart_view_create(panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "depth") == 0)
        return umi_trading_ui_depth_view_create(panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "order-entry") == 0)
        return umi_trading_ui_order_ticket_view_create(panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "blotter") == 0)
        return umi_trading_ui_orders_view_create(panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "positions") == 0 || strcmp(panel_id, "risk") == 0)
        return umi_trading_ui_portfolio_risk_view_create(panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "account") == 0)
        return umi_trading_ui_dashboard_view_create(panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "scanner") == 0)
        return umi_trading_ui_scanner_view_create(panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "predictive-lab") == 0)
        return umi_trading_ui_predictive_lab_view_create(panel_id, workspace,
                                                          out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "news") == 0)
        return umi_trading_ui_news_view_create(panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "context-inspector") == 0)
        return umi_trading_ui_context_inspector_view_create(
            panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "strategy") == 0)
        return umi_trading_ui_strategy_view_create(panel_id, workspace,
                                                    out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "replay") == 0)
        return umi_trading_ui_replay_view_create(panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "output") == 0)
        return umi_trading_ui_research_output_view_create(
            panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "time-and-sales") == 0)
        return umi_trading_ui_time_and_sales_view_create(
            panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "economic-calendar") == 0)
        return umi_trading_ui_economic_calendar_view_create(
            panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "fundamentals") == 0)
        return umi_trading_ui_fundamentals_view_create(
            panel_id, workspace, out_view);
    /* Analysis and performance panels reuse established strategy and research
     * projections until their specialised engines contribute richer evidence. */
    if (strcmp(panel_id, "strategy-analysis") == 0)
        return umi_trading_ui_strategy_view_create(
            panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "trade-performance") == 0)
        return umi_trading_ui_research_output_view_create(
            panel_id, workspace, out_view);
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(panel_id, "price-ladder") == 0)
        return umi_trading_ui_depth_view_create(
            panel_id, workspace, out_view);
    return UMI_STATUS_NOT_IMPLEMENTED;
}

/*
 * Provide the trader runtime create component view operation used by this module and its
 * client applications.
 */
UmiStatus umi_trader_runtime_create_component_view(
    const char *component_id,
    UmiTradingWorkspace *workspace,
    UmiUiViewModel **out_view)
{
    const char *panel_id = NULL;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (component_id == NULL || workspace == NULL || out_view == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.trading.watchlist") == 0)
        panel_id = "watchlist";
    else /* Use the stable identifier comparison to choose the matching record or policy. */ if (strcmp(component_id, "umicom.trading.chart") == 0)
        panel_id = "chart";
    else /* Use the stable identifier comparison to choose the matching record or policy. */ if (strcmp(component_id, "umicom.trading.market-depth") == 0)
        panel_id = "depth";
    else /* Use the stable identifier comparison to choose the matching record or policy. */ if (strcmp(component_id, "umicom.trading.order-ticket") == 0)
        panel_id = "order-entry";
    else /* Use the stable identifier comparison to choose the matching record or policy. */ if (strcmp(component_id, "umicom.trading.orders") == 0)
        panel_id = "blotter";
    else /* Use the stable identifier comparison to choose the matching record or policy. */ if (strcmp(component_id, "umicom.trading.portfolio") == 0)
        panel_id = "positions";
    else /* Use the stable identifier comparison to choose the matching record or policy. */ if (strcmp(component_id, "umicom.trading.risk") == 0)
        panel_id = "risk";
    else /* Use the stable identifier comparison to choose the matching record or policy. */ if (strcmp(component_id, "umicom.trading.strategy") == 0)
        panel_id = "strategy";
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (panel_id == NULL) return UMI_STATUS_NOT_IMPLEMENTED;
    return umi_trader_runtime_create_panel_view(panel_id, workspace, out_view);
}
