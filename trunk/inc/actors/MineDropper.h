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

   MineDropper(const std::string& filename = "../examples/data/models/physics_happy_sphere.ive");

   virtual void FireWeapon();

protected:
   virtual ~MineDropper();

private:
};
#endif // MineDropper_h__

////////////////////////////////////////////////////////////////////////////////
