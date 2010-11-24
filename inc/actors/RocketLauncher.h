#ifndef RocketLauncher_h__
#define RocketLauncher_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/FrontWeapon.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT RocketLauncher : public FrontWeapon
{
public:
   const static std::string ROCKET_LAUNCHER_ACTOR_TYPE;

   RocketLauncher(const dtDAL::ResourceDescriptor& resource = dtDAL::ResourceDescriptor("StaticMeshes:Boat:RocketRackGreen.IVE"));

   virtual void FireWeapon();

protected:
   virtual ~RocketLauncher();

private:
   osg::Matrix GetLaunchLocation();
   std::string GetRocketNodeToFire();

   dtCore::RefPtr<dtCore::ParticleSystem> mpLaunchParticles;
   int mCurrentRocketNode;
};
#endif // RocketLauncher_h__

////////////////////////////////////////////////////////////////////////////////
