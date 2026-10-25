/*-----------------------------------------------------------------------------
 * Umicom Trader Module
 * File: applications/trader/src/console/main.c
 *
 * PURPOSE:
 *   Provide a native verification frontend for the Framework-composed Trader workbench and running trading layout.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#ifdef UMICOM_HAS_UFMP_OUTBOX
#include "umicom/financial_messaging/outbox.h"
#include "umicom/financial_messaging/outbox_review.h"
#endif
#ifdef UMICOM_HAS_UFMP_FOUNDATION
#include "umicom/financial_messaging/simulation.h"
#endif
#ifdef UMICOM_HAS_UFMP_CLEARING
#include "umicom/financial_messaging/clearing_preview.h"
#endif

#include "umicom/application/runtime/readiness.h"
#include "umicom/application/experience_catalogue.h"
#include "umicom/application/runtime/command_surface.h"
#include "umicom/trader/runtime.h"
#include "umicom/trader/workbench_profile.h"

/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
/* Preserve the original entry signature for review. RunDefault keeps the
 * complete existing console workflow; main now dispatches an optional shared
 * Framework simulation before invoking that unchanged workflow. */
#if 0
int main(void)
#endif
static int RunDefault(void)
{
    UmiTraderWorkbenchProfile *profile = NULL;
    UmiTraderWorkbenchProfileSnapshot snapshot;
    UmiUiWorkspaceLayout layout;
    UmiApplicationLaunchReadiness launch_readiness;
    UmiApplicationLaunchReadinessSummary portfolio_readiness;
    UmiApplicationCommandSurface command_surface;
    UmiApplicationCommandQuery command_query = {0};
    UmiApplicationCommandQueryResult command_result;
    UmiStatus status;

    status = umi_trader_workbench_profile_create(&profile);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        (void)fprintf(stderr,
                      "Unable to create Trader workbench profile: %d\n",
                      (int)status);
        return 1;
    }

    status = umi_trader_workbench_profile_snapshot(profile, &snapshot);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        (void)fprintf(stderr,
                      "Unable to read Trader workbench profile: %d\n",
                      (int)status);
        umi_trader_workbench_profile_destroy(profile);
        return 1;
    }

    status = umi_trader_runtime_layout_default(&layout);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        (void)fprintf(stderr,
                      "Unable to materialise Trader layout: %d\n",
                      (int)status);
        umi_trader_workbench_profile_destroy(profile);
        return 1;
    }

    (void)printf("Umicom Trader Module\n");
    (void)printf("Profile: %s\n", snapshot.profile_id);
    (void)printf("Groups: %zu\n", snapshot.group_count);
    (void)printf("Endpoints: %zu\n", snapshot.endpoint_count);
    (void)printf("Trading group: %s\n", snapshot.trading_group_id);
    (void)printf("Operations group: %s\n", snapshot.operations_group_id);
    (void)printf("Layout: %s\n", layout.name);
    (void)printf("Layout windows: %zu\n", layout.window_count);

    /* Report the same Framework launch gate used by Umicom Desk. */
    status = umi_application_launch_readiness_check(
        "org.umicom.trader", &launch_readiness);
    if (status == UMI_STATUS_OK) {
        (void)printf("Launch readiness: %s (%u%%) - %s\n",
                     umi_application_launch_readiness_state_text(
                         launch_readiness.state),
                     launch_readiness.feature_readiness_percent,
                     launch_readiness.reason);
    }
    /* Report the same portfolio health summary that Umicom Desk uses for product selection. */
    status = umi_application_launch_readiness_summary(&portfolio_readiness);
    if (status == UMI_STATUS_OK) {
        (void)printf("Portfolio readiness: %zu/%zu launchable, %zu blocked, %u%% average\n",
                     portfolio_readiness.ready_count,
                     portfolio_readiness.application_count,
                     portfolio_readiness.blocked_count,
                     portfolio_readiness.average_feature_readiness_percent);
    }
    /* Use the shared command palette query to count searchable Trader layout commands. */
    command_query.structure_size = sizeof(command_query);
    command_query.text = "layout";
    if (umi_application_command_surface_build(
            umi_application_experience_catalogue_find("org.umicom.trader"),
            &command_surface) == UMI_STATUS_OK &&
        umi_application_command_surface_query(
            &command_surface, &command_query, &command_result) == UMI_STATUS_OK) {
        (void)printf("Palette layout commands: %zu\n", command_result.match_count);
    }

    umi_trader_workbench_profile_destroy(profile);
    return 0;
}

/* Financial rules remain in Framework; this host owns only command routing
 * and writing the synthetic report to its console. */
int main(int argc, char **argv)
{
    if (argc == 1) return RunDefault();
    if (argc == 2 && strcmp(argv[1], "--ufmp-simulate") == 0) {
#ifdef UMICOM_HAS_UFMP_FOUNDATION
        char report[1024];
        size_t required = 0U;
        UmiStatus status = UmiUfmpSimulationFormat(report, sizeof report, &required);
        if (status != UMI_STATUS_OK) {
            fprintf(stderr, "UFMP simulation failed: %d\n", (int)status);
            return 1;
        }
        return fputs(report, stdout) == EOF ? 1 : 0;
#else
        fputs("This Framework SDK does not include UFMP simulation support.\n", stderr);
        return 2;
#endif
    }
    if (argc == 2 && strcmp(argv[1], "--clearing-preview") == 0) {
#ifdef UMICOM_HAS_UFMP_CLEARING
        char report[1024];
        size_t required = 0U;
        UmiStatus status = UmiUfmpClearingExampleFormat(report, sizeof report, &required);
        if (status != UMI_STATUS_OK) {
            fprintf(stderr, "Clearing preview failed: %d\n", (int)status);
            return 1;
        }
        return fputs(report, stdout) == EOF ? 1 : 0;
#else
        fputs("This Framework SDK does not include clearing previews.\n", stderr);
        return 2;
#endif
    }
    if ((argc == 3 || argc == 4) && strcmp(argv[1], "--outbox-list") == 0) {
#ifdef UMICOM_HAS_UFMP_OUTBOX
        char report[16384]; size_t required = 0U;
        UmiStatus status = UmiUfmpOutboxInspect(argv[2], argc == 4 ? argv[3] : NULL,
            report, sizeof report, &required);
        if (status != UMI_STATUS_OK) {
            fprintf(stderr, "Outgoing delivery review failed: %d\n", (int)status);
            return 1;
        }
        return fputs(report, stdout) == EOF ? 1 : 0;
#else
        fputs("This Framework SDK does not include outgoing review support.\n", stderr);
        return 2;
#endif
    }
    if (argc == 2 && strcmp(argv[1], "--outbox-recovery") == 0) {
#ifdef UMICOM_HAS_UFMP_OUTBOX
        char report[768];
        size_t required = 0U;
        UmiStatus status = UmiUfmpOutboxExampleFormat(report, sizeof report, &required);
        if (status != UMI_STATUS_OK) {
            fprintf(stderr, "Outgoing recovery example failed: %d\n", (int)status);
            return 1;
        }
        return fputs(report, stdout) == EOF ? 1 : 0;
#else
        fputs("This Framework SDK does not include outgoing recovery support.\n", stderr);
        return 2;
#endif
    }
    /* Retain the earlier help text for review; the expanded line documents both optional commands. */
#if 0
    fputs("Usage: umicom-trader-console [--ufmp-simulate]\n", stderr);
#endif
    /* Retain the former usage while exposing the shared recovery walkthrough. */
#if 0
    fputs("Usage: umicom-trader-console [--ufmp-simulate | --clearing-preview]\n", stderr);
#endif
    fputs("Usage: umicom-trader-console [--ufmp-simulate | --clearing-preview | --outbox-recovery]\n", stderr);
    fputs("       --outbox-list <existing-sqlite-database> [next-cursor]\n", stderr);
    return 2;
}
