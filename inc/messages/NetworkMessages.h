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
   static const SMKNetworkMessages INFO_PICKUP_ITEM_ACQUIRED; ///<a player was granted the pick up from the server
   static const SMKNetworkMessages ACTION_FRONT_WEAPON_FIRED;
   static const SMKNetworkMessages ACTION_BACK_WEAPON_FIRED;
   static const SMKNetworkMessages ACTION_BOAT_HIT;
   static const SMKNetworkMessages ACTION_PROJECTILE_EXPLODED;
   static const SMKNetworkMessages ACTION_BOAT_EXPLODED;
   static const SMKNetworkMessages REQUEST_PICKUP_PICKUP; //<A request to pickup a pickup item

   enum
   {
      ID_NULL_NETWORK_MESSAGE = dtGame::MessageType::USER_DEFINED_MESSAGE_TYPE + 5000,

      // Network messages
      ID_INFO_CLIENT_MAP_LOADED,
      ID_INFO_PICKUP_ITEM_ACQUIRED,
      ID_ACTION_FRONT_WEAPON_FIRED,
      ID_ACTION_BACK_WEAPON_FIRED,
      ID_ACTION_BOAT_HIT,
      ID_ACTION_PROJECTILE_EXPLODED,
      ID_ACTION_BOAT_EXPLODED,
      ID_REQUEST_PICKUP_PICKUP
   };

   DT_DECLARE_MESSAGE_TYPE_CLASS_END()

}
#endif // NETWORKMESSAGES_h__

///////////////////////////////////////////////////////////////////////////////
