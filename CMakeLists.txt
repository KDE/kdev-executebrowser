project(executebrowser)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${executebrowser_SOURCE_DIR}/cmake)

find_package(KDE4 REQUIRED)
find_package(KDevPlatform 1.5.60 REQUIRED)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 5)
set(VERSION_PATCH 60)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h"
  @ONLY
)

remove_definitions( -DKDE_DEFAULT_DEBUG_AREA=24000 )
add_definitions( -DKDE_DEFAULT_DEBUG_AREA=9515 )

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
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

kde4_add_ui_files( kdevexecutebrowser_PART_SRCS ${kdevexecutebrowser_PART_UIS} )
kde4_add_plugin(kdevexecutebrowser ${kdevexecutebrowser_PART_SRCS}  )
target_link_libraries(kdevexecutebrowser 
    ${KDE4_KDEUI_LIBS} 
    ${KDEVPLATFORM_INTERFACES_LIBRARIES} 
    ${KDEVPLATFORM_OUTPUTVIEW_LIBRARIES})

install(TARGETS kdevexecutebrowser DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############
install(FILES iexecutebrowserplugin.h DESTINATION ${INCLUDE_INSTALL_DIR}/kdevexecutebrowser COMPONENT Devel)
install(FILES kdevexecutebrowser.desktop DESTINATION ${SERVICES_INSTALL_DIR})
