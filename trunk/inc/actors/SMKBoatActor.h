#ifndef SMKBoatActor_h__
#define SMKBoatActor_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/Health.h>

#include <BoatActors/BoatActor.h>

////////////////////////////////////////////////////////////////////////////////

class SMKBoatActorProxy;
class TurretWeapon;
class Weapon;

namespace dtAudio { class Sound; }

namespace dtGame
{
   class DeadReckoningHelper;
   class DRPublishingActComp;
}

class SMK_ACTOR_EXPORT SMKBoatActor : public BoatActor
{
public:  

   /**
   * Constructor
   */
   SMKBoatActor(SMKBoatActorProxy& proxy);

   virtual void TickLocal(const dtGame::Message& msg);

   /**
   * Get the front weapon.
   */
   Weapon* GetFrontWeapon();
   const Weapon* GetFrontWeapon() const;

   /**
   * Get the back weapon.
   */
   Weapon* GetBackWeapon();
   const Weapon* GetBackWeapon() const;

   /**
   * Callback from Scene when a contact occurs. This normally is used to 
   * filter out Transformables that you do not want to perform expensive
   * physics calculations on, but here we use it to fire our Trigger.
   */
   virtual bool FilterContact(dContact* contact, Transformable* collider);

   /**
   * Called when an actor is first placed in the "world"
   */
   virtual void OnEnteredWorld();

   /** 
   * Inherited from FloatingActor.  Called when this actor is about to be nuked.
   */
   virtual void OnRemovedFromWorld();

   virtual void BuildActorComponents();

   virtual void ProcessMessage(const dtGame::Message& message);

   dtGame::DeadReckoningHelper* GetDeadReckoningHelper() { return mDeadReckoningHelper; }
   const dtGame::DeadReckoningHelper* GetDeadReckoningHelper() const { return mDeadReckoningHelper; }
   dtGame::DRPublishingActComp* GetDRPublishingActComp() { return mDRPublishingActComp; }
   const dtGame::DRPublishingActComp* GetDRPublishingActComp() const { return mDRPublishingActComp; }

   void CauseFullUpdate();

protected:

   /**
   * Destructor
   */
   virtual ~SMKBoatActor();

   /**
   * Initialize the actor.
   */
   virtual void Initialize();

private:
   void SetupDefaultWeapon();

   void FireWeapon(const dtGame::Message& weaponFiredMessage);

   dtCore::RefPtr<TurretWeapon> mpFrontWeapon;
   dtCore::RefPtr<Weapon> mpBackWeapon;
   SMK::Health mHealth;  ///<The current health of this boat
   dtCore::RefPtr<dtAudio::Sound> mPickupAcquireSound;

   dtCore::RefPtr<dtGame::DeadReckoningHelper> mDeadReckoningHelper;
   dtCore::RefPtr<dtGame::DRPublishingActComp> mDRPublishingActComp;
};

class SMKBoatActorProxy : public BoatActorProxy
{
public:

   /**
   * Constructor
   */
   SMKBoatActorProxy();

   /**
   * Adds the properties associated with this actor
   */
   virtual void BuildPropertyMap();

   /**
   * Build invokables
   */
   virtual void BuildInvokables();

   /**
   * Creates the actor
   */
   virtual void CreateActor();

   virtual void OnEnteredWorld();

   /** 
   * Called when this ActorProxy is being removed from the world.  Time to clean
   * up our junk.
   */
   virtual void OnRemovedFromWorld();

   virtual void GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill);

   virtual void NotifyFullActorUpdate();

protected:

   // Destructor
   virtual ~SMKBoatActorProxy();
};

////////////////////////////////////////////////////////////////////////////////

#endif // SMKBoatActor_h__
