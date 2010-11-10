#ifndef SMK_NETWORK_COMPONENT__H
#define SMK_NETWORK_COMPONENT__H

////////////////////////////////////////////////////////////////////////////////

//#include <CCTS_ActorLibrary/ActorLibraryExport.h>
#include <DeltaNetworkAdapter/NetworkEngineComponent.h>

////////////////////////////////////////////////////////////////////////////////

class SMK_NetworkComponent : public NetworkEngineComponent
{
public:
   SMK_NetworkComponent();

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
};

////////////////////////////////////////////////////////////////////////////////

#endif // SMK_NETWORK_COMPONENT__H
