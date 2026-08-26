#-----------------------------------------------------------------------------
# Umicom Trader Module
# File: CMakeLists.txt
#
# PURPOSE:
#   Build the thin Umicom Trader application composition over Framework-owned
#   linked-workbench, selection-provider, context and experience contracts.
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
    src/workbench_profile.c
    src/experience.c
    src/runtime.c
    src/readiness.c
    src/workspace_commands.c
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

endif()

install(TARGETS umicom_trader_module
    EXPORT UmicomTraderModuleTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(TARGET umicom-trader-console)
    install(TARGETS umicom-trader-console
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES application.umicom.yaml README.md LICENSE
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/applications/trader)

message(STATUS "Umicom Trader Module version: ${PROJECT_VERSION}")
message(STATUS "Umicom Trader Framework workbench and experience profiles enabled")
