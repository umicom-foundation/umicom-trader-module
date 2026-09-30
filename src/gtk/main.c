/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: applications/trader/src/gtk/main.c
 *
 * PURPOSE:
 *   Start the native Trader workstation through the Framework-owned startup
 *   surface, then present the safe simulation workspace after services are
 *   prepared.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include <gtk/gtk.h>
#include <stdio.h>
#include "umicom/security/gtk4/local_profile_gate.h"

/* Framework owns the explicit read-only Paper/Live connection window. */
#ifdef UMICOM_HAS_IBKR_CONNECTION_GTK4
#include "umicom/broker_connectivity/connection_gtk4.h"
#endif

/* The shared UI target advertises this optional Framework component. */
#ifdef UMICOM_HAS_MARKET_TAPE_GTK4
#include "umicom/ui/gtk4/market_tape.h"
#endif

#include "umicom/trader/gtk_workstation.h"
#include "umicom/ui/gtk4/workstation/shell_header.h"
#include "umicom/ui/gtk4/workstation/window_fit.h"

typedef struct UmiTraderGtkApplicationState {
    GtkWindow *window;
    GtkWindow *startup_window;
    UmiTraderGtkWorkstation *workstation;
    UmiGtk4WorkstationStartupSplash *splash;
    guint startup_source_id;
    UmiLocalProfileGate *profile_gate;
} UmiTraderGtkApplicationState;

/* Clear the borrowed window pointer when the native window is destroyed. */
static void on_window_destroyed(gpointer data, GObject *where_the_object_was)
{
    UmiTraderGtkApplicationState *state =
        (UmiTraderGtkApplicationState *)data;

    (void)where_the_object_was;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (state == NULL) return;
    if ((GObject *)state->window == where_the_object_was) {
        state->window = NULL;
        if (state->startup_window != NULL) gtk_window_destroy(state->startup_window);
    }
    if ((GObject *)state->startup_window == where_the_object_was) {
        state->startup_window = NULL;
        if (state->startup_source_id != 0U) {
            g_source_remove(state->startup_source_id);
            state->startup_source_id = 0U;
        }
        if (state->window != NULL) gtk_window_destroy(state->window);
    }
}

/* Disconnect weak and signal observers before releasing a borrowed GTK window. */
static void release_application_window(UmiTraderGtkApplicationState *state, GtkWindow **slot)
{
    GtkWindow *window = *slot;
    if (window == NULL) return;
    *slot = NULL;
    g_signal_handlers_disconnect_by_data(window, state);
    g_object_weak_unref(G_OBJECT(window), on_window_destroyed, state);
    gtk_window_destroy(window);
}

/* Destruction is earlier than finalization when another owner retains a
 * window. Cancel immediately and remove weak observers before clearing state. */
static void on_application_window_destroy(GtkWidget *widget, gpointer data)
{
    UmiTraderGtkApplicationState *state = data;
    UmiLocalProfileGateDestroy(state->profile_gate);
    state->profile_gate = NULL;
    g_signal_handlers_disconnect_by_data(widget, state);
    g_object_weak_unref(G_OBJECT(widget), on_window_destroyed, state);
    if (state->startup_source_id != 0U) {
        g_source_remove(state->startup_source_id);
        state->startup_source_id = 0U;
    }
    if ((GtkWidget *)state->startup_window == widget) state->startup_window = NULL;
    if ((GtkWidget *)state->window == widget) state->window = NULL;
    release_application_window(state, &state->startup_window);
    release_application_window(state, &state->window);
}

/* Closing the temporary startup surface cancels deferred construction. It
 * must never be followed by a surprise presentation of the hidden main window. */
static gboolean on_startup_window_close(GtkWindow *window, gpointer user_data)
{
    UmiTraderGtkApplicationState *state = user_data;
    (void)window;
    UmiLocalProfileGateDestroy(state->profile_gate);
    state->profile_gate = NULL;
    if (state->startup_source_id != 0U) {
        g_source_remove(state->startup_source_id);
        state->startup_source_id = 0U;
    }
    release_application_window(state, &state->startup_window);
    release_application_window(state, &state->window);
    return TRUE;
}

/* Close the temporary window before presenting the completed main surface.
 * A failure keeps its existing readable error content, without a pending timer. */
static void finish_startup_window(UmiTraderGtkApplicationState *state, int failed)
{
    if (state->window == NULL) return;
    if (failed && state->splash != NULL) {
        if (state->startup_window != NULL)
            gtk_window_set_child(state->startup_window, NULL);
        gtk_window_set_child(state->window,
            umi_gtk4_ws_startup_splash_widget(state->splash));
    }
    release_application_window(state, &state->startup_window);
    gtk_window_present(state->window);
}

/* Finish the safe product workspace after GTK has shown the startup surface. */
/* The original automatic simulator launch is superseded by an explicit local-profile or simulator choice after the splash screen. Shared Framework services own authentication and per-profile persistence. The previous implementation remains for engineering review. */
#if 0
static gboolean complete_startup(gpointer user_data)
{
    UmiTraderGtkApplicationState *state =
        (UmiTraderGtkApplicationState *)user_data;
    GtkWidget *content;
    UmiStatus status;

    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (state == NULL) return G_SOURCE_REMOVE;
    state->startup_source_id = 0U;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (state->window == NULL || state->startup_window == NULL || state->splash == NULL) {
        return G_SOURCE_REMOVE;
    }

    (void)umi_gtk4_ws_startup_splash_set_status(
        state->splash,
        "Preparing market data, risk and workspace services…",
        "Simulation");
    (void)umi_gtk4_ws_startup_splash_set_progress(
        state->splash, 0.55, 1);
    status = umi_trader_gtk_workstation_create(&state->workstation);
    /* The final window has never been presented or realized. */
    if (status == UMI_STATUS_OK)
        status = umi_trader_gtk_workstation_bind_window(state->workstation, state->window);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        char message[192U];

        (void)snprintf(
            message,
            sizeof(message),
            "Trader startup could not complete: %s",
            umi_status_text(status));
        (void)umi_gtk4_ws_startup_splash_set_status(
            state->splash, message, "Action required");
        (void)umi_gtk4_ws_startup_splash_set_progress(
            state->splash, 1.0, 0);
        (void)fprintf(
            stderr, "Umicom Trader workstation failed: %s\n",
            umi_status_text(status));
        finish_startup_window(state, 1);
        return G_SOURCE_REMOVE;
    }

    /* Only the real native launcher opts into disk checkpoints. */
    {
        UmiStatus storage_status = umi_trader_gtk_workstation_enable_checkpoint_storage(state->workstation, 1);
        if (storage_status != UMI_STATUS_OK)
            (void)fprintf(stderr, "Layout storage unavailable: %s\n", umi_status_text(storage_status));
    }

    content = umi_trader_gtk_workstation_widget(state->workstation);
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (content == NULL) {
        (void)umi_gtk4_ws_startup_splash_set_status(
            state->splash,
            "Trader startup completed without a visible workspace.",
            "Action required");
        (void)umi_gtk4_ws_startup_splash_set_progress(
            state->splash, 1.0, 0);
        finish_startup_window(state, 1);
        return G_SOURCE_REMOVE;
    }

    (void)umi_gtk4_ws_startup_splash_set_status(
        state->splash, "Trader workspace is ready", "Simulation");
    (void)umi_gtk4_ws_startup_splash_set_progress(
        state->splash, 1.0, 1);
    /* Attach a separate read-only market-data practice entry without replacing
     * the existing trading controllers, simulation or layout persistence. */
#ifdef UMICOM_HAS_MARKET_TAPE_GTK4
    content = UmiMarketTapeGtkWrap(content, state->window);
#endif
    /* Connection inspection is separate from the existing simulation and OMS. */
#ifdef UMICOM_HAS_IBKR_CONNECTION_GTK4
    content = UmiIbkrGtkWrap(content, state->window);
#endif
    gtk_window_set_child(state->window, content);
    finish_startup_window(state, 0);
    umi_gtk4_ws_startup_splash_destroy(state->splash);
    state->splash = NULL;
    return G_SOURCE_REMOVE;
}

#endif
/* The product chooses when to create its workstation. Authentication,
 * password handling and profile storage remain reusable Framework services. */
static UmiStatus open_profile_workspace(void *data, const char *profile)
{
    UmiTraderGtkApplicationState *state = data;
    if (state->window == NULL || state->startup_window == NULL) return UMI_STATUS_CANCELLED;
    UmiStatus status = umi_trader_gtk_workstation_create(&state->workstation);
    if (status == UMI_STATUS_OK)
        status = umi_trader_gtk_workstation_bind_window(state->workstation,state->window);
    if (status != UMI_STATUS_OK) {
        umi_trader_gtk_workstation_destroy(state->workstation); state->workstation = NULL; return status;
    }
    UmiStatus storage_status = profile[0] != '\0'
        ? UmiTraderGtkEnableProfileStorage(state->workstation,profile,1)
        : umi_trader_gtk_workstation_enable_checkpoint_storage(state->workstation,1);
    if (storage_status != UMI_STATUS_OK)
        (void)fprintf(stderr,"Layout storage unavailable: %s\n",umi_status_text(storage_status));
    GtkWidget *content = umi_trader_gtk_workstation_widget(state->workstation);
    if (content == NULL) {
        umi_trader_gtk_workstation_destroy(state->workstation); state->workstation = NULL;
        return UMI_STATUS_INTERNAL_ERROR;
    }
#ifdef UMICOM_HAS_MARKET_TAPE_GTK4
    content = UmiMarketTapeGtkWrap(content,state->window);
#endif
#ifdef UMICOM_HAS_IBKR_CONNECTION_GTK4
    content = UmiIbkrGtkWrap(content,state->window);
#endif
    GtkWidget *host = gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
    char identity[160];
    (void)snprintf(identity,sizeof(identity),"%s%s",
        profile[0] != '\0' ? "Local profile: " : "",profile[0] != '\0' ? profile : "Simulator without a profile");
    GtkWidget *label = gtk_label_new(identity); gtk_label_set_xalign(GTK_LABEL(label),0.0F);
    gtk_widget_set_margin_start(label,12); gtk_widget_set_margin_end(label,12);
    gtk_widget_set_tooltip_text(label,"Local profile identity. Broker connection state is shown separately. Save layouts before closing.");
    gtk_box_append(GTK_BOX(host),label); gtk_widget_set_vexpand(content,TRUE); gtk_box_append(GTK_BOX(host),content);
    gtk_window_set_child(state->window,host);
    /* Close the start window without its close-request callback cancelling the
     * successfully constructed workspace. The gate's worker holds its own ref. */
    finish_startup_window(state,0);
    UmiLocalProfileGateDestroy(state->profile_gate); state->profile_gate = NULL;
    return UMI_STATUS_OK;
}
#ifdef UMICOM_HAS_IBKR_CONNECTION_GTK4
static void open_profile_broker(void *data)
{
    UmiTraderGtkApplicationState *state = data;
    if (state->startup_window != NULL) {
        GtkWindow *connection = UmiIbkrGtkCreate(state->startup_window);
        if (connection != NULL) gtk_window_present(connection);
    }
}
#endif
static gboolean complete_startup(gpointer data)
{
    UmiTraderGtkApplicationState *state = data;
    if (state == NULL) return G_SOURCE_REMOVE;
    state->startup_source_id = 0U;
    if (state->window == NULL || state->startup_window == NULL) return G_SOURCE_REMOVE;
    UmiLocalProfileGateConfig config = {0};
    config.application_id = "org.umicom.trader"; config.title = "Welcome to Umicom Trader";
    config.user_data = state; config.open_workspace = open_profile_workspace;
#ifdef UMICOM_HAS_IBKR_CONNECTION_GTK4
    config.open_broker = open_profile_broker;
#endif
    UmiStatus status = UmiLocalProfileGateCreate(&config,&state->profile_gate);
    if (status != UMI_STATUS_OK) {
        (void)umi_gtk4_ws_startup_splash_set_status(state->splash,"The local profile screen could not open.","Action required");
        finish_startup_window(state,1); return G_SOURCE_REMOVE;
    }
    gtk_window_set_title(state->startup_window,"Umicom Trader — Local profile or Simulator");
    (void)umi_gtk4_ws_window_fit(state->startup_window,720,780,480,360);
    gtk_window_set_child(state->startup_window,UmiLocalProfileGateWidget(state->profile_gate));
    umi_gtk4_ws_startup_splash_destroy(state->splash); state->splash = NULL;
    return G_SOURCE_REMOVE;
}

/* Present the startup surface immediately, then construct the trading
 * composition from the GTK main context. */
static void on_activate(GtkApplication *application, gpointer user_data)
{
    UmiTraderGtkApplicationState *state =
        (UmiTraderGtkApplicationState *)user_data;
    UmiGtk4WorkstationStartupSplashConfig splash_config;
    UmiStatus status;

    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (state == NULL) return;
    /* Apply this branch only when its contract condition is satisfied. */
    if (state->window != NULL) {
        gtk_window_present(state->startup_window != NULL ? state->startup_window : state->window);
        return;
    }

    /* A prior closed window may leave its owned controllers until activation. */
    umi_gtk4_ws_startup_splash_destroy(state->splash);
    state->splash = NULL;
    umi_trader_gtk_workstation_destroy(state->workstation);
    state->workstation = NULL;
    state->window = GTK_WINDOW(gtk_application_window_new(application));
    gtk_window_set_title(state->window, "Umicom Trader");
    (void)umi_gtk4_ws_apply_window_identity(state->window);
    /* Fit large trading compositions without forcing maximisation, retaining
     * normal movement and resizing through the shared Framework policy. */
    (void)umi_gtk4_ws_window_fit(state->window, 1760, 1040, 1024, 680);
    g_object_weak_ref(
        G_OBJECT(state->window), on_window_destroyed, state);
    g_signal_connect(state->window, "destroy", G_CALLBACK(on_application_window_destroy), state);

    splash_config = umi_gtk4_ws_startup_splash_config_default(
        "org.umicom.trader", "Umicom Trader");
    splash_config.subtitle =
        "Markets, charts, execution, positions and risk";
    splash_config.status = "Starting safe trading services…";
    splash_config.mode_badge = "Simulation";
    status = umi_gtk4_ws_startup_splash_create(
        &splash_config, &state->splash);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        (void)fprintf(
            stderr, "Umicom Trader startup surface failed: %s\n",
            umi_status_text(status));
        gtk_window_destroy(state->window);
        return;
    }

    /* Only the temporary startup window is visible before native binding. */
    state->startup_window = GTK_WINDOW(gtk_application_window_new(application));
    gtk_window_set_title(state->startup_window, "Umicom Trader — Starting");
    (void)umi_gtk4_ws_apply_window_identity(state->startup_window);
    (void)umi_gtk4_ws_window_fit(state->startup_window, 680, 440, 400, 280);
    g_object_weak_ref(G_OBJECT(state->startup_window), on_window_destroyed, state);
    g_signal_connect(state->startup_window, "destroy", G_CALLBACK(on_application_window_destroy), state);
    g_signal_connect(state->startup_window, "close-request",
        G_CALLBACK(on_startup_window_close), state);
    gtk_window_set_child(state->startup_window,
        umi_gtk4_ws_startup_splash_widget(state->splash));
    gtk_window_present(state->startup_window);
    state->startup_source_id = g_timeout_add(
        80U, complete_startup, state);
    if (state->startup_source_id == 0U)
        (void)on_startup_window_close(state->startup_window, state);
}

/* Release the Framework-owned startup and product controllers in reverse
 * creation order after the native application loop ends. */
static void application_state_dispose(UmiTraderGtkApplicationState *state)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (state == NULL) return;
    UmiLocalProfileGateDestroy(state->profile_gate);
    state->profile_gate = NULL;
    /* Apply this branch only when its contract condition is satisfied. */
    if (state->startup_source_id != 0U) {
        (void)g_source_remove(state->startup_source_id);
        state->startup_source_id = 0U;
    }
    release_application_window(state, &state->startup_window);
    release_application_window(state, &state->window);
    umi_gtk4_ws_startup_splash_destroy(state->splash);
    state->splash = NULL;
    umi_trader_gtk_workstation_destroy(state->workstation);
    state->workstation = NULL;
}

/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
int main(int argc, char **argv)
{
    GtkApplication *application;
    UmiTraderGtkApplicationState state = {0};
    int result;
 /* The program name matches the packaged desktop identity so operating
   * systems can resolve the canonical Umicom icon instead of a GTK icon. */
    application = gtk_application_new(
        "org.umicom.trader.gtk",
        G_APPLICATION_DEFAULT_FLAGS);
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (application == NULL) return 1;
    g_signal_connect(
        application, "activate", G_CALLBACK(on_activate), &state);
    result = g_application_run(G_APPLICATION(application), argc, argv);
    application_state_dispose(&state);
    g_object_unref(application);
    return result;
}
