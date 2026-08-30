#-----------------------------------------------------------------------------
# Umicom Trader Module
# File: CMakeLists.txt
#
# PURPOSE:
#   Build the thin Umicom Trader application composition over Framework-owned
#   linked-workbench, selection-provider, context, experience and GTK4 layout
#   rendering contracts.
#
# Created by: Sammy Hegab
# Organisation: Umicom Foundation
# Licence: MIT
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

if(NOT TARGET Umicom::Framework)
    find_package(UmicomFramework CONFIG REQUIRED)
endif()

if(NOT TARGET Umicom::workbench_selection_provider)
    message(FATAL_ERROR
        "Umicom Trader requires Framework target "
        "Umicom::workbench_selection_provider")
endif()
if(NOT TARGET Umicom::application)
    message(FATAL_ERROR
        "Umicom Trader requires Framework target Umicom::application")
endif()
if(NOT TARGET Umicom::trading_ui)
    message(FATAL_ERROR
        "Umicom Trader requires Framework target Umicom::trading_ui")
endif()

if(NOT COMMAND umicom_apply_warnings)
    function(umicom_apply_warnings target)
        if(MSVC)
            target_compile_options(${target} PRIVATE /W4)
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

if(NOT COMMAND umicom_apply_sanitizers)
    function(umicom_apply_sanitizers target)
    endfunction()
endif()

add_library(umicom_trader_module STATIC
    src/application_surface.c
    src/application_surface_controllers.c
    src/application_surface_commands.c
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
elseif(UMICOM_TRADER_BUILD_GTK4)
    message(STATUS
        "Umicom Trader GTK4 workstation disabled because Umicom::ui_gtk4 is unavailable")
endif()

if(BUILD_TESTING)
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
    if(COMMAND umicom_register_validation_target)
        umicom_register_validation_target(
            trader-test-readiness-contribution-test)
    endif()

    if(TARGET Umicom::trader_gtk)
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
if(TARGET umicom_trader_gtk)
    install(TARGETS umicom_trader_gtk
        EXPORT UmicomTraderModuleTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()
if(TARGET umicom-trader-console)
    install(TARGETS umicom-trader-console
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
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

if(BUILD_TESTING)
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
    if(COMMAND umicom_register_validation_target)
        umicom_register_validation_target(
            umicom-trader-experience-acceptance-test)
    endif()

    # Product-level application surface tests are headless and verify the
    # canonical recipe, controller coverage and safe command preparation path.
    foreach(UMICOM_TRADER_APPLICATION_SURFACE_TEST
            application_surface
            application_surface_commands
            application_surface_controllers)
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
