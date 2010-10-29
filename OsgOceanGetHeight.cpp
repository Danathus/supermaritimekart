#include "OsgOceanGetHeight.h"

////////////////////////////////////////////////////////////////////////////////
OsgOceanGetHeight::OsgOceanGetHeight(dtOcean::OceanActor& oceanActor):
mOceanActor(&oceanActor)
{
}

////////////////////////////////////////////////////////////////////////////////
OsgOceanGetHeight::~OsgOceanGetHeight(void)
{
}

////////////////////////////////////////////////////////////////////////////////
float OsgOceanGetHeight::GetHeight(float x, float y)
{
   return mOceanActor->GetOceanSurfaceHeightAt(x, y);
}
