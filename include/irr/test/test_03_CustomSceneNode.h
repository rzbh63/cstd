/** Example 003 Custom SceneNode
This Tutorial is more advanced than the previous ones.
If you are currently just playing around with the Irrlicht
engine, you may want to look at other examples first.
This tutorials shows how to create a custom scene node and
how to use it in the engine. A custom scene node is needed
if you want to implement a render technique the Irrlicht
Engine currently does not support. For example, you can write
an indoor portal based renderer or an advanced terrain scene
node with it. By creating custom scene nodes, you can
easily extend the Irrlicht Engine and adapt it to your own
needs.
I will keep the tutorial simple: Keep everything very
short, everything in one .cpp file, and I'll use the engine
here as in all other tutorials.
To start, I include the header files, use the irr namespace,
and tell the linker to link with the .lib file.
*/
//#include "driverChoice.h"
#ifdef _MSC_VER
//#pragma comment(lib, "Irrlicht.lib")
#endif
/*
Here comes the more sophisticated part of this tutorial:
The class of our very own custom scene node. To keep it simple,
our scene node will not be an indoor portal renderer nor a terrain
scene node, but a simple tetraeder, a 3d object consisting of 4
connected vertices, which only draws itself and does nothing more.
Note that this scenario does not require a custom scene node in Irrlicht.
Instead one would create a mesh from the geometry and pass it to a
IMeshSceneNode. This example just illustrates creation of a custom
scene node in a very simple setting.
To let our scene node be able to be inserted into the Irrlicht
Engine scene, the class we create needs to be derived from the
ISceneNode class and has to override some methods.
*/
class CSampleSceneNode : public ISceneNode
{
  /*
  First, we declare some member variables:
  The bounding box, 4 vertices, and the material of the tetraeder.
  */
  F3AABBOX Box;
  S3DVertex Vertices[4];
  SMaterial Material;
  /*
  The parameters of the constructor specify the parent of the scene node,
  a pointer to the scene manager, and an id of the scene node.
  In the constructor we call the parent class' constructor,
  set some properties of the material, and
  create the 4 vertices of the tetraeder we will draw later.
  */
public:
  CSampleSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id)
    : ISceneNode(parent, mgr, id) {
    Material.Wireframe = false;
    Material.Lighting = false;
    Vertices[0] = S3DVertex(0, 0, 10, 1, 1, 0,
        _ARGB(255, 0, 255, 255), 0, 1);
    Vertices[1] = S3DVertex(10, 0, -10, 1, 0, 0,
        _ARGB(255, 255, 0, 255), 1, 1);
    Vertices[2] = S3DVertex(0, 20, 0, 0, 1, 1,
        _ARGB(255, 255, 255, 0), 1, 0);
    Vertices[3] = S3DVertex(-10, 0, -10, 0, 0, 1,
        _ARGB(255, 0, 255, 0), 0, 0);
    /*
    The Irrlicht Engine needs to know the bounding box of a scene node.
    It will use it for automatic culling and other things. Hence, we
    need to create a bounding box from the 4 vertices we use.
    If you do not want the engine to use the box for automatic culling,
    and/or don't want to create the box, you could also call
    ISceneNode::setAutomaticCulling() with EAC_OFF.
    */
    Box.reset(Vertices[0].Pos);
    for (s32 i = 1; i < 4; ++i) {
      Box.addInternalPoint(Vertices[i].Pos);
    }
  }
  /*
  Before it is drawn, the ISceneNode::OnRegisterSceneNode()
  method of every scene node in the scene is called by the scene manager.
  If the scene node wishes to draw itself, it may register itself in the
  scene manager to be drawn. This is necessary to tell the scene manager
  when it should call ISceneNode::render(). For
  example, normal scene nodes render their content one after another,
  while stencil buffer shadows would like to be drawn after all other
  scene nodes. And camera or light scene nodes need to be rendered before
  all other scene nodes (if at all). So here we simply register the
  scene node to render normally. If we would like to let it be rendered
  like cameras or light, we would have to call
  SceneManager->registerNodeForRendering(this, SNRT_LIGHT_AND_CAMERA);
  After this, we call the actual
  ISceneNode::OnRegisterSceneNode() method of the base class,
  which simply lets also all the child scene nodes of this node register
  themselves.
  */
  void OnRegisterSceneNode() {
    if (IsVisible) {
      SceneManager->registerNodeForRendering(this);
    }
    ISceneNode::OnRegisterSceneNode();
  }
  /*
  In the render() method most of the interesting stuff happens: The
  Scene node renders itself. We override this method and draw the
  tetraeder.
  */
  void render() {
    u16 indices[] = { 0, 2, 3, 2, 1, 3, 1, 0, 3, 2, 0, 1  };
    IVideoDriver* driver = SceneManager->getVideoDriver();
    driver->setMaterial(Material);
    driver->setTransform(ETS_WORLD, AbsoluteTransformation);
    driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 4, EVT_STANDARD, EPT_TRIANGLES, EIT_16BIT);
  }
  /*
  And finally we create three small additional methods.
  ISceneNode::getBoundingBox() returns the bounding box of
  this scene node, ISceneNode::getMaterialCount() returns the
  amount of materials in this scene node (our tetraeder only has one
  material), and ISceneNode::getMaterial() returns the
  material at an index. Because we have only one material here, we can
  return the only one material, assuming that no one ever calls
  getMaterial() with an index greater than 0.
  */
  const F3AABBOX& getBoundingBox() const {
    return Box;
  }
  u32 getMaterialCount() const {
    return 1;
  }
  SMaterial& getMaterial(u32 i) {
    return Material;
  }
};
/*
That's it. The Scene node is done. Now we simply have to start
the engine, create the scene node and a camera, and look at the result.
*/
int main()
{
  // ask user for driver
  E_DRIVER_TYPE driverType = driverChoiceConsole();
  if (driverType == EDT_COUNT) {
    return 1;
  }
  // create device
  IrrlichtDevice* device = createDevice(driverType,
      dimension2du(640, 480), 16, false);
  if (device == 0) {
    return 1;  // could not create selected driver.
  }
  // create engine and camera
  device->setWindowCaption("Custom Scene Node - Irrlicht Engine Demo");
  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr = device->getSceneManager();
  smgr->addCameraSceneNode(0, f3VECTOR(0, -40, 0), f3VECTOR(0, 0, 0));
  /*
  Create our scene node. I don't check the result of calling new, as it
  should throw an exception rather than returning 0 on failure. Because
  the new node will create itself with a reference count of 1, and then
  will have another reference added by its parent scene node when it is
  added to the scene, I need to drop my reference to it. Best practice is
  to drop it only *after* I have finished using it, regardless of what
  the reference count of the object is after creation.
  */
  CSampleSceneNode* myNode =
      new CSampleSceneNode(smgr->getRootSceneNode(), smgr, 666);
  /*
  To animate something in this boring scene consisting only of one
  tetraeder, and to show that you now can use your scene node like any
  other scene node in the engine, we add an animator to the scene node,
  which rotates the node a little bit.
  ISceneManager::createRotationAnimator() could return 0, so
  should be checked.
  */
  ISceneNodeAnimator* anim =
      smgr->createRotationAnimator(f3VECTOR(0.8f, 0, 0.8f));
  if (anim) {
    myNode->addAnimator(anim);
    /*
    I'm done referring to anim, so must
    IReferenceCounted::drop() this reference now because it
    was produced by a createFoo() function. As I shouldn't refer to
    it again, ensure that I can't by setting to 0.
    */
    anim->drop();
    anim = 0;
  }
  /*
  I'm done with my CSampleSceneNode object, and so must drop my reference.
  This won't delete the object, yet, because it is still attached to the
  scene graph, which prevents the deletion until the graph is deleted or the
  custom scene node is removed from it.
  */
  myNode->drop();
  myNode = 0; // As I shouldn't refer to it again, ensure that I can't
  /*
  Now draw everything and finish.
  */
  u32 frames = 0;
  while (device->run()) {
    driver->beginScene(true, true, _ARGB(0, 100, 100, 100));
    smgr->drawAll();
    driver->endScene();
    if (++frames == 100) {
      char* str = "Irrlicht Engine [";
      str += driver->getName();
      str += "] FPS: ";
      str += (s32)driver->getFPS();
      device->setWindowCaption(str);
      frames = 0;
    }
  }
  device->drop();
  return 0;
}
/*
That's it. Compile and play around with the program.
**/

