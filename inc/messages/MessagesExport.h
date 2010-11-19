#ifndef MESSAGESEXPORT_h__
#define MESSAGESEXPORT_h__



////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#   ifdef SMKMESSAGE_EXPORTS
#      define SMK_MESSAGE_EXPORT __declspec(dllexport)
#   else
#      define SMK_MESSAGE_EXPORT __declspec(dllimport)
#   endif 
#else
#   define SMK_MESSAGE_EXPORT
#endif

////////////////////////////////////////////////////////////////////////////////

#endif // MESSAGESEXPORT_h__
