#ifndef NETWORK_MESSAGE_H
#define NETWORK_MESSAGE_H

////////////////////////////////////////////////////////////////////////////////

#include <NetCore/NodeID.h>

#include <DeltaNetworkAdapter/LibraryExport.h>
#include <DeltaNetworkAdapter/TransmissionProtocol.h>

#include <dtGame/message.h>

////////////////////////////////////////////////////////////////////////////////

namespace dtGame
{
   class GameManager;
}

namespace DeltaNetworkAdapter
{
   class DELTANETWORKADAPTER_LIBRARY_EXPORT NetworkMessage : public dtGame::Message
   {
   public:
      typedef net::NodeID TerminalIdentifier;

      /// Constructor
      NetworkMessage();

      void SetDestination(const TerminalIdentifier& destination) { mDestination = destination; }
      const TerminalIdentifier& GetDestination() const { return mDestination; }

      void SetProtocol(TransmissionProtocol protocol) { mProtocol = protocol; }
      TransmissionProtocol GetProtocol() const { return mProtocol; }

      void SetPayload(dtCore::RefPtr<const dtGame::Message> message) { mpMessage = message; }
      const dtGame::Message* GetPayload() const { return mpMessage; }

      // implementation of serialization suite
      void ToString(std::string& toFill) const;
      bool FromString(const std::string& source);
      void ToDataStream(dtUtil::DataStream& stream) const;
      bool FromDataStream(dtUtil::DataStream& stream);

   protected:
      /// Destructor
      virtual ~NetworkMessage() {}

   private:
      TerminalIdentifier mDestination;
      TransmissionProtocol mProtocol;
      dtCore::RefPtr<const dtGame::Message> mpMessage;
   };

} // namespace DeltaNetworkAdapter

#endif // NETWORK_MESSAGE_H
