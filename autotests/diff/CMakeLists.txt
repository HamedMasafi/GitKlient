include_directories(${CMAKE_SOURCE_DIR}/app)

ecm_add_test(
    difftest.cpp

    ../common/gittestmanager.cpp
    LINK_LIBRARIES KF5::CoreAddons Qt::Test Qt::Core KF5::I18n
    TEST_NAME DiffTest
    NAME_PREFIX app-
)
