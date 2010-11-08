#ifndef DeltaNetworkAdapter_LibraryExport_h__
#define DeltaNetworkAdapter_LibraryExport_h__

////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#   ifdef DELTANETWORKADAPTERLIBRARY
#      define DELTANETWORKADAPTER_LIBRARY_EXPORT __declspec(dllexport)
#   else
#      define DELTANETWORKADAPTER_LIBRARY_EXPORT __declspec(dllimport)
#   endif
#else
#   ifdef DELTANETWORKADAPTERLIBRARY
#      define DELTANETWORKADAPTER_LIBRARY_EXPORT __attribute__ ((visibility("default")))
#   else
#      define DELTANETWORKADAPTER_LIBRARY_EXPORT
#   endif
#endif

////////////////////////////////////////////////////////////////////////////////

#endif // DeltaNetworkAdapter_LibraryExport_h__
