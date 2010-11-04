#ifndef SUPERMARITIMEKART_H
#define SUPERMARITIMEKART_H

#include <dtABC/application.h>

////////////////////////////////////////////////////////////////////////////////

namespace dtGame
{
   class GameManager;
}

////////////////////////////////////////////////////////////////////////////////

class SuperMaritimeKart : public dtABC::Application
{
public:
   SuperMaritimeKart(const std::string& configFilename = "config.xml");

   virtual void Config();

   virtual void PreFrame(const double deltaFrameTime);
   virtual void PostFrame(const double deltaFrameTime);

   virtual bool KeyPressed(const dtCore::Keyboard* keyboard, int kc);

protected:
   virtual ~SuperMaritimeKart();

private:
   dtCore::RefPtr<dtGame::GameManager> mGameManager;
};

////////////////////////////////////////////////////////////////////////////////

#endif // SUPERMARITIMEKART_H
