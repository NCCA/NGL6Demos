#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/Camera.h>
#include <ngl/Light.h>
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
  setTitle("VAOPrimitives Demo");
  // this timer is going to trigger an event every 40ms which will be processed in the
  //
  m_lightTimer =startTimer(40);
  m_animate=true;
}


NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_light;
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
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
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
    shader->setShaderParam1i("Normalize",1);
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,2,6);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  // now load to our new camera
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.05,350);
  shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);

  // now pass the modelView and projection values to the shader
  // the shader will use the currently active material and light0 so set them
  ngl::Material m(ngl::GOLD);
  m.loadToShader("material");
  m_lightAngle=0.0;
  m_light = new ngl::Light(ngl::Vec3(sin(m_lightAngle),2,cos(m_lightAngle)),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT);

  // now create our light this is done after the camera so we can pass the
  // transpose of the projection matrix to the light to do correct eye space
  // transformations
  ngl::Mat4 iv=m_cam->getViewMatrix();
  iv.transpose();
  m_light->setTransform(iv);
  // load these values to the shader as well
  m_light->loadToShader("light");


  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createSphere("sphere",0.5,50);

  prim->createCylinder("cylinder",0.5,1.4,40,40);

  prim->createCone("cone",0.5,1.4,20,20);

  prim->createDisk("disk",0.8,120);
  prim->createTorus("torus",0.15,0.4,40,40);
  prim->createTrianglePlane("plane",14,14,80,80,ngl::Vec3(0,1,0));
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());

}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_transform.getMatrix()*m_mouseGlobalTX;
  MV=  M*m_cam->getViewMatrix();
  MVP=  MV*m_cam->getProjectionMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);
}

void NGLScene::drawScene(const std::string &_shader)
{
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[_shader]->use();
  // clear the screen and depth buffer
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

   // get the VBO instance and draw the built in teapot
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

  m_transform.reset();
  {
    loadMatricesToShader();
    prim->draw("teapot");
  } // and before a pop

  m_transform.reset();
  {
    m_transform.setPosition(-3,0.0,0.0);
    loadMatricesToShader();
    prim->draw("sphere");
  } // and before a pop

  m_transform.reset();
  {
    m_transform.setPosition(3,0.0,0.0);
    loadMatricesToShader();
    prim->draw("cylinder");
  } // and before a pop

  m_transform.reset();
  {
    m_transform.setPosition(0.0,0.0,3.0);
    loadMatricesToShader();
    prim->draw("cube");
  } // and before a pop

  m_transform.reset();
  {
    m_transform.setPosition(-3.0,0.0,3.0);
    loadMatricesToShader();
    prim->draw("torus");
  } // and before a pop

  m_transform.reset();
  {
    m_transform.setPosition(3.0,0.5,3.0);
    loadMatricesToShader();
    prim->draw("icosahedron");
  } // and before a pop

  m_transform.reset();
  {
    m_transform.setPosition(0.0,0.0,-3.0);
    loadMatricesToShader();
    prim->draw("cone");
  } // and before a pop


  m_transform.reset();
  {
    m_transform.setPosition(-3.0,0.5,-3.0);
    loadMatricesToShader();
    prim->draw("tetrahedron");
  } // and before a pop


  m_transform.reset();
  {
    m_transform.setPosition(3.0,0.5,-3.0);
    loadMatricesToShader();
    prim->draw("octahedron");
  } // and before a pop


  m_transform.reset();
  {
    m_transform.setPosition(0.0,0.5,-6.0);
    loadMatricesToShader();
    prim->draw("football");
  } // and before a pop

  m_transform.reset();
  {
    m_transform.setPosition(-3.0,0.5,-6.0);
    m_transform.setRotation(0,180,0);
    loadMatricesToShader();
    prim->draw("disk");
  } // and before a pop


  m_transform.reset();
  {
    m_transform.setPosition(3.0,0.5,-6.0);
    loadMatricesToShader();
    prim->draw("dodecahedron");
  } // and before a pop

  m_transform.reset();
  {
    m_transform.setPosition(1.0,0.35,1.0);
    m_transform.setScale(1.5,1.5,1.5);
    loadMatricesToShader();
    prim->draw("troll");
  } // and before a pop

#ifdef ADDLARGEMODELS
  m_transform.reset();
  {
    m_transform.setPosition(-1.0,-0.5,1.0);
    m_transform.setScale(0.1,0.1,0.1);
    loadMatricesToShader();
    prim->draw("dragon");
  } // and before a pop

  m_transform.reset();
  {
    m_transform.setPosition(-2.5,-0.5,1.0);
    m_transform.setScale(0.1,0.1,0.1);
    loadMatricesToShader();
    prim->draw("buddah");
  } // and before a pop

  m_transform.reset();
  {
    m_transform.setPosition(2.5,-0.5,1.0);
    m_transform.setScale(0.1,0.1,0.1);
    loadMatricesToShader();
    prim->draw("bunny");
  } // and before a pop
#endif

  m_transform.reset();
  {
    m_transform.setPosition(0.0,-0.5,0.0);
    loadMatricesToShader();
    prim->draw("plane");
  } // and before a pop


}

void NGLScene::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawScene("Phong");

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
  if (isExposed())
    renderLater();
}

void NGLScene::updateLight()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  (*shader)["Phong"]->use();
  // change the light angle
  m_lightAngle+=0.1;

  // now set this value and load to the shader
  m_light->setPosition(ngl::Vec3(4.0*cos(m_lightAngle),2,4.0*sin(m_lightAngle)));
  m_light->loadToShader("light");
}

void NGLScene::timerEvent(QTimerEvent *_event )
{
// if the timer is the light timer call the update light method
  if(_event->timerId() == m_lightTimer && m_animate==true)
  {
    updateLight();
  }
    // re-draw GL
renderLater();
}

