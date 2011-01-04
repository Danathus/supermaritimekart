# Locate NetSetGo libraries
#
# Assumes that you want to locate all NetSetGo libraries.
#
# Will define:
# ${NETSETGO_LIBRARIES} for use with TARGET_LINK_LIBRARIES()
# ${NETSETGO_INCLUDE_DIRECTORIES} for use with INCLUDE_DIRECTORIES()

FIND_PATH(NETCORE_INCLUDE_DIR NetSetGo/NetCore/NetCoreExport.h
         PATH_SUFFIXES
            include
         HINTS
            ${NETSETGO_ROOT}
            $ENV{NETSETGO_ROOT}
            ${CMAKE_SOURCE_DIR}/ext
         PATHS
           /usr/local/include
           /usr/freeware/include     
         DOC "The NetCore include folder. Should contain 'NetCore/NetCoreExport.h'"
)

# where to find the lib dir
SET(SHARED_CODE_LIB_SEARCH_PATH 
    ${NETSETGO_ROOT}
    ${NETSETGO_ROOT}/build
    $ENV{NETSETGO_ROOT}/build
    ${CMAKE_SOURCE_DIR}/ext/
) 

MACRO(FIND_NETCORE_LIBRARY LIB_VAR LIB_NAME)
  FIND_LIBRARY(${LIB_VAR} NAMES ${LIB_NAME}
               PATH_SUFFIXES
                  lib
               PATHS
                  ${SHARED_CODE_LIB_SEARCH_PATH}
              )
ENDMACRO(FIND_NETCORE_LIBRARY LIB_VAR LIB_NAME)

# variable names of the individual NetSetGo libraries.  Can be used in application cmakelist.txt files.
FIND_NETCORE_LIBRARY(NETCORE_LIBRARY       NetCore)
FIND_NETCORE_LIBRARY(NETCORE_DEBUG_LIBRARY NetCoreD)

IF (NOT NETCORE_DEBUG_LIBRARY)
  SET(NETCORE_DEBUG_LIBRARY NETCORE_LIBRARY)
  MESSAGE(STATUS "No debug library was found for NETCORE_DEBUG_LIBRARY")
ENDIF()

# convienent list of libraries to link with when using NetCore
SET(NETCORE_LIBRARIES
    optimized ${NETCORE_LIBRARY} debug ${NETCORE_DEBUG_LIBRARY}
)

# convienent list of libraries to link with when using NetSetGo
SET(NETSETGO_LIBRARIES
    ${NETCORE_LIBRARIES}
)

SET(NETCORE_INCLUDE_DIRECTORIES 
    ${NETCORE_INCLUDE_DIR})
    
SET(NETSETGO_INCLUDE_DIRECTORIES 
    ${NETCORE_INCLUDE_DIR})

# handle the QUIETLY and REQUIRED arguments and set NETSETGO_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(NetSetGo DEFAULT_MSG NETSETGO_INCLUDE_DIRECTORIES NETSETGO_LIBRARIES)
