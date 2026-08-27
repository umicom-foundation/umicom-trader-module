/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: applications/trader/include/umicom/trader/gtk_workstation.h
 *
 * PURPOSE:
 *   Expose the thin Trader GTK4 product composition over the Framework-owned
 *   interactive trading suite workstation and canonical application layouts.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_TRADER_GTK_WORKSTATION_H
#define UMICOM_TRADER_GTK_WORKSTATION_H

#include <gtk/gtk.h>

#include "umicom/application/suite_layout/gtk4_workstation.h"
#include "umicom/trader/runtime.h"
#include "umicom/trading_ui/gtk4/trading_suite_workstation.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UmiTraderGtkWorkstation UmiTraderGtkWorkstation;

UmiStatus umi_trader_gtk_workstation_create(
    UmiTraderGtkWorkstation **out_workstation);
void umi_trader_gtk_workstation_destroy(
    UmiTraderGtkWorkstation *workstation);
GtkWidget *umi_trader_gtk_workstation_widget(
    UmiTraderGtkWorkstation *workstation);
UmiStatus umi_trader_gtk_workstation_select_layout(
    UmiTraderGtkWorkstation *workstation,
    const char *layout_id);
UmiApplicationSuiteGtk4WorkstationSnapshot
umi_trader_gtk_workstation_snapshot(
    const UmiTraderGtkWorkstation *workstation);
UmiStatus umi_trader_gtk_workstation_trading_snapshot(
    UmiTraderGtkWorkstation *workstation,
    UmiTradingWorkspaceSnapshot *out_snapshot);

#ifdef __cplusplus
}
#endif
#endif
