#ifndef BEACON_H
#define BEACON_H

#include <string>

#include <NetCore/NetCoreExport.h>

#include <NetCore/Socket.h>
#include <NetCore/Serialization.h>

// beacon
//  + sends broadcast UDP packets to the LAN
//  + use a beacon to advertise the existence of a server on the LAN

namespace net {

////////////////////////////////////////////////////////////////////////////////

class NETCORE_EXPORT BeaconHeader : public Serializable
{
public:
   struct NETCORE_EXPORT Data
   {
      const unsigned int mZero;
      unsigned int mProtocolID;
      unsigned int mServerPort;
      unsigned char mNameLength;
      char mName[64];

      Data();
      bool SetName(const std::string& name);
   };

   BeaconHeader() : Serializable() {}

   Data& GetData() { return mData; }
   const Data& GetData() const { return mData; }

   // implementations of pure virtual methods
   size_t Serialize(unsigned char* buffer, size_t numBytes) const;
   size_t Deserialize(const unsigned char* buffer, size_t numBytes);
   //
   size_t GetSize() const;

protected:
private:
   Data mData;
};

////////////////////////////////////////////////////////////////////////////////

class NETCORE_EXPORT BeaconTransmitter
{
public:
   BeaconTransmitter(Serializable* userData);
   ~BeaconTransmitter();

   BeaconHeader& GetHeader() { return mHeader; }
   const BeaconHeader& GetHeader() const { return mHeader; }
   Serializable* GetUserData() { return mpUserData; }
   const Serializable* GetUserData() const { return mpUserData; }

   /**
    * @param name The name of the server to be included in the Beacon::Packet
    * @param protocolId An identifier for this game type (to avoid confusion with other beacons)
    * @param listenerPort The port a client should listen on to receive broadcasts of Beacon::Packets
    * @param serverPort The port a client should connect to to join the server (included in Beacon::Packet)
    */
   void Configure(const std::string& name, unsigned int protocolID, unsigned int listenerPort, unsigned int serverPort, Serializable* userData = 0);
   bool IsConfigured() const { return mIsConfigured; }

   bool IsRunning() const { return mIsRunning; }
   bool Start(int port);
   void Stop();
   void Update(float deltaTime);

protected:
   bool SendBeacon();

private:
   bool mIsConfigured;
   bool mIsRunning;
   Socket mSocket;
   unsigned int mListenerPort;

   BeaconHeader mHeader; // configured once, used for writing
   Serializable* mpUserData;
};

////////////////////////////////////////////////////////////////////////////////

class NETCORE_EXPORT BeaconReceiver
{
public:
   BeaconReceiver(int beaconListenerPort, Serializable* userData);
   ~BeaconReceiver();

   BeaconHeader& GetHeader() { return mHeader; }
   const BeaconHeader& GetHeader() const { return mHeader; }
   Serializable* GetUserData() { return mpUserData; }
   const Serializable* GetUserData() const { return mpUserData; }

   bool ReceiveBeacon(net::Address& senderAddress);

protected:
private:
   BeaconHeader mHeader; // buffer read headers here
   Serializable* mpUserData;
   net::Socket mSocket;
};

////////////////////////////////////////////////////////////////////////////////

} // namespace net

#endif // BEACON_H
