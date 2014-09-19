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
#include <boost/foreach.hpp>


//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1;

NGLScene::NGLScene(int _numSpheres,QWindow *_parent) : OpenGLWindow(_parent)
{
  m_numSpheres=_numSpheres;

  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  setTitle("Sphere -> Plane Collision");
  m_sphereUpdateTimer=startTimer(130);
  m_animate=true;
  // now create the actual spheres for our program

  m_plane = new Plane(ngl::Vec3(0,0,0),5,5);
  ngl::Vec3 pos;
  // now create the actual spheres for our program
  ngl::Random *rng=ngl::Random::instance();

	for(int i=0; i<_numSpheres; i++)
	{
		// use the uniform function from the rng class to get a nice
		// even random number
		pos.set(rng->randomNumber(6),8,rng->randomNumber(6));
		// add the spheres to the end of the particle list
		m_sphereArray.push_back(Sphere(pos,ngl::Vec3(0,-1,0),0.2));
	}

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
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,0,15);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720/576,0.05,350);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  (*shader)["nglDiffuseShader"]->use();

  shader->setShaderParam4f("Colour",1,1,0,1);
  shader->setShaderParam3f("lightPos",1,1,1);
  shader->setShaderParam4f("lightDiffuse",1,1,1,1);

  (*shader)["nglColourShader"]->use();
  shader->setShaderParam4f("Colour",1,1,1,1);


  glEnable(GL_DEPTH_TEST); // for removal of hidden surfaces

  ngl::VAOPrimitives *prim =  ngl::VAOPrimitives::instance();
  prim->createSphere("sphere",1.0,40);

   // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglDiffuseShader"]->use();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_transformStack.getCurrentTransform().getMatrix()*m_mouseGlobalTX;
  MV=  M*m_cam->getViewMatrix();
  MVP=  MV*m_cam->getProjectionMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);
 }

void NGLScene::loadMatricesToColourShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglColourShader"]->use();
  ngl::Mat4 MV;
  ngl::Mat4 MVP;

  MV=m_transformStack.getCurrentTransform().getMatrix()
     *m_mouseGlobalTX*m_cam->getViewMatrix() ;
  MVP=MV*m_cam->getProjectionMatrix();
  shader->setShaderParamFromMat4("MVP",MVP);

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

	m_plane->draw("nglDiffuseShader",m_transformStack,m_cam);
	BOOST_FOREACH(Sphere s,m_sphereArray)
	{
		s.draw("nglDiffuseShader",m_mouseGlobalTX,m_cam);
	}
}

void NGLScene::update()
{
  static int updateCount=0;
  BOOST_FOREACH(Sphere &s,m_sphereArray)
  {
    s.move();

	}
	spherePlaneCollide();

	if(++updateCount==20)
	{
		updateCount=0;
		ngl::Vec3 pos;
		BOOST_FOREACH(Sphere &s,m_sphereArray)
		{
			ngl::Random *rng=ngl::Random::instance();
			pos.set(rng->randomNumber(6),8,rng->randomNumber(6));
			s.set(pos,ngl::Vec3(0,-1,0),0.2);
		}

	}

}
void NGLScene::timerEvent( QTimerEvent *_event )
{
	if(_event->timerId() == m_sphereUpdateTimer)
	{
		if (m_animate !=true)
		{
			return;
		}
		update();
		renderNow();
	}
}
void NGLScene::spherePlaneCollide()
{
ngl::Vec3 p;
GLfloat D;
BOOST_FOREACH(Sphere &s , m_sphereArray)
{
  p=s.getPos();

  //If a collision is found we change the m_dir of the Sphere then Break
  //with the new point P
  D=m_plane->getNormal().dot(p);
  //Now Add the radius of the sphere to the offsett
  D+=s.getRadius();
  // If this is greater or equal to the BBox extent /2 then there is a collision
  //So we calculate the Spheres new direction
  if(D <=0.0)
  {

  //we on the plane now see if we hit it or not
    if(s.getPos().m_x > m_plane->getCenter().m_x-(m_plane->getWidth()/2.0) &&
       s.getPos().m_x < m_plane->getCenter().m_x+(m_plane->getWidth()/2.0) &&
       s.getPos().m_z > m_plane->getCenter().m_z-(m_plane->getDepth()/2.0) &&
       s.getPos().m_z < m_plane->getCenter().m_z+(m_plane->getDepth()/2.0)
      )
      {

        s.setDirection(m_plane->getNormal());
        s.setHit();
      }//end of hit test
    }
  }//end of each face test
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
  case Qt::Key_Up    : m_plane->tilt(1.0,1,0); break;
  case Qt::Key_Down  : m_plane->tilt(-1.0,1,0); break;
  case Qt::Key_Left  : m_plane->tilt(-1.0,0,1); break;
  case Qt::Key_Right : m_plane->tilt(1.0,0,1); break;
  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
    renderLater();
}
