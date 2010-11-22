#include <actors/MineDropper.h>

#include <actors/SMKActorLibraryRegistry.h>
#include <actors/MineActor.h>

#include <dtCore/transform.h>
#include <dtGame/gamemanager.h>

////////////////////////////////////////////////////////////////////////////////
const std::string MineDropper::MINE_DROPPER_WEAPON_TYPE = "MineDropper";

//////////////////////////////////////////////////////////////////////////
MineDropper::MineDropper(const dtDAL::ResourceDescriptor& resource)
: BackWeapon(resource)
{
   SetName(MINE_DROPPER_WEAPON_TYPE);

   // We want to drop one mine every six seconds
   SetFiringRate(1.0f / 6.0f);

   // Setup our damage
   mDamage.SetDamageType(SMK::Damage::DAMAGE_BLAST);
   mDamage.SetAmount(25);
   mDamage.SetRadius(15.0f);

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
      dtCore::RefPtr<MineActorProxy> mineActorProxy;
      mpSMKBoatActorProxy->GetGameManager()->CreateActor(*SMKActorLibraryRegistry::MINE_ACTOR_TYPE, mineActorProxy);
      if (mineActorProxy.valid())
      {
         MineActor* mineActor = dynamic_cast<MineActor*>(&mineActorProxy->GetGameActor());
         dtCore::Transform currentTransform;
         GetTransform(currentTransform);
         mineActor->SetTransform(currentTransform);
         mineActor->SetDamage(mDamage);
         mpSMKBoatActorProxy->GetGameManager()->AddActor(*mineActorProxy, false, true);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
