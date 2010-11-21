#include <actors/MineDropper.h>

#include <actors/SMKActorLibraryRegistry.h>
#include <actors/MineActor.h>

#include <dtCore/transform.h>
#include <dtGame/gamemanager.h>

////////////////////////////////////////////////////////////////////////////////
const std::string MineDropper::MINE_DROPPER_WEAPON_TYPE = "MineDropper";

//////////////////////////////////////////////////////////////////////////
MineDropper::MineDropper(const std::string& filename /*= ""*/)
: BackWeapon(filename)
{
   SetName(MINE_DROPPER_WEAPON_TYPE);

   // We want to drop one mine every six seconds
   SetFiringRate(1.0f / 6.0f);

   //// Load any sounds we have
   //mpFireSound = LoadSound("/sounds/exp57.wav");
   //mpFireSound->SetGain(0.5f);
}

//////////////////////////////////////////////////////////////////////////
MineDropper::~MineDropper()
{
}

///////////////////////////////////////////////////////////////////////////////
void MineDropper::FireWeapon()
{
   BackWeapon::FireWeapon();

   if (!mpSMKBoatActorProxy->GetGameActor().IsRemote())
   {
      // Create a MineActor and publish it
      dtCore::RefPtr<MineActorProxy> mineActor;
      mpSMKBoatActorProxy->GetGameManager()->CreateActor(*SMKActorLibraryRegistry::MINE_ACTOR_TYPE, mineActor);
      if (mineActor.valid())
      {
         dtCore::Transform currentTransform;
         GetTransform(currentTransform);
         (&mineActor->GetGameActor())->SetTransform(currentTransform);
         mpSMKBoatActorProxy->GetGameManager()->AddActor(*mineActor, false, true);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
