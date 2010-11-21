SET(LIB_NAME SMKUtil)

SET(SOURCE_PATH ${CMAKE_SOURCE_DIR}/src/util)
SET(HEADER_PATH ${CMAKE_SOURCE_DIR}/inc/util)

################### SOURCE FILES #####################
    
SET(LIB_SOURCE_FILES
    ${SOURCE_PATH}/Armor.cpp
    ${SOURCE_PATH}/Damage.cpp
    ${SOURCE_PATH}/DamageAssessor.cpp
    ${SOURCE_PATH}/Health.cpp
)

SET(LIB_HEADER_FILES
    ${HEADER_PATH}/Armor.h
    ${HEADER_PATH}/Damage.h
    ${HEADER_PATH}/DamageAssessor.h
    ${HEADER_PATH}/Health.h
    ${HEADER_PATH}/SMKUtilExport.h
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/inc
                    )

ADD_LIBRARY(${LIB_NAME} SHARED
            ${LIB_SOURCE_FILES}
            ${LIB_HEADER_FILES}
)

TARGET_LINK_LIBRARIES(${LIB_NAME}
                      ${DTCORE_LIBRARIES}
                      )


SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES DEFINE_SYMBOL SMKUTIL_EXPORTS)

# little hack to get the .dll into /bin and .lib into /lib
IF (MSVC)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES PREFIX "../../../../bin/")
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES IMPORT_PREFIX "../../../../lib/")
ENDIF (MSVC)