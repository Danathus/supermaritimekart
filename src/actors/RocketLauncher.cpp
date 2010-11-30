#include <actors/RocketLauncher.h>
#include <actors/RocketActor.h>
#include <actors/SMKActorLibraryRegistry.h>
#include <util/SMKUtilFunctions.h>

#include <dtAudio/audiomanager.h>
#include <dtCore/particlesystem.h>
#include <dtCore/scene.h>
#include <dtCore/transform.h>
#include <dtGame/gamemanager.h>
#include <dtUtil/nodecollector.h>
#include <dtUtil/stringutils.h>

#include <cassert>

////////////////////////////////////////////////////////////////////////////////
const std::string RocketLauncher::ROCKET_LAUNCHER_ACTOR_TYPE = "RocketLauncher";
const int MAX_ROCKET_NODES = 3;

//////////////////////////////////////////////////////////////////////////
RocketLauncher::RocketLauncher(const dtDAL::ResourceDescriptor& resource)
: FrontWeapon(resource)
, mCurrentRocketNode(0)
{
   SetName(ROCKET_LAUNCHER_ACTOR_TYPE);

   SetFiringRate(1.0f);

   // Load any sounds we have
   mpFireSound = LoadSound("/sounds/explosion.wav");
   mpFireSound->SetGain(0.75f);

   // Setup launch particles
   mpLaunchParticles = new dtCore::ParticleSystem;
   mpLaunchParticles->LoadFile("particles/rocket_launch.osg");
   mpLaunchParticles->SetEnabled(false);
   AddChild(mpLaunchParticles);

   // Setup our damage
   mDamage.SetDamageType(SMK::Damage::DAMAGE_BLAST);
   mDamage.SetAmount(15);
   mDamage.SetRadius(5.0f);
}

//////////////////////////////////////////////////////////////////////////
RocketLauncher::~RocketLauncher()
{
}

///////////////////////////////////////////////////////////////////////////////
void RocketLauncher::FireWeapon()
{
   FrontWeapon::FireWeapon();

   osg::Matrix launchLocation = GetLaunchLocation();
   dtCore::Transform currentTransform;
   currentTransform.Set(launchLocation);
   mpLaunchParticles->SetTransform(currentTransform);
   mpLaunchParticles->ResetTime();
   mpLaunchParticles->SetEnabled(true);

   if (!mpSMKBoatActorProxy->GetGameActor().IsRemote())
   {
      // Create a RocketActor and publish it
      dtCore::RefPtr<RocketActorProxy> rocketActorProxy;
      mpSMKBoatActorProxy->GetGameManager()->CreateActor(*SMKActorLibraryRegistry::ROCKET_ACTOR_TYPE, rocketActorProxy);
      if (rocketActorProxy.valid())
      {
         RocketActor* rocketActor = dynamic_cast<RocketActor*>(&rocketActorProxy->GetGameActor());
         rocketActor->SetTransform(currentTransform);
         rocketActor->SetDamage(mDamage);
         mpSMKBoatActorProxy->GetGameManager()->AddActor(*rocketActorProxy, false, true);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
osg::Matrix RocketLauncher::GetLaunchLocation()
{
   osg::Matrix launchLocation;

   dtCore::RefPtr<dtUtil::NodeCollector> collect = 
      new dtUtil::NodeCollector(GetOSGNode(),
      dtUtil::NodeCollector::MatrixTransformFlag);

   osg::MatrixTransform* launchTransform = collect->GetMatrixTransform(GetRocketNodeToFire());
   if (launchTransform != NULL)
   {
      launchLocation = SMK::GetAbsoluteMatrix(GetOSGNode(), launchTransform);
   }
   else
   {
      dtCore::Transform currentTransform;
      GetTransform(currentTransform);
      currentTransform.Get(launchLocation);
   }

   return launchLocation;
}

///////////////////////////////////////////////////////////////////////////////
std::string RocketLauncher::GetRocketNodeToFire()
{
   std::string rocketNode = "Node_Rocket" + dtUtil::ToString(++mCurrentRocketNode);
   mCurrentRocketNode %= MAX_ROCKET_NODES;
   return rocketNode;
}

////////////////////////////////////////////////////////////////////////////////
