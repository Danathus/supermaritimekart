#ifndef SMK_NETWORK_COMPONENT__H
#define SMK_NETWORK_COMPONENT__H

////////////////////////////////////////////////////////////////////////////////

//#include <CCTS_ActorLibrary/ActorLibraryExport.h>
#include <DeltaNetworkAdapter/NetworkEngineComponent.h>

////////////////////////////////////////////////////////////////////////////////

namespace dtDAL
{
   class BaseActorObject;
}

class SMK_NetworkComponent : public NetworkEngineComponent
{
public:
   SMK_NetworkComponent();

   /** Add the supplied proxy to the list of actors which need to be 
     * sent to new clients who connect.  Used to blast the game state as well
     * as locally created prototype actors.
     */
   void AddToNewClientPublishList(dtDAL::BaseActorObject& actorProxy);
   
   /** Empty out the list of Actors which get sent to newly connecting clients
   */
   void ClearNewClientPublishList();

protected:
   ~SMK_NetworkComponent();

   // implementations of virtual methods
   // from dtGame::GMComponent
   void OnAddedToGM();
   void OnRemovedFromGM();
   void ProcessMessage(const dtGame::Message& message);
   // from NetworkEngineComponent
   void QueueMessage(net::NodeID nodeID, const dtGame::Message* message);


private:

   typedef std::vector<dtCore::RefPtr<dtDAL::BaseActorObject> > ProxyContainer;
   ProxyContainer mProxiesToSendToNewClients;

   void SendGameDataToClient();
   void HandleThePickUpRequest(const dtGame::Message& message);

   double mTimeLeftBeforeUpdatingClients; ///<delay sending game state to connecting clients by this many seconds
   bool mNeedToUpdateClients; ///<a connected client needs to be pushed the game state

};

////////////////////////////////////////////////////////////////////////////////

#endif // SMK_NETWORK_COMPONENT__H
