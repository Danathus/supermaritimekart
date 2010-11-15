#ifndef WeaponActor_h__
#define WeaponActor_h__

////////////////////////////////////////////////////////////////////////////////
#include <SMKActorExport.h>

#include <dtAudio/sound.h>
#include <dtCore/object.h>
#include <dtCore/particlesystem.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT WeaponActor : public dtCore::Object
{
public:
   const static std::string WEAPON_ACTOR_TYPE;

   WeaponActor(const std::string& filename = "../examples/data/models/physics_happy_sphere.ive");

   void Update(float deltaTime);

   virtual void StartWeaponFiring();
   virtual void StopWeaponFiring();

protected:
   virtual ~WeaponActor();
   virtual void SetIsFiring(bool isFiring);
   virtual void FireWeapon();

   float mFiringRate;
   dtCore::RefPtr<dtCore::ParticleSystem> mpStartFiringParticles;
   dtCore::RefPtr<dtAudio::Sound> mpStartFiringSound;
   dtCore::RefPtr<dtAudio::Sound> mpFireSound;
   dtCore::RefPtr<dtAudio::Sound> mpStopFiringSound;

private:
   dtAudio::Sound* LoadSound(const std::string& file);
   dtCore::ParticleSystem* LoadParticleSystem(const std::string& file);

   bool mIsFiring;
   float mFiringCounter;
};
#endif // WeaponActor_h__

////////////////////////////////////////////////////////////////////////////////
