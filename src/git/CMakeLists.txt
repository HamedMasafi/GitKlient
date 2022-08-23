set(
    GIT_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/branch.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gitglobal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gitloglist.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gitsubmodule.cpp
    ${CMAKE_CURRENT_LIST_DIR}/stash.cpp
        ${CMAKE_CURRENT_LIST_DIR}/filestatus.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gitgraphlane.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gitmanager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/nonemptylinescommand.cpp
    ${CMAKE_CURRENT_LIST_DIR}/blamedata.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gitfile.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gitlog.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gitremote.cpp
    ${CMAKE_CURRENT_LIST_DIR}/parameterescommand.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gittag.cpp

    ${CMAKE_CURRENT_LIST_DIR}/models/abstractgititemsmodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/models/logsmodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/models/remotesmodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/models/submodulesmodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/models/branchesmodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/models/stashesmodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/models/tagsmodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/models/authorsmodel.cpp

    ${CMAKE_CURRENT_LIST_DIR}/commands/abstractcommand.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commands/commandchangedfiles.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commands/commandclone.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commands/commandcommit.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commands/commandfetch.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commands/commandpull.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commands/commandpush.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commands/commandswitchbranch.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commands/commandmerge.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commands/addsubmodulecommand.cpp
    ${CMAKE_CURRENT_LIST_DIR}/commands/commandaddremote.cpp
)

#include(${CMAKE_CURRENT_LIST_DIR}/models/CMakeLists.txt)
#include(${CMAKE_CURRENT_LIST_DIR}/commands/CMakeLists.txt)

#set(gitklient_SRCS ${SOURCE} PARENT_SCOPE)
#target_sources(gitklient PUBLIC ${SOURCE})
