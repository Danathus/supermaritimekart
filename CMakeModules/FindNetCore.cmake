# Locate NetCore library
# Will define:
# ${NETCORE_LIBRARIES} for use with TARGET_LINK_LIBRARIES()
# ${NETCORE_INCLUDE_DIRECTORIES} for use with INCLUDE_DIRECTORIES()
#

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

# where to find the SharedCode lib dir
SET(SHARED_CODE_LIB_SEARCH_PATH 
    ${NETSETGO_ROOT}/lib
    ${NETSETGO_ROOT}/build/lib
    $ENV{NETSETGO_ROOT}/build/lib
	${CMAKE_SOURCE_DIR}/ext
) 

MACRO(FIND_NETCORE_LIBRARY LIB_VAR LIB_NAME)
  FIND_LIBRARY(${LIB_VAR} NAMES ${LIB_NAME}
               PATH_SUFFIXES
                  lib
               PATHS
                  ${SHARED_CODE_LIB_SEARCH_PATH}
              )
ENDMACRO(FIND_NETCORE_LIBRARY LIB_VAR LIB_NAME)    
 
# variable names of the individual Delta3D libraries.  Can be used in application cmakelist.txt files.
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
    
SET(NETCORE_INCLUDE_DIRECTORIES ${NETCORE_INCLUDE_DIR})

# handle the QUIETLY and REQUIRED arguments and set DELTA3D_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(NetCore DEFAULT_MSG NETCORE_INCLUDE_DIR NETCORE_LIBRARY)
