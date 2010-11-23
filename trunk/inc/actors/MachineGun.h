#ifndef MachineGun_h__
#define MachineGun_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/FrontWeapon.h>

namespace dtCore
{
   class ShaderParamOscillator;
}

namespace osg 
{
   class Billboard; 
   class Geometry;
   class StateSet;  
   class Node;
}

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT MachineGun : public FrontWeapon
{
public:
   const static std::string MACHINE_GUN_ACTOR_TYPE;

   MachineGun(const dtDAL::ResourceDescriptor& resource = dtDAL::ResourceDescriptor("StaticMeshes:physics_happy_sphere.ive"));

   virtual void FireWeapon();

protected:
   virtual ~MachineGun();

private:
   void CreateBulletTrail();
   void ShowBulletTrail(osg::Vec3 target);
   osg::Vec3 CheckForBoatCollision(osg::Vec3 fireDirection);

   dtCore::RefPtr<osg::Billboard> mpBeam;  
   dtCore::RefPtr<osg::StateSet>  mpBeamStateSet;
   dtCore::RefPtr<osg::Geometry> mpQuad;
   dtCore::RefPtr<dtCore::ShaderParamOscillator> mpTimeParam;
};
#endif // MachineGun_h__

////////////////////////////////////////////////////////////////////////////////