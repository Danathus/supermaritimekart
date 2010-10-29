// WaterSprint.cpp : defines the implementation of the application

#include "WaterSprint.h"
#include <osgDB/FileUtils>
#include <dtCore/deltawin.h>
#include <dtCore/scene.h>
#include <dtDAL/project.h>
#include <dtGame/gamemanager.h>
#include <dtAudio/audiomanager.h>
#include "BoatFloaterComponent.h"
#include "initcomponent.h"
#include "CameraControl.h"
#include "CollisionCallback.h"
#include "BoatDemoMode.h"

using namespace dtCore;
using namespace dtABC;

//////////////////////////////////////////////////////////////////////////
static const osg::Vec3 TARGET_XYZ(0, 6, 5);
const std::string WaterSprint::kProjectContext = "ProjectContext";

//////////////////////////////////////////////////////////////////////////
WaterSprint::WaterSprint(const std::string& mapToLoad = "")
: Application("config.xml"),
mMapToLoad(mapToLoad),
mBoatFloaterComponent(new BoatFloaterComponent()),
mCameraComponent(NULL),
mBoatDemoMode(new BoatDemoMode())
{
}


WaterSprint::~WaterSprint()
{
   dtAudio::AudioManager::Destroy();

   GetScene()->SetUserCollisionCallback(NULL, NULL);

   mGameManager->RemoveComponent(*mBoatFloaterComponent);
   mGameManager->RemoveComponent(*mInitComponent);
   mGameManager->RemoveComponent(*mCameraComponent);
   mGameManager->RemoveComponent(*mBoatDemoMode);
   mGameManager->Shutdown();
}
   
void WaterSprint::Config()
{   
   Application::Config();

   GetWindow()->SetWindowTitle("WaterSprint");   

   dtAudio::AudioManager::Instantiate();
   GetCamera()->AddChild(dtAudio::AudioManager::GetListener());

   GetScene()->SetUserCollisionCallback(nearCallback, GetScene()->GetPhysicsController());

   mInitComponent = new InitComponent(*GetKeyboard(), *GetWindow());
   mCameraComponent = new CameraControl(*GetCamera(),*GetKeyboard(), *GetMouse());

   mGameManager = new dtGame::GameManager(*GetScene());
   mGameManager->SetApplication(*this);

   mGameManager->AddComponent(*mBoatFloaterComponent, dtGame::GameManager::ComponentPriority::NORMAL);
   mGameManager->AddComponent(*mInitComponent, dtGame::GameManager::ComponentPriority::NORMAL);
   mGameManager->AddComponent(*mCameraComponent, dtGame::GameManager::ComponentPriority::NORMAL);

   GetScene()->SetPhysicsStepSize(0.001);
   GetScene()->SetGravity(0.f, 0.f, -18.f);

   if (mMapToLoad.empty()) //could be defined on command line
   {
      mMapToLoad = this->GetConfigPropertyValue("MapToLoad", "map1");
   }

   SetContextAndLoadMap(mMapToLoad);
}

bool WaterSprint::KeyPressed(const dtCore::Keyboard* keyboard, int kc)
{
   bool handled(true);
   switch(kc)
   {
      case osgGA::GUIEventAdapter::KEY_Escape:
         Quit();
         break;

      case '~':
         SetNextStatisticsType();
         break;


      default:
         handled = false;
         break;
   }
   
   return handled;
}

////////////////////////////////////////////////////////////////////////////////
bool WaterSprint::KeyReleased(const dtCore::Keyboard* keyboard, int kc)
{
   bool handled(true);
   switch(kc)
   {

   case osgGA::GUIEventAdapter::KEY_F1:
      SetContextAndLoadMap(mMapToLoad);
      break;

   case osgGA::GUIEventAdapter::KEY_F2:
      LoadNextMap(mMapToLoad);
      break;

   case 'c':
   case 'C':
      mCameraComponent->CycleCameraModes();
      break;

   case 'r':
   case 'R':
      mInitComponent->Reset();
      break;

   case osgGA::GUIEventAdapter::KEY_F10:
      GetCamera()->TakeScreenShot("WaterSprint");
      break;

   case osgGA::GUIEventAdapter::KEY_Return:
      if (mBoatDemoMode->GetIsEnabled())
      {
         mBoatDemoMode->SetEnabled(false);
         mGameManager->RemoveComponent(*mBoatDemoMode);
      }
      else
      {
         mBoatDemoMode->SetEnabled(true);
         mGameManager->AddComponent(*mBoatDemoMode, dtGame::GameManager::ComponentPriority::NORMAL);
      }
      break;

   case '=':
   case '+':
      mInitComponent->AdjustOceanHeight(0.1f);
      break;

   case '-':
   case '_':
      mInitComponent->AdjustOceanHeight(-0.1f);
      break;

   default:
      handled = false;
      break;
   }

   return handled;

}

void WaterSprint::PreFrame(const double deltaFrameTime)
{
   //called prior to rendering of frame, do you scene updates here
}

void WaterSprint::PostFrame(const double deltaFrameTime)
{
   //called after frame has been rendering, collect information about results from scene interaction here
}

////////////////////////////////////////////////////////////////////////////////
void WaterSprint::SetContextAndLoadMap(const std::string& mapName)
{
   try
   {
      if (mGameManager->GetProjectContext().empty())
      {
         mGameManager->SetProjectContext(kProjectContext, true);

      }

      LOG_ALWAYS("loading Map: " +  mapName);
      mGameManager->ChangeMap(mapName);
      
      mMapToLoad = mapName;
   }
   catch (const dtUtil::Exception& e)
   {
      e.LogException(dtUtil::Log::LOG_ERROR);   	
   }
}


////////////////////////////////////////////////////////////////////////////////
void WaterSprint::LoadNextMap(const std::string& currentMap)
{
   if (mGameManager->GetProjectContext().empty())
   {
      return;
   }

   const std::set<std::string> maps = dtDAL::Project::GetInstance().GetMapNames();
   if (maps.empty())
   {
      return;
   }

   std::set<std::string>::const_iterator itr = maps.find(currentMap);
   if (itr == maps.end())
   {
      return; //current map not found?
   }

   ++itr;
   
   if (itr == maps.end())
   {
     SetContextAndLoadMap(*(maps.begin())); //go to first
   }
   else
   {
     SetContextAndLoadMap(*(itr));
   }
}
