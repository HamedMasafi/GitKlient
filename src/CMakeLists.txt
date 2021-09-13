aux_source_directory(git GIT_SRC)
aux_source_directory(diff DIFF_SRC)
aux_source_directory(common COMMON_SRC)
aux_source_directory(models MODELS_SRC)
aux_source_directory(widgets WIDGETS_SRC)
aux_source_directory(dialogs DIALOGS_SRC)
include_directories(${CMAKE_SOURCE_DIR}/core)


set(gitklient_SRCS
    ${GIT_SRC}
    ${DIFF_SRC}
    ${CORE_WIDGETS_SRC}
    ${MODELS_SRC}
    ${WIDGETS_SRC}
    ${DIALOGS_SRC}
    ${COMMON_SRC}

    main.cpp
    gitklientwindow.cpp
    gitklientview.cpp
#    widgetbase.cpp
    mainwidget.cpp
    buttonsgroup.cpp
    multipagewidget.cpp
    argsparser.cpp
    diffwindow.cpp
    gitklientmergewindow.cpp
    commandargsparser.cpp
)
#add_subdirectory(merge)

ecm_qt_declare_logging_category(gitklient_SRCS
    HEADER gitklientdebug.h
    IDENTIFIER GITKLIENT
    CATEGORY_NAME "gitklient"
)
kconfig_add_kcfg_files(gitklient_SRCS GitKlientSettings.kcfgc)
ki18n_wrap_ui(gitklient_SRCS
    gitklientViewBase.ui
    settingsBase.ui
    mainwidget.ui
    diffsettings.ui
    multipagewidget.ui
    gitklientmergeview.ui

    widgets/branchesstatuswidget.ui
    widgets/remotedetailswidget.ui
    widgets/remoteinfodialog.ui
    widgets/remoteswidget.ui
    widgets/stasheswidget.ui
    widgets/historyviewwidget.ui
    widgets/submoduleswidget.ui
    widgets/commitswidget.ui
    widgets/tagswidget.ui
    widgets/difftreeview.ui

    dialogs/mergeopenfilesdialog.ui
    dialogs/mergecloseeventdialog.ui
    dialogs/changedfilesdialog.ui
    dialogs/filestreedialog.ui
    dialogs/commitpushdialog.ui
    dialogs/pulldialog.ui
    dialogs/mergedialog.ui
    dialogs/runnerdialog.ui
    dialogs/fileviewerdialog.ui
    dialogs/diffdialog.ui
    dialogs/clonedialog.ui
    dialogs/submoduleinfodialog.ui
    dialogs/modificationsdialog.ui
    dialogs/taginfodialog.ui
    dialogs/diffopendialog.ui
    dialogs/selectbranchestodiffdialog.ui
    dialogs/filehistorydialog.ui
)

add_executable(gitklient ${gitklient_SRCS})

target_link_libraries(gitklient
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

install(TARGETS gitklient ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS gitklientdiff.sh DESTINATION bin RENAME gitklientdiff)
install(PROGRAMS gitklientmerge.sh DESTINATION bin RENAME gitklientmerge)
install(PROGRAMS org.kde.gitklient.desktop        DESTINATION ${KDE_INSTALL_APPDIR})
install(PROGRAMS org.kde.gitklient.diff.desktop   DESTINATION ${KDE_INSTALL_APPDIR})
install(PROGRAMS org.kde.gitklient.merge.desktop  DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.gitklient.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES gitklientui.rc      DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/gitklient)
install(FILES gitklientdiffui.rc  DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/gitklient)
install(FILES gitklientmergeui.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/gitklient)
