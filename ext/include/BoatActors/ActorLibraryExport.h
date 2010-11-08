#ifndef BOAT_ACTORS_LIBRARY_EXPORT__H
#define BOAT_ACTORS_LIBRARY_EXPORT__H

////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#   ifdef BOATACTORS_EXPORTS
#      define BOAT_ACTORS_LIBRARY_EXPORT __declspec(dllexport)
#   else
#      define BOAT_ACTORS_LIBRARY_EXPORT __declspec(dllimport)
#   endif 
#else
#   define BOAT_ACTORS_LIBRARY_EXPORT
#endif

////////////////////////////////////////////////////////////////////////////////

#endif // BOAT_ACTORS_LIBRARY_EXPORT__H
