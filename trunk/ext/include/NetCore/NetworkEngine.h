#ifndef NETWORK_ENGINE__H
#define NETWORK_ENGINE__H

////////////////////////////////////////////////////////////////////////////////

#include <NetCore/NetCoreExport.h>

#include <NetCore/Address.h>
#include <NetCore/Beacon.h>
#include <NetCore/Node.h>
#include <NetCore/Mesh.h>
#include <NetCore/PacketProcessor.h>

////////////////////////////////////////////////////////////////////////////////

namespace net {

   class NETCORE_EXPORT NetworkEngine
   {
   public:
      static NetworkEngine& GetRef();
      static void Destroy();

      static std::string GetHostName();
      static Address GetAddressFromHostName(const std::string& hostName); // note: returns 0 for port
      void HostNetwork();
      bool IsHostingNetwork() const { return mAcceptConnection; }
      void JoinNetwork(const net::Address& hostAddress);
      bool IsJoiningNetwork() const { return mAttemptConnection; }
      const net::Address& GetHostAddress() const { return mHostAddress; } // get the address of the host we're connecting/connected to

      // start advertising the network by sending out a beacon
      // note this will fail (return false) iff we're not hosting
      bool StartAdvertising(const std::string& name, unsigned int protocolID, unsigned int listenerPort, unsigned int serverPort, int beaconPort, Serializable* userData);
      void StopAdvertising(); // stop if we're advertising

      net::Node& GetNode() { return mNode; }
      const net::Node& GetNode() const { return mNode; }
      net::Mesh& GetMesh() { return mMesh; }
      const net::Mesh& GetMesh() const { return mMesh; }

      PacketProcessor& GetPacketProcessor() { return mPacketProcessor; }
      const PacketProcessor& GetPacketProcessor() const { return mPacketProcessor; }

      void Update(float deltaTime);

   protected:
      NetworkEngine();
      ~NetworkEngine();

   private:
      static NetworkEngine* sgSelf;

      bool mAcceptConnection;  // serve
      bool mAttemptConnection; // join as client

      net::Address mHostAddress; // address to connect to

      net::BeaconTransmitter mBeaconTransmitter; // beacon for advertising hosted game

      // for network tracking
      net::Node mNode;
      net::Mesh mMesh;

      // for packet handling
      PacketProcessor mPacketProcessor;
   };

} // namespace net

////////////////////////////////////////////////////////////////////////////////

#endif // NETWORK_ENGINE__H
