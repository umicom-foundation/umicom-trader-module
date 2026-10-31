/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: tests/test_broker_startup.c
 *
 * PURPOSE:
 *   Exercise the real application startup while replacing only persistent layout storage.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include "umicom/trader/gtk_workstation.h"
#include <stdio.h>
#include <string.h>

/* Startup tests use the actual workstation and callback composition. Only
 * checkpoint persistence is replaced so no personal layout is read or written. */
static UmiStatus IsolatedCheckpoint(UmiTraderGtkWorkstation *workstation, int restore)
{
    (void)workstation; (void)restore;
    return UMI_STATUS_OK;
}
static UmiStatus IsolatedProfile(UmiTraderGtkWorkstation *workstation,
    const char *profile, int restore)
{
    (void)workstation; (void)profile; (void)restore;
    return UMI_STATUS_OK;
}
#define umi_trader_gtk_workstation_enable_checkpoint_storage IsolatedCheckpoint
#define UmiTraderGtkEnableProfileStorage IsolatedProfile
#define main UmicomTraderEmbeddedMain
#include "../src/gtk/main.c"
#undef main
#undef UmiTraderGtkEnableProfileStorage
#undef umi_trader_gtk_workstation_enable_checkpoint_storage

#define CHECK(condition) do { if (!(condition)) { \
    fprintf(stderr, "Check failed at %d: %s\n", __LINE__, #condition); return 1; } } while (0)

static GtkWidget *FindControl(GtkWidget *root, const char *name)
{
    if (strcmp(gtk_widget_get_name(root), name) == 0) return root;
    for (GtkWidget *child = gtk_widget_get_first_child(root); child != NULL;
         child = gtk_widget_get_next_sibling(child)) {
        GtkWidget *found = FindControl(child, name);
        if (found != NULL) return found;
    }
    return NULL;
}
static GtkWindow *FindBroker(GtkWindow *parent)
{
    GListModel *windows = gtk_window_get_toplevels();
    for (guint i = 0; i < g_list_model_get_n_items(windows); ++i) {
        GtkWindow *window = g_list_model_get_item(windows, i);
        if (gtk_window_get_transient_for(window) == parent &&
            FindControl(GTK_WIDGET(window), "ibkr-mode") != NULL) return window;
        g_object_unref(window);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 2) return 2;
    if (!gtk_init_check()) return 77;
    GtkApplication *application = gtk_application_new("org.umicom.trader.startup-test",
        G_APPLICATION_NON_UNIQUE);
    GError *error = NULL;
    CHECK(g_application_register(G_APPLICATION(application), NULL, &error));
    UmiTraderGtkApplicationState state = {0};
    on_activate(application, &state);
    CHECK(state.startup_window != NULL && state.startup_source_id != 0U);
#ifdef UMICOM_HAS_BRANDED_STARTUP_SPLASH
    CHECK(FindControl(GTK_WIDGET(state.startup_window), "umicom-branded-startup") != NULL);
    CHECK(FindControl(GTK_WIDGET(state.startup_window), "umicom.brand.logo") != NULL);
#endif
    (void)g_source_remove(state.startup_source_id);
    state.startup_source_id = 0U;
    (void)complete_startup(&state);
    CHECK(state.broker_welcome != NULL && state.workstation == NULL);
    if (strcmp(argv[1], "cancel") == 0) {
        (void)on_startup_window_close(state.startup_window, &state);
        CHECK(state.window == NULL && state.startup_window == NULL);
        CHECK(state.broker_welcome == NULL && state.workstation == NULL);
    } else {
        const char *choice = NULL;
        if (strcmp(argv[1], "guest") == 0) choice = "broker-welcome-guest";
        if (strcmp(argv[1], "paper") == 0 || strcmp(argv[1], "gateway") == 0 ||
            strcmp(argv[1], "paper-layout") == 0 || strcmp(argv[1], "gateway-layout") == 0)
            choice = "broker-welcome-paper";
        if ((strcmp(argv[1], "live") == 0 || strcmp(argv[1], "live-layout") == 0)) choice = "broker-welcome-live";
        CHECK(choice != NULL);
        GtkWidget *welcome = GTK_WIDGET(state.startup_window);
        if (strcmp(argv[1], "gateway") == 0 || strcmp(argv[1], "gateway-layout") == 0)
            gtk_drop_down_set_selected(GTK_DROP_DOWN(FindControl(welcome, "broker-welcome-program")), 1U);
        /* Follow the visible account and guidance path before dispatching the
         * final action. This exercises the production sequence, not hidden controls. */
        GtkWidget *modes = FindControl(welcome, "broker-welcome-modes");
        const char *page = strcmp(choice, "broker-welcome-guest") == 0 ? "demo"
            : strcmp(choice, "broker-welcome-live") == 0 ? "live" : "paper";
        gtk_stack_set_visible_child_name(GTK_STACK(modes), page);
        if (strcmp(page, "demo") != 0) {
            GtkWidget *begin = FindControl(welcome, strcmp(page, "live") == 0
                ? "broker-welcome-begin-live" : "broker-welcome-begin-paper");
            CHECK(begin != NULL);
            g_signal_emit_by_name(begin, "clicked");
            CHECK(state.workstation == NULL && state.broker_welcome != NULL);
            GtkWidget *stage = FindControl(welcome, "broker-welcome-stage");
            CHECK(strcmp(gtk_stack_get_visible_child_name(GTK_STACK(stage)), "authentication") == 0);
        }
        GtkWidget *button = FindControl(welcome, choice);
        CHECK(button != NULL);
        g_signal_emit_by_name(button, "clicked");
        CHECK(state.workstation != NULL && state.window != NULL);
        CHECK(state.startup_window == NULL && state.broker_welcome == NULL);
        GtkWindow *broker = FindBroker(state.window);
        if (strcmp(argv[1], "guest") == 0) {
            CHECK(state.access_plan.mode == UMI_BROKER_ACCESS_GUEST && broker == NULL);
        } else {
#ifdef UMICOM_HAS_BROKER_WORKSPACE_GTK4
            CHECK(broker == NULL && state.broker_workspace != NULL);
            GtkWidget *pages = FindControl(GTK_WIDGET(state.window), "trader-session-pages");
            CHECK(pages != NULL);

#ifdef UMICOM_HAS_BROKER_WORKSPACE_LAYOUT_GTK4
            if (strstr(argv[1], "-layout") != NULL) {
                UmiBrokerWorkspaceLayout layout;
                GtkWidget *selector = FindControl(GTK_WIDGET(state.window), "broker-workspace-page-choice");
                GtkWidget *move = FindControl(GTK_WIDGET(state.window), "broker-workspace-page-move");
                CHECK(selector != NULL && move != NULL);
                gtk_drop_down_set_selected(GTK_DROP_DOWN(selector), UMI_BROKER_PAGE_MARKET);
                g_signal_emit_by_name(move, "clicked");
                CHECK(UmiBrokerWorkspaceGtkCopyLayout(state.broker_workspace, &layout) == UMI_STATUS_OK);
                CHECK(layout.pane[UMI_BROKER_PAGE_MARKET] == 1U &&
                    layout.selected[1] == UMI_BROKER_PAGE_MARKET);
                CHECK(UmiBrokerWorkspaceGtkSelectPage(state.broker_workspace,
                    UMI_BROKER_PAGE_CHART) == UMI_STATUS_OK);
                CHECK(UmiBrokerWorkspaceGtkSetSplit(state.broker_workspace,
                    GTK_ORIENTATION_VERTICAL) == UMI_STATUS_OK);
                CHECK(FindBroker(state.window) == NULL);
            }
#endif

            CHECK(strcmp(gtk_stack_get_visible_child_name(GTK_STACK(pages)), "broker") == 0);
            GtkWidget *open = FindControl(GTK_WIDGET(state.window), "broker-workspace-connect");
            CHECK(open != NULL);
            g_signal_emit_by_name(open, "clicked");
            broker = FindBroker(state.window);
            CHECK(broker != NULL);
            g_signal_emit_by_name(open, "clicked");
            GtkWindow *same = FindBroker(state.window);
            CHECK(same == broker);
            g_object_unref(same);
            gtk_stack_set_visible_child_name(GTK_STACK(pages), "demo");
            CHECK(strcmp(gtk_stack_get_visible_child_name(GTK_STACK(pages)), "demo") == 0);
            gtk_stack_set_visible_child_name(GTK_STACK(pages), "broker");
#endif
            CHECK(broker != NULL);
            GtkWidget *root = GTK_WIDGET(broker);
            CHECK(!gtk_widget_get_sensitive(FindControl(root, "ibkr-disconnect")));
            CHECK(!gtk_check_button_get_active(GTK_CHECK_BUTTON(FindControl(root, "ibkr-live-ack"))));
            CHECK(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(FindControl(root, "ibkr-port")))
                == (int)state.access_plan.port);
            CHECK(state.access_plan.mode == ((strcmp(argv[1], "live") == 0 || strcmp(argv[1], "live-layout") == 0)
                ? UMI_BROKER_ACCESS_LIVE : UMI_BROKER_ACCESS_PAPER));
            if (strcmp(argv[1], "gateway") == 0 || strcmp(argv[1], "gateway-layout") == 0)
                CHECK(state.access_plan.program == UMI_IBKR_GATEWAY && state.access_plan.port == 4002U);
            gtk_window_destroy(broker);
            g_object_unref(broker);
        }
    }
    application_state_dispose(&state);
    g_object_unref(application);
    return 0;
}
