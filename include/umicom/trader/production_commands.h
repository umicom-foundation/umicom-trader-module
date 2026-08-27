/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: include/umicom/trader/production_commands.h
 *
 * PURPOSE:
 *   Project the Framework-owned application production control plane into this
 *   thin product without duplicating layout, readiness or acceptance logic.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_TRADER_PRODUCTION_COMMANDS_H
#define UMICOM_TRADER_PRODUCTION_COMMANDS_H
#include "umicom/trader/production_profile.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct UmiTraderProductionCommandContribution {
    const UmiApplicationProductionControlCommand *command;
    const char *menu_path;
    int allowed_during_live_session;
} UmiTraderProductionCommandContribution;
typedef struct UmiTraderProductionCommandCatalogue {
    UmiTraderProductionCommandContribution entries[32];
    size_t count;
    size_t live_safe_count;
} UmiTraderProductionCommandCatalogue;
UmiStatus umi_trader_production_commands_build(
    UmiTraderProductionCommandCatalogue *out_catalogue);
#ifdef __cplusplus
}
#endif
#endif

