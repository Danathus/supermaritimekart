#ifndef DELTA_WaterSprint
#define DELTA_WaterSprint

// WaterSprint.cpp : declares the interface of the application

#include <dtABC/application.h>
#include <dtCore/refptr.h>

namespace dtGame
{
   class GameManager;
}

class BoatFloaterComponent;
class InitComponent;
class CameraControl;
class BoatDemoMode;

class WaterSprint : public dtABC::Application
{
   public:
      WaterSprint(const std::string& mapToLoad);
 
      virtual void Config();

      virtual bool KeyPressed(const dtCore::Keyboard* keyboard, int kc);
      virtual bool KeyReleased(const dtCore::Keyboard* keyboard, int kc);

      virtual void PreFrame(const double deltaFrameTime);
      virtual void PostFrame(const double deltaFrameTime);

   protected:
     virtual ~WaterSprint();

   private:
      void SetContextAndLoadMap(const std::string& mapName);
      void LoadNextMap(const std::string& currentMap);

      std::string mMapToLoad;
      static const std::string kProjectContext;

      dtCore::RefPtr<dtGame::GameManager> mGameManager;
      dtCore::RefPtr<BoatFloaterComponent> mBoatFloaterComponent;
      dtCore::RefPtr<InitComponent> mInitComponent;
      dtCore::RefPtr<CameraControl> mCameraComponent;
      dtCore::RefPtr<BoatDemoMode> mBoatDemoMode;

};

#endif // DELTA_WaterSprint
