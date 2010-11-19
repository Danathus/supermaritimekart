#ifndef NET_CONFIG__H
#define NET_CONFIG__H

// here lie all the defines and enum that specify the network configuration
// for this set of applications

//////////////////////////////////////////////////////////////////////////////

#define APP_PROTOCOL_ID 1235

enum Ports
{
   FTP_PORT  = 21, // this one may not work
   SSL_PORT  = 443,
   HTTP_PORT = 80,
   MAPI_PORT = 135,
   POP3_PORT = 110,

   //*
   BEACON_SENDING_PORT = 3333,
   BEACON_LISTENER_PORT,
   MULTICAST_PORT,
   GAME_HOST_PORT,
   GAME_PLAYER_0_PORT
   /*/
   BEACON_SENDING_PORT  = SSL_PORT,
   BEACON_LISTENER_PORT = HTTP_PORT,
   GAME_HOST_PORT       = POP3_PORT,
   GAME_PLAYER_0_PORT   = MAPI_PORT
   //*/
};
#define GAME_PLAYER_PORT(id) (GAME_PLAYER_0_PORT + (id))

#if _DEBUG
#  define NET_TIMEOUT_DURATION 30.0f // in seconds
#else
#  define NET_TIMEOUT_DURATION 10.0f // in seconds
#endif

#define RADIO_MULTICAST_ADDRESS(channel) DOTTED_QUAD_TO_INT(224,0,1,(channel))

//////////////////////////////////////////////////////////////////////////////

#endif // NET_CONFIG__H
