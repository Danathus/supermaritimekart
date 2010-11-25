#ifndef SMKUtilFunctions_h__
#define SMKUtilFunctions_h__

#include <util/SMKUtilExport.h>

#include <osg/Node>

namespace SMK
{
   SMK_UTIL_EXPORT osg::Matrix GetAbsoluteMatrix(osg::Node* beginNode, osg::Node* endNode);
}

#endif // SMKUtilFunctions_h__