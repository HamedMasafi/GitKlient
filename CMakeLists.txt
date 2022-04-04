cmake_minimum_required(VERSION 2.8.12)

project(gitklient)

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(
    CMAKE_MODULE_PATH
    ${ECM_MODULE_PATH}
    ${ECM_KDE_MODULE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings NO_POLICY_SCOPE)
include(FeatureSummary)

include(ECMInstallIcons)
include(ECMQtDeclareLoggingCategory)
include(ECMAddAppIcon)
include(ECMSetupVersion)
include(ECMAddTests)

set (CMAKE_CXX_STANDARD 14)

set(QT_MIN_VERSION "5.5.0")
find_package(
    Qt5 ${QT_MIN_VERSION}
    CONFIG REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Concurrent
        Test
)

set(REQUIRED_KF5_VERSION "5.37.0")
find_package(KF5KIO)
find_package(
    KF5 ${REQUIRED_KF5_VERSION}
    REQUIRED COMPONENTS
        ConfigWidgets
        CoreAddons
        Crash
        DBusAddons
        DocTools
        I18n
        XmlGui
        KIO
        TextWidgets
        TextEditor
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

if(KF5DocTools_FOUND)
    add_subdirectory(doc)
    kdoctools_install(po)
else()
    message(WARNING "DocTools not found.")
endif()

add_subdirectory(icons)
add_subdirectory(src)
#add_subdirectory(autotests)

if (UNIX)
    add_subdirectory(dolphinplugins)
endif (UNIX)
