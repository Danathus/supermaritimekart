# Locate DeltaNetworkAdapter library
# Will define:
# ${DELTANETWORKADAPTER_LIBRARIES} for use with TARGET_LINK_LIBRARIES()
# ${DELTANETWORKADAPTER_INCLUDE_DIRECTORIES} for use with INCLUDE_DIRECTORIES()
#

FIND_PATH(DELTANETWORKADAPTER_INCLUDE_DIR DeltaNetworkAdapter/NetworkEngineComponent.h
         PATH_SUFFIXES
            include
         HINTS
            $ENV{DELTA_SHARED_CODE}
            $ENV{ProgramFiles}/SharedCode
            ${CMAKE_SOURCE_DIR}/ext
         PATHS         
           /usr/local/include
           /usr/freeware/include     
         DOC "The DeltaNetworkAdapter include folder. Should contain 'DeltaNetworkAdapter/NetworkEngineComponent.h'"
)


# where to find the SharedCode lib dir
SET(SHARED_CODE_LIB_SEARCH_PATH 
             $ENV{DELTA_SHARED_CODE}/build
             $ENV{ProgramFiles}/SharedCode
             ${CMAKE_SOURCE_DIR}/ext
)

MACRO(FIND_DELTANETWORKADAPTER_LIBRARY LIB_VAR LIB_NAME)
  FIND_LIBRARY(${LIB_VAR} NAMES ${LIB_NAME}
               PATH_SUFFIXES
                  lib
               PATHS
                  ${SHARED_CODE_LIB_SEARCH_PATH}
              )
ENDMACRO(FIND_DELTANETWORKADAPTER_LIBRARY LIB_VAR LIB_NAME)            

 
# variable names of the individual Delta3D libraries.  Can be used in application cmakelist.txt files.
FIND_DELTANETWORKADAPTER_LIBRARY(DELTANETWORKADAPTER_LIBRARY       DeltaNetworkAdapter)
FIND_DELTANETWORKADAPTER_LIBRARY(DELTANETWORKADAPTER_DEBUG_LIBRARY DeltaNetworkAdapterd)

IF (NOT DELTANETWORKADAPTER_DEBUG_LIBRARY)
  SET(DELTANETWORKADAPTER_DEBUG_LIBRARY DELTANETWORKADAPTER_LIBRARY)
  MESSAGE(STATUS "No debug library was found for DELTANETWORKADAPTER_DEBUG_LIBRARY")
ENDIF()

# convienent list of libraries to link with when using DeltaNetworkAdapter
SET(DELTANETWORKADAPTER_LIBRARIES
    optimized ${DELTANETWORKADAPTER_LIBRARY} debug ${DELTANETWORKADAPTER_DEBUG_LIBRARY}
)
    
SET(DELTANETWORKADAPTER_INCLUDE_DIRECTORIES ${DELTANETWORKADAPTER_INCLUDE_DIR})

# handle the QUIETLY and REQUIRED arguments and set DELTA3D_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DeltaNetworkAdapter DEFAULT_MSG DELTANETWORKADAPTER_INCLUDE_DIR DELTANETWORKADAPTER_LIBRARY)
