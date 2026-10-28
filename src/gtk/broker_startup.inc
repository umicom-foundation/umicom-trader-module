/*-----------------------------------------------------------------------------
 * Umicom Trader
 * File: src/gtk/broker_startup.inc
 *
 * PURPOSE:
 *   Compose the shared mode screen and keep broker windows owned by the workspace.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#ifdef UMICOM_HAS_BROKER_WELCOME_GTK4
static UmiStatus open_selected_broker_mode(void *data, const UmiBrokerAccessPlan *plan)
{
    UmiTraderGtkApplicationState *state = data;
    if (state == NULL || state->window == NULL || state->startup_window == NULL)
        return UMI_STATUS_CANCELLED;
    if (UmiBrokerAccessPlanValidate(plan) != UMI_STATUS_OK)
        return UMI_STATUS_INVALID_ARGUMENT;
    GtkWindow *broker = NULL;
    if (plan->mode != UMI_BROKER_ACCESS_GUEST) {
        /* The main window owns the inspector. Retiring the welcome window
         * therefore cannot retire a newly requested broker connection. */
        broker = UmiIbkrGtkCreateForPlan(state->window, plan);
        if (broker == NULL) return UMI_STATUS_UNAVAILABLE;
        g_object_ref(broker);
    }
    UmiBrokerAccessPlan previous = state->access_plan;
    state->access_plan = *plan;
    UmiStatus status = open_profile_workspace(state, "");
    if (status != UMI_STATUS_OK) state->access_plan = previous;
    if (broker != NULL) {
        if (status == UMI_STATUS_OK && state->window != NULL)
            gtk_window_present(broker);
        else
            gtk_window_destroy(broker);
        g_object_unref(broker);
    }
    return status;
}

static UmiStatus open_local_workspace_profiles(void *data)
{
    UmiTraderGtkApplicationState *state = data;
    if (state == NULL || state->startup_window == NULL) return UMI_STATUS_CANCELLED;
    close_mode_welcome(state);
    (void)complete_local_profiles(state);
    return state->profile_gate != NULL ? UMI_STATUS_OK : UMI_STATUS_UNAVAILABLE;
}
#endif

static gboolean complete_startup(gpointer data)
{
#ifndef UMICOM_HAS_BROKER_WELCOME_GTK4
    /* Older installed SDKs retain their original profile and simulator route. */
    return complete_local_profiles(data);
#else
    UmiTraderGtkApplicationState *state = data;
    if (state == NULL) return G_SOURCE_REMOVE;
    state->startup_source_id = 0U;
    if (state->window == NULL || state->startup_window == NULL) return G_SOURCE_REMOVE;
    UmiBrokerWelcomeGtkConfig config = {state,
        open_selected_broker_mode, open_local_workspace_profiles};
    UmiStatus status = UmiBrokerWelcomeGtkCreate(&config, &state->broker_welcome);
    if (status != UMI_STATUS_OK) {
        (void)umi_gtk4_ws_startup_splash_set_status(state->splash,
            "The broker welcome screen could not open.", "Action required");
        finish_startup_window(state, 1);
        return G_SOURCE_REMOVE;
    }
    gtk_window_set_title(state->startup_window, "Umicom Trader - Choose account mode");
    (void)umi_gtk4_ws_window_fit(state->startup_window, 760, 780, 480, 360);
    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
        GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll),
        UmiBrokerWelcomeGtkWidget(state->broker_welcome));
    gtk_window_set_child(state->startup_window, scroll);
    umi_gtk4_ws_startup_splash_destroy(state->splash);
    state->splash = NULL;
    return G_SOURCE_REMOVE;
#endif
}
