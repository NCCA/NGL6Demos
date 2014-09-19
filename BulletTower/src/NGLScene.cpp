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
#include <ngl/Quaternion.h>
#include "PhysicsWorld.h"
#include "CollisionShape.h"
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1;
const static float BOXW=10.0f;
const static float BOXH=1.5f;
const static float BOXD=1.5f;
NGLScene::NGLScene(QWindow *_parent) : OpenGLWindow(_parent)
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  m_numBlocks=135;

  setTitle("Bullet Physics and  NGL Demo");
  m_animate=true;
  m_randomPlace=false;
  m_bboxDraw=false;
  m_wireframe=false;
  m_physics = new PhysicsWorld();
  m_physics->setGravity(ngl::Vec3(0,-10,0));
  m_physics->addGroundPlane(ngl::Vec3(0,0,0),ngl::Vec3(50,0.01,50));
  ngl::Random *rng=ngl::Random::instance();
  rng->setSeed();

  CollisionShape *shapes=CollisionShape::instance();

  shapes->addBox("box",BOXW,BOXH,BOXD);
  shapes->addSphere("sphere",1.0f);
  m_x=0.0f;
  m_y=10.0f;
  m_rot=0.0;
  addCube();
  m_radius=20;
  m_firePos.set(cos(ngl::radians((m_rot)))*m_radius, m_y, sin(ngl::radians(m_rot))*m_radius);


}
const static float EXT=5.0f;

void NGLScene::addCube()
{

  ngl::Vec3 pos;
  float angle;
  ngl::Vec3 axis(0,1,0);
  float y=0.0f;

  while(y<=m_numBlocks)
  {
      for(float x=-EXT; x<=EXT; x+=BOXD)
      {
        pos.set(x,y,0);
        angle=90.0f;
        m_physics->addBox("box",pos,axis,angle);
     }
      y+=BOXH;
      for(float z=-EXT; z<=EXT; z+=BOXD)
      {
        pos.set(0,y,z);
        angle=0.0f;
        m_physics->addBox("box",pos,axis,angle);
      }
      y+=BOXH;
}


//  for(float y=0.0; y<10.0; y+=BOXH)
//  {
//    for(float x=-5.0; x<=5.0; x+=0.5)
//    {
//      if(row % 2)
//      {
//        pos.set(x,y,z);
//        angle=90.0f;
//      }
//      else
//      {
//        pos.set(z,y,x);
//        angle=0.0f;
//      }

//      m_physics->addBox("box",pos,axis,angle);
//    }
//    ++row;

//  }
}

void NGLScene::addSphere()
{

  ngl::Random *rng=ngl::Random::instance();
  ngl::Vec3 dir=ngl::Vec3(0,m_y,0)-m_firePos;
  m_physics->addSphere("sphere",m_firePos,dir*5,30,rng->getRandomVec3()*20);

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
  ngl::Vec3 from(0,0,60);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  // now load to our new camera
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(50,(float)720.0/576.0,0.05,350);

  ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
  prim->createSphere("sphere",0.5,40);
  prim->createLineGrid("plane",240,240,40);

 startTimer(5);
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
  m_text = new  ngl::Text(QFont("Arial",18));
  m_text->setScreenSize(this->size().width(),this->size().height());
  m_x=0.0f;
  m_y=10.0f;

}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;

  MV=  m_bodyTransform*m_globalTransformMatrix*m_cam->getViewMatrix();
  MVP= MV*m_cam->getVPMatrix();
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
    ngl::Mat4 scale;
    scale.scale(BOXW,BOXH,BOXD);

    shader->setRegisteredUniform("Colour",0.0f,0.0f,1.0f,1.0f);
    switch(m_physics->getCollisionShape(i))
    {
      case BOX_SHAPE_PROXYTYPE :
        shader->setRegisteredUniform("Colour",1.0f,0.0f,0.0f,1.0f);
        m_bodyTransform*=scale;
        loadMatricesToShader();

        prim->draw("cube");
      break;
      case SPHERE_SHAPE_PROXYTYPE :
        shader->setRegisteredUniform("Colour",0.0f,1.0f,0.0f,1.0f);
        prim->draw("sphere");

      break;
    }
  }


  m_bodyTransform.identity();
  m_bodyTransform.translate(m_firePos.m_x,m_firePos.m_y,m_firePos.m_z);
  shader->setShaderParam4f("Colour",0.0f,0.0f,1.0f,1.0f);
  loadMatricesToShader();
  prim->draw("cube");


  shader->setShaderParam4f("Colour",1.0f,1.0f,1.0f,1.0f);

  m_bodyTransform.identity();
  loadMatricesToShader();

  prim->draw("plane");
  m_text->setColour(1,1,1);
  QString text=QString("Number of Bodies=%1").arg(bodies-1);
  m_text->renderText(10,18,text );
  text=QString("x=%1 y=%2").arg(m_x).arg(m_y);
  m_text->renderText(10,38,text );


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
  case Qt::Key_Space : toggleAnimation(); break;
  case Qt::Key_X : stepAnimation(); break;
  case Qt::Key_1 : addCube(); break;
  case Qt::Key_2 : addSphere(); break;
  case Qt::Key_Left : m_rot+=5; break;
  case Qt::Key_Right : m_rot-=5; break;
  case Qt::Key_Down : m_y-=1.0; break;
  case Qt::Key_Up : m_y+=1.0; break;
  case Qt::Key_Plus :
  case Qt::Key_Equal :
    m_numBlocks+=5;
    if(m_numBlocks > 300)
      m_numBlocks=300;
  resetSim();
  addCube();
  break;
  case Qt::Key_Minus :
    m_numBlocks-=5;
    if(m_numBlocks<10)
      m_numBlocks=10;
    resetSim();
    addCube();
    break;

  case Qt::Key_0 : resetSim(); addCube(); break;

  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
  m_firePos.set(cos(ngl::radians((m_rot)))*m_radius, m_y, sin(ngl::radians(m_rot))*m_radius);

  renderNow();
}

void NGLScene::resetSim()
{
   m_physics->reset();
}

void NGLScene::timerEvent(QTimerEvent *_e)
{
  if(m_animate == true)
  {
    m_physics->step(1.0/60.0,1);
  }
  renderLater();

}
void NGLScene::stepAnimation()
{
  m_physics->step(1.0/20.0,10);

}
