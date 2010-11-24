#include <util/SMKUtilFunctions.h>

#include <osg/Transform>

///////////////////////////////////////////////////////////////////////////////
osg::Matrix SMK::GetAbsoluteMatrix(osg::Node* beginNode, osg::Node* endNode)
{
   osg::Matrix absoluteMatrix;
   if (beginNode != NULL && endNode != NULL)
   {
      osg::NodePathList nodePathList = endNode->getParentalNodePaths();
      for (size_t pathIndex = 0; pathIndex < nodePathList.size(); ++pathIndex)
      {
         // If beginNode is in this parental path, then this is the path we want
         for (size_t nodeIndex = 0; nodeIndex < nodePathList[pathIndex].size(); ++nodeIndex)
         {
            if (nodePathList[pathIndex][nodeIndex] == beginNode)
            {
               absoluteMatrix.set(osg::computeLocalToWorld(nodePathList[pathIndex]));
            }
         }
      }
   }
   return absoluteMatrix;
}

///////////////////////////////////////////////////////////////////////////////

