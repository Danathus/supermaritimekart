#ifndef WeaponActor_h__
#define WeaponActor_h__

////////////////////////////////////////////////////////////////////////////////
#include <SMKActorExport.h>

#include <dtCore/object.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT WeaponActor : public dtCore::Object
{
public:
   const static std::string WEAPON_ACTOR_TYPE;

   WeaponActor(const std::string& filename = "../examples/data/models/physics_happy_sphere.ive");

protected:
   virtual ~WeaponActor();

private:
};
#endif // WeaponActor_h__

////////////////////////////////////////////////////////////////////////////////
