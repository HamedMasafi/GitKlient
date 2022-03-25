#target_sources(
#        gitklient_SRCS
#
#        logs.cpp
#        cache.cpp
#)
set(SOURCE
        ${SOURCE}
        ${CMAKE_CURRENT_SOURCE_DIR}/logs.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cache.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/remotescache.cpp
        PARENT_SCOPE
        )
