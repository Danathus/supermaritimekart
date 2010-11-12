#ifndef SMKBoatActor_h__
#define SMKBoatActor_h__

////////////////////////////////////////////////////////////////////////////////
#include <SMKActorExport.h>
#include <BoatActors/BoatActor.h>

////////////////////////////////////////////////////////////////////////////////

class SMKBoatActorProxy;
class Weapon;

class SMK_ACTOR_EXPORT SMKBoatActor : public BoatActor
{
public:  

   /**
   * Constructor
   */
   SMKBoatActor(SMKBoatActorProxy& proxy);

   /**
   * Get the front weapon.
   */
   Weapon* GetFrontWeapon();
   const Weapon* GetFrontWeapon() const;

   /** 
   * Inherited from FloatingActor.  Called when this actor is about to be nuked.
   */
   virtual void OnRemovedFromWorld();

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
   dtCore::RefPtr<Weapon> mpFrontWeapon;
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
