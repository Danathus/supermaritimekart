#ifndef SOCKET_H
#define SOCKET_H

////////////////////////////////////////////////////////////////////////////////

#include <NetCore/NetCoreExport.h>

#include <NetCore/Address.h>
#include <NetCore/NetPlatform.h>

namespace net {

////////////////////////////////////////////////////////////////////////////////

// socket functionality

bool InitializeSockets();
void ShutdownSockets();

class NETCORE_EXPORT Socket
{
public:
   enum Options // | these together as desired
   {
      NonBlocking    = 1 << 0,
      Broadcast      = 1 << 1,
      AllowMultiBind = 1 << 2
   };

   Socket(int options = NonBlocking);
   ~Socket();

   bool Open(unsigned short port);
   void Close();
   bool IsOpen() const;
   unsigned short GetPort() const { return mPort; }

   bool Send(const net::Address& destination, const void* data, int size);
   int Receive(net::Address& sender, void* data, int size);

   // multicast calls
   bool Subscribe(const net::Address& multicastAddress);
   bool Unsubscribe(const net::Address& multicastAddress);

private:
#if NET_PLATFORM == NET_PLATFORM_WINDOWS
   typedef unsigned int* SocketExternalType;
#else
   typedef int SocketExternalType;
#endif

   SocketExternalType mSocket;
   int mOptions;
   unsigned short mPort;
};

////////////////////////////////////////////////////////////////////////////////

} // namespace net

#endif // SOCKET_H
