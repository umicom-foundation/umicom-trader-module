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

typedef struct UmiTraderGtkApplicationState {
    GtkWindow *window;
    UmiTraderGtkWorkstation *workstation;
    UmiGtk4WorkstationStartupSplash *splash;
    guint startup_source_id;
} UmiTraderGtkApplicationState;

/* Fit large trading compositions to the active monitor's work area without
 * forcing maximisation. This preserves normal window movement and resizing. */
static void fit_workstation_to_monitor(GtkWindow *window)
{
    GdkDisplay *display;
    GdkSeat *seat;
    GdkDevice *pointer;
    GdkSurface *surface;
    GdkMonitor *monitor = NULL;
    GdkRectangle workarea;
    gboolean monitor_owned = FALSE;
    int width = 1440;
    int height = 900;

    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (window == NULL) return;
    display = gtk_widget_get_display(GTK_WIDGET(window));
    seat = display != NULL ? gdk_display_get_default_seat(display) : NULL;
    pointer = seat != NULL ? gdk_seat_get_pointer(seat) : NULL;
    surface = pointer != NULL ? gdk_device_get_surface_at_position(
        pointer, NULL, NULL) : NULL;
    /* Apply this branch only when its contract condition is satisfied. */
    if (display != NULL && surface != NULL)
        monitor = gdk_display_get_monitor_at_surface(display, surface);
    /* Apply this branch only when its contract condition is satisfied. */
    if (monitor == NULL && display != NULL) {
        GListModel *monitors = gdk_display_get_monitors(display);
        /*
         * Protect caller-owned memory by checking that required state is available before it is
         * used.
         */
        if (monitors != NULL && g_list_model_get_n_items(monitors) > 0U) {
            monitor = g_list_model_get_item(monitors, 0U);
            monitor_owned = monitor != NULL ? TRUE : FALSE;
        }
    }
    /* Apply this branch only when its contract condition is satisfied. */
    if (monitor != NULL) {
        gdk_monitor_get_geometry(monitor, &workarea);
        width = workarea.width > 320 ? workarea.width - 80 : workarea.width;
        height = workarea.height > 240 ? workarea.height - 80 : workarea.height;
        /* Keep the operation inside its valid bounds before reading, writing or adding data. */
        if (width < 1024) width = 1024;
        /* Keep the operation inside its valid bounds before reading, writing or adding data. */
        if (height < 680) height = 680;
        /* Keep the operation inside its valid bounds before reading, writing or adding data. */
        if (width > 1760) width = 1760;
        /* Keep the operation inside its valid bounds before reading, writing or adding data. */
        if (height > 1040) height = 1040;
        /* Release only the reference returned by the monitor list. The
         * surface lookup returns a borrowed monitor. */
        if (monitor_owned) g_object_unref(monitor);
    }
    gtk_window_set_default_size(window, width, height);
}

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
    if (state != NULL) state->window = NULL;
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
    if (state->window == NULL || state->splash == NULL) {
        return G_SOURCE_REMOVE;
    }

    (void)umi_gtk4_ws_startup_splash_set_status(
        state->splash,
        "Preparing market data, risk and workspace services…",
        "Simulation");
    (void)umi_gtk4_ws_startup_splash_set_progress(
        state->splash, 0.55, 1);
    status = umi_trader_gtk_workstation_create(&state->workstation);
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
        return G_SOURCE_REMOVE;
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
        return G_SOURCE_REMOVE;
    }

    (void)umi_gtk4_ws_startup_splash_set_status(
        state->splash, "Trader workspace is ready", "Simulation");
    (void)umi_gtk4_ws_startup_splash_set_progress(
        state->splash, 1.0, 1);
    gtk_window_set_child(state->window, content);
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
        gtk_window_present(state->window);
        return;
    }

    state->window = GTK_WINDOW(gtk_application_window_new(application));
    gtk_window_set_title(state->window, "Umicom Trader");
    fit_workstation_to_monitor(state->window);
    g_object_weak_ref(
        G_OBJECT(state->window), on_window_destroyed, state);

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

    gtk_window_set_child(
        state->window,
        umi_gtk4_ws_startup_splash_widget(state->splash));
    gtk_window_present(state->window);
    state->startup_source_id = g_timeout_add(
        80U, complete_startup, state);
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
    umi_gtk4_ws_startup_splash_destroy(state->splash);
    state->splash = NULL;
    umi_trader_gtk_workstation_destroy(state->workstation);
    state->workstation = NULL;
    state->window = NULL;
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
