

#add_library(${PLUGIN} MODULE actionmanager.cpp gitmanager.cpp)
#target_link_libraries(${PLUGIN}
#    Qt5::Core
#    KF5::CoreAddons
#    KF5::KIOWidgets
#    KF5::KIOCore
#    KF5::KIOFileWidgets
#    KF5::KIOWidgets
#    KF5::KIONTLM
#)
#install(FILES actionmanager.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR} RENAME ${PLUGIN}.desktop)
#install(TARGETS ${PLUGIN} DESTINATION /usr/lib/x86_64-linux-gnu/qt5/plugins)



#kcoreaddons_add_plugin(${PLUGIN} SOURCES actionmanager.cpp INSTALL_NAMESPACE "kf5/kfileitemaction")
#kcoreaddons_desktop_to_json(${PLUGIN} actionmanager.desktop) # generate the json file
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS KIO)

#---ACTION PLUGIN---
set(PLUGIN kdegititemaction)

include_directories(../src)
aux_source_directory(../src/git GIT_SRC)

set(PLUGIN_SRC actionmanager.cpp ${GIT_SRC})
kcoreaddons_add_plugin(${PLUGIN}
    SOURCES ${PLUGIN_SRC} statuscache.cpp
    JSON ${PLUGIN}.json
    INSTALL_NAMESPACE "kf5/kfileitemaction")

target_link_libraries(${PLUGIN}
    KF5::I18n
    KF5::WidgetsAddons
    KF5::KIOWidgets)


#---OVERLAY PLUGIN---
set(OVERLAY_SRC klientdolphinoverlayplugin.cpp ${GIT_SRC})
set(OVERLAY_PLUGIN klientdolphinoverlayplugin)
kcoreaddons_add_plugin(
    ${OVERLAY_PLUGIN}
    INSTALL_NAMESPACE "kf5/overlayicon"
    JSON ${OVERLAY_PLUGIN}.json
    SOURCES ${OVERLAY_SRC} statuscache.cpp
)
target_link_libraries(
    ${OVERLAY_PLUGIN}
    KF5::CoreAddons
    KF5::KIOCore
    KF5::KIOWidgets
)
