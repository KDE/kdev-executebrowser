project(executebrowser)

cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${executebrowser_SOURCE_DIR}/cmake)

find_package(ECM REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEInstallDirs)
include(KDECMakeSettings)
find_package(KF5 REQUIRED I18n ItemModels ConfigWidgets)

find_package(KDevPlatform REQUIRED 5.0.0)

set(VERSION_MAJOR 5)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h"
  @ONLY
)

remove_definitions( -DKDE_DEFAULT_DEBUG_AREA=24000 )
add_definitions( -DKDE_DEFAULT_DEBUG_AREA=9515 )

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}

    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

########### next target ###############

set(kdevexecutebrowser_PART_UIS
    browserappconfig.ui
)

set(kdevexecutebrowser_PART_SRCS
    executebrowserplugin.cpp
    browserappconfig.cpp
    browserappjob.cpp
)

ki18n_wrap_ui(kdevexecutebrowser_PART_SRCS ${kdevexecutebrowser_PART_UIS})

kdevplatform_add_plugin(kdevexecutebrowser JSON kdevexecutebrowser.json SOURCES ${kdevexecutebrowser_PART_SRCS})
target_link_libraries(kdevexecutebrowser
    KDev::Interfaces
    KDev::Util
    KDev::Project
    KDev::OutputView
    KF5::I18n
)

install(FILES iexecutebrowserplugin.h DESTINATION ${INCLUDE_INSTALL_DIR}/kdevexecutebrowser COMPONENT Devel)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
