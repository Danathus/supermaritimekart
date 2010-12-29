# Locate NetCore library
# Will define:
# ${NETCORE_LIBRARIES} for use with TARGET_LINK_LIBRARIES()
# ${NETCORE_INCLUDE_DIRECTORIES} for use with INCLUDE_DIRECTORIES()
#

FIND_PATH(NETROSPECT_INCLUDE_DIR netrospect.h
         PATH_SUFFIXES
            include
         HINTS
            ${NETROSPECT_ROOT}
            $ENV{NETROSPECT_ROOT}
            ${CMAKE_SOURCE_DIR}/ext
         PATHS
           /usr/local/include
           /usr/freeware/include     
         DOC "The netrospect folder. Should contain 'netrospect.h'"
)


