include_directories(${CMAKE_SOURCE_DIR}/app)
include_directories(../../core)
include(../../src/git/CMakeLists.txt)

add_definitions(-DGK_TEST)

ecm_add_test(
    ${GIT_SOURCE}
    overlaytest.cpp
    ../common/gittestmanager.cpp
    ../../dolphinplugins/statuscache.cpp
    LINK_LIBRARIES Qt::Test Qt::Core Qt::Gui KF5::KIOWidgets
    TEST_NAME OverlayTest
    NAME_PREFIX app-)
