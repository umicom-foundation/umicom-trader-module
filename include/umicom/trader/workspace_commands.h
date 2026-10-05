/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: include/umicom/trader/workspace_commands.h
 *
 * PURPOSE:
 *   Expose product-facing layout, panel and context commands implemented by the Framework runtime.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_TRADER_WORKSPACE_COMMANDS_H
#define UMICOM_TRADER_WORKSPACE_COMMANDS_H

#include "umicom/application/runtime/context_review.h"
#include "umicom/trader/runtime.h"
#include "umicom/application/runtime/session_snapshot.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Provide the trader workspace select layout operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_workspace_select_layout(
    UmiApplicationWorkspaceRuntime *runtime,
    const char *layout_id);
/**
 * Provide the trader workspace activate panel operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_workspace_activate_panel(
    UmiApplicationWorkspaceRuntime *runtime,
    const char *panel_id);
/**
 * Provide the trader workspace set context operation used by this module and its client
 * applications.
 */
UmiStatus umi_trader_workspace_set_context(
    UmiApplicationWorkspaceRuntime *runtime,
    const char *group_id,
    const char *value);
/**
 * Provide the trader workspace commands operation used by this module and its client
 * applications.
 */
const UmiApplicationCommandSurface *umi_trader_workspace_commands(
    const UmiApplicationWorkspaceRuntime *runtime);

/* Capture, preview and apply this product's passive application session.
 * The session must borrow the canonical Umicom Trader experience. Preview resolves
 * every saved panel against that catalogue without changing a live session.
 * Apply requires the revision observed before review and advances it once.
 * These APIs perform no storage, GUI activation, build or trading operation.
 * A host prepares its presentation separately before publishing its session. */
UmiStatus umi_trader_workspace_session_capture(const UmiApplicationSession *session,
    void *bytes, size_t capacity, size_t *out_size);
UmiStatus umi_trader_workspace_session_preview(const void *bytes, size_t byte_count,
    UmiApplicationSession *out_candidate);
UmiStatus umi_trader_workspace_session_apply(UmiApplicationSession *session,
    uint64_t expected_revision, const void *bytes, size_t byte_count);

/* Review several linked groups without changing this product's runtime.
 * Framework owns validation, copied review rows and atomic UI publication.
 * The runtime must borrow this product's canonical experience. Use the shared
 * review summary/row functions to display the proposal, then explicitly apply.
 * Destroy the review after use; context values never authorise commands. */
UmiStatus umi_trader_workspace_context_review(
    UmiApplicationWorkspaceRuntime *runtime,
    const UmiApplicationContextChange *changes, size_t count,
    UmiApplicationContextReview **out_review);
UmiStatus umi_trader_workspace_context_apply(
    UmiApplicationWorkspaceRuntime *runtime, UmiApplicationContextReview *review);
/* Remove a linked group through the same Framework transaction. */
UmiStatus umi_trader_workspace_clear_context(
    UmiApplicationWorkspaceRuntime *runtime, const char *group_id);

#ifdef __cplusplus
}
#endif

#endif
