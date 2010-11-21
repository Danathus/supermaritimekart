#ifndef MineActor_h__
#define MineActor_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/ProjectileActor.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT MineActor : public ProjectileActor
{
public:
   MineActor(dtGame::GameActorProxy& proxy);

   virtual void TickLocal(const dtGame::Message& msg);

   virtual void OnEnteredWorld();

protected:
   virtual ~MineActor();

private:
};

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT MineActorProxy : public ProjectileActorProxy
{
public:
   MineActorProxy();

      /**
   * Creates the actor
   */
   virtual void CreateActor();

protected:
   virtual ~MineActorProxy();

private:
};
#endif // MineActor_h__

////////////////////////////////////////////////////////////////////////////////
