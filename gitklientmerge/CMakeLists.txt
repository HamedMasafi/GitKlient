aux_source_directory(../core/git GIT_SRC)
aux_source_directory(../core/diff DIFF_SRC)
aux_source_directory(../core/widgets CORE_WIDGETS_SRC)
include_directories(${CMAKE_SOURCE_DIR}/core)

set(gitklient_SRCS
    ${GIT_SRC}
    ${DIFF_SRC}
    ${CORE_WIDGETS_SRC}

    main.cpp
    gitklientmergewindow.cpp
    mainmergewidget.cpp
    openfilesdialog.cpp
    closeeventdialog.cpp
)

ecm_qt_declare_logging_category(gitklient_SRCS
    HEADER gitklientdebug.h
    IDENTIFIER GITKLIENT
    CATEGORY_NAME "gitklient"
)
kconfig_add_kcfg_files(gitklient_SRCS GitKlientMergeSettings.kcfgc)
ki18n_wrap_ui(gitklient_SRCS
    gitklientmergeview.ui
    settingsBase.ui
    mainmergewidget.ui
    openfilesdialog.ui
    closeeventdialog.ui
)

add_executable(gitklientmerge ${gitklient_SRCS})

target_link_libraries(gitklientmerge
    KF5::CoreAddons
    KF5::I18n
    KF5::XmlGui
    KF5::ConfigWidgets
    KF5::DBusAddons
    KF5::Crash
    Qt5::Core
    Qt5::Widgets
    Qt5::Concurrent
    KF5::KIOCore
    KF5::KIOWidgets
    KF5::TextWidgets
    KF5SyntaxHighlighting
    KF5::TextEditor
)

install(TARGETS gitklientmerge ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.gitklientmerge.desktop  DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.gitklientmerge.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES gitklientmergeui.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/gitklientmerge)
