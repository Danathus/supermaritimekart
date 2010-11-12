CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Convert to CMake standard path string
FILE(TO_CMAKE_PATH "$ENV{DELTA_ROOT}" DELTA_ROOT_CMAKE)
FILE(TO_CMAKE_PATH "$ENV{DELTA_SHARED_CODE}" DELTA_SHARED_CODE_CMAKE)

# where the Find*.cmake files are
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules
                      ${DELTA_ROOT_CMAKE}/CMakeModules
                      ${DELTA_SHARED_CODE_CMAKE}/CMakeModules
                      ${CMAKE_MODULE_PATH})
                      
SET(APP_NAME SuperMaritimeKart) # USE_FUN
SET(ACTOR_LIB_NAME SMKActors)

IF (WIN32)
   SET(CMAKE_DEBUG_POSTFIX "d")
ENDIF (WIN32)

PROJECT(${APP_NAME})

FIND_PACKAGE(dtABC               REQUIRED)
FIND_PACKAGE(dtActors            REQUIRED)
FIND_PACKAGE(dtCore              REQUIRED)
FIND_PACKAGE(dtDAL               REQUIRED)
FIND_PACKAGE(dtAudio             REQUIRED)
FIND_PACKAGE(dtUtil              REQUIRED)
FIND_PACKAGE(dtGame              REQUIRED)
FIND_PACKAGE(OpenGL              REQUIRED)
FIND_PACKAGE(dtOcean             REQUIRED) #For the OceanActor
FIND_PACKAGE(NetCore             REQUIRED)
FIND_PACKAGE(DeltaNetworkAdapter REQUIRED)
FIND_PACKAGE(BoatActors          REQUIRED)

OPTION(BUILD_WITH_DTOCEAN "Enables building with dtOcean" ON)

SET(SOURCE_PATH ${CMAKE_SOURCE_DIR}/src)
SET(HEADER_PATH ${CMAKE_SOURCE_DIR}/inc)

################### SOURCE FILES #####################
SET(ACTOR_SOURCE_FILES
    ${SOURCE_PATH}/DOFDrawable.cpp
    ${SOURCE_PATH}/SMKActorLibraryRegistry.cpp
    ${SOURCE_PATH}/SMKBoatActor.cpp
    ${SOURCE_PATH}/Weapon.cpp
)
    
SET(APP_SOURCE_FILES
    ${SOURCE_PATH}/Armor.cpp
    ${SOURCE_PATH}/BoatController.cpp
    ${SOURCE_PATH}/BoatKeyboardListener.cpp
    ${SOURCE_PATH}/CameraController.cpp
    ${SOURCE_PATH}/CameraMouseListener.cpp
    ${SOURCE_PATH}/Damage.cpp
    ${SOURCE_PATH}/DamageAssessor.cpp
    ${SOURCE_PATH}/DeltaOceanGetHeight.cpp
    ${SOURCE_PATH}/FloaterController.cpp
    ${SOURCE_PATH}/GameFinder.cpp
    ${SOURCE_PATH}/GeometryUtils.cpp
    ${SOURCE_PATH}/Health.cpp
    ${SOURCE_PATH}/main.cpp
    ${SOURCE_PATH}/NetworkBuddy.cpp
    ${SOURCE_PATH}/OceanWindowResize.cpp
    ${SOURCE_PATH}/ScenarioComponent.cpp
    ${SOURCE_PATH}/SMK_BeaconData.cpp
    ${SOURCE_PATH}/SMK_NetworkComponent.cpp
    ${SOURCE_PATH}/SuperMaritimeKart.cpp
    ${SOURCE_PATH}/SuperMaritimeKartMessenger.cpp
)

################### HEADER FILES #####################
SET(ACTOR_HEADER_FILES
    ${HEADER_PATH}/DOFDrawable.h
    ${HEADER_PATH}/SMKActorExport.h
    ${HEADER_PATH}/SMKActorLibraryRegistry.h
    ${HEADER_PATH}/SMKBoatActor.h
    ${HEADER_PATH}/Weapon.h
)

SET(APP_HEADER_FILES
    ${HEADER_PATH}/Armor.h
    ${HEADER_PATH}/BoatController.h
    ${HEADER_PATH}/BoatKeyboardListener.h
    ${HEADER_PATH}/CameraController.h
    ${HEADER_PATH}/CameraMouseListener.h
    ${HEADER_PATH}/Damage.h
    ${HEADER_PATH}/DamageAssessor.h
    ${HEADER_PATH}/DeltaOceanGetHeight.h
    ${HEADER_PATH}/FloaterController.h
    ${HEADER_PATH}/GameFinder.h
    ${HEADER_PATH}/GeometryUtils.h
    ${HEADER_PATH}/Health.h 
    ${HEADER_PATH}/NetConfig.h
    ${HEADER_PATH}/NetworkBuddy.h
    ${HEADER_PATH}/OceanWindowResize.h
    ${HEADER_PATH}/ScenarioComponent.h
    ${HEADER_PATH}/SMK_BeaconData.h
    ${HEADER_PATH}/SMK_NetworkComponent.h
    ${HEADER_PATH}/SuperMaritimeKart.h
    ${HEADER_PATH}/SuperMaritimeKartMessenger.h
)

###################### RESOURCE FILES ######################
SET(REAPP_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/SMK.rc
)

##################### INCLUDE DIRECTORIES ######################
SET(DIRS_TO_INCLUDE ${HEADER_PATH}
                    ${DTABC_INCLUDE_DIRECTORIES}
                    ${DTACTORS_INCLUDE_DIRECTORIES}
                    ${DTCORE_INCLUDE_DIRECTORIES}
                    ${DTDAL_INCLUDE_DIRECTORIES}
                    ${DTUTIL_INCLUDE_DIRECTORIES}
                    ${DTGAME_INCLUDE_DIRECTORIES}
                    ${OPENGL_INCLUDE_DIR}
                    ${BOATACTORS_INCLUDE_DIRECTORIES}
)

IF (BUILD_WITH_DTOCEAN)
  SET(DIRS_TO_INCLUDE ${DIRS_TO_INCLUDE}
                      ${DTOCEAN_INCLUDE_DIR}
  )
ENDIF (BUILD_WITH_DTOCEAN)

INCLUDE_DIRECTORIES(${DIRS_TO_INCLUDE})

##################### ACTOR LIBRARY BUILDING ######################

ADD_LIBRARY(${ACTOR_LIB_NAME} SHARED
            ${ACTOR_SOURCE_FILES}
            ${ACTOR_HEADER_FILES}
)

# little hack to get the .dll into /bin and .lib into /lib
IF (MSVC)
  SET_TARGET_PROPERTIES(${ACTOR_LIB_NAME} PROPERTIES PREFIX "../../bin/")
  SET_TARGET_PROPERTIES(${ACTOR_LIB_NAME} PROPERTIES IMPORT_PREFIX "../../lib/")
ENDIF (MSVC)

TARGET_LINK_LIBRARIES(${ACTOR_LIB_NAME}
                      ${DTACTORS_LIBRARIES}
                      ${DTCORE_LIBRARIES}
                      ${DTDAL_LIBRARIES}
                      ${DTGAME_LIBRARIES}
                      ${DTUTIL_LIBRARIES}
                      ${BOATACTORS_LIBRARIES}
)

SET_TARGET_PROPERTIES(${ACTOR_LIB_NAME} PROPERTIES DEFINE_SYMBOL SMKACTOR_EXPORTS)

SET_TARGET_PROPERTIES(${ACTOR_LIB_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

##################### EXECUTABLE BUILDING ######################

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

ADD_EXECUTABLE(${APP_NAME} ${APP_TYPE}
               ${APP_SOURCE_FILES}
               ${APP_HEADER_FILES}
)

SET(LIBS_TO_LINK ${DTCORE_LIBRARIES}
                 ${DTABC_LIBRARIES}
                 ${DTACTORS_LIBRARIES}
                 ${DTDAL_LIBRARIES}
                 ${DTUTIL_LIBRARIES}
                 ${DTGAME_LIBRARIES}
                 ${DTAUDIO_LIBRARIES}
                 ${OPENGL_gl_LIBRARY}
                 ${NETCORE_LIBRARIES}
                 ${DELTANETWORKADAPTER_LIBRARIES}
                 ${BOATACTORS_LIBRARIES}
                 ${ACTOR_LIB_NAME}
)
IF (BUILD_WITH_DTOCEAN)
  SET(LIBS_TO_LINK ${LIBS_TO_LINK}
                   ${DTOCEAN_LIBRARIES}
  )
ENDIF (BUILD_WITH_DTOCEAN)

TARGET_LINK_LIBRARIES(${APP_NAME} ${LIBS_TO_LINK}
)

IF (BUILD_WITH_DTOCEAN)
  ADD_DEFINITIONS(-DBUILD_WITH_DTOCEAN)
ENDIF (BUILD_WITH_DTOCEAN)

# little hack to get the .dll into /bin and .lib into /lib
IF (MSVC)
  SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES PREFIX "../../bin/")
  SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES IMPORT_PREFIX "../../lib/")
ENDIF (MSVC)

SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
