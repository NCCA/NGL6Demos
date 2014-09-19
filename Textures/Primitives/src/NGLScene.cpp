#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/Camera.h>
#include <ngl/TransformStack.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Texture.h>
#include <QFont>

const std::string NGLScene::s_vboNames[7]=
{
  "sphere",
  "cylinder",
  "cone",
  "disk",
  "plane",
  "torus",
  "teapot",
};
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
  setTitle("Simple OpenGL Texture");

  m_polyMode=GL_FILL;
  m_primIndex=0;
}


NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  Init->NGLQuit();
  // remove the texture now we are done
  glDeleteTextures(1,&m_textureName);

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
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,1,4);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.5,150);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // load a frag and vert shaders

  shader->createShaderProgram("TextureShader");

  shader->attachShader("TextureVertex",ngl::VERTEX);
  shader->attachShader("TextureFragment",ngl::FRAGMENT);
  shader->loadShaderSource("TextureVertex","shaders/TextureVert.glsl");
  shader->loadShaderSource("TextureFragment","shaders/TextureFrag.glsl");

  shader->compileShader("TextureVertex");
  shader->compileShader("TextureFragment");
  shader->attachShaderToProgram("TextureShader","TextureVertex");
  shader->attachShaderToProgram("TextureShader","TextureFragment");


  shader->linkProgramObject("TextureShader");
  shader->use("TextureShader");
  shader->registerUniform("TextureShader","MVP");

  ngl::Texture texture("textures/ratGrid.png");
  m_textureName=texture.setTextureGL();
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createSphere("sphere",1.0,40);
  prim->createCylinder("cylinder",0.5,5,30,30);
  prim->createCone("cone",0.5,1.4,20,20);
  prim->createDisk("disk",0.5,40);
  prim->createTrianglePlane("plane",1,1,10,10,ngl::Vec3(0,1,0));
  prim->createTorus("torus",0.15,0.4,40,40);
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());

}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["TextureShader"]->use();

  ngl::Mat4 MVP=m_transformStack.getCurrentTransform().getMatrix()
                *m_mouseGlobalTX*m_cam->getVPMatrix();

  shader->setRegisteredUniform("MVP",MVP);
}

void NGLScene::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Rotation based on the mouse position for our global
  // transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotY*rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

  // need to bind the active texture before drawing
  glBindTexture(GL_TEXTURE_2D,m_textureName);
  glPolygonMode(GL_FRONT_AND_BACK,m_polyMode);

  loadMatricesToShader();
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->draw(s_vboNames[m_primIndex]);
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
  // turn on wirframe rendering
  case Qt::Key_W : m_polyMode=GL_LINE; break;
  // turn off wire frame
  case Qt::Key_S : m_polyMode=GL_FILL; break;
  // show full screen
  case Qt::Key_F : showFullScreen(); break;
  // show windowed
  case Qt::Key_N : showNormal(); break;
  case Qt::Key_Left : previousPrim(); break;
  case Qt::Key_Right : nextPrim(); break;

  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
    renderLater();
}

void NGLScene::nextPrim()
{

  if( ++m_primIndex >=6)
  {
    m_primIndex=6;
  }
}

void NGLScene::previousPrim()
{
  if( --m_primIndex < 0)
  {
    m_primIndex=0;
  }
}


