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
  setTitle("Simple Projectiles");
  m_particleTimer=startTimer(20);
  m_fps=0;
  m_frames=0;
  m_animate=true;


}


NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
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
  m_cam = new ngl::PathCamera(ngl::Vec4(0,1,-0.2),"data/loop.dat",0.001);
  m_cam->setShape(40.0,1024.0/720.0,0.5,150.0);
  m_cam->updateLooped();
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Phong
  shader->createShaderProgram("PointSprite");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("PointSpriteVertex",ngl::VERTEX);
  shader->attachShader("PointSpriteFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("PointSpriteVertex","shaders/Vertex.vs");
  shader->loadShaderSource("PointSpriteFragment","shaders/Fragment.fs");
  // compile the shaders
  shader->compileShader("PointSpriteVertex");
  shader->compileShader("PointSpriteFragment");
  // add them to the program
  shader->attachShaderToProgram("PointSprite","PointSpriteVertex");
  shader->attachShaderToProgram("PointSprite","PointSpriteFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("PointSprite",0,"inVert");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("PointSprite");


  shader->createShaderProgram("Colour");

  shader->attachShader("ColourVertex",ngl::VERTEX);
  shader->attachShader("ColourFragment",ngl::FRAGMENT);
  shader->loadShaderSource("ColourVertex","shaders/Colour.vs");
  shader->loadShaderSource("ColourFragment","shaders/Colour.fs");

  shader->compileShader("ColourVertex");
  shader->compileShader("ColourFragment");
  shader->attachShaderToProgram("Colour","ColourVertex");
  shader->attachShaderToProgram("Colour","ColourFragment");

  shader->bindAttribute("Colour",0,"inVert");

  shader->linkProgramObject("Colour");
  (*shader)["Colour"]->use();
  // the shader will use the currently active material and light0 so set them
  shader->setShaderParam4f("Colour",0.2,0.2,0.2,1);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_POINT_SPRITE);

  glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE, 32.0f );
  glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN,GL_LOWER_LEFT);
  glPointParameterf( GL_POINT_SIZE_MIN, 1.0f );
  glPointParameterf( GL_POINT_SIZE_MAX, 32.0f );


  m_emitter1 = new Emitter(ngl::Vec3(0,0,0),"textures/SmokeTest.tiff",2000);
  m_emitter2 = new Emitter(ngl::Vec3(2,0,2),"textures/SmokeTest2.tiff",2000);
  m_emitter3 = new Emitter(ngl::Vec3(2,0,2),"textures/SmokeTest3.tiff",5000);
  m_emitter1->setCam(m_cam);
  m_emitter2->setCam(m_cam);
  m_emitter3->setCam(m_cam);
  m_emitter1->setShaderName("PointSprite");
  m_emitter2->setShaderName("PointSprite");
  m_emitter3->setShaderName("PointSprite");
  // pre-roll the simulation to make it stable
  for(int i=0; i<500; ++i)
  {
    m_emitter1->update();
    m_emitter2->update();
    m_emitter3->update();
  }
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createLineGrid("plane",10,10,40);

  m_fpsTimer =startTimer(10);
  m_camTimer=startTimer(20);

  m_text->setScreenSize(width(),height());
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
}




void NGLScene::render()
{
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["PointSprite"]->use();
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Rotation based on the mouse position for our global
  // transform
  ngl::Transformation trans;
  trans.setRotation(m_spinXFace,m_spinYFace,0);
  // set this in the TX stack
  m_transformStack.setGlobal(trans);
  ngl::Mat4 MV;
  ngl::Mat4 MVP;

  MV=m_transformStack.getCurrAndGlobal().getMatrix()*m_cam->getViewMatrix() ;
  MVP=MV*m_cam->getProjectionMatrix() ;

  glBindTexture(GL_TEXTURE_2D,m_emitter1->getTextureID());
  glTexEnvf(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

  m_emitter1->draw(MVP);

  glBindTexture(GL_TEXTURE_2D,m_emitter2->getTextureID());
  glTexEnvf(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

  m_emitter2->draw(MVP);

  glBindTexture(GL_TEXTURE_2D,m_emitter3->getTextureID());
  glTexEnvf(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

  m_emitter3->draw(MVP);


  (*shader)["Colour"]->use();
  shader->setShaderParamFromMat4("MVP",MVP);

  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->draw("plane");

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
    renderLater();

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
    renderLater();

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

  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
    renderLater();
}

void NGLScene::timerEvent(QTimerEvent *_event )
{
	if( m_animate == true)
	{
		m_emitter1->update();
		m_emitter2->update();
		m_emitter3->update();

	}
	if(_event->timerId() == m_camTimer)
	{
		m_cam->updateLooped();
	}
	renderNow();
		// re-draw GL
}
