#include <messages/NetworkMessages.h>
 
using namespace SMK;

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
DEFINE_APP_MESSAGE_TYPE(SMKNetworkMessages, INFO,     CLIENT_MAP_LOADED,                 "Sent when client is finished loading it's map",  dtGame::Message);