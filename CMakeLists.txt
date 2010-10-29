CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(APP_NAME WaterSprint)

PROJECT(${APP_NAME})

#where the Find*.cmake files are
SET(CMAKE_MODULE_PATH $ENV{DELTA_ROOT}/CMakeModules
                      $ENV{DELTA_SHARED_CODE}/CmakeModules
                      ${CMAKE_SOURCE_DIR}/CMakeModules
                      ${CMAKE_MODULE_PATH}
    )


FIND_PACKAGE(dtABC REQUIRED)
FIND_PACKAGE(dtActors REQUIRED)
FIND_PACKAGE(dtAudio REQUIRED)
FIND_PACKAGE(dtCore REQUIRED)
FIND_PACKAGE(dtDAL REQUIRED)
FIND_PACKAGE(dtGame REQUIRED)
FIND_PACKAGE(dtUtil REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(BoatActors REQUIRED)
FIND_PACKAGE(dtOcean REQUIRED) #For the OceanActor


INCLUDE_DIRECTORIES(${DTABC_INCLUDE_DIRECTORIES}
                    ${DTACTORS_INCLUDE_DIRECTORIES}
                    ${DTAUDIO_INCLUDE_DIRECTORIES}
                    ${DTCORE_INCLUDE_DIRECTORIES}
                    ${DTDAL_INCLUDE_DIRECTORIES}
                    ${DTGAME_INCLUDE_DIRECTORIES}
                    ${DTUTIL_INCLUDE_DIRECTORIES}
                    ${OPENGL_INCLUDE_DIR}
                    ${BOATACTORS_INCLUDE_DIRECTORIES}
                    ${DTOCEAN_INCLUDE_DIR}
                    )

#dump the executables to the root folder.
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

IF (WIN32)
  #Double speed compile.  Booyah!
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

  #put the little "d" on the debug files  
  SET(CMAKE_DEBUG_POSTFIX  "d")
ENDIF (WIN32)


SET(PROG_HEADERS 
    BoatDemoMode.h
    BoatFloaterComponent.h
    BoatKeyboardListener.h
    CameraControl.h
    CameraMouseListener.h
    CollisionCallback.h
    initcomponent.h
    OceanWindowResize.h
    OsgOceanGetHeight.h
    WaterSprint.h
    )
    
SET(PROG_SOURCES
    BoatDemoMode.cpp
    BoatFloaterComponent.cpp
    BoatKeyboardListener.cpp
    CameraControl.cpp
    CameraMouseListener.cpp
    CollisionCallback.cpp
    initcomponent.cpp
    main.cpp
    OceanWindowResize.cpp
    OsgOceanGetHeight.cpp
    WaterSprint.cpp
    readme.txt
   )
       
ADD_EXECUTABLE(${APP_NAME}
               ${PROG_HEADERS}
               ${PROG_SOURCES}
              )

TARGET_LINK_LIBRARIES(${APP_NAME}
                    ${DTABC_LIBRARIES}
                    ${DTACTORS_LIBRARIES}
                    ${DTAUDIO_LIBRARIES}
                    ${DTCORE_LIBRARIES}
                    ${DTDAL_LIBRARIES}
                    ${DTGAME_LIBRARIES}
                    ${DTUTIL_LIBRARIES}
                    ${OPENGL_LIBRARY}
                    ${BOATACTORS_LIBRARIES}
                    ${DTOCEAN_LIBRARIES}
                    )

IF (WIN32)  
  SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES PREFIX "../") #get it out of the Release/Debug folders
  SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}") #put the 'd' on it
ENDIF (WIN32)
