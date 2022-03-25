set(
    SOURCE

    ${CMAKE_CURRENT_SOURCE_DIR}/branch.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gitglobal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gitloglist.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gitsubmodule.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractcontainer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/filestatus.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gitgraphlane.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gitmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/nonemptylinescommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/blamedata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gitfile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gitlog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gitremote.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parameterescommand.cpp
)

add_subdirectory(cache)
add_subdirectory(commands)

#set(gitklient_SRCS ${SOURCE} PARENT_SCOPE)
target_sources(gitklient PUBLIC ${SOURCE})