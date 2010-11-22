#ifndef RocketActor_h__
#define RocketActor_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/ProjectileActor.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT RocketActor : public ProjectileActor
{
public:
   RocketActor(dtGame::GameActorProxy& proxy);

   virtual void TickLocal(const dtGame::Message& msg);

   virtual void OnEnteredWorld();

protected:
   virtual ~RocketActor();

private:
};

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT RocketActorProxy : public ProjectileActorProxy
{
public:
   RocketActorProxy();

      /**
   * Creates the actor
   */
   virtual void CreateActor();

protected:
   virtual ~RocketActorProxy();

private:
};
#endif // RocketActor_h__

////////////////////////////////////////////////////////////////////////////////
