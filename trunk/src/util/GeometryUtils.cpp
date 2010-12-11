#include <util/GeometryUtils.h>

#include <dtCore/transform.h>
#include <dtCore/transformable.h>

#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <osg/BoundingBox>
#include <osg/Geode>
#include <osg/PolygonMode>
#include <osg/Material>
#include <osg/LineWidth>

// debug
static bool sUseDisplayLists = true;

////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> GeomUtils::CreateLineStripGeometry(osg::Vec3Array& points, const osg::Vec4& color, float lineWidth)
{
   osg::ref_ptr<osg::Geode> geode = new osg::Geode();
   osg::ref_ptr<osg::Geometry> pointsGeom = new osg::Geometry();

   pointsGeom->setUseDisplayList(sUseDisplayLists);
   pointsGeom->setUseVertexBufferObjects(!sUseDisplayLists);

   // pass the created vertex array to the points geometry object.
   pointsGeom->setVertexArray(&points);

   // create the color of the geometry, one single for the whole geometry.
   // for consistency of design even one single color must added as an element
   // in a color array.
   osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
   colors->push_back(color);

   // pass the color array to points geometry, note the binding to tell the geometry
   // that only use one color for the whole object.
   pointsGeom->setColorArray(colors);
   pointsGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

   // set the normal in the same way color.
   osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
   normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
   pointsGeom->setNormalArray(normals);
   pointsGeom->setNormalBinding(osg::Geometry::BIND_OVERALL);

   pointsGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0 ,points.size()));

   geode->addDrawable(pointsGeom);

   osg::ref_ptr<osg::LineWidth> lw = new osg::LineWidth(lineWidth);
   geode->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON);

   return geode;
}

////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> GeomUtils::CreateCylinderStripGeometry(const osg::Vec3Array& points,
                                                               const osg::Vec4& color,
                                                               float radius,
                                                               bool roundEnds)
{
   osg::ref_ptr<osg::Group> group = new osg::Group();

   int count = (int)points.size();
   for (int index = 0; index < count; ++index)
   {
      osg::Vec3 point = points[index];

      dtCore::RefPtr<dtCore::Transformable> transformable = new dtCore::Transformable();
      osg::ref_ptr<osg::Geode> geode = new osg::Geode();
      group->addChild(transformable->GetOSGNode());

      dtCore::Transform transform;
      transform.SetTranslation(point);

      if (index < count - 1)
      {
         osg::Vec3 nextPoint = points[index+1];
         osg::Vec3 vec = nextPoint - point;
         osg::Quat quat;
         quat.makeRotate(osg::Vec3(0, 0, 1), vec);
         transform.SetRotation(quat);

         float height = (point - nextPoint).length();
         osg::Vec3 center = osg::Vec3(0, 0, height / 2);
         osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(center, radius, height);
         osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints();

         if (roundEnds || index > 0)
         {
            hints->setCreateBottom(false);
         }

         if (roundEnds || index < count - 2)
         {
            hints->setCreateTop(false);
         }

         hints->setDetailRatio(0.2f);
         osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(cylinder, hints);
         drawable->setColor(color);
         drawable->setUseDisplayList(sUseDisplayLists);
         drawable->setUseVertexBufferObjects(!sUseDisplayLists);
         geode->addDrawable(drawable);
      }

      if (roundEnds || (index > 0 && index < count - 1))
      {
         osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(osg::Vec3(0,0,0), radius);
         osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints();
         hints->setDetailRatio(0.2f);
         osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(sphere, hints);
         drawable->setColor(color);
         drawable->setUseDisplayList(sUseDisplayLists);
         drawable->setUseVertexBufferObjects(!sUseDisplayLists);
         geode->addDrawable(drawable);
      }

      transformable->SetTransform(transform);
      transformable->GetOSGNode()->asGroup()->addChild(geode);
   }

   return group;
}

////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> GeomUtils::CreateBoundingBoxGeometry(const osg::BoundingBox &bb,
                                                 const osg::Vec4& color)
{
   osg::Vec3 sizes(bb.xMax()-bb.xMin(),
                   bb.yMax()-bb.yMin(),
                   bb.zMax()-bb.zMin());

   return CreateBoxGeometry(bb.center(), sizes, color);
}

////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> GeomUtils::CreateBoxGeometry(const osg::Vec3& center,
                                                     const osg::Vec3& sizes,
                                                     const osg::Vec4& color)
{
   osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints();

   osg::ref_ptr<osg::Geode> geode = new osg::Geode();
   osg::ref_ptr<osg::Box> box = new osg::Box(center, sizes[0], sizes[1], sizes[2]);
   osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(box, hints);
   shape->setColor(color);
   shape->setUseDisplayList(sUseDisplayLists);
   shape->setUseVertexBufferObjects(!sUseDisplayLists);

   geode->addDrawable(shape);

   return geode;
}

////////////////////////////////////////////////////////////////////////////////
void GeomUtils::RenderAsWireframe(bool wireframe, osg::Node& node)
{
   if (wireframe)
   {
      osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode();
      pm->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
      node.getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::ON|osg::StateAttribute::PROTECTED);
      node.getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF|osg::StateAttribute::PROTECTED );
   }
   else
   {
      osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode();
      pm->setMode(osg::PolygonMode::FRONT,osg::PolygonMode::FILL);
      node.getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::ON);
      node.getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
   }
}

////////////////////////////////////////////////////////////////////////////////
void GeomUtils::RenderAsTransparent(osg::Node& node)
{
   node.getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::OVERRIDE |
                                                  osg::StateAttribute::PROTECTED | osg::StateAttribute::ON);
   node.getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
}

////////////////////////////////////////////////////////////////////////////////
void GeomUtils::RenderAsEmissiveOutline(osg::Node& node, const osg::Vec4& color)
{
   osg::StateSet* stateSet = node.getOrCreateStateSet();

   // We want the outline to be lines on both sides
   osg::PolygonMode* polyMode = new osg::PolygonMode;
   polyMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);

   // Set up a material to show a flat emissive color
   osg::Material* lineMaterial = new osg::Material;
   lineMaterial->setColorMode(osg::Material::OFF);
   lineMaterial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
   lineMaterial->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
   lineMaterial->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
   lineMaterial->setEmission(osg::Material::FRONT_AND_BACK, color);

   stateSet->setAttributeAndModes(polyMode, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
   stateSet->setAttributeAndModes(lineMaterial, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
   stateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
}

////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> GeomUtils::CreatePrimitiveGeometry(osg::Vec3Array& points, osg::PrimitiveSet::Mode type, const osg::Vec4& color /*= osg::Vec4(1.f,1.f,1.f,1.f)*/)
{
   osg::ref_ptr<osg::Geode> geode = new osg::Geode();
   osg::ref_ptr<osg::Geometry> polyGeom = new osg::Geometry();

   polyGeom->setVertexArray(&points);
   polyGeom->setUseDisplayList(sUseDisplayLists);
   polyGeom->setUseVertexBufferObjects(!sUseDisplayLists);

   osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
   colors->push_back(color);
   polyGeom->setColorArray(colors);
   polyGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

   osg::ref_ptr<osg::Vec3Array> shared_normals = new osg::Vec3Array;

   //TODO danger Will, danger!  Check for vector size and such.
   osg::Vec3 normal = (points[1]-points[0])^(points[2]-points[1]);
   normal.normalize();
   shared_normals->push_back(normal);

   polyGeom->setNormalArray(shared_normals.get());
   polyGeom->setNormalBinding(osg::Geometry::BIND_OVERALL);

   osg::ref_ptr<osg::DrawArrays> drawArrays = new osg::DrawArrays(type, 0, points.size());
   polyGeom->addPrimitiveSet(drawArrays);

   // add the points geometry to the geode.
   geode->addDrawable(polyGeom);

   return geode;
}

////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> GeomUtils::CreateBeamGeometry(bool isIBeam, osg::Vec3Array& points, const osg::Vec4& flangeColor, const osg::Vec4& webColor, float flangeWidth, float webWidth, osg::Vec3 webDirection)
{
   osg::Vec4 wireColor = osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f);

   // Create our geometry.
   osg::ref_ptr<osg::Geode> geode = new osg::Geode();

   int count = (int)points.size();
   for (int index = 0; index < count - 1; ++index)
   {
      osg::Vec3 point1 = points[index];
      osg::Vec3 point2 = points[index + 1];

      osg::Vec3 upVec = point2 - point1;
      upVec.normalize();
      osg::Vec3 rightVec = webDirection ^ upVec;

      osg::ref_ptr<osg::Vec3Array> beamPoints = new osg::Vec3Array();
      osg::ref_ptr<osg::Vec3Array> beamNormals = new osg::Vec3Array();
      osg::ref_ptr<osg::Vec4Array> beamColors = new osg::Vec4Array();

      osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

      // Make the Web geometry.
      beamPoints->push_back(point2);
      beamPoints->push_back(point2 + (webDirection * webWidth));
      beamPoints->push_back(point1 + (webDirection * webWidth));
      beamPoints->push_back(point1);
      beamNormals->push_back(rightVec);
      beamColors->push_back(webColor);

      // Make the Flange geometry.
      beamPoints->push_back(point1 + (webDirection * 0.1f) + (rightVec * flangeWidth * 0.5));
      beamPoints->push_back(point1 + (webDirection * 0.1f) - (rightVec * flangeWidth * 0.5));
      beamPoints->push_back(point2 + (webDirection * 0.1f) - (rightVec * flangeWidth * 0.5));
      beamPoints->push_back(point2 + (webDirection * 0.1f) + (rightVec * flangeWidth * 0.5));
      beamNormals->push_back(webDirection);
      beamColors->push_back(flangeColor);

      // Make the second Flange geometry if we are rendering an I beam.
      if (isIBeam)
      {
         beamPoints->push_back(point1 + (webDirection * (webWidth - 0.1f)) + (rightVec * flangeWidth * 0.5));
         beamPoints->push_back(point1 + (webDirection * (webWidth - 0.1f)) - (rightVec * flangeWidth * 0.5));
         beamPoints->push_back(point2 + (webDirection * (webWidth - 0.1f)) - (rightVec * flangeWidth * 0.5));
         beamPoints->push_back(point2 + (webDirection * (webWidth - 0.1f)) + (rightVec * flangeWidth * 0.5));
         beamNormals->push_back(webDirection * -1);
         beamColors->push_back(flangeColor);
      }

      // Bind our geometry data.
      geom->setVertexArray(beamPoints);

      geom->setColorArray(beamColors);
      geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

      geom->setNormalArray(beamNormals);
      geom->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

      geom->setUseDisplayList(sUseDisplayLists);
      geom->setUseVertexBufferObjects(!sUseDisplayLists);

      // Draw the Beam
      osg::ref_ptr<osg::DrawArrays> drawArrays = new osg::DrawArrays(osg::PrimitiveSet::POLYGON, 0, 4);
      geom->addPrimitiveSet(drawArrays);
      drawArrays = new osg::DrawArrays(osg::PrimitiveSet::POLYGON, 4, 4);
      geom->addPrimitiveSet(drawArrays);
      if (isIBeam)
      {
         drawArrays = new osg::DrawArrays(osg::PrimitiveSet::POLYGON, 8, 4);
         geom->addPrimitiveSet(drawArrays);
      }

      // Draw the outline.
      if (isIBeam)
      {
         beamNormals->push_back(rightVec);
         beamNormals->push_back(rightVec);
         beamNormals->push_back(webDirection);
         beamNormals->push_back(webDirection * -1);

         beamColors->push_back(wireColor);
         beamColors->push_back(wireColor);
         beamColors->push_back(wireColor);
         beamColors->push_back(wireColor);

         drawArrays = new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2);
         geom->addPrimitiveSet(drawArrays);
         drawArrays = new osg::DrawArrays(osg::PrimitiveSet::LINES, 2, 2);
         geom->addPrimitiveSet(drawArrays);
         drawArrays = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 4, 4);
         geom->addPrimitiveSet(drawArrays);
         drawArrays = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 8, 4);
         geom->addPrimitiveSet(drawArrays);
      }
      else
      {
         beamNormals->push_back(rightVec);
         beamNormals->push_back(rightVec);
         beamNormals->push_back(webDirection);

         beamColors->push_back(wireColor);
         beamColors->push_back(wireColor);
         beamColors->push_back(wireColor);

         drawArrays = new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 4);
         geom->addPrimitiveSet(drawArrays);
         drawArrays = new osg::DrawArrays(osg::PrimitiveSet::LINES, 1, 2);
         geom->addPrimitiveSet(drawArrays);
         drawArrays = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 4, 4);
         geom->addPrimitiveSet(drawArrays);
      }

      geode->addDrawable(geom);

      osg::ref_ptr<osg::LineWidth> lw = new osg::LineWidth(2);
      geode->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON);
   }

   return geode;
}

////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> GeomUtils::CreateSphere(const osg::Vec3& center, float radius, const osg::Vec4& color)
{
   osg::ref_ptr<osg::Geode> geode = new osg::Geode();

   osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(center, radius);
   osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints();
   hints->setDetailRatio(0.2f);
   osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(sphere, hints);
   drawable->setColor(color);
   drawable->setUseDisplayList(sUseDisplayLists);
   drawable->setUseVertexBufferObjects(!sUseDisplayLists);
   geode->addDrawable(drawable);
   return geode;
}

////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> GeomUtils::CreateCylinderGeometry(const osg::Vec3& center,
                                                          float radius,
                                                          float height,
                                                          const osg::Vec4& color)
{
   osg::ref_ptr<osg::Geode> geode = new osg::Geode();

   osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(center, radius, height);
   osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints();
   hints->setCreateTop(false);
   hints->setCreateBottom(false);
   hints->setDetailRatio(0.2f);
   osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(cylinder, hints);
   drawable->setColor(color);
   drawable->setUseDisplayList(sUseDisplayLists);
   drawable->setUseVertexBufferObjects(!sUseDisplayLists);
   geode->addDrawable(drawable);
   return geode;
}

////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> GeomUtils::CreateConeGeometry(const osg::Vec3& center,
                                                          float radius,
                                                          float height,
                                                          const osg::Vec4& color)
{
   osg::ref_ptr<osg::Geode> geode = new osg::Geode();

   osg::ref_ptr<osg::Cone> cone = new osg::Cone(center, radius, height);
   osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints();
   hints->setDetailRatio(0.2f);
   osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(cone, hints);
   drawable->setColor(color);
   drawable->setUseDisplayList(sUseDisplayLists);
   drawable->setUseVertexBufferObjects(!sUseDisplayLists);
   geode->addDrawable(drawable);
   return geode;
}

////////////////////////////////////////////////////////////////////////////////

