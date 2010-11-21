SET(LIB_NAME SMKMessages)

SET(SOURCE_PATH ${CMAKE_SOURCE_DIR}/src/messages)
SET(HEADER_PATH ${CMAKE_SOURCE_DIR}/inc/messages)

################### SOURCE FILES #####################
    
SET(LIB_SOURCE_FILES
    ${SOURCE_PATH}/DamageMessage.cpp
    ${SOURCE_PATH}/NetworkMessages.cpp
)

SET(LIB_HEADER_FILES
    ${HEADER_PATH}/DamageMessage.h
    ${HEADER_PATH}/NetworkMessages.h
    ${HEADER_PATH}/MessagesExport.h
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/inc
                    )

ADD_LIBRARY(${LIB_NAME} SHARED
            ${LIB_SOURCE_FILES}
            ${LIB_HEADER_FILES}
)

TARGET_LINK_LIBRARIES(${LIB_NAME}
                      ${DTCORE_LIBRARIES}
                      ${DTDAL_LIBRARIES}
                      ${DTGAME_LIBRARIES}
                      ${DTUTIL_LIBRARIES}
					  SMKUtil
                      )


SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES DEFINE_SYMBOL SMKMESSAGE_EXPORTS)

# little hack to get the .dll into /bin and .lib into /lib
IF (MSVC)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES PREFIX "../../../../bin/")
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES IMPORT_PREFIX "../../../../lib/")
ENDIF (MSVC)