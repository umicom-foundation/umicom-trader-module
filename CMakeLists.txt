#-----------------------------------------------------------------------------
# Umicom Trader Module
# File: CMakeLists.txt
#
# PURPOSE:
#   Build the thin Umicom Trader application composition over Framework-owned
#   linked-workbench, selection-provider, context, experience and GTK4 layout
#   rendering contracts.
#
# AUTHOR AND ORGANISATION:
#   Sammy Hegab
#   Umicom Foundation
#
# LICENCE:
#   MIT
#-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.24)

project(
    UmicomTraderModule
    VERSION 0.2.0
    DESCRIPTION "Thin Umicom Trader workbench composition"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(UMICOM_TRADER_BUILD_CONSOLE
       "Build the native Umicom Trader verification console"
       ON)
option(UMICOM_TRADER_BUILD_GTK4
       "Build the Framework-rendered GTK4 Umicom Trader workstation"
       ON)

include(CTest)
include(GNUInstallDirs)

# Use the shared build helper when it is available from the parent composition.
if(COMMAND umicom_register_header_governance_test AND
   CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    umicom_register_header_governance_test(
        trader.sdk.public_headers
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "Umicom Trader")
endif()

# Load the dependency only when the parent build has not already provided its target.
if(NOT TARGET Umicom::Framework)
    find_package(UmicomFramework CONFIG REQUIRED)
endif()

# Load the dependency only when the parent build has not already provided its target.
if(NOT TARGET Umicom::workbench_selection_provider)
    message(FATAL_ERROR
        "Umicom Trader requires Framework target "
        "Umicom::workbench_selection_provider")
endif()
# Load the dependency only when the parent build has not already provided its target.
if(NOT TARGET Umicom::application)
    message(FATAL_ERROR
        "Umicom Trader requires Framework target Umicom::application")
endif()
# Load the dependency only when the parent build has not already provided its target.
if(NOT TARGET Umicom::trading_ui)
    message(FATAL_ERROR
        "Umicom Trader requires Framework target Umicom::trading_ui")
endif()

# Define the local fallback only when the parent build did not provide the shared helper.
if(NOT COMMAND umicom_apply_warnings)
    # Define the apply warnings build helper so parent and application projects apply one
    # consistent rule.
    function(umicom_apply_warnings target)
        # Select the warning options understood by the active compiler.
        if(MSVC)
            target_compile_options(${target} PRIVATE /W4)
        # Use this fallback path when the earlier condition does not apply.
        else()
            target_compile_options(${target} PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Wconversion
                -Wshadow
            )
        endif()
    endfunction()
endif()

# Define the local fallback only when the parent build did not provide the shared helper.
if(NOT COMMAND umicom_apply_sanitizers)
    # Define the apply sanitizers build helper so parent and application projects apply one
    # consistent rule.
    function(umicom_apply_sanitizers target)
    endfunction()
endif()

add_library(umicom_trader_module STATIC
    src/application_surface.c
    src/application_surface_controllers.c
    src/application_surface_commands.c
    src/application_surface_policy.c
    src/workbench_profile.c
    src/experience.c
    src/runtime.c
    src/readiness.c
    src/workspace_commands.c
    src/production_profile.c
    src/production_panels.c
    src/production_commands.c
    src/production_readiness.c
    src/experience_acceptance.c
    src/test_readiness_contribution.c
    # Capability Centre remains a thin projection of Framework-owned truth.
    src/capability_centre.c
)
add_library(Umicom::trader_module ALIAS umicom_trader_module)

target_include_directories(umicom_trader_module PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(umicom_trader_module PUBLIC
    Umicom::Framework
    Umicom::workbench_selection_provider
    Umicom::application
    Umicom::trading_ui
)
umicom_apply_warnings(umicom_trader_module)
umicom_apply_sanitizers(umicom_trader_module)

# Create this optional product surface only when its build option is enabled.
if(UMICOM_TRADER_BUILD_CONSOLE)
    add_executable(umicom-trader-console
        src/console/main.c
    )
    target_link_libraries(umicom-trader-console PRIVATE
        Umicom::trader_module
    )
    umicom_apply_warnings(umicom-trader-console)
    umicom_apply_sanitizers(umicom-trader-console)
endif()

# Trader remains a thin product. The executable and this small adapter own only
# product lifecycle/composition; all layout, GTK rendering and trading state
# stay in Framework targets.
if(UMICOM_TRADER_BUILD_GTK4 AND TARGET Umicom::ui_gtk4)
    add_library(umicom_trader_gtk STATIC
        src/gtk/workstation.c
    )
    add_library(Umicom::trader_gtk ALIAS umicom_trader_gtk)
    target_include_directories(umicom_trader_gtk PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
    target_link_libraries(umicom_trader_gtk PUBLIC
        Umicom::trader_module
        Umicom::ui_gtk4
    )
    umicom_apply_warnings(umicom_trader_gtk)
    umicom_apply_sanitizers(umicom_trader_gtk)

    add_executable(umicom-trader
        src/gtk/main.c
    )
    target_link_libraries(umicom-trader PRIVATE
        Umicom::trader_gtk
    )
    umicom_apply_warnings(umicom-trader)
    umicom_apply_sanitizers(umicom-trader)
    # Define the local fallback only when the parent build did not provide the shared helper.
    if(NOT COMMAND umicom_apply_application_branding)
        include(
            "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/cmake/UmicomApplicationBranding.cmake")
    endif()
    umicom_apply_application_branding(
        TARGET umicom-trader
        PRODUCT_NAME "Umicom Trader"
        INTERNAL_NAME "umicom-trader"
        APPLICATION_ID "org.umicom.trader"
        VERSION_MAJOR ${PROJECT_VERSION_MAJOR}
        VERSION_MINOR ${PROJECT_VERSION_MINOR}
        VERSION_PATCH ${PROJECT_VERSION_PATCH}
        RESOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/resources"
        DESKTOP_ENTRY
        WINDOWS_GUI)
# Apply this branch only when its contract condition is satisfied.
elseif(UMICOM_TRADER_BUILD_GTK4)
    message(STATUS
        "Umicom Trader GTK4 workstation disabled because Umicom::ui_gtk4 is unavailable")
endif()

# Register verification targets only when the developer has enabled testing.
if(BUILD_TESTING)
    # This focused test keeps planned and foundational trading work visible.
    add_executable(trader-capability-centre-test
        tests/test_capability_centre.c)
    target_link_libraries(trader-capability-centre-test PRIVATE
        Umicom::trader_module)
    umicom_apply_warnings(trader-capability-centre-test)
    umicom_apply_sanitizers(trader-capability-centre-test)
    add_test(
        NAME trader.module.capability_centre
        COMMAND trader-capability-centre-test)

    add_executable(trader-workbench-profile-test
        tests/test_workbench_profile.c
    )
    target_link_libraries(trader-workbench-profile-test PRIVATE
        Umicom::trader_module
    )
    umicom_apply_warnings(trader-workbench-profile-test)
    umicom_apply_sanitizers(trader-workbench-profile-test)
    add_test(
        NAME trader.module.workbench_profile
        COMMAND trader-workbench-profile-test
    )

    add_executable(trader-experience-test
        tests/test_experience.c
    )
    target_link_libraries(trader-experience-test PRIVATE
        Umicom::trader_module
    )
    umicom_apply_warnings(trader-experience-test)
    umicom_apply_sanitizers(trader-experience-test)
    add_test(
        NAME trader.module.experience
        COMMAND trader-experience-test
    )

    add_executable(trader-runtime-test
        tests/test_runtime.c
    )
    target_link_libraries(trader-runtime-test PRIVATE
        umicom_trader_module
    )
    umicom_apply_warnings(trader-runtime-test)
    umicom_apply_sanitizers(trader-runtime-test)
    add_test(
        NAME trader.module.runtime
        COMMAND trader-runtime-test
    )

    add_executable(trader-readiness-runtime-test
        tests/test_readiness_runtime.c
    )
    target_link_libraries(trader-readiness-runtime-test PRIVATE
        umicom_trader_module
    )
    umicom_apply_warnings(trader-readiness-runtime-test)
    umicom_apply_sanitizers(trader-readiness-runtime-test)
    add_test(
        NAME trader.module.readiness_runtime
        COMMAND trader-readiness-runtime-test
    )

    add_executable(trader-framework-views-test
        tests/test_framework_views.c
    )
    target_link_libraries(trader-framework-views-test PRIVATE
        Umicom::trader_module
    )
    umicom_apply_warnings(trader-framework-views-test)
    umicom_apply_sanitizers(trader-framework-views-test)
    add_test(
        NAME trader.module.framework_views
        COMMAND trader-framework-views-test
    )

    add_executable(trader-test-readiness-contribution-test
        tests/test_test_readiness_contribution.c)
    target_link_libraries(trader-test-readiness-contribution-test PRIVATE
        Umicom::trader_module)
    umicom_apply_warnings(trader-test-readiness-contribution-test)
    umicom_apply_sanitizers(trader-test-readiness-contribution-test)
    add_test(NAME trader.module.test_readiness_contribution
        COMMAND trader-test-readiness-contribution-test)
    # Use the shared build helper when it is available from the parent composition.
    if(COMMAND umicom_register_validation_target)
        umicom_register_validation_target(
            trader-test-readiness-contribution-test)
    endif()

    # Configure the optional target only when its feature has created it.
    if(TARGET Umicom::trader_gtk)
        # Exercise the real product owner and native controls with isolated storage.
        add_executable(trader-layout-library-order-test tests/test_layout_library_order.c)
        target_link_libraries(trader-layout-library-order-test PRIVATE Umicom::trader_gtk)
        umicom_apply_warnings(trader-layout-library-order-test)
        umicom_apply_sanitizers(trader-layout-library-order-test)
        add_test(NAME trader.module.layout_library_order COMMAND trader-layout-library-order-test)
        set_tests_properties(trader.module.layout_library_order PROPERTIES
            TIMEOUT 120 SKIP_RETURN_CODE 77 LABELS "trader;gtk4;layout-order;regression")
        if(COMMAND umicom_register_validation_target)
            umicom_register_validation_target(trader-layout-library-order-test)
        endif()
        add_executable(trader-gtk-workstation-test
            tests/test_gtk_workstation.c
        )
        target_link_libraries(trader-gtk-workstation-test PRIVATE
            Umicom::trader_gtk
        )
        umicom_apply_warnings(trader-gtk-workstation-test)
        umicom_apply_sanitizers(trader-gtk-workstation-test)
        add_test(
            NAME trader.module.gtk_workstation
            COMMAND trader-gtk-workstation-test
        )
        set_tests_properties(trader.module.gtk_workstation PROPERTIES
            LABELS "trader;gtk4;layout;workstation;smoke")
    endif()
endif()

install(TARGETS umicom_trader_module
    EXPORT UmicomTraderModuleTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
# Configure the optional target only when its feature has created it.
if(TARGET umicom_trader_gtk)
    install(TARGETS umicom_trader_gtk
        EXPORT UmicomTraderModuleTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()
# Configure the optional target only when its feature has created it.
if(TARGET umicom-trader-console)
    install(TARGETS umicom-trader-console
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
# Configure the optional target only when its feature has created it.
if(TARGET umicom-trader)
    install(TARGETS umicom-trader
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES application.umicom.yaml README.md LICENSE
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/applications/trader)

message(STATUS "Umicom Trader Module version: ${PROJECT_VERSION}")
message(STATUS "Umicom Trader Framework workbench and experience profiles enabled")

include(
    "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/cmake/UmicomApplicationProductisationContribution.cmake")
umicom_add_application_productisation_contribution(
    NAME "trader"
    TARGET "umicom_trader_productisation"
    SOURCE "src/productisation_contribution.c"
    TEST_SOURCE "tests/test_productisation_contribution.c")
target_link_libraries(umicom_trader_module PUBLIC
    umicom_trader_productisation)

# Register verification targets only when the developer has enabled testing.
if(BUILD_TESTING)
    # Visit each bounded item once so every record receives the same rule.
    foreach(UMICOM_TRADER_PRODUCTION_TEST
            production_profile
            production_panels
            production_commands
            production_readiness)
        string(REPLACE "_" "-"
            UMICOM_TRADER_PRODUCTION_TEST_SUFFIX
            "${UMICOM_TRADER_PRODUCTION_TEST}")
        add_executable(
            "umicom-trader-${UMICOM_TRADER_PRODUCTION_TEST_SUFFIX}-test"
            "tests/test_${UMICOM_TRADER_PRODUCTION_TEST}.c")
        target_link_libraries(
            "umicom-trader-${UMICOM_TRADER_PRODUCTION_TEST_SUFFIX}-test"
            PRIVATE Umicom::trader_module)
        umicom_apply_warnings(
            "umicom-trader-${UMICOM_TRADER_PRODUCTION_TEST_SUFFIX}-test")
        umicom_apply_sanitizers(
            "umicom-trader-${UMICOM_TRADER_PRODUCTION_TEST_SUFFIX}-test")
        add_test(
            NAME "trader.${UMICOM_TRADER_PRODUCTION_TEST}"
            COMMAND
                "umicom-trader-${UMICOM_TRADER_PRODUCTION_TEST_SUFFIX}-test")
        # Use the shared build helper when it is available from the parent composition.
        if(COMMAND umicom_register_validation_target)
            umicom_register_validation_target(
                "umicom-trader-${UMICOM_TRADER_PRODUCTION_TEST_SUFFIX}-test")
        endif()
    endforeach()

    add_executable(umicom-trader-experience-acceptance-test
        tests/test_experience_acceptance.c)
    target_link_libraries(umicom-trader-experience-acceptance-test
        PRIVATE Umicom::trader_module)
    umicom_apply_warnings(umicom-trader-experience-acceptance-test)
    umicom_apply_sanitizers(umicom-trader-experience-acceptance-test)
    add_test(NAME trader.experience_acceptance
        COMMAND umicom-trader-experience-acceptance-test)
    # Use the shared build helper when it is available from the parent composition.
    if(COMMAND umicom_register_validation_target)
        umicom_register_validation_target(
            umicom-trader-experience-acceptance-test)
    endif()

    # Product-level application surface tests are headless and verify the
    # canonical recipe, controller coverage and safe command preparation path.
    foreach(UMICOM_TRADER_APPLICATION_SURFACE_TEST
            application_surface
            application_surface_commands
            application_surface_controllers
            application_surface_policy)
        string(REPLACE "_" "-" UMICOM_TRADER_APPLICATION_SURFACE_TARGET
            "${UMICOM_TRADER_APPLICATION_SURFACE_TEST}")
        add_executable(
            "umicom-trader-${UMICOM_TRADER_APPLICATION_SURFACE_TARGET}-test"
            "tests/test_${UMICOM_TRADER_APPLICATION_SURFACE_TEST}.c")
        target_link_libraries(
            "umicom-trader-${UMICOM_TRADER_APPLICATION_SURFACE_TARGET}-test"
            PRIVATE Umicom::trader_module)
        umicom_apply_warnings(
            "umicom-trader-${UMICOM_TRADER_APPLICATION_SURFACE_TARGET}-test")
        umicom_apply_sanitizers(
            "umicom-trader-${UMICOM_TRADER_APPLICATION_SURFACE_TARGET}-test")
        add_test(
            NAME "trader.${UMICOM_TRADER_APPLICATION_SURFACE_TEST}"
            COMMAND
                "umicom-trader-${UMICOM_TRADER_APPLICATION_SURFACE_TARGET}-test")
    endforeach()
endif()

# Install the public workflow lesson beside this product's other guides.
install(FILES docs/WORKFLOW_TOOLS.html
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/trader/docs)

# Keep the beginner ordering workflow available with the installed product.
install(FILES docs/ORDERING_WORKSPACE_LAYOUTS.md
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/trader/docs
    COMPONENT Trader)

# The product must expose the same native chart and review controls as Framework.
if(BUILD_TESTING AND TARGET Umicom::trader_gtk)
    add_executable(trader-chart-order-review-test tests/test_chart_and_order_review.c)
    target_link_libraries(trader-chart-order-review-test PRIVATE Umicom::trader_gtk)
    umicom_apply_warnings(trader-chart-order-review-test)
    umicom_apply_sanitizers(trader-chart-order-review-test)
    add_test(NAME trader.module.chart_order_review COMMAND trader-chart-order-review-test)
    set_tests_properties(trader.module.chart_order_review PROPERTIES
        TIMEOUT 90 SKIP_RETURN_CODE 77 LABELS "trader;gtk4;chart;orders;regression")
    if(COMMAND umicom_register_validation_target)
        umicom_register_validation_target(trader-chart-order-review-test)
    endif()
endif()

# Keep the beginner report workflow with the installed documentation.
install(FILES docs/COPYING_ORDER_REPORTS.md
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/trader/docs)

# Install the educational chart persistence workflow with the product.
install(FILES docs/SAVING_CHARTS.md
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/trader/docs)

# Qualify the actual product layout and shared report lifetime, not a mock view.
if(BUILD_TESTING AND TARGET Umicom::trader_gtk)
    add_executable(trader-session-review-test tests/test_session_review.c)
    target_link_libraries(trader-session-review-test PRIVATE Umicom::trader_gtk)
    umicom_apply_warnings(trader-session-review-test)
    umicom_apply_sanitizers(trader-session-review-test)
    add_test(NAME trader.module.session_review COMMAND trader-session-review-test)
    set_tests_properties(trader.module.session_review PROPERTIES TIMEOUT 90 SKIP_RETURN_CODE 77 LABELS "trader;report;gtk4;regression")
    if(COMMAND umicom_register_validation_target)
        umicom_register_validation_target(trader-session-review-test)
    endif()
endif()
install(FILES docs/REVIEWING_A_TRADING_SESSION.md
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/trader/docs)

# Install the beginner chart annotation workflow beside saved-chart guidance.
install(FILES docs/CHART_DRAWING_OBJECTS.md
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/trader/docs)


# Product qualification uses the real chart controls and borrowed test storage.
if(BUILD_TESTING AND TARGET Umicom::trader_gtk)
    add_executable(trader-drawing-appearance-test tests/test_drawing_appearance.c)
    target_link_libraries(trader-drawing-appearance-test PRIVATE Umicom::trader_gtk)
    umicom_apply_warnings(trader-drawing-appearance-test)
    umicom_apply_sanitizers(trader-drawing-appearance-test)
    foreach(case save restore retained)
        add_test(NAME "trader.drawing_appearance.${case}" COMMAND trader-drawing-appearance-test "${case}")
        set_tests_properties("trader.drawing_appearance.${case}" PROPERTIES
            TIMEOUT 90 SKIP_RETURN_CODE 77 LABELS "trader;chart;gtk4;regression")
    endforeach()
    if(COMMAND umicom_register_validation_target)
        umicom_register_validation_target(trader-drawing-appearance-test)
    endif()
endif()
install(FILES docs/DRAWING_APPEARANCE.md
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/trader/docs)

# Qualify the real native chart and public product facade together.
if(BUILD_TESTING AND TARGET Umicom::trader_gtk)
    add_executable(trader-chart-timeframes-test tests/test_chart_timeframes.c)
    target_link_libraries(trader-chart-timeframes-test PRIVATE Umicom::trader_gtk)
    umicom_apply_warnings(trader-chart-timeframes-test)
    umicom_apply_sanitizers(trader-chart-timeframes-test)
    foreach(case save restore invalid retained)
        add_test(NAME "trader.chart_timeframe.${case}" COMMAND trader-chart-timeframes-test "${case}")
        set_tests_properties("trader.chart_timeframe.${case}" PROPERTIES
            TIMEOUT 90 SKIP_RETURN_CODE 77 LABELS "trader;chart;gtk4;regression")
    endforeach()
    if(COMMAND umicom_register_validation_target)
        umicom_register_validation_target(trader-chart-timeframes-test)
    endif()
endif()
install(FILES docs/CHART_TIMEFRAMES.md DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/trader/docs)

# Product qualification uses the real chart controls and borrowed test storage.
if(BUILD_TESTING AND TARGET Umicom::trader_gtk)
    add_executable(trader-drawing-history-test tests/test_drawing_history.c)
    target_link_libraries(trader-drawing-history-test PRIVATE Umicom::trader_gtk)
    umicom_apply_warnings(trader-drawing-history-test)
    umicom_apply_sanitizers(trader-drawing-history-test)
    foreach(case facade native restore retained)
        add_test(NAME "trader.drawing_history.${case}" COMMAND trader-drawing-history-test "${case}")
        set_tests_properties("trader.drawing_history.${case}" PROPERTIES
            TIMEOUT 90 SKIP_RETURN_CODE 77 LABELS "trader;chart;gtk4;regression")
    endforeach()
    if(COMMAND umicom_register_validation_target)
        umicom_register_validation_target(trader-drawing-history-test)
    endif()
endif()
install(FILES docs/DRAWING_HISTORY.md
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/trader/docs)

# The thin product must consume the real Framework session codec and preserve
# its active value when imported data is damaged or its review is stale.
if(BUILD_TESTING)
    add_executable(umicom-trader-session-transfer-test tests/test_session_transfer.c)
    target_link_libraries(umicom-trader-session-transfer-test PRIVATE Umicom::trader_module)
    umicom_apply_warnings(umicom-trader-session-transfer-test)
    umicom_apply_sanitizers(umicom-trader-session-transfer-test)
    if(COMMAND umicom_register_validation_target)
        umicom_register_validation_target(umicom-trader-session-transfer-test)
    endif()
    add_test(NAME trader.workspace.session_transfer COMMAND umicom-trader-session-transfer-test)
    set_tests_properties(trader.workspace.session_transfer PROPERTIES
        TIMEOUT 60 LABELS "trader;workspace;value-archive;regression")
endif()

install(FILES docs/WORKSPACE_SESSION_EXCHANGE.html DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/trader/docs)

# Exercise the product identity boundary against the shared transaction owner.
if(BUILD_TESTING)
    add_executable(umicom_trader_module-context-review-test tests/test_context_review.c)
    target_link_libraries(umicom_trader_module-context-review-test PRIVATE umicom_trader_module)
    umicom_apply_warnings(umicom_trader_module-context-review-test)
    umicom_apply_sanitizers(umicom_trader_module-context-review-test)
    foreach(context_case publication stale foreign clear divergent_ui)
        add_test(NAME trader.context_review.${context_case}
            COMMAND umicom_trader_module-context-review-test ${context_case})
        set_tests_properties(trader.context_review.${context_case} PROPERTIES
            LABELS "application;context-review;trader")
    endforeach()
    if(COMMAND umicom_register_validation_target)
        umicom_register_validation_target(umicom_trader_module-context-review-test)
    endif()
endif()

# Verify the product host stores exact coordinates through shared chart persistence.
if(BUILD_TESTING AND TARGET Umicom::trader_gtk)
    add_executable(trader-drawing-coordinates-test tests/test_drawing_coordinates.c)
    target_link_libraries(trader-drawing-coordinates-test PRIVATE Umicom::trader_gtk)
    umicom_apply_warnings(trader-drawing-coordinates-test)
    umicom_apply_sanitizers(trader-drawing-coordinates-test)
    foreach(case save restore invalid retained)
        add_test(NAME trader.drawing_coordinates.${case} COMMAND trader-drawing-coordinates-test ${case})
        set_tests_properties(trader.drawing_coordinates.${case} PROPERTIES
            TIMEOUT 90 SKIP_RETURN_CODE 77 LABELS "trader;chart;gtk4;regression")
    endforeach()
    if(COMMAND umicom_register_validation_target)
        umicom_register_validation_target(trader-drawing-coordinates-test)
    endif()
endif()
install(FILES docs/DRAWING_COORDINATES.html DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/trader/docs)

# Explain local account storage and the broker-owned authentication boundary.
install(FILES docs/sign-in-and-broker-access.html
    DESTINATION "${CMAKE_INSTALL_DATADIR}/umicom/trader/docs")
