#ifndef SYNCHRONIZED_EVENT_MESSAGE_H
#define SYNCHRONIZED_EVENT_MESSAGE_H

////////////////////////////////////////////////////////////////////////////////

#include <dtGame/message.h>

#include <DeltaNetworkAdapter/LibraryExport.h>

////////////////////////////////////////////////////////////////////////////////

namespace dtGame
{
   class GameManager;
}

namespace DeltaNetworkAdapter
{
   // used for logging
   class DELTANETWORKADAPTER_LIBRARY_EXPORT SynchronizedEventMessage : public dtGame::Message
   {
   public:
      static void SetGameManager(dtGame::GameManager* gameManager = NULL) { gpGameManager = gameManager; }

      /// Constructor
      SynchronizedEventMessage();

      void SetSimTime(double simTime) { mSimTime = simTime; }
      double GetSimTime() const { return mSimTime; }

      void SetSourceID(const std::string& sourceID) { mSourceID = sourceID; }
      const std::string& GetSourceID() const { return mSourceID; }
      void SetDestinationID(const std::string& destinationID) { mDestinationID = destinationID; }
      const std::string& GetDestinationID() const { return mDestinationID; }
      void SetPayload(const dtGame::Message* message) { mpMessage = message; }
      const dtGame::Message* GetPayload() const { return mpMessage; }

      // implementation of serialization suite
      void ToString(std::string& toFill) const;
      bool FromString(const std::string& source);
      void ToDataStream(dtUtil::DataStream& stream) const;
      bool FromDataStream(dtUtil::DataStream& stream);

   protected:
      /// Destructor
      virtual ~SynchronizedEventMessage() {}

   private:
      static dtGame::GameManager* gpGameManager;
      double mSimTime;
      dtCore::RefPtr<const dtGame::Message> mpMessage;
      std::string mSourceID;
      std::string mDestinationID;
   };
} // namespace DeltaNetworkAdapter

//////////////////////////////////////////////////////////////////////////////

#endif // SYNCHRONIZED_EVENT_MESSAGE_H
