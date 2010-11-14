#include <PickUpItemHandle.h>
#include <dtDAL/propertymacros.h> //for dtDAL::PropertyRegHelper
#include <dtCore/scene.h>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <osg/Geode>

using namespace SMK;


DT_IMPLEMENT_ACCESSOR(PickUpItemBaseProxy, std::string, Type);
DT_IMPLEMENT_ACCESSOR(PickUpItemBaseProxy, dtDAL::ResourceDescriptor, IconImage);

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
   
   //wtf is this?
   typedef dtDAL::PropertyRegHelper<dtDAL::PropertyContainer&, PickUpItemBaseProxy> RegHelperType;
   RegHelperType regHelper(*this, this, "PickUp Properties");

   //add in additional properties
   DT_REGISTER_PROPERTY(Type, "Defines the type of PickUp; matches what's registered in the PickUp Factor", RegHelperType, regHelper);
   DT_REGISTER_RESOURCE_PROPERTY(dtDAL::DataType::TEXTURE, IconImage, "Icon Image", "Used on the rendered geometry", RegHelperType, regHelper);
}


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
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
   //if (!mIsInitialized)
   //{
   //   Initialize(1.0f, 6.0f, 6.0f, 6.0f);
   //   mIsInitialized = true;
   //}
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
   
   //TODO apply the texture; "IconImage" in the parent proxy

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

