#include <actors/ProjectileActor.h>

#include <dtDAL/datatype.h>
#include <dtDAL/project.h>
#include <dtDAL/resourceactorproperty.h>
#include <dtDAL/resourcedescriptor.h>
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

   dtCore::Transform currentTransform;
   GetTransform(currentTransform);
   currentTransform.SetRotation(currentTransform.GetRotation() + osg::Vec3(1.0, 0.0, 0.0));
   SetTransform(currentTransform);
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
void ProjectileActor::SetMeshResource(const std::string& name, const std::string& file)
{
   dtDAL::ResourceActorProperty* meshProperty =
      dynamic_cast<dtDAL::ResourceActorProperty*>(GetGameActorProxy().GetProperty("static mesh"));
   meshProperty->SetValue(dtDAL::Project::GetInstance().AddResource(name, file,
      "StaticMeshes", dtDAL::DataType::STATIC_MESH));
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
   propNamesToFill.push_back(dtDAL::TransformableActorProxy::PROPERTY_TRANSLATION);
   propNamesToFill.push_back(dtDAL::TransformableActorProxy::PROPERTY_ROTATION);
}

///////////////////////////////////////////////////////////////////////////////
void ProjectileActorProxy::OnRotation(const osg::Vec3 &oldValue, const osg::Vec3 &newValue)
{
   int i = 0;
   --i;
}

///////////////////////////////////////////////////////////////////////////////
void ProjectileActorProxy::OnTranslation(const osg::Vec3 &oldValue, const osg::Vec3 &newValue)
{
   int i = 0;
   --i;
}

////////////////////////////////////////////////////////////////////////////////

