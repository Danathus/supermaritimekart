#ifndef ProjectileActor_h__
#define ProjectileActor_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <dtActors/GameMeshActor.h>

#include <util/Damage.h>

namespace dtGame
{
   class DeadReckoningHelper;
   class DRPublishingActComp;
}

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT ProjectileActor : public dtActors::GameMeshActor
{
public:
   ProjectileActor(dtGame::GameActorProxy& proxy);

   virtual void TickLocal(const dtGame::Message& msg);

   /**
   * Called when an actor is first placed in the "world"
   */
   virtual void OnEnteredWorld();

   /** 
   * Inherited from FloatingActor.  Called when this actor is about to be nuked.
   */
   virtual void OnRemovedFromWorld();

   virtual void BuildActorComponents();

   dtGame::DeadReckoningHelper* GetDeadReckoningHelper() { return mDeadReckoningHelper; }
   const dtGame::DeadReckoningHelper* GetDeadReckoningHelper() const { return mDeadReckoningHelper; }
   dtGame::DRPublishingActComp* GetDRPublishingActComp() { return mDRPublishingActComp; }
   const dtGame::DRPublishingActComp* GetDRPublishingActComp() const { return mDRPublishingActComp; }

   void SetDamage(const SMK::Damage& damage) { mDamage = damage; }
   SMK::Damage GetDamage() const             { return mDamage;   }

   void SetLifetime(float lifetime) { mLifetime = lifetime; }
   float GetLifetime() const        { return mLifetime;     }

   ///How long to wait before enabling Collision Detection. This gives the player
   ///a chance to not be hit by it's own weapons.
   void SetArmingDelay(float delay) { mArmingDelay = delay; }
   float GetArmingDelay() const     { return mArmingDelay;  }
   bool IsArmed() { return mLifeCounter >= mArmingDelay; }

   virtual bool FilterContact(dContact* contact, Transformable* collider);

protected:
   virtual ~ProjectileActor();
   void SetMeshResource(const std::string& name);

private:
   void DetonateProjectile();

   dtCore::RefPtr<dtGame::DeadReckoningHelper> mDeadReckoningHelper;
   dtCore::RefPtr<dtGame::DRPublishingActComp> mDRPublishingActComp;

   SMK::Damage mDamage;
   float mLifetime;
   float mLifeCounter;
   float mArmingDelay;
};

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT ProjectileActorProxy : public dtActors::GameMeshActorProxy
{
public:
   ProjectileActorProxy();

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
   virtual ~ProjectileActorProxy();

private:
};
#endif // ProjectileActor_h__

////////////////////////////////////////////////////////////////////////////////
