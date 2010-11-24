#ifndef MachineGun_h__
#define MachineGun_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/FrontWeapon.h>

namespace dtCore
{
   class ShaderParamOscillator;
   class ISector;
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

   MachineGun(const dtDAL::ResourceDescriptor& resource = dtDAL::ResourceDescriptor("StaticMeshes:Boat:DeckGun.ive"));

   virtual void FireWeapon();

protected:
   virtual ~MachineGun();

private:
   void CreateBulletTrail();
   void ShowBulletTrail(osg::Vec3 target);

   osg::Vec3 ConvertAbsoluteToRelativePosition(osg::Vec3 relativePos);
   osg::Vec3 CheckForBoatCollision();
   osg::Matrix GetGunfireLocation();

   dtCore::RefPtr<osg::Billboard> mpBeam;  
   dtCore::RefPtr<osg::StateSet>  mpBeamStateSet;
   dtCore::RefPtr<osg::Geometry> mpQuad;
   dtCore::RefPtr<dtCore::ShaderParamOscillator> mpTimeParam;
   //dtCore::RefPtr<dtCore::ISector> mISector;
};
#endif // MachineGun_h__

////////////////////////////////////////////////////////////////////////////////
