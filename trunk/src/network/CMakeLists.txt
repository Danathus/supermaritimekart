SET(LIB_NAME SMKNetwork)

SET(SOURCE_PATH ${CMAKE_SOURCE_DIR}/src/network)
SET(HEADER_PATH ${CMAKE_SOURCE_DIR}/inc/network)

################### SOURCE FILES #####################
    
SET(LIB_SOURCE_FILES
    ${SOURCE_PATH}/NetworkBuddy.cpp
)

SET(LIB_HEADER_FILES
    ${HEADER_PATH}/NetConfig.h
    ${HEADER_PATH}/NetworkBuddy.h
    ${HEADER_PATH}/SMKNetworkExport.h
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/inc
                    )

ADD_LIBRARY(${LIB_NAME} SHARED
            ${LIB_SOURCE_FILES}
            ${LIB_HEADER_FILES}
)

TARGET_LINK_LIBRARIES(${LIB_NAME}
                      ${DTCORE_LIBRARIES}
                      ${DTGAME_LIBRARIES}
                      ${DTUTIL_LIBRARIES}
                      ${DELTANETWORKADAPTER_LIBRARIES}
                      ${NETCORE_LIBRARIES}
                      )


SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES DEFINE_SYMBOL SMKNETWORK_EXPORTS)

# little hack to get the .dll into /bin and .lib into /lib
IF (MSVC)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES PREFIX "../../../../bin/")
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES IMPORT_PREFIX "../../../../lib/")
ENDIF (MSVC)