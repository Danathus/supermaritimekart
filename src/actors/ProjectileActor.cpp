#include <actors/ProjectileActor.h>
#include <actors/SMKBoatActor.h>
#include <messages/DamageMessage.h>
#include <messages/NetworkMessages.h>

#include <dtDAL/datatype.h>
#include <dtDAL/project.h>
#include <dtDAL/resourceactorproperty.h>
#include <dtDAL/resourcedescriptor.h>
#include <dtGame/basemessages.h>
#include <dtGame/deadreckoninghelper.h>
#include <dtGame/drpublishingactcomp.h>
#include <dtGame/gamemanager.h>
#include <dtGame/message.h>
#include <dtGame/messagetype.h>
#include <dtUtil/configproperties.h>

//////////////////////////////////////////////////////////////////////////
ProjectileActor::ProjectileActor(dtGame::GameActorProxy& proxy)
: dtActors::GameMeshActor(proxy)
, mDeadReckoningHelper(new dtGame::DeadReckoningHelper)
, mLifetime(10.0f)
, mLifeCounter(0.0f)
{
}

//////////////////////////////////////////////////////////////////////////
ProjectileActor::~ProjectileActor()
{
}

///////////////////////////////////////////////////////////////////////////////
void ProjectileActor::TickLocal(const dtGame::Message& msg)
{
   dtActors::GameMeshActor::TickLocal(msg);

   const dtGame::TickMessage& tickMessage = static_cast<const dtGame::TickMessage&>(msg);
   float dt = tickMessage.GetDeltaSimTime();

   mLifeCounter += dt;
   if (mLifeCounter > mLifetime)
   {
      if (mDamage.GetRadius() > 0.0f)
      {
         DetonateProjectile();
      }

      // Destroy ourselves
      SetCollisionDetection(false);
      GetGameActorProxy().GetGameManager()->DeleteActor(GetGameActorProxy());
   }
}

///////////////////////////////////////////////////////////////////////////////
void ProjectileActor::OnEnteredWorld()
{
   GetOSGNode()->setName(GetName());

   if (!IsRemote())
   {
      dtCore::Transform xform;
      GetTransform(xform);
      osg::Vec3 pos;
      xform.GetTranslation(pos);
      GetDeadReckoningHelper()->SetLastKnownTranslation(pos);
      osg::Vec3 rot;
      xform.GetRotation(rot);
      GetDeadReckoningHelper()->SetLastKnownRotation(rot);
   }

   //////////////////////////////
   // DR CONFIGURATION OPTIONS
   dtUtil::ConfigProperties& configParams = GetGameActorProxy().GetGameManager()->GetConfiguration();

   // Use Cubic Splines (vs the older Linear Blend) - If not specified, don't override default
   std::string useCubicSplines = "true"; //configParams.GetConfigPropertyValue("SimCore.DR.UseCubicSpline", "");
   if (useCubicSplines == "true" || useCubicSplines == "TRUE" || useCubicSplines == "1")
   {
      GetDeadReckoningHelper()->SetUseCubicSplineTransBlend(true);
   }
   else if (useCubicSplines == "false" || useCubicSplines == "FALSE" || useCubicSplines == "0")
   {
      GetDeadReckoningHelper()->SetUseCubicSplineTransBlend(false);
   }

   // Always Use Max Smoothing Time (as opposed to averaged update rate)
   // Some systems publish regularly, and some don't. If a system doesn't
   // publish updates like clockwork, then we use the average publish rate to blend. 
   std::string useFixedTimeBlends = ""; //configParams.GetConfigPropertyValue("SimCore.DR.UseFixedTimeBlends", "");
   if (useFixedTimeBlends == "true" || useFixedTimeBlends == "TRUE" || useFixedTimeBlends == "1")
   {
      GetDeadReckoningHelper()->SetUseFixedSmoothingTime(true);
   }
   else if (useFixedTimeBlends == "false" || useFixedTimeBlends == "FALSE" || useFixedTimeBlends == "0")
   {
      GetDeadReckoningHelper()->SetUseFixedSmoothingTime(false);
   }

   // The MaxTransSmoothingTime is usually set, but there are very obscure cases where it might
   // not have been set or not published for some reason. In that case, we need a non-zero value.
   // In practice, a vehicle that publishes will typically set these directly (for example, see 
   // BasePhysicsVehicleActor.SetMaxUpdateSendRate()). 
   // Previously, it set the smoothing time to 0.0 so that local actors would not smooth
   // their DR pos & rot to potentially make a cleaner comparison with less publishes.
   // Turning local smoothing on allows better vis & debugging of DR values (ex the DRGhostActor).
   if (GetDeadReckoningHelper()->GetMaxTranslationSmoothingTime() == 0.0f)
      GetDeadReckoningHelper()->SetMaxTranslationSmoothingTime(0.5f);
   if (GetDeadReckoningHelper()->GetMaxRotationSmoothingTime() == 0.0f)
      GetDeadReckoningHelper()->SetMaxRotationSmoothingTime(0.5f);
}

///////////////////////////////////////////////////////////////////////////////
void ProjectileActor::OnRemovedFromWorld()
{
}

///////////////////////////////////////////////////////////////////////////////
void ProjectileActor::BuildActorComponents()
{
   // DEAD RECKONING - ACT COMPONENT
   if (!HasComponent(dtGame::DeadReckoningHelper::TYPE)) // not added by a subclass
   {
      mDeadReckoningHelper = new dtGame::DeadReckoningHelper();

      // Flying was replaced with GroundClampType, and the default is already 'KeepAbove'
      //////mDeadReckoningHelper->SetFlying(false); // Causes ground clamping by default
      //mDeadReckoningHelper->SetGroundClampType(dtGame::GroundClampTypeEnum::KEEP_ABOVE);

      // attempt to fix the z-fighting on treads and wheels that are
      // very close to the ground. We move the vehicle up about 3-4 inches...
      mDeadReckoningHelper->SetGroundOffset(0.09f);

      AddComponent(*mDeadReckoningHelper);
   }
   else
   {
      GetComponent(mDeadReckoningHelper);
   }

   // DEAD RECKONING - PUBLISHING ACTOR COMPONENT
   if (!HasComponent(dtGame::DRPublishingActComp::TYPE)) // not added by a subclass
   {
      mDRPublishingActComp = new dtGame::DRPublishingActComp();
      AddComponent(*mDRPublishingActComp);  // Add AFTER the DRhelper.
   }
   else
   {
      GetComponent(mDRPublishingActComp);
   }

   //mDeadReckoningHelper->SetDeadReckoningAlgorithm(DeadReckoningAlgorithm::NONE);
   mDeadReckoningHelper->SetDeadReckoningAlgorithm(dtGame::DeadReckoningAlgorithm::VELOCITY_AND_ACCELERATION);
}

///////////////////////////////////////////////////////////////////////////////
bool ProjectileActor::FilterContact(dContact* contact, Transformable* collider)
{
   if (!GetGameActorProxy().IsInSTAGE() && !IsRemote() && GetCollisionDetection())
   {
      // Set the damage's location to where we are at the time of impact
      mDamage.SetLocation(GetGameActorProxy().GetTranslation());

      // If our damage radius is greater than 0, then tell everyone we exploded
      if (mDamage.GetRadius() > 0.0f)
      {
         DetonateProjectile();
      }
      else
      {
         SMKBoatActor* boat = dynamic_cast<SMKBoatActor*>(collider);
         if (boat != NULL)
         {
            // Tell the boat we hit that we hit it both locally and remotely
            dtCore::RefPtr<SMK::DamageMessage> collisionMessage;
            GetGameActorProxy().GetGameManager()->GetMessageFactory().CreateMessage(SMK::SMKNetworkMessages::ACTION_BOAT_HIT, collisionMessage);
            collisionMessage->SetAboutActorId(boat->GetUniqueId());
            collisionMessage->SetDamage(GetDamage());
            GetGameActorProxy().GetGameManager()->SendNetworkMessage(*collisionMessage);
            GetGameActorProxy().GetGameManager()->SendMessage(*collisionMessage);
         }
      }
      // Destroy ourselves
      SetCollisionDetection(false);
      GetGameActorProxy().GetGameManager()->DeleteActor(GetGameActorProxy());

      return dtActors::GameMeshActor::FilterContact(contact, collider);
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////
void ProjectileActor::SetMeshResource(const std::string& name)
{
   // Get the static mesh property
   dtDAL::ResourceActorProperty* meshProperty =
      dynamic_cast<dtDAL::ResourceActorProperty*>(GetGameActorProxy().GetProperty("static mesh"));
   meshProperty->SetValue(dtDAL::ResourceDescriptor("StaticMeshes:" + name));
   SetCollisionBox();
   SetCollisionDetection(true);
}

///////////////////////////////////////////////////////////////////////////////
void ProjectileActor::DetonateProjectile()
{
   dtCore::RefPtr<SMK::DamageMessage> explosionMessage;
   GetGameActorProxy().GetGameManager()->GetMessageFactory().CreateMessage(SMK::SMKNetworkMessages::ACTION_PROJECTILE_EXPLODED, explosionMessage);
   explosionMessage->SetDamage(GetDamage());
   GetGameActorProxy().GetGameManager()->SendNetworkMessage(*explosionMessage);
   GetGameActorProxy().GetGameManager()->SendMessage(*explosionMessage);
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
ProjectileActorProxy::ProjectileActorProxy()
{

}

///////////////////////////////////////////////////////////////////////////////
ProjectileActorProxy::~ProjectileActorProxy()
{

}

////////////////////////////////////////////////////////////////////////////////
void ProjectileActorProxy::CreateActor()
{
   ProjectileActor* pActor = new ProjectileActor(*this);
   SetActor(*pActor);
}

////////////////////////////////////////////////////////////////////////////////
void ProjectileActorProxy::OnEnteredWorld()
{
   dtActors::GameMeshActorProxy::OnEnteredWorld();

   if (!IsRemote())
   {
      RegisterForMessages(dtGame::MessageType::TICK_LOCAL, dtGame::GameActorProxy::TICK_LOCAL_INVOKABLE);
   }
}

////////////////////////////////////////////////////////////////////////////////
void ProjectileActorProxy::OnRemovedFromWorld()
{
   //tell our BoatActor it's time to go
   static_cast<ProjectileActor*>(GetActor())->OnRemovedFromWorld();
}

///////////////////////////////////////////////////////////////////////////////
void ProjectileActorProxy::GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill)
{
   ProjectileActor *actor = NULL;
   GetActor(actor);

   if (!actor)
   {
      return;
   }

   // Add the properties for dead reckoning such as last known translation, etc...
   actor->GetDeadReckoningHelper()->GetPartialUpdateProperties(propNamesToFill);

   //propNamesToFill.push_back(dtDAL::TransformableActorProxy::PROPERTY_TRANSLATION);
   //propNamesToFill.push_back(dtDAL::TransformableActorProxy::PROPERTY_ROTATION);
}

////////////////////////////////////////////////////////////////////////////////
void ProjectileActorProxy::NotifyFullActorUpdate()
{
   // Remove the rot and trans from the full actor update.
   // If we send pos & rot out in an update, then that sometimes causes problems
   // on remote items. Network components usually pick up their data on tick local, which
   // sends a message to the DefaultmessageProcessorComponent. However, before that message
   // gets processed, the tick-remote gets picked up by the DeadReckoningComponent. Causes jumpiness.
   std::vector<dtDAL::ActorProperty* > allProperties;
   GetPropertyList(allProperties);

   std::vector<dtUtil::RefString> finalPropNameList;
   finalPropNameList.reserve(allProperties.size());

   for (size_t i = 0; i < allProperties.size(); ++i)
   {
      if (allProperties[i]->GetName() != dtDAL::TransformableActorProxy::PROPERTY_ROTATION &&
         allProperties[i]->GetName() != dtDAL::TransformableActorProxy::PROPERTY_TRANSLATION)
      {
         finalPropNameList.push_back(allProperties[i]->GetName());
      }
   }

   // Even though we are using the partialactorupdate method - it should be treated as a full
   NotifyPartialActorUpdate(finalPropNameList, false);
}

////////////////////////////////////////////////////////////////////////////////

