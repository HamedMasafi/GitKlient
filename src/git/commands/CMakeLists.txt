set(
    GIT_SOURCE
    ${GIT_SOURCE}

    ${CMAKE_CURRENT_LIST_DIR}/abstractcommand.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commandchangedfiles.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commandclone.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commandcommit.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commandfetch.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commandpull.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commandpush.cpp

    PARENT_SCOPE
)
