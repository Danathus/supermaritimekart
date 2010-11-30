#ifndef TorpedoLauncher_h__
#define TorpedoLauncher_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/FrontWeapon.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT TorpedoLauncher : public FrontWeapon
{
public:
   const static std::string TORPEDO_LAUNCHER_ACTOR_TYPE;

   TorpedoLauncher(const dtDAL::ResourceDescriptor& resource = dtDAL::ResourceDescriptor("StaticMeshes:Boat:TorpRackGreen.IVE"));

   virtual void FireWeapon();

protected:
   virtual ~TorpedoLauncher();

private:
   osg::Matrix GetLaunchLocation();
   std::string GetTorpedoNodeToFire();

   dtCore::RefPtr<dtCore::ParticleSystem> mpLaunchParticles;
   int mCurrentTorpedoNode;
};
#endif // TorpedoLauncher_h__

////////////////////////////////////////////////////////////////////////////////
