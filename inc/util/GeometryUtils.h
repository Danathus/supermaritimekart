#ifndef GEOMETRYUTILS_h__
#define GEOMETRYUTILS_h__

#include <util/SMKUtilExport.h>

#include <vector>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/BoundingBox>
#include <osg/ref_ptr>
#include <osg/Node>
#include <osg/PrimitiveSet>

namespace osg
{
   class Geometry;
   class Geode;
}

namespace GeomUtils
{
   ///Render the supplied node as wireframe or not
   SMK_UTIL_EXPORT void RenderAsWireframe(bool wireframe, osg::Node& node);

   ///Render the supplied node with transparency (the Geometry should have alpha color on it)
   SMK_UTIL_EXPORT void RenderAsTransparent(osg::Node& node);

   ///Render the supplied node as a colored wireframe
   SMK_UTIL_EXPORT void RenderAsEmissiveOutline(osg::Node& node, const osg::Vec4& color);

   SMK_UTIL_EXPORT typedef std::vector<osg::Vec3> PointVec;
   SMK_UTIL_EXPORT osg::ref_ptr<osg::Node> CreateLineStripGeometry(osg::Vec3Array& points, const osg::Vec4& color = osg::Vec4(1.f,1.f,1.f,1.f), float lineWidth = 1.f);

   SMK_UTIL_EXPORT osg::ref_ptr<osg::Node> CreateCylinderStripGeometry(const osg::Vec3Array& points,
                                                       const osg::Vec4& color = osg::Vec4(1.f,1.f,1.f,1.f),
                                                       float radius = 1.f, bool roundEnds = false);

   ///create a renderable Geode based on the supplied BoundingBox
   SMK_UTIL_EXPORT osg::ref_ptr<osg::Node> CreateBoundingBoxGeometry(const osg::BoundingBox &bb,
                                                     const osg::Vec4& color = osg::Vec4(1.f,1.f,1.f,1.f));

   ///Create a renderable box, given the center and x,y,z lengths
   SMK_UTIL_EXPORT osg::ref_ptr<osg::Node> CreateBoxGeometry(const osg::Vec3& center, const osg::Vec3& sizes,
                                             const osg::Vec4& color = osg::Vec4(1.f,1.f,1.f,1.f));

   SMK_UTIL_EXPORT osg::ref_ptr<osg::Node> CreatePrimitiveGeometry(osg::Vec3Array& points, osg::PrimitiveSet::Mode type,
                                                   const osg::Vec4& color = osg::Vec4(1.f,1.f,1.f,1.f));

   ///Create a renderable T or I beam.
   SMK_UTIL_EXPORT osg::ref_ptr<osg::Node> CreateBeamGeometry(bool isIBeam, osg::Vec3Array& points, const osg::Vec4& flangeColor, const osg::Vec4& webColor, float flangeWidth, float webWidth, osg::Vec3 webDirection);

   ///Create a sphere at the given center, with the given radius.
   SMK_UTIL_EXPORT osg::ref_ptr<osg::Node> CreateSphere(const osg::Vec3& center,
                                        float radius,
                                        const osg::Vec4& color = osg::Vec4(1.f, 0.f, 0.f, 1.f));

   ///Create a cylinder, with no end cap geometry
   SMK_UTIL_EXPORT osg::ref_ptr<osg::Node> CreateCylinderGeometry(const osg::Vec3& center,
                                                  float radius,
                                                  float height,
                                                  const osg::Vec4& color = osg::Vec4(1.f, 1.f, 1.f, 1.f));

   ///Create a cone geometry
   SMK_UTIL_EXPORT osg::ref_ptr<osg::Node> CreateConeGeometry(const osg::Vec3& center,
                                                  float radius,
                                                  float height,
                                                  const osg::Vec4& color = osg::Vec4(1.f, 1.f, 1.f, 1.f));
}

#endif // GEOMETRYUTILS_h__
