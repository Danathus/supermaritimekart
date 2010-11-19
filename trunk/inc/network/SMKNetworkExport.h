#ifndef SMKNETWORKEXPORT_h__
#define SMKNETWORKEXPORT_h__

////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#   ifdef SMKNETWORK_EXPORTS
#      define SMK_NETWORK_EXPORT __declspec(dllexport)
#   else
#      define SMK_NETWORK_EXPORT __declspec(dllimport)
#   endif 
#else
#   define SMK_NETWORK_EXPORT
#endif

////////////////////////////////////////////////////////////////////////////////

#endif // SMKNETWORKEXPORT_h__
