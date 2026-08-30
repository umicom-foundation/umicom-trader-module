/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: tests/test_experience.c
 *
 * PURPOSE:
 *   Verify the existing product composition resolves its Framework-owned experience definition.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include <assert.h>

#include "umicom/trader/experience.h"

int main(void)
{
    UmiApplicationExperienceStatus status;
    const UmiApplicationExperienceDefinition *definition =
        umi_trader_experience();
    assert(definition != NULL);
    assert(umi_application_experience_validate(definition) == UMI_STATUS_OK);
    assert(umi_trader_experience_status(&status) == UMI_STATUS_OK);
    assert(status.layout_count > 0U);
    assert(status.panel_count > 0U);
    return 0;
}
