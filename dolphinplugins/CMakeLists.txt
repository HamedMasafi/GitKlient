find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS KIO)
include_directories(../src)
#aux_source_directory(../src/git GIT_SRC)
include(../src/git/CMakeLists.txt)
#file(GLOB_RECURSE GIT_SOURCE ../src/git *.cpp)


#---ACTION PLUGIN---
set(ACTIONS_PLUGIN gitklientitemaction)


set(
    ACTIONS_PLUGIN_SRC
    actionmanager.cpp
    statuscache.cpp
    ${GIT_SOURCE}
)
kcoreaddons_add_plugin(
    ${ACTIONS_PLUGIN}
    SOURCES ${ACTIONS_PLUGIN_SRC}
    INSTALL_NAMESPACE "kf5/kfileitemaction"
    JSON ${ACTIONS_PLUGIN}.json
)

target_link_libraries(
    ${ACTIONS_PLUGIN}
    KF5::I18n
    KF5::WidgetsAddons
    KF5::KIOWidgets
)


#---OVERLAY PLUGIN---
set(OVERLAY_PLUGIN gitklientoverlayplugin)
set(
    OVERLAY_SRC
    overlayplugin.cpp
    statuscache.cpp
    ${GIT_SOURCE}
)

kcoreaddons_add_plugin(
    ${OVERLAY_PLUGIN}
    SOURCES ${OVERLAY_SRC}
    INSTALL_NAMESPACE "kf5/overlayicon"
    JSON ${OVERLAY_PLUGIN}.json
)

target_link_libraries(
    ${OVERLAY_PLUGIN}
    KF5::CoreAddons
    KF5::KIOCore
    KF5::KIOWidgets
)
