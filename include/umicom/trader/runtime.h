/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: applications/trader/include/umicom/trader/runtime.h
 *
 * PURPOSE:
 *   Bind the thin Trader product to Framework application runtime, trading views and canonical suite layouts.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_TRADER_RUNTIME_H
#define UMICOM_TRADER_RUNTIME_H

#include "umicom/application/runtime/runtime.h"
#include "umicom/application/suite_layout/suite_layout.h"
#include "umicom/trading/workspace.h"
#include "umicom/ui/view_model.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialise trader runtime from caller-provided values so later operations receive a
 * known state.
 */
UmiStatus umi_trader_runtime_init(
    UmiApplicationWorkspaceRuntime *out_runtime);
/**
 * Provide the trader runtime health operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_runtime_health(
    UmiApplicationCapabilityProbe probe,
    void *user_data,
    UmiApplicationRuntimeHealth *out_health);
/**
 * Provide the trader runtime experience operation used by this module and its client
 * applications.
 */
const UmiApplicationExperienceDefinition *umi_trader_runtime_experience(void);
/**
 * Provide the trader runtime layout default operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_runtime_layout_default(
    UmiUiWorkspaceLayout *out_layout);
/**
 * Provide the trader runtime layout select operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_runtime_layout_select(
    const char *layout_id,
    UmiUiWorkspaceLayout *out_layout);
/* Resolve Trader panel identifiers to the existing Framework trading view
 * factories. The application only chooses composition; trading logic remains Framework-owned. */
UmiStatus umi_trader_runtime_create_panel_view(
    const char *panel_id,
    UmiTradingWorkspace *workspace,
    UmiUiViewModel **out_view);
/* Bridge canonical presentation component identities to the retained Trader
 * panel-id compatibility API. Unsupported future components remain explicit. */
UmiStatus umi_trader_runtime_create_component_view(
    const char *component_id,
    UmiTradingWorkspace *workspace,
    UmiUiViewModel **out_view);

#ifdef __cplusplus
}
#endif
#endif
