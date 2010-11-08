#ifndef NETWORK_MESSAGES_H
#define NETWORK_MESSAGES_H

////////////////////////////////////////////////////////////////////////////////

#include <fstream>

#include <dtGame/message.h>
#include <dtGame/messagetype.h>

#include <NetCore/NodeID.h>

#include <DeltaNetworkAdapter/LibraryExport.h>

////////////////////////////////////////////////////////////////////////////////

namespace dtGame
{
   class GameManager;
}

namespace DeltaNetworkAdapter
{

////////////////////////////////////////////////////////////////////////////////

   class DELTANETWORKADAPTER_LIBRARY_EXPORT NetworkMessageType : public dtGame::MessageType
   {
      DECLARE_ENUM(NetworkMessageType);

   public:
      static void RegisterMessageTypes(dtGame::GameManager& gameManager);
      static bool IsNetworkMessageType(const dtGame::MessageType& msgType);

      enum
      {
         //ID_NULL_NETWORK_MESSAGE = SharedUtilMessageType::ID_USER_DEFINED_MESSAGE_TYPE + 1,
         ID_NULL_NETWORK_MESSAGE = dtGame::MessageType::USER_DEFINED_MESSAGE_TYPE + 1,

         // Network messages
         ID_INFO_NETWORK_MESSAGE,
         ID_INFO_JOINED_SERVER,
         ID_INFO_LEFT_SERVER,
         ID_INFO_SYNCHRONIZED_EVENT_MESSAGE,
      };

      // Network messages
      static const NetworkMessageType INFO_NETWORK_MESSAGE;
      static const NetworkMessageType INFO_JOINED_SERVER;
      static const NetworkMessageType INFO_LEFT_SERVER;
      static const NetworkMessageType INFO_SYNCHRONIZED_EVENT_MESSAGE;

   private:
      /// Constructor
      NetworkMessageType(const std::string& name, const std::string& cat,
         const std::string& desc, const unsigned short int id)
         : dtGame::MessageType(name, cat, desc, id)
      {
      }
   };

//////////////////////////////////////////////////////////////////////////////

} // namespace DeltaNetworkAdapter

#endif // NETWORK_MESSAGES_H
