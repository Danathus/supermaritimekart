#ifndef Weapon_h__
#define Weapon_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/SMKBoatActor.h>
#include <util/Damage.h>

#include <dtAudio/sound.h>
#include <dtCore/object.h>
#include <dtCore/observerptr.h>
#include <dtDAL/resourcedescriptor.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT Weapon : public dtCore::Object
{
public:
   const static std::string WEAPON_ACTOR_TYPE;

   Weapon(const dtDAL::ResourceDescriptor& resource = dtDAL::ResourceDescriptor());

   void SetSMKBoatActorProxy(dtGame::GameActorProxy* proxy) { mpSMKBoatActorProxy = proxy;      }
   dtGame::GameActorProxy* GetSMKBoatActorProxy() const     { return mpSMKBoatActorProxy.get(); }

   //virtual osg::Node* LoadFile(const std::string& filename, bool useCache);

   void SetFiringRate(float rate);

   void Update(float deltaTime);

   virtual void StartWeaponFiring();
   virtual void FireWeapon();
   virtual void StopWeaponFiring();

protected:
   virtual ~Weapon();

   virtual void SetIsFiring(bool isFiring);
   dtAudio::Sound* LoadSound(const std::string& file);

   dtCore::RefPtr<dtAudio::Sound> mpStartFiringSound;
   dtCore::RefPtr<dtAudio::Sound> mpFireSound;
   dtCore::RefPtr<dtAudio::Sound> mpStopFiringSound;
   dtCore::ObserverPtr<dtGame::GameActorProxy> mpSMKBoatActorProxy;

   SMK::Damage mDamage;

private:
   bool mIsFiring;
   float mFiringRate;
   float mFiringCounter;
};
#endif // Weapon_h__

////////////////////////////////////////////////////////////////////////////////
