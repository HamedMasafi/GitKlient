include_directories(${CMAKE_SOURCE_DIR}/app)

add_definitions()
ecm_add_test(
    difftest.cpp
    NAME_PREFIX "app-"
    TEST_NAME DiffTest
    LINK_LIBRARIES KF5::CoreAddons Qt5::Test Qt5::Core KF5::I18n
)

#configure_file(sqlmodel_REMOTE_32623.h sqlmodel_REMOTE_32623.h COPYONLY)
#configure_file(sqlmodel_LOCAL_32623.h sqlmodel_LOCAL_32623.h COPYONLY)
#configure_file(sqlmodel_BASE_32623.h sqlmodel_BASE_32623.h COPYONLY)
#configure_file(sqlmodel_BACKUP_32623.h sqlmodel_BACKUP_32623.h COPYONLY)
