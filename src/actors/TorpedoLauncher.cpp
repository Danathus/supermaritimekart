#include <actors/TorpedoLauncher.h>
#include <actors/TorpedoActor.h>
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
const std::string TorpedoLauncher::TORPEDO_LAUNCHER_ACTOR_TYPE = "TorpedoLauncher";
const int MAX_TORPEDO_NODES = 2;

//////////////////////////////////////////////////////////////////////////
TorpedoLauncher::TorpedoLauncher(const dtDAL::ResourceDescriptor& resource)
: FrontWeapon(resource)
, mCurrentTorpedoNode(0)
{
   SetName(TORPEDO_LAUNCHER_ACTOR_TYPE);

   SetFiringRate(1.0f);

   // Load any sounds we have
   mpFireSound = LoadSound("/sounds/explosion.wav");
   mpFireSound->SetGain(0.75f);

   // Setup launch particles
   mpLaunchParticles = new dtCore::ParticleSystem;
   mpLaunchParticles->LoadFile("particles/torpedo_launch.osg");
   mpLaunchParticles->SetEnabled(false);
   AddChild(mpLaunchParticles);

   // Setup our damage
   mDamage.SetDamageType(SMK::Damage::DAMAGE_BLAST);
   mDamage.SetAmount(15);
   mDamage.SetRadius(5.0f);
}

//////////////////////////////////////////////////////////////////////////
TorpedoLauncher::~TorpedoLauncher()
{
}

///////////////////////////////////////////////////////////////////////////////
void TorpedoLauncher::FireWeapon()
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
      // Create a TorpedoActor and publish it
      dtCore::RefPtr<TorpedoActorProxy> torpedoActorProxy;
      mpSMKBoatActorProxy->GetGameManager()->CreateActor(*SMKActorLibraryRegistry::TORPEDO_ACTOR_TYPE, torpedoActorProxy);
      if (torpedoActorProxy.valid())
      {
         TorpedoActor* torpedoActor = dynamic_cast<TorpedoActor*>(&torpedoActorProxy->GetGameActor());
         torpedoActor->SetTransform(currentTransform);
         torpedoActor->SetDamage(mDamage);
         mpSMKBoatActorProxy->GetGameManager()->AddActor(*torpedoActorProxy, false, true);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
osg::Matrix TorpedoLauncher::GetLaunchLocation()
{
   osg::Matrix launchLocation;

   dtCore::RefPtr<dtUtil::NodeCollector> collect = 
      new dtUtil::NodeCollector(GetOSGNode(),
      dtUtil::NodeCollector::MatrixTransformFlag);

   osg::MatrixTransform* launchTransform = collect->GetMatrixTransform(GetTorpedoNodeToFire());
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
std::string TorpedoLauncher::GetTorpedoNodeToFire()
{
   std::string TorpedoNode = "Node_Torp" + dtUtil::ToString(++mCurrentTorpedoNode);
   mCurrentTorpedoNode %= MAX_TORPEDO_NODES;
   return TorpedoNode;
}

////////////////////////////////////////////////////////////////////////////////
