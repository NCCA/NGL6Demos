#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/TransformStack.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Random.h>
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
  setTitle("Using ngl::Light as a point light");
  m_teapotRotation=0.0;
  m_rotationTimer=startTimer(20);
  m_lightChangeTimer=startTimer(1000);
  m_scale=8.0;

}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  for(int i=0; i<NumLights; ++i)
   {
      delete m_lightArray[i];
   }
   ngl::NGLInit *Init = ngl::NGLInit::instance();
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
  renderNow();
  }
}


void NGLScene::initialize()
{
  ngl::NGLInit::instance();

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,10,20);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);

  std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.5,150);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  shader->createShaderProgram("Phong");

  shader->attachShader("PhongVertex",ngl::VERTEX);
  shader->attachShader("PhongFragment",ngl::FRAGMENT);
  shader->loadShaderSource("PhongVertex","shaders/PhongVert.glsl");
  shader->loadShaderSource("PhongFragment","shaders/PhongFrag.glsl");

  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");

  shader->linkProgramObject("Phong");
  (*shader)["Phong"]->use();

  // now pass the modelView and projection values to the shader
  shader->setShaderParam1i("Normalize",1);
  shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);

  // now set the material and light values
  ngl::Material m(ngl::POLISHEDSILVER);
  m.loadToShader("material");
  // create the lights
  createLights();
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
  M=m_transformStack.getCurrentTransform().getMatrix()*m_mouseGlobalTX;
  MV=M*m_cam->getViewMatrix() ;
  MVP=MV*m_cam->getProjectionMatrix() ;
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);
}

void NGLScene::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ngl::Transformation trans;
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
  shader->use("Phong");

  // grab an instance of the primitives for drawing
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();


  for(int i=0; i<NumLights; ++i)
  {
    // enable the light (this will set the values)
    m_lightArray[i]->enable();
    m_transformStack.pushTransform();
    {
      m_transformStack.setPosition(m_lightArray[i]->getPos());
      loadMatricesToShader();
      prim->draw("cube");
    }
    m_transformStack.popTransform();
  }

  m_transformStack.pushTransform();
  {
    m_transformStack.setScale(m_scale,m_scale,m_scale);
    m_transformStack.setRotation(m_teapotRotation,m_teapotRotation,m_teapotRotation);
    // now set this value in the shader for the current ModelMatrix
    loadMatricesToShader();
  prim->draw("teapot");
  }
  m_transformStack.popTransform();
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
  case Qt::Key_Equal :
  case Qt::Key_Plus :
        ++m_scale;
  break;

    case Qt::Key_Minus :
          --m_scale;
    break;

  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
    renderNow();
}


void NGLScene::createLights()
{
  // light position
  ngl::Vec3 pos;
  // light colour
  ngl::Colour col,speccol;
  ngl::Random *rand=ngl::Random::instance();
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("Phong");
  // we need to load the inverse view matrix to the light transform
  // this will give us the correct light positions in eye space
  ngl::Mat4 iv=m_cam->getViewMatrix();
  iv.transpose();
  // loop for the NumLights lights and set the position and colour
  for(int i=0; i<NumLights; ++i)
  {
    // get a random light position
      pos=rand->getRandomPoint(20,20,20);
      // create random colour
      col=rand->getRandomColour();
      col.clamp(0.05,0.3);
      speccol=rand->getRandomColour();
      speccol.clamp(0.1,0.2);
      // create an instance of the light and put it in the array
      m_lightArray[i] = new ngl::Light(pos,col,speccol,ngl::POINTLIGHT);
      QString lightName=QString("light[%1]").arg(i);
      m_lightArray[i]->setTransform(iv);
      m_lightArray[i]->loadToShader(lightName.toStdString());
  }
}

void NGLScene::timerEvent( QTimerEvent *_event )
{
  if(_event->timerId() == m_rotationTimer)
  {
    ++m_teapotRotation;
    // re-draw GL
    renderNow();
  }

  else if(_event->timerId() == m_lightChangeTimer)
  {
    ngl::Vec3 pos;
    ngl::Colour col;
    ngl::Colour speccol;
    ngl::Random *rand=ngl::Random::instance();
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    shader->use("Phong");
    ngl::Mat4 iv=m_cam->getViewMatrix();
    //iv.transpose();

    for(int i=0; i<NumLights; ++i)
    {
      // get a random light position
      pos=rand->getRandomPoint(20,20,20);
        // create random colour
      col=rand->getRandomColour();
      col.clamp(0.05,0.3);
      speccol=rand->getRandomColour();
      speccol.clamp(0.0,0.4);

      m_lightArray[i]->setPosition(pos);
      m_lightArray[i]->setColour(col);
      m_lightArray[i]->setSpecColour(speccol);
      m_lightArray[i]->setTransform(iv);

      QString lightName=QString("light[%1]").arg(i);
      m_lightArray[i]->loadToShader(lightName.toStdString());
    }
    // re-draw GL
    renderNow();

  }
}

