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
  setTitle("Qt5 Simple NGL Demo");
  m_emitterPos.set(0,0,0);

}


NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_emitter;
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
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(20,20,35);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.5,150);
  // now to load the shader and set the values
  // grab an instance of shader manager
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Phong
  shader->createShaderProgram("Phong");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("PhongVertex",ngl::VERTEX);
  shader->attachShader("PhongFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
  shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
  // compile the shaders
  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  // add them to the program
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("Phong",0,"inVert");
  // attribute 1 is the UV data u,v (if present)
  shader->bindAttribute("Phong",1,"inUV");
  // attribute 2 are the normals x,y,z
  shader->bindAttribute("Phong",2,"inNormal");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Phong");
  // and make it active ready to load values
  (*shader)["Phong"]->use();

  // now pass the modelView and projection values to the shader
  // the shader will use the currently active material and light0 so set them
  ngl::Light light(ngl::Vec3(0,0,1),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT);
  // now create our light this is done after the camera so we can pass the
  // transpose of the projection matrix to the light to do correct eye space
  // transformations
  ngl::Mat4 iv=m_cam->getViewMatrix();
  iv=iv.inverse();
  iv.transpose();
  light.setTransform(iv);
  light.enable();
  // load these values to the shader as well
  light.loadToShader("light");
  glEnable(GL_DEPTH_TEST); // for removal of hidden surfaces

  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createSphere("sphere",1.0,20);

  prim->createTorus("torus",0.2,0.8,20,20);
  // create the default particle inital position
  m_emitter=new Emitter(m_emitterPos,1000,m_cam);
  m_text=new ngl::Text(QFont("Arial",14));
  m_text->setScreenSize(width(),height());
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
  startTimer(40);

}




void NGLScene::render()
{
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // clear the screen and depth buffer
  shader->use("Phong");

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Rotation based on the mouse position for our global transform
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
  m_emitter->setGlobalTransform(m_mouseGlobalTX);
  m_emitter->draw();
  QString text;
  text.sprintf("number of particles %d",m_emitter->getNumParticles());
  m_text->setColour(1,1,1);
  m_text->renderText(10,10,text);
  m_text->renderText(10,30,QString("1 add Sphere 2 add Teapot"));
  m_text->renderText(10,50,QString("r remove last particle"));
  m_text->renderText(10,70,QString("z,x,c,v change emit angle of sphere"));
  m_text->renderText(10,90,QString("Space clears all particles"));
}

void NGLScene::addParticles( int _type)
{
switch(_type)
{
  case 0 : m_emitter->addParticle(SPHERE); break;
  case 1 : m_emitter->addParticle(TEAPOT); break;

}

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
  case Qt::Key_1 : addParticles(0); break;
  case Qt::Key_2 : addParticles(1); break;
  case Qt::Key_Z : m_emitter->updateEmitAngle(30); break;
  case Qt::Key_X : m_emitter->updateEmitAngle(90); break;
  case Qt::Key_C : m_emitter->updateEmitAngle(200); break;
  case Qt::Key_V : m_emitter->updateEmitAngle(360); break;
  case Qt::Key_Space : m_emitter->clearParticles(); break;
  case Qt::Key_R : m_emitter->removeParticle(); break;
  case Qt::Key_Left : updateEmitterPos(-1,0,0); break;
  case Qt::Key_Right : updateEmitterPos(1,0,0); break;
  case Qt::Key_Up : updateEmitterPos(0,1,0); break;
  case Qt::Key_Down : updateEmitterPos(0,-1,0); break;
  case Qt::Key_I : updateEmitterPos(0,0,-1); break;
  case Qt::Key_O : updateEmitterPos(0,0,1); break;

  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
    renderLater();
}

void NGLScene::timerEvent( QTimerEvent *_event)
{
	m_emitter->update();
	renderLater();
}
void NGLScene::updateEmitterPos(float _dx, float _dy, float _dz)
{
  m_emitterPos.m_x+=_dx;
  m_emitterPos.m_y+=_dy;
  m_emitterPos.m_z+=_dz;
  m_emitter->setPos(m_emitterPos);
}

