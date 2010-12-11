#include <actors/MineDropper.h>
#include <util/SMKUtilFunctions.h>

#include <actors/SMKActorLibraryRegistry.h>
#include <actors/MineActor.h>

#include <dtCore/particlesystem.h>
#include <dtCore/transform.h>
#include <dtGame/gamemanager.h>
#include <dtUtil/nodecollector.h>
#include <dtUtil/stringutils.h>

////////////////////////////////////////////////////////////////////////////////
const std::string MineDropper::MINE_DROPPER_WEAPON_TYPE = "MineDropper";
const int MAX_MINE_NODES = 2;

//////////////////////////////////////////////////////////////////////////
MineDropper::MineDropper(const dtDAL::ResourceDescriptor& resource)
: BackWeapon(resource)
, mCurrentMineNode(0)
{
   SetName(MINE_DROPPER_WEAPON_TYPE);

   // We want to drop one mine every two seconds
   SetFiringRate(1.0f / 2.0f);

   // Load any sounds we have
   mpFireSound = LoadSound("/sounds/splash.wav");

   // Setup launch particles
   mpLaunchParticles = new dtCore::ParticleSystem;
   mpLaunchParticles->LoadFile("particles/mine_launch.osg");
   mpLaunchParticles->SetEnabled(false);
   AddChild(mpLaunchParticles);

   // Setup our damage
   mDamage.SetDamageType(SMK::Damage::DAMAGE_BLAST);
   mDamage.SetAmount(25);
   mDamage.SetRadius(15.0f);
}

//////////////////////////////////////////////////////////////////////////
MineDropper::~MineDropper()
{
}

///////////////////////////////////////////////////////////////////////////////
void MineDropper::FireWeapon()
{
   BackWeapon::FireWeapon();

   osg::Matrix launchLocation = GetLaunchLocation();
   dtCore::Transform currentTransform;
   currentTransform.Set(launchLocation);
   mpLaunchParticles->SetTransform(currentTransform);
   mpLaunchParticles->ResetTime();
   mpLaunchParticles->SetEnabled(true);

   if (!mpSMKBoatActorProxy->GetGameActor().IsRemote())
   {
      // Create a MineActor and publish it
      dtCore::RefPtr<MineActorProxy> mineActorProxy;
      mpSMKBoatActorProxy->GetGameManager()->CreateActor(*SMKActorLibraryRegistry::MINE_ACTOR_TYPE, mineActorProxy);
      if (mineActorProxy.valid())
      {
         MineActor* mineActor = dynamic_cast<MineActor*>(&mineActorProxy->GetGameActor());
         mineActor->SetTransform(currentTransform);
         mineActor->SetDamage(mDamage);
         mineActor->SetOwner(&mpSMKBoatActorProxy->GetGameActor());
         mpSMKBoatActorProxy->GetGameManager()->AddActor(*mineActorProxy, false, true);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
osg::Matrix MineDropper::GetLaunchLocation()
{
   osg::Matrix launchLocation;

   dtCore::RefPtr<dtUtil::NodeCollector> collect =
      new dtUtil::NodeCollector(GetOSGNode(),
      dtUtil::NodeCollector::MatrixTransformFlag);

   osg::MatrixTransform* launchTransform = collect->GetMatrixTransform(GetMineNodeToFire());
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
std::string MineDropper::GetMineNodeToFire()
{
   std::string mineNode = "Node_Mine" + dtUtil::ToString(++mCurrentMineNode);
   mCurrentMineNode %= MAX_MINE_NODES;
   return mineNode;
}

////////////////////////////////////////////////////////////////////////////////
