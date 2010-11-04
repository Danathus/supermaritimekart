#include <SuperMaritimeKart.h>
#include <SuperMaritimeKartMessenger.h>
#include <CameraController.h>

#include <dtGame/gamemanager.h>
#include <dtAudio/audiomanager.h>
#include <dtUtil/exception.h>


////////////////////////////////////////////////////////////////////////////////
SuperMaritimeKart::SuperMaritimeKart(const std::string& configFilename)
: Application(configFilename)
{
}

////////////////////////////////////////////////////////////////////////////////
SuperMaritimeKart::~SuperMaritimeKart()
{
   dtAudio::AudioManager::Destroy();
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::Config()
{
   Application::Config();

   dtAudio::AudioManager::Instantiate();
   GetCamera()->AddChild(dtAudio::AudioManager::GetListener());

   mGameManager = new dtGame::GameManager(*GetScene());
   mGameManager->SetApplication(*this);

   try
   {
      if (mGameManager->GetProjectContext().empty())
      {
         mGameManager->SetProjectContext("./data", true);
         mGameManager->ChangeMap("JustOcean");

         SuperMaritimeKartMessenger* appComponent = new SuperMaritimeKartMessenger(*this);
         CameraController* cameraComponent = new CameraController(*GetCamera(),*GetKeyboard(), *GetMouse());

         mGameManager->AddComponent(*appComponent);
         mGameManager->AddComponent(*cameraComponent);
      }
   }
   catch (const dtUtil::Exception& e)
   {
      e.LogException(dtUtil::Log::LOG_ERROR);
   }
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::PreFrame(const double deltaFrameTime)
{
   dtABC::Application::PreFrame(deltaFrameTime);
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::PostFrame(const double deltaFrameTime)
{
   dtABC::Application::PostFrame(deltaFrameTime);
}

////////////////////////////////////////////////////////////////////////////////
bool SuperMaritimeKart::KeyPressed(const dtCore::Keyboard* keyboard, int kc)
{
   return false;
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::OnMapLoaded()
{

}


////////////////////////////////////////////////////////////////////////////////

