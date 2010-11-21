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

   virtual bool FilterContact(dContact* contact, Transformable* collider);

protected:
   virtual ~ProjectileActor();
   void SetMeshResource(const std::string& name, const std::string& file);

private:
   dtCore::RefPtr<dtGame::DeadReckoningHelper> mDeadReckoningHelper;
   dtCore::RefPtr<dtGame::DRPublishingActComp> mDRPublishingActComp;

   SMK::Damage mDamage;
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

   virtual void OnRotation(const osg::Vec3 &oldValue, const osg::Vec3 &newValue);
   virtual void OnTranslation(const osg::Vec3 &oldValue, const osg::Vec3 &newValue);

protected:
   virtual ~ProjectileActorProxy();

private:
};
#endif // ProjectileActor_h__

////////////////////////////////////////////////////////////////////////////////
