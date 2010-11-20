#ifndef NETWORK_MESSAGES_H
#define NETWORK_MESSAGES_H

////////////////////////////////////////////////////////////////////////////////

#include <dtGame/messagetype.h>
#include <DeltaNetworkAdapter/LibraryExport.h>

////////////////////////////////////////////////////////////////////////////////


namespace DeltaNetworkAdapter
{

////////////////////////////////////////////////////////////////////////////////

   DT_DECLARE_MESSAGE_TYPE_CLASS_BEGIN(NetworkMessageType, DELTANETWORKADAPTER_LIBRARY_EXPORT)

         static const NetworkMessageType INFO_NETWORK_MESSAGE;
         static const NetworkMessageType INFO_JOINED_SERVER;
         static const NetworkMessageType INFO_LEFT_SERVER;
         static const NetworkMessageType INFO_SYNCHRONIZED_EVENT_MESSAGE;

         enum
         {
            //ID_NULL_NETWORK_MESSAGE = SharedUtilMessageType::ID_USER_DEFINED_MESSAGE_TYPE + 1,
            ID_NULL_NETWORK_MESSAGE = dtGame::MessageType::USER_DEFINED_MESSAGE_TYPE + 1000,

            // Network messages
            ID_INFO_NETWORK_MESSAGE,
            ID_INFO_JOINED_SERVER,
            ID_INFO_LEFT_SERVER,
            ID_INFO_SYNCHRONIZED_EVENT_MESSAGE,
         };

   DT_DECLARE_MESSAGE_TYPE_CLASS_END()

} // namespace DeltaNetworkAdapter

#endif // NETWORK_MESSAGES_H
