#ifndef cameracontrol_h__
#define cameracontrol_h__

#include <dtGame/gmcomponent.h>


namespace dtCore
{
   class Camera;
   class Keyboard;
   class Mouse;
   class OrbitMotionModel;
}

class BoatActor;
class CameraMouseListener;

class CameraController : public dtGame::GMComponent
{
public:
   CameraController(dtCore::Camera& cam, dtCore::Keyboard& keyboard, dtCore::Mouse& mouse);

   virtual void ProcessMessage(const dtGame::Message& message);

   enum CameraModes
   {
      CAM_FOLLOW, //follow behind boat
      CAM_ORBIT, //orbit around boat
      CAM_LAST
   };

   void SetCameraMode(CameraModes mode);

   void CycleCameraModes();

protected:
   virtual ~CameraController();
	
private:
   BoatActor* GetBoatToControl() const;
   void UpdateOrbit(const dtCore::Transform &xform);
   void UpdateFollow(const dtCore::Transform &xform,const double deltaTime);
   void UpdateCameraPosition(const double deltaTime);
   void SetupControls();
   dtCore::RefPtr<dtCore::Camera> mCamera;
   dtCore::RefPtr<dtCore::Mouse> mMouse;
   dtCore::RefPtr<CameraMouseListener> mMouseListener;
   dtCore::RefPtr<BoatActor> mBoat;
   dtCore::RefPtr<dtCore::OrbitMotionModel> mMotion;
   CameraModes mCameraMode; ///<current camera mode
};
#endif //cameracontrol_h__
