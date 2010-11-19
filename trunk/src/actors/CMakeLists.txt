SET(LIB_NAME SMKActors)

SET(SOURCE_PATH ${CMAKE_SOURCE_DIR}/src/actors)
SET(HEADER_PATH ${CMAKE_SOURCE_DIR}/inc/actors)

################### SOURCE FILES #####################
    
SET(SOURCE_FILES
    ${SOURCE_PATH}/DOFDrawable.cpp
    ${SOURCE_PATH}/Health.cpp
    ${SOURCE_PATH}/MachineGunActor.cpp
    ${SOURCE_PATH}/PickUpItemHandle.cpp
    ${SOURCE_PATH}/SMKActorLibraryRegistry.cpp
    ${SOURCE_PATH}/SMKBoatActor.cpp
    ${SOURCE_PATH}/TurretWeapon.cpp
    ${SOURCE_PATH}/Weapon.cpp
    ${SOURCE_PATH}/WeaponActor.cpp
    ${SOURCE_PATH}/WeaponFactory.cpp
)

SET(HEADER_FILES
    ${HEADER_PATH}/DOFDrawable.h
    ${HEADER_PATH}/Health.h 
    ${HEADER_PATH}/MachineGunActor.h
    ${HEADER_PATH}/PickUpItemHandle.h
    ${HEADER_PATH}/SMKActorExport.h
    ${HEADER_PATH}/SMKActorLibraryRegistry.h
    ${HEADER_PATH}/SMKBoatActor.h
    ${HEADER_PATH}/TurretWeapon.h
    ${HEADER_PATH}/Weapon.h
    ${HEADER_PATH}/WeaponActor.h
    ${HEADER_PATH}/WeaponFactory.h
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/inc
                    )

ADD_LIBRARY(${LIB_NAME} SHARED
            ${SOURCE_FILES}
            ${HEADER_FILES}
)

TARGET_LINK_LIBRARIES(${LIB_NAME}
                      ${DTACTORS_LIBRARIES}
                      ${DTAUDIO_LIBRARIES}
                      ${DTCORE_LIBRARIES}
                      ${DTDAL_LIBRARIES}
                      ${DTGAME_LIBRARIES}
                      ${DTUTIL_LIBRARIES}
                      ${BOATACTORS_LIBRARIES}
                      ${DELTANETWORKADAPTER_LIBRARIES}
                      ${NETCORE_LIBRARIES}
					  SMKMessages
					  SMKNetwork
)

SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES DEFINE_SYMBOL SMKACTOR_EXPORTS)

SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

# little hack to get the .dll into /bin and .lib into /lib
IF (MSVC)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES PREFIX "../../../../bin/")
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES IMPORT_PREFIX "../../../../lib/")
ENDIF (MSVC)
