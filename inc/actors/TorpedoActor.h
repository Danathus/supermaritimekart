#ifndef TorpedoActor_h__
#define TorpedoActor_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/ProjectileActor.h>

////////////////////////////////////////////////////////////////////////////////
class SimpleFloaterActorComponent;

class SMK_ACTOR_EXPORT TorpedoActor : public ProjectileActor
{
public:
   TorpedoActor(dtGame::GameActorProxy& proxy);

   virtual void TickLocal(const dtGame::Message& msg);

   virtual void OnEnteredWorld();

protected:
   virtual ~TorpedoActor();

private:
   void SetupFloaterComponent();

   dtCore::RefPtr<SimpleFloaterActorComponent> mpFloaterComponent;
};

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT TorpedoActorProxy : public ProjectileActorProxy
{
public:
   TorpedoActorProxy();

   /**
    * Creates the actor
    */
   virtual void CreateActor();

protected:
   virtual ~TorpedoActorProxy();

private:
};
#endif // TorpedoActor_h__

////////////////////////////////////////////////////////////////////////////////
