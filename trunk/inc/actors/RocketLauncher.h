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

   RocketLauncher(const std::string& filename = "../examples/data/models/physics_happy_sphere.ive");

   virtual void FireWeapon();

protected:
   virtual ~RocketLauncher();

private:
};
#endif // RocketLauncher_h__

////////////////////////////////////////////////////////////////////////////////
