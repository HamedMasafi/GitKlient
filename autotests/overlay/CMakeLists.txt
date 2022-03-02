include_directories(${CMAKE_SOURCE_DIR}/app)
include_directories(../../core)
aux_source_directory(../../src/git GIT_SRC)

add_definitions(-DGK_TEST)

ecm_add_test(
    overlaytest.cpp
    ${GIT_SRC}
    ../common/gittestmanager.cpp
    ../../dolphinplugins/klientdolphinoverlayplugin.cpp
    LINK_LIBRARIES Qt::Test Qt::Core Qt::Gui KF5::KIOWidgets
    TEST_NAME OverlayTest
    NAME_PREFIX app-)
