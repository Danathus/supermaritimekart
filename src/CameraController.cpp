#include <CameraController.h>
#include <CameraMouseListener.h>
#include <SMKActorLibraryRegistry.h>

#include <BoatActors/BoatActor.h>

#include <dtCore/camera.h>
#include <dtCore/orbitmotionmodel.h>
#include <dtCore/transform.h>
#include <dtGame/basemessages.h>
#include <dtGame/messagetype.h>
#include <dtUtil/matrixutil.h>

#include <iostream>
#include <cassert>

////////////////////////////////////////////////////////////////////////////////
CameraController::CameraController(dtCore::Camera& cam, dtCore::Keyboard& keyboard, dtCore::Mouse& mouse)
: dtGame::GMComponent("CameraControl")
, mCamera(&cam)
, mMouse(&mouse)
, mMouseListener(new CameraMouseListener())
, mMotion(NULL)
, mCameraMode(CAM_FOLLOW)
{
   mMotion = new dtCore::OrbitMotionModel(&keyboard, mMouse.get());
}

////////////////////////////////////////////////////////////////////////////////
CameraController::~CameraController()
{
   mCamera = NULL;
   mMouse->RemoveMouseListener(mMouseListener.get());
   mMouse = NULL;
   mMouseListener = NULL;
   mBoat = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void CameraController::ProcessMessage(const dtGame::Message& message)
{
   if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_LOADED)
   {
      mBoat = GetBoatToControl();
      SetupControls();
   }
   else if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_UNLOAD_BEGIN)
   {
      mBoat = NULL;
      mMouse->RemoveMouseListener(mMouseListener.get());
      mMotion->SetTarget(NULL);
   }

   else if (message.GetMessageType() == dtGame::MessageType::TICK_LOCAL)
   {
      if (mBoat.valid())
      {
         const dtGame::TickMessage& mess = static_cast<const dtGame::TickMessage&>(message);

         UpdateCameraPosition(mess.GetDeltaRealTime());
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
BoatActor* CameraController::GetBoatToControl() const
{
   dtDAL::ActorProxy* boatActor;
   GetGameManager()->FindActorByType(*SMKActorLibraryRegistry::SMK_BOAT_ACTOR_TYPE, boatActor);
   if (boatActor)
   {
      return dynamic_cast<BoatActor*>(boatActor->GetActor());
   }
   else
   {
      return NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
void CameraController::UpdateCameraPosition(const double deltaTime)
{
   dtCore::Transform xform;
   mBoat->GetTransform(xform);

    switch (mCameraMode)
    {
    case CAM_ORBIT:
       UpdateOrbit(xform);
       break;

    case CAM_FOLLOW:
       UpdateFollow(xform, deltaTime);
       break;

    default:
       assert(NULL);
    }
}

////////////////////////////////////////////////////////////////////////////////
void CameraController::SetCameraMode(CameraModes mode)
{
   mCameraMode = mode;
}

////////////////////////////////////////////////////////////////////////////////
void CameraController::UpdateOrbit(const dtCore::Transform &xform)
{
   mMotion->SetFocalPoint(xform.GetTranslation());
}

////////////////////////////////////////////////////////////////////////////////
void CameraController::UpdateFollow(const dtCore::Transform &xform, const double deltaTime)
{
   osg::Matrix boatMat;
   xform.Get(boatMat); //get the boat's matrix

   //calc the heading of the boat (angle = arctan(y/x))
   osg::Vec3 hpr;
   hpr[0] = osg::RadiansToDegrees(-atan2(boatMat(1,0), boatMat(1,1)));

   //create a rotation matrix using the heading and the boat's translation
   osg::Matrix headingMat;
   dtUtil::MatrixUtil::PositionAndHprToMatrix(headingMat,
                                             dtUtil::MatrixUtil::GetRow3(boatMat, 3),
                                             hpr);

   const float kDistanceOffset = -40.f;
   const float kHeightOffset = 10.f;

   const osg::Vec3 offset(mMouseListener->GetSideOffset(),
                           kDistanceOffset/(float)mMouseListener->GetZoomFactor(),
                           kHeightOffset/(float)mMouseListener->GetZoomFactor());//camera offset

   const float scale = 0.9f; //scale to slow the camera movement down

   dtCore::Transform camXform;
   mCamera->GetTransform(camXform);
   osg::Matrix camMat;
   camXform.Get(camMat);
   const osg::Vec3 currXYZ = camXform.GetTranslation();
   const osg::Vec3 newXYZ = offset*headingMat;//calc new camera xyx

   osg::Vec3 diff(newXYZ-currXYZ);
   diff *= (scale);

   camXform.SetTranslation(currXYZ + diff);
   camXform.SetRotation(hpr[0], -10.f, 0.f);
   mCamera->SetTransform(camXform);
}

////////////////////////////////////////////////////////////////////////////////
void CameraController::CycleCameraModes()
{
   if (mCameraMode == CAM_FOLLOW)
   {
      mCameraMode = CAM_ORBIT;
   }
   else if (mCameraMode == CAM_ORBIT)
   {
      mCameraMode = CAM_FOLLOW;
   }
   else
   {
      assert(NULL);
   }

   SetupControls();
}

////////////////////////////////////////////////////////////////////////////////
void CameraController::SetupControls()
{
   if (mCameraMode == CAM_FOLLOW)
   {
      mMouseListener->Reset();
      mMouse->AddMouseListener(mMouseListener.get());

      mMotion->SetTarget(NULL);
   }
   else if (mCameraMode == CAM_ORBIT)
   {
      mMouseListener->Reset();
      mMouse->RemoveMouseListener(mMouseListener.get());

      mMotion->SetTarget(mCamera.get());
   }
}
