#include <QMouseEvent>
#include <QGuiApplication>

#include "Noise.h"
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
  setTitle("Qt5 Simple NGL Demo");
}


NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  Init->NGLQuit();
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

void NGLScene::makeMarbleTexture(float amp, float strength)
{
  // create a new instance of the noise class (which also creates the lattice noise tables)
  Noise *n = new Noise();
  // pointer to the Texture data
  GLfloat *data;
  // size of the texture width
  const static int MSIZE=128;
  // make room for the texture (only using Luminance value here)
  data = new GLfloat [MSIZE*MSIZE*MSIZE*3];
  // calc step size for S and T rangeing from 0.0 1.0
  float step=1.0/(float)MSIZE;
  float S=0.0;
  float T=0.0;
  float U=0.0;
  // create index into data set
  unsigned int index=0;
  // loop for width and height for the texture to create
  std::cout <<"Creating texture"<<std::endl;
  ngl::Vec3 p;
  for(int z=0; z<MSIZE; ++z)
  {
    for(int y=0; y<MSIZE; ++y)
    {
      for(int x=0; x<MSIZE; ++x)
      {
        // the marble functions requires an input of a point in 3d space
        // we use S and T for the x,y and vary Z along the U axis
        p.set(S,T,U);
        // now get the value from the marble function (from noise class)
        float d=n->marble(amp,strength,p);
        data[index++]=d;
        data[index++]=d;//n->marble(amp,strength,p);
        data[index++]=d;//n->marble(amp,strength,p);
        // inc to next data set
        //		++index;
        S+=step;
      }
    // inc T and reset S
      T+=step;
      S=0.0;
    }
    T=0.0;
    U+=step;
  }
  glGenTextures(1,&m_textureName);
  glBindTexture(GL_TEXTURE_3D,m_textureName);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

  glTexImage3D(GL_TEXTURE_3D,0,GL_RGB,MSIZE,MSIZE,MSIZE,0,GL_RGB,GL_FLOAT,data);
  glGenerateMipmap(GL_TEXTURE_3D); //  Allocate the mipmaps
  //  Allocate the mipmaps
  std::cout <<"done texture"<<std::endl;
  // remove the Data
  delete [] data;
  // return the id of the texture object
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
    // Now we will create a basic Camera from the graphics library
    // This is a static camera so it only needs to be set once
    // First create Values for the camera position
    ngl::Vec3 from(0,1,2);
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
    makeMarbleTexture(0.00007f,18.0);

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
   // as re-size is not explicitly called we need to do this.
   glViewport(0,0,width(),height());


}

void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

   ngl::Mat4 MVP=m_transformStack.getCurrentTransform().getMatrix()
       *m_mouseGlobalTX * m_cam->getVPMatrix();

   shader->setShaderParamFromMat4("MVP",MVP);
}

void NGLScene::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 // build our transform stack
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

	ngl::ShaderLib *shader=ngl::ShaderLib::instance();
	(*shader)["TextureShader"]->use();
	loadMatricesToShader();
	glBindTexture(GL_TEXTURE_3D,m_textureName);

	ngl::VAOPrimitives *prim =  ngl::VAOPrimitives::instance();
	prim->draw("teapot");

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
  case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
  // turn off wire frame
  case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
  // show full screen
  case Qt::Key_F : showFullScreen(); break;
  // show windowed
  case Qt::Key_N : showNormal(); break;
  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
    renderLater();
}
