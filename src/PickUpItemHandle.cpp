#include <PickUpItemHandle.h>
#include <SMKBoatActor.h>

#include <dtDAL/actortype.h>
#include <dtDAL/propertymacros.h> //for dtDAL::PropertyRegHelper
#include <dtCore/scene.h>
#include <dtDAL/project.h>//for loading resources

#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <osg/Geode>
using namespace SMK;


////////////////////////////////////////////////////////////////////////////////
PickUpItemBaseProxy::PickUpItemBaseProxy()
: FloatingActorProxy()
{
   SetClassName("PickUpItemBase");
}

////////////////////////////////////////////////////////////////////////////////
PickUpItemBaseProxy::~PickUpItemBaseProxy()
{

}

////////////////////////////////////////////////////////////////////////////////
void PickUpItemBaseProxy::CreateActor()
{
   PickUpItemHandle* pickup = new PickUpItemHandle(*this);
   SetActor(*pickup);
}

////////////////////////////////////////////////////////////////////////////////
void PickUpItemBaseProxy::BuildPropertyMap()
{
   dtGame::GameActorProxy::BuildPropertyMap();
   
   //some inherited API to ensure we default to a 1x1x1 collision box
   this->SetCollisionBoxDims(osg::Vec3(1.f, 1.f, 1.f));
   this->SetCollisionType(dtCore::Transformable::CollisionGeomType::CUBE);
   
   //Properties reside in the Proxy, but the get/set functors reside on the actor itself
   typedef dtDAL::PropertyRegHelper<dtDAL::PropertyContainer&, PickUpItemHandle> RegHelperType;
   PickUpItemHandle* actor = static_cast<PickUpItemHandle*>(GetActor());
   RegHelperType regHelper(*this, actor, "PickUp Properties");

   //add in additional properties
   DT_REGISTER_PROPERTY(Type, "Defines the type of PickUp; matches what's registered in the PickUp Factor", RegHelperType, regHelper);
   DT_REGISTER_RESOURCE_PROPERTY(dtDAL::DataType::TEXTURE, IconImage, "Icon Image", "Used on the rendered geometry", RegHelperType, regHelper);
}


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

DT_IMPLEMENT_ACCESSOR(PickUpItemHandle, std::string, Type);
DT_IMPLEMENT_ACCESSOR_GETTER(PickUpItemHandle, dtDAL::ResourceDescriptor, IconImage);

PickUpItemHandle::PickUpItemHandle(FloatingActorProxy& proxy)
: FloatingActor(proxy)
{
   SetName("pick up item");   
   osg::Node* root = GetOSGNode();
   if (root)
   {
      if (root->asGroup())
      {
         osg::ref_ptr<osg::Node> geometry = CreateGeometry();
         root->asGroup()->addChild(geometry);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
PickUpItemHandle::~PickUpItemHandle()
{

}

////////////////////////////////////////////////////////////////////////////////
void PickUpItemHandle::TickLocal(const dtGame::Message& msg)
{
   FloatingActor::TickLocal(msg);
}


//////////////////////////////////////////////////////////////////////////
void PickUpItemHandle::SetIconImage(dtUtil::TypeTraits<dtDAL::ResourceDescriptor>::param_type value)
{
   mIconImage = value;

   if (mIconImage.IsEmpty())
   {
      GetOSGNode()->getOrCreateStateSet()->removeTextureMode(0, osg::StateAttribute::ON);
      return;
   }

   const std::string textureFile = dtDAL::Project::GetInstance().GetResourcePath(mIconImage);

   if (!textureFile.empty())
   {
      osg::Image* image = osgDB::readImageFile(textureFile);
      if (image)
      {
         osg::Texture2D* texture = new osg::Texture2D(image);
         GetOSGNode()->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> PickUpItemHandle::CreateGeometry()
{
   const osg::Vec3 center(0.f, 0.f, 0.f);
   const  osg::Vec4 color(1.f, 1.f, 0.f, 1.f);

   osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints();
   hints->setCreateTextureCoords(true);

   osg::ref_ptr<osg::Geode> geode = new osg::Geode();
   
   osg::ref_ptr<osg::Box> box = new osg::Box(center, 1.f);

   osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(box, hints);
   
   

   shape->setColor(color);
   geode->addDrawable(shape);

   return geode;
}

////////////////////////////////////////////////////////////////////////////////
void PickUpItemHandle::Initialize()
{
   SetupBuoyancy(0.1f, 10.0f, 10.0f, 100.0f);

   AddSender(GetSceneParent());
}

