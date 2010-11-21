#ifndef UTILEXPORT_h__
#define UTILEXPORT_h__

////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#   ifdef SMKUTIL_EXPORTS
#      define SMK_UTIL_EXPORT __declspec(dllexport)
#   else
#      define SMK_UTIL_EXPORT __declspec(dllimport)
#   endif 
#else
#   define SMK_UTIL_EXPORT
#endif

////////////////////////////////////////////////////////////////////////////////

#endif // UTILEXPORT_h__
