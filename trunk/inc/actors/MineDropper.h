#ifndef MineDropper_h__
#define MineDropper_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/BackWeaponActor.h>

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
class SMK_ACTOR_EXPORT MineDropper : public BackWeaponActor
{
public:
   const static std::string MINE_DROPPER_WEAPON_TYPE;

   MineDropper(const std::string& filename = "../examples/data/models/physics_happy_sphere.ive");

   virtual void FireWeapon();

protected:
   virtual ~MineDropper();

private:
};
#endif // MineDropper_h__

////////////////////////////////////////////////////////////////////////////////
