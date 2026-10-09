/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: tests/test_layout_library_order.c
 * PURPOSE: Exercise layout ordering and saved-library restoration in real Trader.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/
#include "umicom/ui/gtk4/automation.h"
#include "umicom/trader/gtk_workstation.h"
#include <stdio.h>
#include <string.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #x); failed = 1; goto cleanup; } } while (0)

/* Locate the public automation tag on native controls, including popovers. */
/* The chart inspector owns controls while collapsed. Shared logical lookup replaces the rendered-child walk, retained here for review. The previous implementation is retained for engineering review. */
#if 0
static GtkWidget *Find(GtkWidget *root, const char *id)
{
    if (root == NULL) return NULL;
    if (g_strcmp0(g_object_get_data(G_OBJECT(root), "umicom-automation-id"), id) == 0) return root;
    for (GtkWidget *child = gtk_widget_get_first_child(root); child != NULL; child = gtk_widget_get_next_sibling(child)) {
        GtkWidget *found = Find(child, id);
        if (found != NULL) return found;
    }
    return NULL;
}
#endif
static GtkWidget *Find(GtkWidget *root, const char *id)
{
    /* Read controls owned by collapsed chart inspectors without changing layout. */
    return umi_gtk4_automation_find_tagged_widget(root, id);
}

static void Drain(void)
{
    for (size_t n = 0U; n < 128U && g_main_context_pending(NULL); ++n)
        (void)g_main_context_iteration(NULL, FALSE);
}

/* Native row activation must use the real application owner and preserve the
 * rest of its layout list. No window is presented and no persistence is asked for. */
static int VerifyKeyboardLayouts(UmiTraderGtkWorkstation *workstation, GtkWidget *popover)
{
    UmiUiWorkspaceLibrarySnapshot before, selected, opened, restored;
    GtkWidget *list = Find(popover, "workstation.layout-library.list");
    size_t original = SIZE_MAX, target = SIZE_MAX;
    int failed = 0;
    CHECK(GTK_IS_LIST_BOX(list));
    CHECK(!gtk_list_box_get_activate_on_single_click(GTK_LIST_BOX(list)));
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &before) == UMI_STATUS_OK);
    for (size_t i = 0U; i < before.layout_count; ++i) {
        if (before.rows[i].active) original = i;
        else if (target == SIZE_MAX) target = i;
    }
    CHECK(original != SIZE_MAX && target != SIZE_MAX);
    GtkListBoxRow *row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(list), (int)target);
    CHECK(row != NULL);
    gtk_list_box_select_row(GTK_LIST_BOX(list), row); Drain();
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &selected) == UMI_STATUS_OK);
    CHECK(selected.customisation_revision == before.customisation_revision && selected.rows[original].active);
    g_signal_emit_by_name(list, "row-activated", row); Drain();
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &opened) == UMI_STATUS_OK);
    CHECK(opened.rows[target].active && opened.customisation_revision == before.customisation_revision + 1U);
    row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(list), (int)original);
    CHECK(row != NULL); g_signal_emit_by_name(list, "row-activated", row); Drain();
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &restored) == UMI_STATUS_OK);
    CHECK(restored.rows[original].active && restored.layout_count == before.layout_count);
    for (size_t i = 0U; i < before.layout_count; ++i) {
        CHECK(strcmp(restored.rows[i].layout_id, before.rows[i].layout_id) == 0);
        CHECK(strcmp(restored.rows[i].name, before.rows[i].name) == 0);
    }
cleanup:
    return failed;
}

/* Duplicate through the real native controls, then save, remove and restore
 * through isolated fixture storage. The source identity and panel count survive. */
static int VerifyAutomaticLayoutCopy(UmiTraderGtkWorkstation *workstation, GtkWidget *popover)
{
    UmiUiWorkspaceLibrarySnapshot before, copied, restored;
    UmiUiWorkspaceLibraryCopySuggestion proposal;
    UmiUiWorkspaceLibraryRequest remove = {0};
    GtkWidget *list = Find(popover, "workstation.layout-library.list");
    GtkWidget *search = Find(popover, "workstation.layout-library.search");
    GtkWidget *id = Find(popover, "workstation.layout-library.new-id");
    GtkWidget *name = Find(popover, "workstation.layout-library.name");
    GtkWidget *duplicate = Find(popover, "workstation.layout-library.duplicate");
    GtkWidget *save = Find(popover, "workstation.layout-library.save-library");
    GtkWidget *restore = Find(popover, "workstation.layout-library.restore-library");
    GtkWidget *confirm = Find(popover, "workstation.layout-library.confirm-restore");
    size_t source = 0U;
    int failed = 0;
    CHECK(GTK_IS_LIST_BOX(list) && GTK_IS_SEARCH_ENTRY(search));
    CHECK(GTK_IS_ENTRY(id) && GTK_IS_ENTRY(name) && GTK_IS_BUTTON(duplicate));
    CHECK(GTK_IS_BUTTON(save) && GTK_IS_BUTTON(restore) && GTK_IS_CHECK_BUTTON(confirm));
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &before) == UMI_STATUS_OK);
    CHECK(before.layout_count > 0U && before.layout_count < UMI_UI_CUSTOM_WORKSPACE_MAX_LAYOUTS);
    for (size_t row = 0U; row < before.layout_count; ++row)
        if (before.rows[row].active) source = row;
    CHECK(umi_ui_workspace_library_suggest_copy(&before, before.rows[source].layout_id, &proposal) == UMI_STATUS_OK);
    gtk_editable_set_text(GTK_EDITABLE(search), ""); g_signal_emit_by_name(search, "search-changed");
    gtk_list_box_select_row(GTK_LIST_BOX(list), gtk_list_box_get_row_at_index(GTK_LIST_BOX(list), (int)source));
    gtk_editable_set_text(GTK_EDITABLE(id), "");
    gtk_editable_set_text(GTK_EDITABLE(name), "My saved layout copy");
    CHECK(gtk_widget_get_sensitive(duplicate));
    g_signal_emit_by_name(duplicate, "clicked"); Drain();
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &copied) == UMI_STATUS_OK);
    CHECK(copied.layout_count == before.layout_count + 1U);
    CHECK(copied.customisation_revision == before.customisation_revision + 1U);
    CHECK(strcmp(copied.rows[before.layout_count].layout_id, proposal.new_layout_id) == 0);
    CHECK(strcmp(copied.rows[before.layout_count].name, "My saved layout copy") == 0);
    CHECK(copied.rows[before.layout_count].active);
    CHECK(copied.rows[before.layout_count].window_count == before.rows[source].window_count);
    for (size_t row = 0U; row < before.layout_count; ++row) {
        CHECK(strcmp(copied.rows[row].layout_id, before.rows[row].layout_id) == 0);
        CHECK(strcmp(copied.rows[row].name, before.rows[row].name) == 0);
        CHECK(copied.rows[row].window_count == before.rows[row].window_count);
    }

    CHECK(gtk_widget_get_sensitive(save));
    g_signal_emit_by_name(save, "clicked"); Drain();
    remove.action = UMI_UI_WORKSPACE_LIBRARY_REMOVE;
    remove.target_layout_id = proposal.new_layout_id;
    remove.expected_customisation_revision = copied.customisation_revision;
    remove.confirmed = true;
    CHECK(umi_trader_gtk_workstation_library_apply(workstation, &remove) == UMI_STATUS_OK);
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &restored) == UMI_STATUS_OK);
    CHECK(restored.layout_count == before.layout_count);
    gtk_check_button_set_active(GTK_CHECK_BUTTON(confirm), TRUE);
    CHECK(gtk_widget_get_sensitive(restore));
    g_signal_emit_by_name(restore, "clicked"); Drain();
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &restored) == UMI_STATUS_OK);
    CHECK(restored.layout_count == copied.layout_count);
    for (size_t row = 0U; row < copied.layout_count; ++row) {
        CHECK(strcmp(restored.rows[row].layout_id, copied.rows[row].layout_id) == 0);
        CHECK(strcmp(restored.rows[row].name, copied.rows[row].name) == 0);
        CHECK(restored.rows[row].active == copied.rows[row].active);
    }
    CHECK(!gtk_check_button_get_active(GTK_CHECK_BUTTON(confirm)));

cleanup:
    return failed;
}

/* Product import uses the existing native publisher. An earlier review is
 * refused after a rename, while a fresh review restores the exported list.
 * The enclosing test checks that order/execution/account observations survive. */
static int VerifyPortableLibrary(UmiTraderGtkWorkstation *workstation, GtkWidget *popover)
{
    UmiUiWorkspaceLibrarySnapshot original, renamed, restored;
    UmiUiWorkspaceLibraryImport *review = NULL;
    char *bytes = NULL;
    size_t size = 0U;
    int failed = 0;
    CHECK(Find(popover, "workstation.layout-library.export") != NULL);
    CHECK(Find(popover, "workstation.layout-library.import") != NULL);
    CHECK(Find(popover, "workstation.layout-library.import-details") != NULL);
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &original) == UMI_STATUS_OK);
    CHECK(original.layout_count != 0U);
    CHECK(umi_trader_gtk_workstation_library_export(workstation, NULL, 0U, &size) == UMI_STATUS_OK);
    bytes = g_try_malloc(size + 1U); CHECK(bytes != NULL);
    CHECK(umi_trader_gtk_workstation_library_export(workstation, bytes, size + 1U, &size) == UMI_STATUS_OK);
    CHECK(umi_trader_gtk_workstation_library_import_review(workstation, bytes, size, &review) == UMI_STATUS_OK);
    UmiUiWorkspaceLibraryRequest rename = {UMI_UI_WORKSPACE_LIBRARY_RENAME,
        original.rows[0].layout_id, NULL, "Layout changed after review", original.customisation_revision, false};
    CHECK(umi_trader_gtk_workstation_library_apply(workstation, &rename) == UMI_STATUS_OK);
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &renamed) == UMI_STATUS_OK);
    CHECK(umi_trader_gtk_workstation_library_import_apply(workstation, review) == UMI_STATUS_INVALID_STATE);
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &restored) == UMI_STATUS_OK);
    CHECK(restored.customisation_revision == renamed.customisation_revision);
    CHECK(strcmp(restored.rows[0].name, renamed.rows[0].name) == 0);
    umi_ui_workspace_library_import_destroy(review); review = NULL;
    CHECK(umi_trader_gtk_workstation_library_import_review(workstation, bytes, size, &review) == UMI_STATUS_OK);
    /* Product reviews expose Framework's copied geometry while retaining
     * the product's existing publisher and document/business state checks. */
    CHECK(umi_ui_workspace_library_import_layout_count(review, true) == original.layout_count);
    for (size_t index = 0U; index < original.layout_count; ++index) {
        const UmiUiWorkspaceLayout *before = umi_ui_workspace_library_import_layout(review, false, index);
        const UmiUiWorkspaceLayout *after = umi_ui_workspace_library_import_layout(review, true, index);
        CHECK(before && after && !strcmp(after->layout_id, original.rows[index].layout_id));
        CHECK(!strcmp(before->name, renamed.rows[index].name));
        CHECK(!strcmp(after->name, original.rows[index].name));
        CHECK(after->window_count == original.rows[index].window_count);
    }
    bytes[0] = '!';
    CHECK(umi_trader_gtk_workstation_library_import_apply(workstation, review) == UMI_STATUS_OK);
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &restored) == UMI_STATUS_OK);
    CHECK(restored.layout_count == original.layout_count && restored.customisation_revision == renamed.customisation_revision + 1U);
    for (size_t index = 0U; index < original.layout_count; ++index) {
        CHECK(strcmp(restored.rows[index].layout_id, original.rows[index].layout_id) == 0);
        CHECK(strcmp(restored.rows[index].name, original.rows[index].name) == 0);
        CHECK(restored.rows[index].active == original.rows[index].active);
        CHECK(restored.rows[index].window_count == original.rows[index].window_count);
    }
    /* Undo the imported arrangement, then redo it through the real button.
     * The surrounding fixture verifies document/trading state remains intact. */
    UmiUiWorkspaceLibraryHistoryState history;
    CHECK(umi_trader_gtk_workstation_library_history_read(workstation, &history) == UMI_STATUS_OK);
    CHECK(history.undo_count != 0U && history.redo_count == 0U && !history.stale);
    CHECK(umi_trader_gtk_workstation_library_history_navigate(workstation, UMI_UI_WORKSPACE_LIBRARY_HISTORY_UNDO,
        renamed.customisation_revision) == UMI_STATUS_INVALID_STATE);
    GtkWidget *undo = Find(popover, "workstation.layout-library.undo");
    GtkWidget *redo = Find(popover, "workstation.layout-library.redo");
    CHECK(GTK_IS_BUTTON(undo) && GTK_IS_BUTTON(redo) && gtk_widget_get_sensitive(undo));
    g_signal_emit_by_name(undo, "clicked"); Drain();
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &restored) == UMI_STATUS_OK);
    CHECK(strcmp(restored.rows[0].name, renamed.rows[0].name) == 0);
    CHECK(gtk_widget_get_sensitive(redo));
    g_signal_emit_by_name(redo, "clicked"); Drain();
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &restored) == UMI_STATUS_OK);
    CHECK(strcmp(restored.rows[0].name, original.rows[0].name) == 0);
    CHECK(umi_trader_gtk_workstation_library_history_read(workstation, &history) == UMI_STATUS_OK && history.redo_count == 0U);

cleanup:
    umi_ui_workspace_library_import_destroy(review); g_free(bytes); return failed;
}

int main(void)
{
    UmiTraderGtkWorkstation *workstation = NULL;
    UmiDataServer *server = NULL;
    UmiUiWorkspaceLibrarySnapshot before, ordered, after;
    UmiTradingWorkspaceSnapshot trading_before, trading_after;
    UmiApplicationSuiteGtk4WorkstationSnapshot native_before, native_after;
    UmiUiWorkspaceLibraryRequest request = {0};
    GtkWidget *root, *menu, *popover, *save, *restore, *confirm, *list, *up, *down;
    int failed = 0;
    if (!gtk_init_check()) return 77;
    CHECK(umi_trader_gtk_workstation_create(&workstation) == UMI_STATUS_OK);
    CHECK(umi_data_server_create_memory(&server) == UMI_STATUS_OK);
    CHECK(umi_trader_gtk_workstation_bind_checkpoint_storage(workstation, server) == UMI_STATUS_OK);
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &before) == UMI_STATUS_OK);
    CHECK(before.layout_count > 1U);
    CHECK(umi_trader_gtk_workstation_trading_snapshot(workstation, &trading_before) == UMI_STATUS_OK);
    native_before = umi_trader_gtk_workstation_snapshot(workstation);
    root = umi_trader_gtk_workstation_widget(workstation);
    menu = Find(root, "umicom.layout.library");
    CHECK(GTK_IS_MENU_BUTTON(menu));
    popover = GTK_WIDGET(gtk_menu_button_get_popover(GTK_MENU_BUTTON(menu)));
    CHECK(popover != NULL);
    list = Find(popover, "workstation.layout-library.list");
    up = Find(popover, "workstation.layout-library.move-up");
    down = Find(popover, "workstation.layout-library.move-down");
    CHECK(GTK_IS_LIST_BOX(list) && GTK_IS_BUTTON(up) && GTK_IS_BUTTON(down));
    gtk_list_box_select_row(GTK_LIST_BOX(list), gtk_list_box_get_row_at_index(GTK_LIST_BOX(list), 1));
    CHECK(gtk_widget_get_sensitive(up));
    g_signal_emit_by_name(up, "clicked"); Drain();
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &ordered) == UMI_STATUS_OK);
    CHECK(strcmp(ordered.rows[0].layout_id, before.rows[1].layout_id) == 0);
    CHECK(ordered.customisation_revision == before.customisation_revision + 1U);
    CHECK(!gtk_widget_get_sensitive(up) && gtk_widget_get_sensitive(down));
    native_after = umi_trader_gtk_workstation_snapshot(workstation);
    CHECK(strcmp(native_after.active_layout_id, native_before.active_layout_id) == 0);
    CHECK(native_after.rendered_panel_count == native_before.rendered_panel_count);
    request.action = UMI_UI_WORKSPACE_LIBRARY_MOVE_LATER;
    request.target_layout_id = ordered.rows[0].layout_id;
    request.expected_customisation_revision = before.customisation_revision;
    CHECK(umi_trader_gtk_workstation_library_apply(workstation, &request) == UMI_STATUS_INVALID_STATE);
    request.expected_customisation_revision = ordered.customisation_revision;
    CHECK(umi_trader_gtk_workstation_begin_layout_edit(workstation) == UMI_STATUS_OK);
    CHECK(umi_trader_gtk_workstation_library_apply(workstation, &request) == UMI_STATUS_BUSY);
    CHECK(umi_trader_gtk_workstation_cancel_layout_edit(workstation) == UMI_STATUS_OK);
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &ordered) == UMI_STATUS_OK);
    save = Find(popover, "workstation.layout-library.save-library");
    restore = Find(popover, "workstation.layout-library.restore-library");
    confirm = Find(popover, "workstation.layout-library.confirm-restore");
    CHECK(GTK_IS_BUTTON(save) && GTK_IS_BUTTON(restore) && GTK_IS_CHECK_BUTTON(confirm));
    CHECK(gtk_widget_get_sensitive(save));
    g_signal_emit_by_name(save, "clicked"); Drain();
    CHECK(umi_data_server_count(server) > 0U);
    request.expected_customisation_revision = ordered.customisation_revision;
    CHECK(umi_trader_gtk_workstation_library_apply(workstation, &request) == UMI_STATUS_OK);
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &after) == UMI_STATUS_OK);
    CHECK(strcmp(after.rows[1].layout_id, ordered.rows[0].layout_id) == 0);
    /* Compare through Trader's thin API and native button. Both delegate to
     * Framework and preserve the same model and trading state until Restore. */
    {
        UmiUiWorkspaceLibraryPreview preview;
        UmiUiWorkspaceLibrarySnapshot preview_before = after, preview_after;
        UmiTradingWorkspaceSnapshot trading_preview_before, trading_preview_after;
        GtkWidget *preview_button = Find(popover, "workstation.layout-library.preview-saved");
        GtkWidget *preview_text = Find(popover, "workstation.layout-library.preview-text");
        CHECK(GTK_IS_BUTTON(preview_button) && GTK_IS_LABEL(preview_text));
        CHECK(gtk_widget_get_sensitive(preview_button));
        size_t records_before = umi_data_server_count(server);
        CHECK(umi_trader_gtk_workstation_trading_snapshot(workstation, &trading_preview_before) == UMI_STATUS_OK);
        CHECK(umi_trader_gtk_workstation_library_preview(workstation, &preview) == UMI_STATUS_OK);
        CHECK(preview.saved.layout_count == ordered.layout_count && preview.comparison.changed_count == 2U);
        CHECK(strcmp(preview.saved.rows[0].layout_id, ordered.rows[0].layout_id) == 0);
        gtk_check_button_set_active(GTK_CHECK_BUTTON(confirm), TRUE);
        g_signal_emit_by_name(preview_button, "clicked"); Drain();
        CHECK(!gtk_check_button_get_active(GTK_CHECK_BUTTON(confirm)));
        CHECK(strstr(gtk_label_get_text(GTK_LABEL(preview_text)), "Position:") != NULL);
        CHECK(strstr(gtk_label_get_text(GTK_LABEL(preview_text)), "memory-only") != NULL);
        CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &preview_after) == UMI_STATUS_OK);
        CHECK(preview_after.customisation_revision == preview_before.customisation_revision);
        CHECK(preview_after.layout_count == preview_before.layout_count);
        for (size_t i = 0U; i < preview_before.layout_count; ++i) {
            CHECK(strcmp(preview_after.rows[i].layout_id, preview_before.rows[i].layout_id) == 0);
            CHECK(preview_after.rows[i].active == preview_before.rows[i].active);
        }
        CHECK(umi_data_server_count(server) == records_before);
        CHECK(umi_trader_gtk_workstation_trading_snapshot(workstation, &trading_preview_after) == UMI_STATUS_OK);
        CHECK(trading_preview_after.environment == trading_preview_before.environment);
        CHECK(trading_preview_after.live_armed == trading_preview_before.live_armed);
        CHECK(trading_preview_after.order_count == trading_preview_before.order_count);
        CHECK(trading_preview_after.execution_count == trading_preview_before.execution_count);
        CHECK(trading_preview_after.position_count == trading_preview_before.position_count);
    }
    gtk_check_button_set_active(GTK_CHECK_BUTTON(confirm), TRUE);
    CHECK(gtk_widget_get_sensitive(restore));
    g_signal_emit_by_name(restore, "clicked"); Drain();
    CHECK(umi_trader_gtk_workstation_library_snapshot(workstation, &after) == UMI_STATUS_OK);
    CHECK(after.layout_count == ordered.layout_count);
    for (size_t row = 0U; row < ordered.layout_count; ++row) {
        CHECK(strcmp(after.rows[row].layout_id, ordered.rows[row].layout_id) == 0);
        CHECK(after.rows[row].active == ordered.rows[row].active);
    }
    CHECK(!gtk_check_button_get_active(GTK_CHECK_BUTTON(confirm)));
    CHECK(VerifyAutomaticLayoutCopy(workstation, popover) == 0);
    CHECK(VerifyKeyboardLayouts(workstation, popover) == 0);
    CHECK(VerifyPortableLibrary(workstation, popover) == 0);
    CHECK(umi_trader_gtk_workstation_trading_snapshot(workstation, &trading_after) == UMI_STATUS_OK);
    CHECK(trading_after.environment == trading_before.environment && trading_after.live_armed == trading_before.live_armed);
    CHECK(trading_after.order_count == trading_before.order_count && trading_after.execution_count == trading_before.execution_count);
    CHECK(trading_after.position_count == trading_before.position_count);
    CHECK(strcmp(trading_after.selected_instrument_id, trading_before.selected_instrument_id) == 0);
    CHECK(umi_trader_gtk_workstation_bind_checkpoint_storage(workstation, NULL) == UMI_STATUS_OK);
cleanup:
    umi_trader_gtk_workstation_destroy(workstation);
    umi_data_server_destroy(server);
    return failed;
}
