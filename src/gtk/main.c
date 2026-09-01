/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: applications/trader/src/gtk/main.c
 *
 * PURPOSE:
 *   Launch the runnable GTK4 Umicom Trader desktop workstation while leaving
 *   reusable layout, trading, panel and rendering behaviour in Framework.
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

static void workstation_destroy_notify(gpointer data) {
  umi_trader_gtk_workstation_destroy((UmiTraderGtkWorkstation *)data);
}

static void on_activate(GtkApplication *application, gpointer user_data) {
  UmiTraderGtkWorkstation *workstation = NULL;
  GtkWidget *content;
  GtkWindow *window;
  UmiStatus status;
  (void)user_data;

  status = umi_trader_gtk_workstation_create(&workstation);
  if (status != UMI_STATUS_OK) {
    (void)fprintf(stderr, "Unable to create Umicom Trader: %s\n", umi_status_text(status));
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
  /* Start at a laptop-friendly size while allowing the user to resize or
   * maximise the workstation across one or several monitors. */
  gtk_window_set_default_size(window, 1180, 760);
  gtk_window_set_resizable(window, TRUE);
  gtk_window_set_child(window, content);
  g_object_set_data_full(G_OBJECT(window), "umicom-trader-workstation", workstation,
                         workstation_destroy_notify);
  gtk_window_present(window);
}

int main(int argc, char **argv) {
  GtkApplication *application;
  int result;

  /* The program name matches the packaged desktop identity so operating
   * systems can resolve the canonical Umicom icon instead of a GTK icon. */
  g_set_prgname("umicom-trader");
  application = gtk_application_new("org.umicom.trader", G_APPLICATION_DEFAULT_FLAGS);
  if (application == NULL)
    return 1;
  g_signal_connect(application, "activate", G_CALLBACK(on_activate), NULL);
  result = g_application_run(G_APPLICATION(application), argc, argv);
  g_object_unref(application);
  return result;
}

#ifdef _WIN32
#include <windows.h>

/* Windows enters a graphical executable through WinMain. Forwarding the
 * original arguments keeps one application startup path on every platform. */
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line,
                   int show_command) {
  (void)instance;
  (void)previous_instance;
  (void)command_line;
  (void)show_command;
  return main(__argc, __argv);
}
#endif
