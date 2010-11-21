#include <messages/NetworkMessages.h>
 
using namespace SMK;

///////////////////////////////////////////////////////////////////////////////
DT_IMPLEMENT_MESSAGE_TYPE_CLASS(SMKNetworkMessages)

#define QUOTEME(x) #x
#define DEFINE_APP_MESSAGE_TYPE(msgtypeclass, category, restofname, description, msgClass) \
   const msgtypeclass msgtypeclass:: category##_##restofname (                   \
   QUOTEME(category##_##restofname),                                             \
#category,                                                                    \
   description,                                                                  \
   SMKNetworkMessages::ID_##category##_##restofname,                             \
   DT_MSG_CLASS(msgClass)                                                        \
   );

// Network messages
DEFINE_APP_MESSAGE_TYPE(SMKNetworkMessages, INFO,    CLIENT_MAP_LOADED,   "Sent when client is finished loading it's map",  dtGame::Message);
DEFINE_APP_MESSAGE_TYPE(SMKNetworkMessages, ACTION,  FRONT_WEAPON_FIRED,  "Sent when a player fires their front weapon",  dtGame::Message);
DEFINE_APP_MESSAGE_TYPE(SMKNetworkMessages, ACTION,  BACK_WEAPON_FIRED,   "Sent when a player fires their back weapon",  dtGame::Message);
DEFINE_APP_MESSAGE_TYPE(SMKNetworkMessages, ACTION,  BOAT_HIT,            "Sent when a boat gets hit by a weapon",  dtGame::Message);
DEFINE_APP_MESSAGE_TYPE(SMKNetworkMessages, ACTION,  PROJECTILE_EXPLODED, "Sent when a projectile explodes",  dtGame::Message);
DEFINE_APP_MESSAGE_TYPE(SMKNetworkMessages, REQUEST, PICKUP_PICKUP,       "Sent when a player requests to pickup a pickup",  dtGame::Message);

///////////////////////////////////////////////////////////////////////////////
