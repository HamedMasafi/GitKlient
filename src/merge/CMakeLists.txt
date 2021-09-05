set(gitklient_SRCS
    ${gitklient_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/gitklientmergewindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/closeeventdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/openfilesdialog.cpp
    PARENT_SCOPE
)

ki18n_wrap_ui(
    ${CMAKE_CURRENT_SOURCE_DIR}/gitklientmergeview.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/closeeventdialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/openfilesdialog.ui
)
