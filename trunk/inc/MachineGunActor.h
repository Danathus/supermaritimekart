#ifndef MachineGunActor_h__
#define MachineGunActor_h__

////////////////////////////////////////////////////////////////////////////////
#include <SMKActorExport.h>
#include <WeaponActor.h>

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
class SMK_ACTOR_EXPORT MachineGunActor : public WeaponActor
{
public:
   const static std::string MACHINE_GUN_ACTOR_TYPE;

   MachineGunActor(const std::string& filename = "../examples/data/models/physics_happy_sphere.ive");

   void Update(float deltaTime);

protected:
   virtual ~MachineGunActor();
   virtual void FireWeapon();

private:
   void CreateBulletTrail();
   void ShowBulletTrail(osg::Vec3 target);

   dtCore::RefPtr<osg::Billboard> mpBeam;  
   dtCore::RefPtr<osg::StateSet>  mpBeamStateSet;
   dtCore::RefPtr<osg::Geometry> mpQuad;
   dtCore::RefPtr<dtCore::ShaderParamOscillator> mpTimeParam;
};
#endif // MachineGunActor_h__

////////////////////////////////////////////////////////////////////////////////
