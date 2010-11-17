#ifndef SMKBoatActor_h__
#define SMKBoatActor_h__

////////////////////////////////////////////////////////////////////////////////
#include <SMKActorExport.h>
#include <BoatActors/BoatActor.h>
#include <Health.h>

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


   void SetupDefaultWeapon();

   dtGame::DeadReckoningHelper *GetDeadReckoningHelper() { return mDeadReckoningHelper; }
   const dtGame::DeadReckoningHelper *GetDeadReckoningHelper() const { return mDeadReckoningHelper; }

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

   /** 
   * Called when this ActorProxy is being removed from the world.  Time to clean
   * up our junk.
   */
   virtual void OnRemovedFromWorld();

   virtual void GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill);

protected:

   // Destructor
   virtual ~SMKBoatActorProxy();
};

////////////////////////////////////////////////////////////////////////////////

#endif // SMKBoatActor_h__
