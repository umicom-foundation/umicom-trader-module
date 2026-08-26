/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: include/umicom/trader/runtime.h
 *
 * PURPOSE:
 *   Bind the thin product to the Framework-owned application workspace runtime without duplicating services.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_TRADER_RUNTIME_H
#define UMICOM_TRADER_RUNTIME_H

#include "umicom/application/runtime/runtime.h"
#include "umicom/trading/workspace.h"
#include "umicom/ui/view_model.h"

#ifdef __cplusplus
extern "C" {
#endif

UmiStatus umi_trader_runtime_init(
    UmiApplicationWorkspaceRuntime *out_runtime);
UmiStatus umi_trader_runtime_health(
    UmiApplicationCapabilityProbe probe,
    void *user_data,
    UmiApplicationRuntimeHealth *out_health);
const UmiApplicationExperienceDefinition *umi_trader_runtime_experience(void);
/* Resolve Trader panel identifiers to the existing Framework trading view
 * factories. The application only chooses composition; trading logic remains Framework-owned. */
UmiStatus umi_trader_runtime_create_panel_view(
    const char *panel_id,
    UmiTradingWorkspace *workspace,
    UmiUiViewModel **out_view);

#ifdef __cplusplus
}
#endif

#endif
