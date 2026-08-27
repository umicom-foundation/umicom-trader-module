/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: applications/trader/src/gtk/main.c
 *
 * PURPOSE:
 *   Launch the runnable GTK4 Umicom Trader desktop workstation while leaving
 *   reusable layout, trading, panel and rendering behaviour in Framework.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/
#include <gtk/gtk.h>
#include <stdio.h>

#include "umicom/trader/gtk_workstation.h"

static void workstation_destroy_notify(gpointer data)
{
    umi_trader_gtk_workstation_destroy((UmiTraderGtkWorkstation *)data);
}

static void on_activate(GtkApplication *application, gpointer user_data)
{
    UmiTraderGtkWorkstation *workstation = NULL;
    GtkWidget *content;
    GtkWindow *window;
    UmiStatus status;
    (void)user_data;

    status = umi_trader_gtk_workstation_create(&workstation);
    if (status != UMI_STATUS_OK) {
        (void)fprintf(stderr, "Unable to create Umicom Trader: %s\n",
                      umi_status_text(status));
        g_application_quit(G_APPLICATION(application));
        return;
    }
    content = umi_trader_gtk_workstation_widget(workstation);
    if (content == NULL) {
        umi_trader_gtk_workstation_destroy(workstation);
        g_application_quit(G_APPLICATION(application));
        return;
    }

    window = GTK_WINDOW(gtk_application_window_new(application));
    gtk_window_set_title(window, "Umicom Trader");
    gtk_window_set_default_size(window, 1500, 900);
    gtk_window_set_child(window, content);
    g_object_set_data_full(G_OBJECT(window),
                           "umicom-trader-workstation",
                           workstation,
                           workstation_destroy_notify);
    gtk_window_present(window);
}

int main(int argc, char **argv)
{
    GtkApplication *application = gtk_application_new(
        "org.umicom.trader",
        G_APPLICATION_DEFAULT_FLAGS);
    int result;
    if (application == NULL) return 1;
    g_signal_connect(application, "activate", G_CALLBACK(on_activate), NULL);
    result = g_application_run(G_APPLICATION(application), argc, argv);
    g_object_unref(application);
    return result;
}
