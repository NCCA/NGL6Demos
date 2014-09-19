#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/TransformStack.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Random.h>
#include "PhysicsWorld.h"
#include "CollisionShape.h"
#include <ngl/NGLStream.h>
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1;

NGLScene::NGLScene(QWindow *_parent) : OpenGLWindow(_parent)
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  setTitle("Bullet Physics and  NGL Demo");
  m_animate=true;
  m_randomPlace=false;
  m_bboxDraw=false;
  m_wireframe=false;
  m_physics = new PhysicsWorld();
  m_physics->setGravity(ngl::Vec3(0,-10,0));
  m_physics->addGroundPlane(ngl::Vec3(0,0,0),ngl::Vec3(50,0.01,50));

  // the car needs the ground plane so really needs to be constructed after
  m_car = new Vehicle(m_physics);

  ngl::Random *rng=ngl::Random::instance();
  rng->setSeed();

}

void NGLScene::addCube()
{
  ngl::Random *rng=ngl::Random::instance();

  ngl::Vec3 pos=rng->getRandomVec3();
  pos*=10;
  pos.m_y=10;
  if(m_randomPlace == false)
    pos.set(0,10,0);
    m_physics->addBox("box",pos);
}

void NGLScene::addSphere()
{

  ngl::Random *rng=ngl::Random::instance();

  ngl::Vec3 pos=rng->getRandomVec3();
  pos*=10;
  pos.m_y=10;
  if(m_randomPlace == false)
    pos.set(0,10,0);
  m_physics->addSphere("sphere",pos,rng->randomPositiveNumber(5.0),rng->getRandomVec3()*20);

}

void NGLScene::addCylinder()
{

  ngl::Random *rng=ngl::Random::instance();

  ngl::Vec3 pos=rng->getRandomVec3();
  pos*=10;
  pos.m_y=10;
  if(m_randomPlace == false)
    pos.set(0,10,0);
  m_physics->addCylinder("cylinder",pos);

}

void NGLScene::addCone()
{

  ngl::Random *rng=ngl::Random::instance();

  ngl::Vec3 pos=rng->getRandomVec3();
  pos*=10;
  pos.m_y=10;
  if(m_randomPlace == false)
    pos.set(0,10,0);
  m_physics->addCone("cone",pos);

}
void NGLScene::addCapsule()
{
  ngl::Random *rng=ngl::Random::instance();

  ngl::Vec3 pos=rng->getRandomVec3();
  pos*=10;
  pos.m_y=10;
  if(m_randomPlace == false)
    pos.set(0,10,0);
  m_physics->addCapsule("capsule",pos);
}

void NGLScene::addMesh(MeshType _m)
{
  ngl::Random *rng=ngl::Random::instance();

  ngl::Vec3 pos=rng->getRandomVec3();
  pos*=10;
  pos.m_y=10;
  if(m_randomPlace == false)
    pos.set(0,10,0);
  if(_m == TEAPOT)
    m_physics->addMesh("teapot",pos);
  else if(_m == APPLE)
    m_physics->addMesh("apple",pos);


}



NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_text;
  delete m_physics;
  Init->NGLQuit();

}

void NGLScene::resizeEvent(QResizeEvent *_event )
{
  if(isExposed())
  {
  int w=_event->size().width();
  int h=_event->size().height();
  // set the viewport for openGL
  glViewport(0,0,w,h);
  // now set the camera size values as the screen size has changed
  m_cam->setShape(45,(float)w/h,0.05,350);
  renderLater();
  m_text->setScreenSize(w,h);

  }
}


void NGLScene::initialize()
{
  // we must call this first before any other GL commands to load and link the
  // gl commands from the lib, if this is not done program will crash
  ngl::NGLInit::instance();

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // we need to initialise the NGL lib, under windows and linux we also need to
  // initialise GLEW, under windows this needs to be done in the app as well
  // as the lib hence the WIN32 define
  ngl::NGLInit::instance();

  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglToonShader"]->use();
  shader->setShaderParam4f("Colour",1,1,1,1);

  (*shader)["nglDiffuseShader"]->use();
  shader->setShaderParam4f("Colour",1,1,0,1);
  shader->setShaderParam3f("lightPos",1,1,1);
  shader->setShaderParam4f("lightDiffuse",1,1,1,1);

  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,1,30);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  // now load to our new camera
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(50,(float)720.0/576.0,0.05,350);
  ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
  prim->createSphere("sphere",0.5,40);
  prim->createLineGrid("plane",140,140,40);
  m_teapotMesh = new ngl::Obj("models/teapot.obj");
  m_teapotMesh->createVAO();
  //create a dynamic rigidbody

  m_appleMesh = new ngl::Obj("models/apple.obj");
  m_appleMesh->createVAO();
  m_appleMesh->calcBoundingSphere();
  prim->createCapsule("defaultCap",0.5,1.0,20);
  prim->createCone("cone",0.5,1.0,20,20);
  prim->createCylinder("cylinder",0.5,2.0,20,20);
  prim->createCylinder("wheel",0.5,0.4,20,20);


 startTimer(10);
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
  // we add the shapes first then refer to them by name in the
  // rest of the system this reduces the overhead to bullet as it
  // can re-use the shapes for efficiency.
  CollisionShape *shapes=CollisionShape::instance();
  shapes->addMesh("teapot","models/teapotCollisionMesh.obj");
  shapes->addMesh("apple","models/appleCollisionMesh.obj");

  shapes->addBox("box",1.0f,1.0f,1.0f);
  shapes->addSphere("sphere",1.0f);
  shapes->addCapsule("capsule",0.5f,1.0f);
  shapes->addCone("cone",0.5f,2.0f);
  shapes->addCylinder("cylinder",0.5f,1.0f);

  m_text = new  ngl::Text(QFont("Arial",18));
  m_text->setScreenSize(this->size().width(),this->size().height());


}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;

  MV=  m_bodyTransform*m_globalTransformMatrix*m_cam->getViewMatrix();
  MVP= MV*m_cam->getProjectionMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setRegisteredUniform("MVP",MVP);
  shader->setRegisteredUniform("normalMatrix",normalMatrix);
}

void NGLScene::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglDiffuseShader"]->use();

  // Rotation based on the mouse position for our global transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  // multiply the rotations
  m_globalTransformMatrix=rotY*rotX;
  // add the translations
  m_globalTransformMatrix.m_m[3][0] = m_modelPos.m_x;
  m_globalTransformMatrix.m_m[3][1] = m_modelPos.m_y;
  m_globalTransformMatrix.m_m[3][2] = m_modelPos.m_z;
  // set this in the TX stack

   // get the VBO instance and draw the built in teapot
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  unsigned int bodies=m_physics->getNumCollisionObjects();
  for(unsigned int i=1; i<bodies; ++i)
  {


    m_bodyTransform=m_physics->getTransformMatrix(i);

    loadMatricesToShader();
    shader->setRegisteredUniform("Colour",0.0f,0.0f,1.0f,1.0f);
    switch(m_physics->getCollisionShape(i))
    {
      case BOX_SHAPE_PROXYTYPE :
      {
        shader->setRegisteredUniform("Colour",1.0f,0.0f,0.0f,1.0f);
        CollisionShape *shapes=CollisionShape::instance();
        float w=shapes->getDim1("car");
        float h=shapes->getDim2("car");
        float d=shapes->getDim3("car");
        ngl::Mat4 s;
        s.scale(w,h,d);
        m_bodyTransform=s*m_bodyTransform;
        loadMatricesToShader();
        prim->draw("cube");
      break;
      }
      case SPHERE_SHAPE_PROXYTYPE :
        shader->setRegisteredUniform("Colour",0.0f,1.0f,0.0f,1.0f);
        prim->draw("sphere");

      break;
      case CAPSULE_SHAPE_PROXYTYPE :
        shader->setRegisteredUniform("Colour",0.0f,0.0f,1.0f,1.0f);
        prim->draw("defaultCap");
      break;

      case CONE_SHAPE_PROXYTYPE :
        shader->setRegisteredUniform("Colour",0.0f,1.0f,1.0f,1.0f);
        prim->draw("cone");
      break;
      case CYLINDER_SHAPE_PROXYTYPE :
        shader->setRegisteredUniform("Colour",1.0f,1.0f,0.0f,1.0f);
        prim->draw("cylinder");
      break;
      case 4 :
         std::string name=m_physics->getBodyNameAtIndex(i);
        if(name =="teapot")
        {
          shader->setRegisteredUniform("Colour",1.0f,1.0f,0.0f,1.0f);
          m_teapotMesh->draw();
        }
        else if(name =="apple")
        {
          shader->setRegisteredUniform("Colour",0.0f,1.0f,0.0f,1.0f);
          m_appleMesh->draw();
        }
      break;
    }
  }
  // draw wheels
  {
    m_bodyTransform.identity();
    shader->setShaderParam4f("Colour",1.0f,1.0f,1.0f,1.0f);

    for(int i=0; i<4; ++i)
    {
      m_bodyTransform=m_car->getWheelTransform(i);
      ngl::Mat4 rotZ;
      rotZ.rotateZ(180);
      ngl::Mat4 rotY;
      rotY.rotateY(90);
      m_bodyTransform=rotZ*rotY*m_bodyTransform;
      loadMatricesToShader();
      prim->draw("wheel");
    }

  }

  shader->setShaderParam4f("Colour",1.0f,1.0f,1.0f,1.0f);

  m_bodyTransform.identity();
  loadMatricesToShader();

  prim->draw("plane");
  m_text->setColour(1,1,1);
  QString text=QString("Number of Bodies=%2").arg(bodies-1);
  m_text->renderText(10,18,text );


}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{
  // note the method buttons() is the button state when event was called
  // this is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
  if(m_rotate && _event->buttons() == Qt::LeftButton)
  {
    int diffx=_event->x()-m_origX;
    int diffy=_event->y()-m_origY;
    m_spinXFace += (float) 0.5f * diffy;
    m_spinYFace += (float) 0.5f * diffx;
    m_origX = _event->x();
    m_origY = _event->y();
    renderNow();

  }
        // right mouse translate code
  else if(m_translate && _event->buttons() == Qt::RightButton)
  {
    int diffX = (int)(_event->x() - m_origXPos);
    int diffY = (int)(_event->y() - m_origYPos);
    m_origXPos=_event->x();
    m_origYPos=_event->y();
    m_modelPos.m_x += INCREMENT * diffX;
    m_modelPos.m_y -= INCREMENT * diffY;
    renderNow();

   }
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent ( QMouseEvent * _event)
{
  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if(_event->button() == Qt::LeftButton)
  {
    m_origX = _event->x();
    m_origY = _event->y();
    m_rotate =true;
  }
  // right mouse translate mode
  else if(_event->button() == Qt::RightButton)
  {
    m_origXPos = _event->x();
    m_origYPos = _event->y();
    m_translate=true;
  }

}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent ( QMouseEvent * _event )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_rotate=false;
  }
        // right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_translate=false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent(QWheelEvent *_event)
{

	// check the diff of the wheel position (0 means no change)
	if(_event->delta() > 0)
	{
		m_modelPos.m_z+=ZOOM;
	}
	else if(_event->delta() <0 )
	{
		m_modelPos.m_z-=ZOOM;
	}
	renderNow();
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the NGLScene
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  // turn on wirframe rendering
  case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
  // turn off wire frame
  case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
  // show full screen
  case Qt::Key_F : showFullScreen(); break;
  // show windowed
  case Qt::Key_N : showNormal(); break;
  case Qt::Key_Space : m_car->reset(); break;
  case Qt::Key_X : stepAnimation(); break;
  case Qt::Key_1 : addCube(); break;
  case Qt::Key_2 : addSphere(); break;
  case Qt::Key_3 : addCapsule(); break;
  case Qt::Key_4 : addCylinder(); break;
  case Qt::Key_5 : addCone(); break;
  case Qt::Key_6 : addMesh(TEAPOT); break;
  case Qt::Key_7 : addMesh(APPLE); break;
/*  case Qt::Key_Left : m_physics->addImpulse(ngl::Vec3(-5,0.0f,0.0f)); break;
  case Qt::Key_Right : m_physics->addImpulse(ngl::Vec3(5.0f,0.0f,0.0f)); break;
  case Qt::Key_Up : m_physics->addImpulse(ngl::Vec3(0.0f,5.0f,0.0f)); break;
  case Qt::Key_Down : m_physics->addImpulse(ngl::Vec3(0.0f,-5.0f,0.0f)); break;
*/
  case Qt::Key_Left : m_car->left(); break;
  case Qt::Key_Right : m_car->right(); break;
  case Qt::Key_Up : m_car->accelerate(); break;
  case Qt::Key_Down : m_car->stop(); break;


  case Qt::Key_B : toggleBBox(); break;
  case Qt::Key_R : toggleRandomPlace(); break;
  case Qt::Key_0 : resetSim();  break;

  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
    renderNow();
}


void NGLScene::keyReleaseEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the NGLScene
  switch (_event->key())
  {

  case Qt::Key_Left : m_car->resetSteer(); break;
  case Qt::Key_Right : m_car->resetSteer(); break;


  }
}

void NGLScene::resetSim()
{
   m_physics->reset();
}

void NGLScene::timerEvent(QTimerEvent *_e)
{
  if(m_animate == true)
  {

     m_car->update();
    m_physics->step(1.0/60.0,10);
  }
  renderLater();

}
void NGLScene::stepAnimation()
{
  m_car->update();
  m_physics->step(1.0/60.0,10);

}
