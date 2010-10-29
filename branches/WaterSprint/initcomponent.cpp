#include "initcomponent.h"
#include "BoatKeyboardListener.h"
#include "OceanWindowResize.h"
#include <BoatActors/BoatActor.h>
#include <BoatActors/ActorLibraryRegistry.h>
#include <BoatActors/Outboard.h>
#include <cassert>
#include <dtCore/keyboard.h>
#include <dtCore/tripod.h>
#include <dtCore/transform.h>
#include <dtCore/deltawin.h>
#include <dtCore/odebodywrap.h>
#include <dtUtil/log.h>
#include <dtGame/messagetype.h>
#include <dtActors/engineactorregistry.h>
#include <dtActors/playerstartactorproxy.h>

#include <dtOcean/actorregistry.h>

//////////////////////////////////////////////////////////////////////////
InitComponent::InitComponent(dtCore::Keyboard& keyboard,
                             dtCore::DeltaWin& win):
dtGame::GMComponent("InitComponent"),
mKeyboardListener(new BoatKeyboardListener()),
mKeyboardToListenTo(&keyboard),
mWindow(&win),
mOceanResizeCB(new OceanWindowResize())
{
   win.AddResizeCallback(*mOceanResizeCB);
}

////////////////////////////////////////////////////////////////////////////////
InitComponent::~InitComponent()
{
   mKeyboardListener->SetOutboard(NULL);
   mKeyboardToListenTo->RemoveKeyboardListener(mKeyboardListener.get());
   mOceanResizeCB->SetOceanActor(NULL);
   mWindow->RemoveResizeCallback(*mOceanResizeCB);
   mWindow = NULL;
   mOceanResizeCB = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void InitComponent::ProcessMessage(const dtGame::Message& message)
{
   if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_LOADED)
   {

      BoatActor* boat = GetBoatToControl();

      if (boat != NULL)
      {
         mKeyboardListener->SetOutboard(boat->GetOutBoard());
         mKeyboardToListenTo->AddKeyboardListener(mKeyboardListener.get());

         MoveToSpawnPoint(*boat);
      }


      mOceanResizeCB->SetOceanActor(GetOceanActor());
   }
   else if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_UNLOADED)
   {
      mKeyboardListener->SetOutboard(NULL);
      mKeyboardToListenTo->RemoveKeyboardListener(mKeyboardListener.get());

      mOceanResizeCB->SetOceanActor(NULL);
   }
}

////////////////////////////////////////////////////////////////////////////////
void InitComponent::Reset()
{
   BoatActor* boat = GetBoatToControl();
   if (boat != NULL)
   {
      Outboard* outboard = boat->GetOutBoard();
      if (outboard)
      {
         outboard->CutEngine();
      }

      boat->GetBodyWrapper()->SetLinearVelocity(osg::Vec3(0.f,0.f,0.f));

      MoveToSpawnPoint(*boat);
   }
}

////////////////////////////////////////////////////////////////////////////////
BoatActor* InitComponent::GetBoatToControl() const
{
   //TODO duplicate of BoatFloaterComponent
   std::vector<dtDAL::ActorProxy*> boatActors;
   GetGameManager()->FindActorsByType(*BoatActorsLibraryRegistry::BOAT_ACTOR_TYPE, boatActors);
   if (!boatActors.empty())
   {
      return static_cast<BoatActor*>(boatActors[0]->GetActor());
   }
   else
   {
      return NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
void InitComponent::MoveToSpawnPoint(BoatActor& boat) const
{
   dtActors::PlayerStartActorProxy* spawnProxy;
   GetGameManager()->FindActorByType(*dtActors::EngineActorRegistry::PLAYER_START_ACTOR_TYPE, spawnProxy);
   if (spawnProxy)
   {
      dtActors::PlayerStartActor* startPoint(NULL);
      spawnProxy->GetActor(startPoint);
      dtCore::Transform xform;
      startPoint->GetTransform(xform);
      boat.SetTransform(xform);
   }
   else
   {
      LOG_WARNING("Could not find a player start point in the map");
   }
}

////////////////////////////////////////////////////////////////////////////////
dtOcean::OceanActor* InitComponent::GetOceanActor() const
{
   //find any OceanActors;
   dtOcean::OceanActorProxy* oceanActorProxy(NULL);

   GetGameManager()->FindActorByType(*OceanActorRegistry::OCEAN_ACTOR_TYPE, oceanActorProxy);
   dtOcean::OceanActor* oceanActor(NULL);
   oceanActorProxy->GetActor(oceanActor);

   return oceanActor;
}


////////////////////////////////////////////////////////////////////////////////
void InitComponent::AdjustOceanHeight(float adjust)
{
   dtOcean::OceanActor* ocean = GetOceanActor();
   if (ocean)
   {
      ocean->SetOceanHeight(ocean->GetOceanHeight() + adjust);
   }
}

////////////////////////////////////////////////////////////////////////////////
float InitComponent::GetOceanHeight()
{
   dtOcean::OceanActor* ocean = GetOceanActor();
   if (ocean)
   {
     return ocean->GetOceanHeight();
   }

   return 0.f;

}
