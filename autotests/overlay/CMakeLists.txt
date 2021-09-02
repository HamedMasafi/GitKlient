include_directories(${CMAKE_SOURCE_DIR}/app)
include_directories(../../core)
aux_source_directory(../../core/git GIT_SRC)

add_definitions(-DGK_TEST)

ecm_add_test(
    overlaytest.cpp
    ${GIT_SRC}
    ../../dolphinplugins/klientdolphinoverlayplugin.cpp
    LINK_LIBRARIES Qt::Test Qt::Core Qt::Gui KF5::KIOWidgets
    TEST_NAME OverlayTest
    NAME_PREFIX app-)
