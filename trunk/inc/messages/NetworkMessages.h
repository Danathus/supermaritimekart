#ifndef NETWORKMESSAGES_h__
#define NETWORKMESSAGES_h__

///////////////////////////////////////////////////////////////////////////////
#include <messages/MessagesExport.h>
#include <dtGame/messagetype.h>

///////////////////////////////////////////////////////////////////////////////
namespace SMK
{
   DT_DECLARE_MESSAGE_TYPE_CLASS_BEGIN(SMKNetworkMessages, SMK_MESSAGE_EXPORT)

   static const SMKNetworkMessages INFO_CLIENT_MAP_LOADED;
   static const SMKNetworkMessages ACTION_WEAPON_FIRED;

   enum
   {
      ID_NULL_NETWORK_MESSAGE = dtGame::MessageType::USER_DEFINED_MESSAGE_TYPE + 5000,

      // Network messages
      ID_INFO_CLIENT_MAP_LOADED,
      ID_ACTION_WEAPON_FIRED
   };

   DT_DECLARE_MESSAGE_TYPE_CLASS_END()

}
#endif // NETWORKMESSAGES_h__

///////////////////////////////////////////////////////////////////////////////
