#include <actors/RocketLauncher.h>
#include <actors/RocketActor.h>
#include <actors/SMKActorLibraryRegistry.h>

#include <dtAudio/audiomanager.h>
#include <dtCore/transform.h>
#include <dtGame/gamemanager.h>

#include <cassert>

////////////////////////////////////////////////////////////////////////////////
const std::string RocketLauncher::ROCKET_LAUNCHER_ACTOR_TYPE = "RocketLauncher";

//////////////////////////////////////////////////////////////////////////
RocketLauncher::RocketLauncher(const std::string& filename /*= ""*/)
: FrontWeapon(filename)
{
   SetName(ROCKET_LAUNCHER_ACTOR_TYPE);

   SetFiringRate(1.0f);

   // Load any sounds we have
   mpFireSound = LoadSound("/sounds/exp57.wav");
   mpFireSound->SetGain(0.5f);

   // Setup our damage
   mDamage.SetDamageType(SMK::Damage::DAMAGE_PROJECTILE);
   mDamage.SetAmount(15);
   mDamage.SetRadius(0.0f);
}

//////////////////////////////////////////////////////////////////////////
RocketLauncher::~RocketLauncher()
{
}

///////////////////////////////////////////////////////////////////////////////
void RocketLauncher::FireWeapon()
{
   FrontWeapon::FireWeapon();

   if (!mpSMKBoatActorProxy->GetGameActor().IsRemote())
   {
      // Create a MineActor and publish it
      dtCore::RefPtr<RocketActorProxy> rocketActorProxy;
      mpSMKBoatActorProxy->GetGameManager()->CreateActor(*SMKActorLibraryRegistry::ROCKET_ACTOR_TYPE, rocketActorProxy);
      if (rocketActorProxy.valid())
      {
         RocketActor* rocketActor = dynamic_cast<RocketActor*>(&rocketActorProxy->GetGameActor());
         dtCore::Transform currentTransform;
         GetTransform(currentTransform);
         rocketActor->SetTransform(currentTransform);
         rocketActor->SetDamage(mDamage);
         mpSMKBoatActorProxy->GetGameManager()->AddActor(*rocketActorProxy, false, true);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
