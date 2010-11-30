SET(LIB_NAME SMKActors)

SET(SOURCE_PATH ${CMAKE_SOURCE_DIR}/src/actors)
SET(HEADER_PATH ${CMAKE_SOURCE_DIR}/inc/actors)

################### SOURCE FILES #####################
    
SET(SOURCE_FILES
    ${SOURCE_PATH}/BackWeapon.cpp
    ${SOURCE_PATH}/BackWeaponSlot.cpp
    ${SOURCE_PATH}/DOFDrawable.cpp
    ${SOURCE_PATH}/FrontWeapon.cpp
    ${SOURCE_PATH}/FrontWeaponSlot.cpp
    ${SOURCE_PATH}/HealthPickup.cpp
    ${SOURCE_PATH}/ArmorPickup.cpp
    ${SOURCE_PATH}/MachineGun.cpp
    ${SOURCE_PATH}/MineActor.cpp
    ${SOURCE_PATH}/MineDropper.cpp
    ${SOURCE_PATH}/PickupCategoryEnum.cpp
    ${SOURCE_PATH}/PickupItemFactory.cpp
    ${SOURCE_PATH}/PickUpItemHandle.cpp
    ${SOURCE_PATH}/ProjectileActor.cpp
    ${SOURCE_PATH}/RocketActor.cpp
    ${SOURCE_PATH}/RocketLauncher.cpp
	${SOURCE_PATH}/SimpleFloaterActorComponent.cpp
    ${SOURCE_PATH}/SMKActorLibraryRegistry.cpp
    ${SOURCE_PATH}/SMKBoatActor.cpp
    ${SOURCE_PATH}/TorpedoActor.cpp
    ${SOURCE_PATH}/TorpedoLauncher.cpp
    ${SOURCE_PATH}/Weapon.cpp
    ${SOURCE_PATH}/WeaponSlot.cpp
    ${SOURCE_PATH}/WeaponFactory.cpp
)

SET(HEADER_FILES
    ${HEADER_PATH}/BackWeapon.h
    ${HEADER_PATH}/BackWeaponSlot.h
    ${HEADER_PATH}/DOFDrawable.h
    ${HEADER_PATH}/FrontWeapon.h
    ${HEADER_PATH}/FrontWeaponSlot.h
    ${HEADER_PATH}/HealthPickup.h
    ${HEADER_PATH}/ArmorPickup.h
    ${HEADER_PATH}/MachineGun.h
    ${HEADER_PATH}/MineActor.h
    ${HEADER_PATH}/MineDropper.h
    ${HEADER_PATH}/PickupItem.h
    ${HEADER_PATH}/PickupCategoryEnum.h
    ${HEADER_PATH}/PickupItemFactory.h
    ${HEADER_PATH}/PickUpItemHandle.h
    ${HEADER_PATH}/ProjectileActor.h
    ${HEADER_PATH}/RocketActor.h
    ${HEADER_PATH}/RocketLauncher.h
	${HEADER_PATH}/SimpleFloaterActorComponent.h
    ${HEADER_PATH}/SMKActorExport.h
    ${HEADER_PATH}/SMKActorLibraryRegistry.h
    ${HEADER_PATH}/SMKBoatActor.h
    ${HEADER_PATH}/TorpedoActor.h
    ${HEADER_PATH}/TorpedoLauncher.h
    ${HEADER_PATH}/Weapon.h
    ${HEADER_PATH}/WeaponSlot.h
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
					  SMKUtil
)

SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES DEFINE_SYMBOL SMKACTOR_EXPORTS)

SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

# little hack to get the .dll into /bin and .lib into /lib
IF (MSVC)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES PREFIX "../../../../bin/")
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES IMPORT_PREFIX "../../../../lib/")
ENDIF (MSVC)
