/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: src/capability_centre.c
 *
 * PURPOSE:
 *   Implement a truthful Trader capability projection using only Framework
 *   experience and engine contracts so unfinished features remain visible.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/trader/capability_centre.h"

#include <stdio.h>
#include <string.h>

#include "umicom/application/experiences/trader.h"

/* Set one copied string property through the shared UI value contract. */
static UmiStatus set_string(
    UmiUiViewModel *view,
    const char *key,
    const char *text)
{
    UmiUiValue value;
    UmiStatus status;
    if (view == NULL || key == NULL || text == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    status = umi_ui_value_set_string(&value, text);
    return status == UMI_STATUS_OK
        ? umi_ui_view_model_set_property(view, key, &value) : status;
}

/* Set a size count only when the common signed UI representation can hold it. */
static UmiStatus set_count(
    UmiUiViewModel *view,
    const char *key,
    size_t count)
{
    UmiUiValue value;
    UmiStatus status;
    if (count > (size_t)INT64_MAX) return UMI_STATUS_CAPACITY_EXCEEDED;
    status = umi_ui_value_set_integer(&value, (int64_t)count);
    return status == UMI_STATUS_OK
        ? umi_ui_view_model_set_property(view, key, &value) : status;
}

/* Calculate feature states and engine readiness from canonical Framework data. */
UmiStatus umi_trader_capability_centre_snapshot(
    UmiTraderCapabilityCentreSnapshot *out_snapshot,
    UmiEngineRequirementReport *out_engine_report)
{
    const UmiApplicationExperienceDefinition *experience;
    UmiEngineRequirement requirements[] = {
        {UMI_ENGINE_CHART_ANALYTICS, UMI_ENGINE_MATURITY_OPERATIONAL,
         UMI_ENGINE_CAPABILITY_CHART_WORKSPACE |
             UMI_ENGINE_CAPABILITY_ANALYTICS},
        {UMI_ENGINE_TRADING_MATCHING, UMI_ENGINE_MATURITY_OPERATIONAL,
         UMI_ENGINE_CAPABILITY_ORDER_MATCHING |
             UMI_ENGINE_CAPABILITY_RISK_CONTROLS},
        {UMI_ENGINE_DATA_CONNECTIVITY, UMI_ENGINE_MATURITY_FOUNDATION,
         UMI_ENGINE_CAPABILITY_SQL_CONNECTOR |
             UMI_ENGINE_CAPABILITY_NOSQL_CONNECTOR}
    };
    size_t index;
    UmiStatus status;
    if (out_snapshot == NULL || out_engine_report == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    (void)memset(out_snapshot, 0, sizeof(*out_snapshot));
    experience = umi_application_experience_trader();
    /* A missing canonical definition is an internal composition error. */
    if (experience == NULL) return UMI_STATUS_INTERNAL_ERROR;
    out_snapshot->panel_count = experience->panel_count;
    out_snapshot->layout_count = experience->layout_count;
    out_snapshot->feature_count = experience->feature_count;
    for (index = 0U; index < experience->feature_count; ++index) {
        /* Every state has a dedicated count so planned work cannot disappear. */
        switch (experience->features[index].state) {
            case UMI_EXPERIENCE_FEATURE_PLANNED:
                out_snapshot->planned_count += 1U;
                break;
            case UMI_EXPERIENCE_FEATURE_FOUNDATION:
                out_snapshot->foundation_count += 1U;
                break;
            case UMI_EXPERIENCE_FEATURE_IMPLEMENTED:
                out_snapshot->implemented_count += 1U;
                break;
            case UMI_EXPERIENCE_FEATURE_VERIFIED:
                out_snapshot->verified_count += 1U;
                break;
            default:
                return UMI_STATUS_INTERNAL_ERROR;
        }
    }
    status = umi_engine_catalogue_validate(
        requirements, sizeof(requirements) / sizeof(requirements[0]),
        out_engine_report);
    if (status != UMI_STATUS_OK) return status;
    out_snapshot->engine_requirements_ready = out_engine_report->ready;
    return UMI_STATUS_OK;
}

/* Build a dockable capability panel that lists available and unfinished work. */
UmiStatus umi_trader_capability_centre_view_create(
    const char *view_id,
    UmiUiViewModel **out_view)
{
    const UmiApplicationExperienceDefinition *experience;
    UmiTraderCapabilityCentreSnapshot snapshot;
    UmiEngineRequirementReport engine_report;
    UmiStatus status;
    size_t index;
    if (view_id == NULL || view_id[0] == '\0' || out_view == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    *out_view = NULL;
    status = umi_trader_capability_centre_snapshot(&snapshot, &engine_report);
    if (status != UMI_STATUS_OK) return status;
    experience = umi_application_experience_trader();
    status = umi_ui_view_model_create(
        view_id, "umicom.trader.capability-centre", UMI_UI_ROLE_PANE, out_view);
    if (status == UMI_STATUS_OK) status = set_string(
        *out_view, "title", "Trading Capability Centre");
    if (status == UMI_STATUS_OK) status = set_string(
        *out_view, "summary",
        "Available, foundational and planned trading windows and services.");
    if (status == UMI_STATUS_OK) status = set_count(
        *out_view, "capability.panel-count", snapshot.panel_count);
    if (status == UMI_STATUS_OK) status = set_count(
        *out_view, "capability.layout-count", snapshot.layout_count);
    if (status == UMI_STATUS_OK) status = set_count(
        *out_view, "capability.planned-count", snapshot.planned_count);
    if (status == UMI_STATUS_OK) status = set_count(
        *out_view, "capability.foundation-count", snapshot.foundation_count);

    for (index = 0U; status == UMI_STATUS_OK &&
         index < experience->feature_count; ++index) {
        const UmiExperienceFeatureDefinition *feature =
            &experience->features[index];
        char key[64];
        char row[512];
        int key_written;
        int row_written;
        /* Rows are copied and truncation is rejected for unambiguous diagnostics. */
        key_written = snprintf(key, sizeof(key), "capability.row.%zu", index);
        row_written = snprintf(row, sizeof(row), "%s — %s — %s",
            feature->title, umi_experience_feature_state_text(feature->state),
            feature->summary);
        if (key_written < 0 || (size_t)key_written >= sizeof(key) ||
            row_written < 0 || (size_t)row_written >= sizeof(row)) {
            status = UMI_STATUS_CAPACITY_EXCEEDED;
        } else {
            status = set_string(*out_view, key, row);
        }
    }
    /* Never return a partially populated capability view to a GUI adapter. */
    if (status != UMI_STATUS_OK && *out_view != NULL) {
        umi_ui_view_model_destroy(*out_view);
        *out_view = NULL;
    }
    return status;
}
