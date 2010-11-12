#ifndef SMKActorExport_h_
#define SMKActorExport_h_

////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#   ifdef SMKACTOR_EXPORTS
#      define SMK_ACTOR_EXPORT __declspec(dllexport)
#   else
#      define SMK_ACTOR_EXPORT __declspec(dllimport)
#   endif 
#else
#   define SMK_ACTOR_EXPORT
#endif

////////////////////////////////////////////////////////////////////////////////

#endif // SMKActorExport_h_
