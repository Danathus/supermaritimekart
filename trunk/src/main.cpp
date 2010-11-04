// main.cpp : defines the entry point for the console application.

#include <SuperMaritimeKart.h>

#include <dtCore/system.h>
#include <dtCore/globals.h>
#include <dtUtil/datapathutils.h>
#include <dtUtil/fileutils.h>
#include <dtUtil/stringutils.h>

#include <conio.h>


int main(int argc, char **argv)
{   
   if (dtUtil::FileUtils::GetInstance().FileExists("data/config.xml"))
   {
      std::string dataPath = dtUtil::GetDeltaDataPathList();
      dtUtil::SetDataFilePathList(dataPath + ";" + dtUtil::FileUtils::GetInstance().CurrentDirectory() + "/data");

      dtCore::RefPtr<dtABC::Application> app = new dtABC::Application("config.xml");
      app->Config();
      app->Run();
   }
   else
   {
      LOG_ALWAYS("Unable to find config file, working directory may need to be set.");

      // Give users a chance to see the console
      LOG_ALWAYS("Hit a key to exit...");
      while (!_kbhit()){}
   }   

   return 0;
}
