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

#include "umicom/application/presentation/presentation.h"
#include "umicom/trader/gtk_workstation.h"

/*
 * Provide the workstation destroy notify operation used by this module and its client
 * applications.
 */
static void workstation_destroy_notify(gpointer data) {
  umi_trader_gtk_workstation_destroy((UmiTraderGtkWorkstation *)data);
}

/* Provide the on activate operation used by this module and its client applications. */
static void on_activate(GtkApplication *application, gpointer user_data) {
  UmiTraderGtkWorkstation *workstation = NULL;
  GtkWidget *content;
  GtkWindow *window;
  const UmiApplicationPresentationWindowSpec *window_spec;
  uint32_t default_width = 1180U;
  uint32_t default_height = 760U;
  int compact = 0;
  UmiStatus status;
  (void)user_data;

  status = umi_trader_gtk_workstation_create(&workstation);
  /* Preserve the original failure result so the caller can respond to the correct cause. */
  if (status != UMI_STATUS_OK) {
    (void)fprintf(stderr, "Unable to create Umicom Trader: %s\n", umi_status_text(status));
    g_application_quit(G_APPLICATION(application));
    return;
  }
  content = umi_trader_gtk_workstation_widget(workstation);
  /*
   * Protect caller-owned memory by checking that required state is available before it is
   * used.
   */
  if (content == NULL) {
    umi_trader_gtk_workstation_destroy(workstation);
    g_application_quit(G_APPLICATION(application));
    return;
  }

  window = GTK_WINDOW(gtk_application_window_new(application));
  gtk_window_set_title(window, "Umicom Trader");
  /* Framework owns the window geometry contract. Trader asks it to fit the
   * standard workspace into a laptop-sized starting area, while the same
   * catalogue can provide different governed sizes to other frontends. */
  window_spec = umi_application_presentation_window_catalogue_find(
      "org.umicom.workspace.trader.standard");
  if (window_spec != NULL) {
    status = umi_application_presentation_window_spec_fit(
        window_spec, default_width, default_height,
        &default_width, &default_height, &compact);
    if (status != UMI_STATUS_OK) {
      default_width = 1180U;
      default_height = 760U;
    }
  }
  gtk_window_set_default_size(window, (int)default_width, (int)default_height);
  /* The shared stylesheet can respond to this class by collapsing optional
   * tools according to their catalogue policies. */
  if (compact)
    gtk_widget_add_css_class(GTK_WIDGET(window), "umicom-compact-workspace");
  gtk_window_set_resizable(window, TRUE);
  gtk_window_set_child(window, content);
  g_object_set_data_full(G_OBJECT(window), "umicom-trader-workstation", workstation,
                         workstation_destroy_notify);
  gtk_window_present(window);
}

/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
int main(int argc, char **argv) {
  GtkApplication *application;
  int result;

  /* The program name matches the packaged desktop identity so operating
   * systems can resolve the canonical Umicom icon instead of a GTK icon. */
  g_set_prgname("umicom-trader");
  application = gtk_application_new("org.umicom.trader", G_APPLICATION_DEFAULT_FLAGS);
  /*
   * Protect caller-owned memory by checking that required state is available before it is
   * used.
   */
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
