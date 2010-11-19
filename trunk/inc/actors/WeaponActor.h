#ifndef WeaponActor_h__
#define WeaponActor_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/SMKBoatActor.h>

#include <dtAudio/sound.h>
#include <dtCore/object.h>
#include <dtCore/observerptr.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT WeaponActor : public dtCore::Object
{
public:
   const static std::string WEAPON_ACTOR_TYPE;

   WeaponActor(const std::string& filename = "");

   void SetSMKBoatActorProxy(SMKBoatActorProxy* proxy) { mpSMKBoatActorProxy = proxy;       }
   SMKBoatActorProxy* GetSMKBoatActorProxy() const     { return mpSMKBoatActorProxy.get();  }

   void Update(float deltaTime);

   virtual void StartWeaponFiring();
   virtual void FireWeapon();
   virtual void StopWeaponFiring();

protected:
   virtual ~WeaponActor();

   virtual void SetIsFiring(bool isFiring);
   dtAudio::Sound* LoadSound(const std::string& file);

   float mFiringRate;
   dtCore::RefPtr<dtAudio::Sound> mpStartFiringSound;
   dtCore::RefPtr<dtAudio::Sound> mpFireSound;
   dtCore::RefPtr<dtAudio::Sound> mpStopFiringSound;

private:
   bool mIsFiring;
   float mFiringCounter;

   dtCore::ObserverPtr<SMKBoatActorProxy> mpSMKBoatActorProxy;
};
#endif // WeaponActor_h__

////////////////////////////////////////////////////////////////////////////////
