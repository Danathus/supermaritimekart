#ifndef DeltaOceanGetHeight_h__
#define DeltaOceanGetHeight_h__

#include <util/SMKUtilExport.h>
#include <BoatActors/IGetHeight.h>

# include <dtCore/refptr.h>
#ifdef BUILD_WITH_DTOCEAN
# include <dtOcean/oceanactor.h>
#else
namespace dtOcean
{
   class OceanActor;
}
#endif

class SMK_UTIL_EXPORT DeltaOceanGetHeight : public IGetHeight
{
public:
   DeltaOceanGetHeight(dtOcean::OceanActor& oceanActor);

   virtual float GetHeight(float x, float y);

protected:
   virtual ~DeltaOceanGetHeight(void);

private:
#if BUILD_WITH_DTOCEAN
   dtCore::RefPtr<dtOcean::OceanActor> mOceanActor;
#endif
};

#endif // DeltaOceanGetHeight_h__
