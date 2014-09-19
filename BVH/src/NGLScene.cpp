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
//#include <ngl/NGLStream.h>


//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.1;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=1;

NGLScene::NGLScene(QWindow *_parent) : OpenGLWindow(_parent)
{
    m_rotate=false;
    // mouse rotation values set to 0
    m_spinXFace=0;
    m_spinYFace=0;
    setTitle("BvhViewer");
}


NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  Init->NGLQuit();

  delete m_scene;
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
  m_text->setScreenSize(w,h);

  renderLater();
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
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,25,100);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.05,1500);

// nglDiffuseShader
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglDiffuseShader"]->use();
  shader->setShaderParam4f("Colour",1,1,0,1);
  shader->setShaderParam3f("lightPos",1,1,1);
  shader->setShaderParam4f("lightDiffuse",1,1,1,1);

  glEnable(GL_DEPTH_TEST); // for removal of hidden surfaces


  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createCylinder("cylinder",0.15,2.0, 16, 16);
  prim->createSphere("sphere",0.25,16);
  prim->createLineGrid("wall", 1, 1, 40);

   // create the default particle inital position
  m_scene=new Scene(&m_transformStack,m_cam);
  m_scene->addNewWall(ngl::Vec3(0,0,0), 200, ngl::Vec3(0.0, 1.0, 0.0),true);

  setBvhFileName("bvh/Slip_down_IceSlip.bvh");
  Bvh *character = new Bvh(m_bvhFileName, m_scene);
  m_scene->addCharacter(character);

  m_text=new ngl::Text(QFont("Arial",14));
  glViewport(0,0,width(),height());
  m_text->setScreenSize(width(),height());
  currentTime = currentTime.currentTime();

  startTimer((int)character->getFrameTime());
  m_trace=false;

}



void NGLScene::render()
{
  // clear the screen and depth buffer
  if(!m_trace)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // calculate the framerate
  QTime newTime = currentTime.currentTime();
  int msecsPassed = currentTime.msecsTo(newTime);
  currentTime = newTime;



  QString text;
  text.sprintf("number of characters %d",m_scene->getNumCharacters());
  m_text->setColour(1,1,1);
  m_text->renderText(10,10,text);
  text.sprintf("framerate is %d",(int)(1000.0/msecsPassed));
  m_text->renderText(10,30,text);
  m_text->renderText(10,50,QString("r to replay; p to pause and continue; -> forward; <- backward"));
  text.sprintf("frame: %d",m_scene->getCurrentFrameNumber());
  m_text->renderText(10,70,text);
  text.sprintf("Filename is %s",m_bvhFileName.c_str());
  m_text->renderText(10,90,text);

  // Rotation based on the mouse position for our global
  // transform
  ngl::Transformation trans;
  trans.setRotation(m_spinXFace,m_spinYFace,0);
  trans.setPosition(m_modelPos.m_x, m_modelPos.m_y, m_modelPos.m_z);
  ngl::Mat4 tmp = trans.getMatrix(); // very important step, otherwise ngl::transform won't compute the matrix
  // set this in the TX stack
  m_transformStack.setGlobal(trans);

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("nglDiffuseShader");

  m_scene->draw();

}

void NGLScene::flipAnimationStatus()
{
    m_scene->pauseAnimationAndContinue();
}


void NGLScene::timerEvent(QTimerEvent *_event)
{
    m_scene->update();
    renderLater();
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{
  // note the method buttons() is the button state when event was called
  // this is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
    if(m_rotate && _event->buttons() == Qt::LeftButton)
    {
        m_spinYFace = ( m_spinYFace + (_event->x() - m_origX) ) % 360 ;
        m_spinXFace = ( m_spinXFace + (_event->y() - m_origY) ) % 360 ;
        m_origX = _event->x();
        m_origY = _event->y();
    }
    // right mouse translate code
    else if(m_translate && _event->buttons() == Qt::RightButton)
    {
        int diffX = (int)(_event->x() - m_origX);
        int diffY = (int)(_event->y() - m_origY);
        m_origX=_event->x();
        m_origY=_event->y();
        m_modelPos.m_x += INCREMENT * diffX;
        m_modelPos.m_y -= INCREMENT * diffY;
    }
    renderLater();
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
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  }
  // right mouse translate mode
  else if(_event->button() == Qt::RightButton)
  {
    m_origXPos = _event->x();
    m_origYPos = _event->y();
    m_translate=true;
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

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
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	renderLater();
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
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
  case Qt::Key_Space : clearCharacters(); break;
  case Qt::Key_R : replay(); break;
  case Qt::Key_P : flipAnimationStatus(); break;
    case Qt::Key_Right : forwardAnimation();break;
    case Qt::Key_Left : backwardAnimation();break;
  case Qt::Key_T : m_trace^=true; break;
  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
    renderLater();
}
