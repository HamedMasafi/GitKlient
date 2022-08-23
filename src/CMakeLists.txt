#aux_source_directory(git GIT_SRC)
aux_source_directory(diff DIFF_SRC)
aux_source_directory(common COMMON_SRC)
aux_source_directory(models MODELS_SRC)
aux_source_directory(widgets WIDGETS_SRC)
aux_source_directory(dialogs DIALOGS_SRC)
aux_source_directory(actions ACTIONS_SRC)
aux_source_directory(core CORE_SRC)
include_directories(${CMAKE_SOURCE_DIR}/core)

set(APP_SRCS
#    ${GIT_SRC}
    ${DIFF_SRC}
    ${CORE_WIDGETS_SRC}
    ${MODELS_SRC}
    ${WIDGETS_SRC}
    ${DIALOGS_SRC}
    ${COMMON_SRC}
    ${ACTIONS_SRC}
    ${CORE_SRC}

    main.cpp
    appwindow.cpp
    buttonsgroup.cpp
    multipagewidget.cpp
    diffwindow.cpp
    mergewindow.cpp
    commandargsparser.cpp
    appconfig.cpp
    settings/settingsmanager.cpp
)

ecm_qt_declare_logging_category(APP_SRCS
    HEADER appdebug.h
    IDENTIFIER GITKLIENT
    CATEGORY_NAME "gitklient"
)
kconfig_add_kcfg_files(APP_SRCS settings/GitKlientSettings.kcfgc)

#set(CMAKE_AUTOUIC ON)

#qt5_wrap_ui(
#    APP_SRCS
#    widgets/commandpullwidget.ui
#)

add_compile_definitions(GIT_GUI)

ki18n_wrap_ui(
    APP_SRCS

    settings/settingspagebase.ui
    settings/settingspagediff.ui
    settings/settingspagegit.ui
    multipagewidget.ui
    gitklientmergeview.ui
    commandpullwidget.ui

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
    widgets/diffwidget.ui
    dialogs/ignorefiledialog.ui

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
    dialogs/fileblamedialog.ui
    dialogs/searchdialog.ui
    dialogs/reposettingsdialog.ui
    dialogs/fetchdialog.ui
    dialogs/switchbranchdialog.ui
    dialogs/initdialog.ui
    dialogs/notedialog.ui
)

include(git/CMakeLists.txt)
add_executable(gitklient ${APP_SRCS} ${GIT_SOURCE})

target_link_libraries(gitklient
    Qt5::Core
    Qt5::Widgets
    Qt5::Concurrent

    KF5::CoreAddons
    KF5::I18n
    KF5::XmlGui
    KF5::ConfigWidgets
    KF5::DBusAddons
    KF5::Crash
    KF5::KIOCore
    KF5::KIOFileWidgets
    KF5::KIOWidgets
    KF5::KIONTLM
    KF5::Parts
    KF5::TextWidgets
    KF5::TextEditor

    KF5::SyntaxHighlighting
)
#set_property(TARGET KI18n APPEND PROPERTY
#  INTERFACE_AUTOUIC_OPTIONS ${autouic_options}
#)

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
install(FILES gitklientfileviewerui.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/gitklient)
