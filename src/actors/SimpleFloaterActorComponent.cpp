#include <actors/SimpleFloaterActorComponent.h>

#include <dtCore/transform.h>
#include <dtCore/transformable.h>

////////////////////////////////////////////////////////////////////////////////
SimpleFloaterActorComponent::SimpleFloaterActorComponent(dtOcean::OceanActor& oceanActor,
   const std::string& type /*= "SimpleFloaterComponent"*/)
: dtGame::ActorComponent(type)
, mpOceanHeightGetter(new DeltaOceanGetHeight(oceanActor))
{
}

////////////////////////////////////////////////////////////////////////////////
SimpleFloaterActorComponent::~SimpleFloaterActorComponent()
{
}

///////////////////////////////////////////////////////////////////////////////
void SimpleFloaterActorComponent::OnAddedToActor(dtGame::GameActor& actor)
{
   dtGame::ActorComponent::OnAddedToActor(actor);

   RegisterForTicks();
}

///////////////////////////////////////////////////////////////////////////////
void SimpleFloaterActorComponent::OnRemovedFromActor(dtGame::GameActor& actor)
{
   dtGame::ActorComponent::OnRemovedFromActor(actor);

   UnregisterForTicks();
}

///////////////////////////////////////////////////////////////////////////////
void SimpleFloaterActorComponent::OnTickLocal(const dtGame::TickMessage& tickMessage)
{
   // Clamp our actor's height to the ocean height
   dtCore::Transformable* owner = dynamic_cast<dtCore::Transformable*>(GetOwner());
   if (owner != NULL)
   {
      dtCore::Transform currentTransform;
      owner->GetTransform(currentTransform);
      osg::Vec3 position = currentTransform.GetTranslation();
      position.z() = mpOceanHeightGetter->GetHeight(position.x(), position.y());
      currentTransform.SetTranslation(position);
      owner->SetTransform(currentTransform);
   }
}

////////////////////////////////////////////////////////////////////////////////
