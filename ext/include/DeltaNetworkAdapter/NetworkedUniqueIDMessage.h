#ifndef NETWORKED_UNIQUEID_MESSAGE_H
#define NETWORKED_UNIQUEID_MESSAGE_H

#include <DeltaNetworkAdapter/LibraryExport.h>
#include <dtGame/message.h>

////////////////////////////////////////////////////////////////////////////////

namespace dtGame
{
   class GameManager;
}

namespace DeltaNetworkAdapter
{
   class DELTANETWORKADAPTER_LIBRARY_EXPORT NetworkedUniqueIDMessage : public dtGame::Message
   {
   public:
      /// Constructor
      NetworkedUniqueIDMessage();

      void SetUniqueId(const std::string& uniqueId) { mUniqueId = uniqueId; }
      const std::string& GetUniqueId() const        { return mUniqueId; }

      // implementation of serialization suite
      void ToString(std::string& toFill) const;
      bool FromString(const std::string& source);
      void ToDataStream(dtUtil::DataStream& stream) const;
      bool FromDataStream(dtUtil::DataStream& stream);

   protected:
      /// Destructor
      virtual ~NetworkedUniqueIDMessage() {}

   private:
      std::string mUniqueId;
   };
} // namespace DeltaNetworkAdapter

#endif // NETWORKED_UNIQUEID_MESSAGE_H
