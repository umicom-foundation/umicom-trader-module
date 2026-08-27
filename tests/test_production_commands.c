/* Umicom Trader production control-plane test | Sammy Hegab | Umicom Foundation | MIT */
#include <assert.h>
#include "umicom/trader/production_commands.h"
int main(void) {
    UmiTraderProductionCommandCatalogue commands;
    assert(umi_trader_production_commands_build(&commands) == UMI_STATUS_OK);
    assert(commands.count == umi_application_production_control_command_count());
    assert(commands.live_safe_count < commands.count);
    return 0;
}

