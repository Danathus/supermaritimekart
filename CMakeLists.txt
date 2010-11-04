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

PROJECT(${APP_NAME})

FIND_PACKAGE(dtABC   REQUIRED)
FIND_PACKAGE(dtCore  REQUIRED)
FIND_PACKAGE(dtDAL   REQUIRED)
FIND_PACKAGE(dtAudio REQUIRED)
FIND_PACKAGE(dtUtil  REQUIRED)
FIND_PACKAGE(dtGame  REQUIRED)
FIND_PACKAGE(OpenGL  REQUIRED)
FIND_PACKAGE(NetCore REQUIRED)

SET(SOURCE_PATH ${CMAKE_SOURCE_DIR}/src)
SET(HEADER_PATH ${CMAKE_SOURCE_DIR}/inc)

################### SOURCE FILES #####################
SET(SOURCE_FILES
    ${SOURCE_PATH}/SuperMaritimeKart.cpp
    ${SOURCE_PATH}/SuperMaritimeKartComponent.cpp
    ${SOURCE_PATH}/GeometryUtils.cpp
    ${SOURCE_PATH}/main.cpp
)

################### HEADER FILES #####################
SET(HEADER_FILES
    ${HEADER_PATH}/SuperMaritimeKart.h
    ${HEADER_PATH}/SuperMaritimeKartComponent.h
    ${HEADER_PATH}/GeometryUtils.h
)

###################### RESOURCE FILES ######################
SET(RESOURCE_FILES
 	${CMAKE_SOURCE_DIR}/SMK.rc
)

##################### INCLUDE DIRECTORIES ######################
INCLUDE_DIRECTORIES(${HEADER_PATH}
                    ${DTABC_INCLUDE_DIRECTORIES}
                    ${DTCORE_INCLUDE_DIRECTORIES}
                    ${DTDAL_INCLUDE_DIRECTORIES}
                    ${DTUTIL_INCLUDE_DIRECTORIES}
                    ${DTGAME_INCLUDE_DIRECTORIES}
                    ${OPENGL_INCLUDE_DIR}
)

##################### EXECUTABLE BUILDING ######################

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

ADD_EXECUTABLE(${APP_NAME} ${APP_TYPE}
               ${SOURCE_FILES}
               ${HEADER_FILES}
)

TARGET_LINK_LIBRARIES(${APP_NAME} ${DTCORE_LIBRARIES}
                                  ${DTABC_LIBRARIES}
                                  ${DTDAL_LIBRARIES}
                                  ${DTUTIL_LIBRARIES}
                                  ${DTGAME_LIBRARIES}
                                  ${DTAUDIO_LIBRARIES}
                                  ${OPENGL_gl_LIBRARY}
                                  ${NETCORE_LIBRARIES}
)
