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
   * Currently called when colliding with pickups.
   */
   void OnCollision(const std::string& type, const osg::Vec3& pos, const osg::Vec3& normal);

   /** 
   * Inherited from FloatingActor.  Called when this actor is about to be nuked.
   */
   virtual void OnRemovedFromWorld();

   void SetupDefaultWeapon();

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
