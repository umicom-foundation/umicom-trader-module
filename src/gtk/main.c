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

#include "umicom/trader/gtk_workstation.h"
#include "umicom/ui/gtk4/workstation/shell_header.h"
#include "umicom/ui/gtk4/workstation/window_fit.h"

typedef struct UmiTraderGtkApplicationState {
    GtkWindow *window;
    GtkWindow *startup_window;
    UmiTraderGtkWorkstation *workstation;
    UmiGtk4WorkstationStartupSplash *splash;
    guint startup_source_id;
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
    gtk_window_set_child(state->window, content);
    finish_startup_window(state, 0);
    umi_gtk4_ws_startup_splash_destroy(state->splash);
    state->splash = NULL;
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
