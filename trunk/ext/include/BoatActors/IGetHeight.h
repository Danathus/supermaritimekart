#ifndef IGetHeight_h__
#define IGetHeight_h__

#include <BoatActors/ActorLibraryExport.h>

#include <osg/Referenced>

///interface used to get the height of something, given the supplied (x,y)
class BOAT_ACTORS_LIBRARY_EXPORT IGetHeight : public osg::Referenced
{
public:
   IGetHeight() {}; 

   virtual float GetHeight(float x, float y) = 0;

protected:
   ~IGetHeight() {};
};

#endif // IGetHeight_h__
