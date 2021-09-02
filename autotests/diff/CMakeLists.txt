include_directories(${CMAKE_SOURCE_DIR}/app)

ecm_add_test(
    difftest.cpp
    ../../core/diff/diff.cpp
    LINK_LIBRARIES Qt::Test Qt::Core
    TEST_NAME DiffTest
    NAME_PREFIX app-)
