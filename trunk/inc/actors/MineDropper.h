#ifndef MineDropper_h__
#define MineDropper_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/BackWeapon.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT MineDropper : public BackWeapon
{
public:
   const static std::string MINE_DROPPER_WEAPON_TYPE;

   MineDropper(const dtDAL::ResourceDescriptor& resource = dtDAL::ResourceDescriptor("StaticMeshes:Boat:MineRack.IVE"));

   virtual void FireWeapon();

protected:
   virtual ~MineDropper();

private:
   osg::Matrix GetLaunchLocation();
   std::string GetMineNodeToFire();

   dtCore::RefPtr<dtCore::ParticleSystem> mpLaunchParticles;
   int mCurrentMineNode;
};
#endif // MineDropper_h__

////////////////////////////////////////////////////////////////////////////////
