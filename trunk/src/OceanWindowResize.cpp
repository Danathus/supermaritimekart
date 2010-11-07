#include <OceanWindowResize.h>

#ifdef BUILD_WITH_DTOCEAN
# include <dtOcean/oceanactor.h>
#endif

////////////////////////////////////////////////////////////////////////////////
OceanWindowResize::OceanWindowResize(dtOcean::OceanActor* ocean)
#ifdef BUILD_WITH_DTOCEAN
   : mOcean(ocean)
#endif
{

}

////////////////////////////////////////////////////////////////////////////////
OceanWindowResize::~OceanWindowResize()
{
#ifdef BUILD_WITH_DTOCEAN
   mOcean = NULL;
#endif
}

////////////////////////////////////////////////////////////////////////////////
void OceanWindowResize::operator()(const dtCore::DeltaWin& win, int x, int y,
                                   int width, int height)
{
#ifdef BUILD_WITH_DTOCEAN
   if (mOcean.valid())
   {
      mOcean->SetWindowSize(osg::Vec2s(width, height));
   }
#endif
}

////////////////////////////////////////////////////////////////////////////////
void OceanWindowResize::SetOceanActor(dtOcean::OceanActor* ocean)
{
#ifdef BUILD_WITH_DTOCEAN
   mOcean = ocean;
#endif
}

////////////////////////////////////////////////////////////////////////////////
