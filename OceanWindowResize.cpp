#include "OceanWindowResize.h"
#include <dtOcean/oceanactor.h>

////////////////////////////////////////////////////////////////////////////////
OceanWindowResize::OceanWindowResize(dtOcean::OceanActor* ocean):
mOcean(ocean)
{

}

////////////////////////////////////////////////////////////////////////////////
OceanWindowResize::~OceanWindowResize()
{
   mOcean = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void OceanWindowResize::operator()(const dtCore::DeltaWin& win, int x, int y,
                                   int width, int height)
{
   if (mOcean.valid())
   {
      mOcean->SetWindowSize(osg::Vec2s(width, height));
   }
}

////////////////////////////////////////////////////////////////////////////////
void OceanWindowResize::SetOceanActor(dtOcean::OceanActor* ocean)
{
   mOcean = ocean;
}
