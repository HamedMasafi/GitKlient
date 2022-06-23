include_directories(${CMAKE_SOURCE_DIR}/app)
include_directories(../../src/git/)

add_definitions(-DGK_TEST)

ecm_add_test(
    overlaytest.cpp
    NAME_PREFIX "app-"
    TEST_NAME OverlayTest
    LINK_LIBRARIES Qt::Test Qt::Core Qt::Gui KF5::KIOWidgets
)
