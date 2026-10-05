/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: src/workspace_commands.c
 *
 * PURPOSE:
 *   Forward product workspace actions into Framework-owned session and context orchestration.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include "umicom/trader/workspace_commands.h"
#include "umicom/application/experience_catalogue.h"

/*
 * Provide the trader workspace select layout operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_workspace_select_layout(
    UmiApplicationWorkspaceRuntime *runtime,
    const char *layout_id)
{
    return umi_application_workspace_runtime_select_layout(runtime, layout_id);
}

/*
 * Provide the trader workspace activate panel operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_workspace_activate_panel(
    UmiApplicationWorkspaceRuntime *runtime,
    const char *panel_id)
{
    return umi_application_workspace_runtime_activate_panel(runtime, panel_id);
}

/*
 * Provide the trader workspace set context operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_workspace_set_context(
    UmiApplicationWorkspaceRuntime *runtime,
    const char *group_id,
    const char *value)
{
    return umi_application_workspace_runtime_set_context(runtime, group_id, value);
}

/*
 * Provide the trader workspace commands operation used by this module and its client
 * applications.
 */
const UmiApplicationCommandSurface *umi_trader_workspace_commands(
    const UmiApplicationWorkspaceRuntime *runtime)
{
    return runtime != NULL ? &runtime->commands : NULL;
}

/* Product code supplies identity only. Framework owns the byte format,
 * complete validation, stable panel lifetimes and revision-checked update.
 * Adding another product should use its own canonical experience here. */
UmiStatus umi_trader_workspace_session_capture(const UmiApplicationSession *session,
    void *bytes, size_t capacity, size_t *out_size)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_application_experience_catalogue_find("org.umicom.trader");
    if (session == NULL || experience == NULL || session->experience != experience)
        return UMI_STATUS_INVALID_ARGUMENT;
    return umi_application_session_archive_capture(session, bytes, capacity, out_size);
}
UmiStatus umi_trader_workspace_session_preview(const void *bytes, size_t byte_count,
    UmiApplicationSession *out_candidate)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_application_experience_catalogue_find("org.umicom.trader");
    if (experience == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_session_archive_preview(experience, bytes, byte_count, out_candidate);
}
UmiStatus umi_trader_workspace_session_apply(UmiApplicationSession *session,
    uint64_t expected_revision, const void *bytes, size_t byte_count)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_application_experience_catalogue_find("org.umicom.trader");
    if (session == NULL || experience == NULL || session->experience != experience)
        return UMI_STATUS_INVALID_ARGUMENT;
    return umi_application_session_archive_apply(session, expected_revision, bytes, byte_count);
}

/* Product adapters contribute identity, not a second context store. Reuse the
 * Framework review so all applications have the same stale-state guarantees. */
UmiStatus umi_trader_workspace_context_review(
    UmiApplicationWorkspaceRuntime *runtime,
    const UmiApplicationContextChange *changes, size_t count,
    UmiApplicationContextReview **out_review)
{
    const UmiApplicationExperienceDefinition *experience = umi_trader_runtime_experience();
    if (runtime == NULL || experience == NULL || runtime->session.experience != experience)
        return UMI_STATUS_INVALID_ARGUMENT;
    return umi_application_context_review_prepare(runtime, changes, count, out_review);
}

UmiStatus umi_trader_workspace_context_apply(
    UmiApplicationWorkspaceRuntime *runtime, UmiApplicationContextReview *review)
{
    const UmiApplicationExperienceDefinition *experience = umi_trader_runtime_experience();
    if (runtime == NULL || experience == NULL || runtime->session.experience != experience)
        return UMI_STATUS_INVALID_ARGUMENT;
    return umi_application_context_review_apply(runtime, review);
}

UmiStatus umi_trader_workspace_clear_context(
    UmiApplicationWorkspaceRuntime *runtime, const char *group_id)
{
    const UmiApplicationExperienceDefinition *experience = umi_trader_runtime_experience();
    if (runtime == NULL || experience == NULL || runtime->session.experience != experience)
        return UMI_STATUS_INVALID_ARGUMENT;
    return umi_application_workspace_runtime_clear_context(runtime, group_id);
}
