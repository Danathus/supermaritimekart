// main.cpp : defines the entry point for the console application.

#include "WaterSprint.h"
#include <dtCore/globals.h>

int main(int argc, char **argv)
{
   //set data search path to parent directory and delta3d/data
   dtCore::SetDataFilePathList( "..;" + dtCore::GetDeltaDataPathList() ); 

   std::string mapToLoad;

   if (argc >= 3)
   {
      if (std::string(argv[1]) == "--map")
      {
         mapToLoad = std::string(argv[2]);
      }
   }
	
   //Instantiate the application and look for the config file
   dtCore::RefPtr<WaterSprint> app = new WaterSprint(mapToLoad);
	
   app->Config(); //configuring the application
   app->Run(); // running the simulation loop
   
   return 0;

}

