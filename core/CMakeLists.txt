aux_source_directory(git GIT_SRC)
aux_source_directory(diff DIFF_SRC)
aux_source_directory(widgets WIDGETS_SRC)

set(APPNAME gitklientcore)


file(GLOB SOURCESFILE
    ${GIT_SRC}
    ${DIFF_SRC}
    ${WIDGETS_SRC}
)
add_library(gitklientcore SHARED ${SOURCESFILE})
target_include_directories(
    gitklientcore
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)
#target_sources(
#    gitklientcore
#    PRIVATE
#        ${GIT_SRC}
#        ${DIFF_SRC}
#        ${WIDGETS_SRC}
#)

#ecm_qt_declare_logging_category(
#    gitcore
#    HEADER ark_debug.h
#    IDENTIFIER ARK
#    CATEGORY_NAME ark.kerfuffle
#    DESCRIPTION "Ark Kerfuffle"
#    EXPORT ARK
#)

include(GenerateExportHeader)
generate_export_header(
    gitklientcore
    BASE_NAME gitklientcore
)

find_package(KF5TextWidgets)
target_link_libraries(
    gitklientcore
    PUBLIC
    Qt5::Core
    Qt5::Widgets
    Qt5::Concurrent
    KF5::CoreAddons
    KF5::I18n
    KF5::ConfigWidgets
    KF5::DBusAddons
    KF5::Crash
    KF5::KIOCore
    KF5::KIOWidgets
    KF5::TextWidgets
    KF5SyntaxHighlighting
)

ki18n_wrap_ui(
    widgets/diffwidget.ui
)
