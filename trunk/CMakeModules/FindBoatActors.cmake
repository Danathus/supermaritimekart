# Locate BoatActors library
# Will define:
# ${BOATACTORS_LIBRARIES} for use with TARGET_LINK_LIBRARIES()
# ${BOATACTORS_INCLUDE_DIRECTORIES} for use with INCLUDE_DIRECTORIES()
#

FIND_PATH(BOATACTORS_INCLUDE_DIR BoatActors/BoatActor.h
         PATH_SUFFIXES
            include
         HINTS
            $ENV{DELTA_SHARED_CODE}
            $ENV{ProgramFiles}/SharedCode
            ${CMAKE_SOURCE_DIR}/ext
         PATHS         
           /usr/local/include
           /usr/freeware/include     
         DOC "The BoatActor include folder. Should contain 'BoatActors/BoatActor.h'"
)

# where to find the SharedCode lib dir
SET(SHARED_CODE_LIB_SEARCH_PATH 
             $ENV{DELTA_SHARED_CODE}/build
             $ENV{ProgramFiles}/SharedCode
             ${CMAKE_SOURCE_DIR}/ext
)

MACRO(FIND_BOATCTOR_LIBRARY LIB_VAR LIB_NAME)
  FIND_LIBRARY(${LIB_VAR} NAMES ${LIB_NAME}
               PATH_SUFFIXES
                  lib
               PATHS
                  ${SHARED_CODE_LIB_SEARCH_PATH}
              )
ENDMACRO(FIND_BOATCTOR_LIBRARY LIB_VAR LIB_NAME)            

#variable names of the individual libraries.  Can be used in application cmakelist.txt files.
FIND_BOATCTOR_LIBRARY(BOATACTORS_LIBRARY       BoatActors)
FIND_BOATCTOR_LIBRARY(BOATACTORS_DEBUG_LIBRARY BoatActorsD)


IF (NOT BOATACTORS_DEBUG_LIBRARY)
  SET(BOATACTORS_DEBUG_LIBRARY BOATACTORS_LIBRARY)
  MESSAGE(STATUS "No debug library was found for BOATACTORS_DEBUG_LIBRARY")
ENDIF()

#convienent list of libraries to link with when using BoatActors
SET(BOATACTORS_LIBRARIES
    optimized ${BOATACTORS_LIBRARY} debug ${BOATACTORS_DEBUG_LIBRARY}
    )
    
SET(BOATACTORS_INCLUDE_DIRECTORIES ${BOATACTORS_INCLUDE_DIR}
   )


# handle the QUIETLY and REQUIRED arguments and set DELTA3D_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BoatActors DEFAULT_MSG BOATACTORS_INCLUDE_DIR     BOATACTORS_LIBRARY
                                 )
