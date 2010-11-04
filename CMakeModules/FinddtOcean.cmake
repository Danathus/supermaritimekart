FIND_PATH(DTOCEAN_INCLUDE_DIR dtOcean
          $ENV{DTOCEAN_ROOT}/include
          ${CMAKE_SOURCE_DIR}/ext/include
)

#where to find the dtOcean lib dir
SET(DTOCEAN_LIB_SEARCH_PATH
    $ENV{DTOCEAN_ROOT}/build/lib
    ${CMAKE_SOURCE_DIR}/ext/lib
    ${DTOCEAN_INCLUDE_DIR}/../build/lib
)

MACRO(FIND_DTOCEAN_LIBRARY LIB_VAR LIB_NAME)
      FIND_LIBRARY(${LIB_VAR} NAMES ${LIB_NAME}
      PATHS
      ${DTOCEAN_LIB_SEARCH_PATH}
)
ENDMACRO(FIND_DTOCEAN_LIBRARY LIB_VAR LIB_NAME)

FIND_DTOCEAN_LIBRARY(DTOCEAN_LIBRARY       dtOcean)
FIND_DTOCEAN_LIBRARY(DTOCEAN_LIBRARY_DEBUG dtOceanD)

#convienent list of libraries to link with when using dtCore
SET(DTOCEAN_LIBRARIES
    optimized ${DTOCEAN_LIBRARY} debug ${DTOCEAN_LIBRARY_DEBUG}
    )
    
    
#handle the QUIETLY and REQUIRED arguments and set DTOCEAN to TRUE if
#all listed vairables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(dtOcean DEFAULT_MSG DTOCEAN_INCLUDE_DIR
                                                      DTOCEAN_LIBRARY     )
