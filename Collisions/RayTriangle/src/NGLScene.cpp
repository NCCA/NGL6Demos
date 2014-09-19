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
#include <boost/foreach.hpp>

//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1;

NGLScene::NGLScene(int _numTriangles,QWindow *_parent) : OpenGLWindow(_parent)
{
  m_numTriangles=_numTriangles;

  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  setTitle("Ray->Triangle Intersections");
  // create the points for our ray
  m_rayStart.set(0,0,0.2);
  m_rayEnd.set(0,0,-20);

}


NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  Init->NGLQuit();
  // delete the triangles to clear the VAO's
  BOOST_FOREACH(Triangle *t, m_triangleArray)
  {
    delete t;
  }

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
  ngl::Vec3 from(0,1,15);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.5,150);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglDiffuseShader"]->use();

  shader->setShaderParam4f("Colour",1,1,1,1);
  shader->setShaderParam3f("lightPos",1,1,1);
  shader->setShaderParam4f("lightDiffuse",1,1,1,1);

  (*shader)["nglColourShader"]->use();
  shader->setShaderParam4f("Colour",1,1,1,1);

  glEnable(GL_DEPTH_TEST); // for removal of hidden surfaces

  ngl::VAOPrimitives *prim =  ngl::VAOPrimitives::instance();

  prim->createSphere("smallSphere",0.05,10);

  // create our triangles, as we create a VAO we need a valid gl context
  ngl::Random *rng=ngl::Random::instance();

  for (int i=0; i<m_numTriangles; ++i)
  {
   ngl::Vec3 c(rng->randomNumber(10),rng->randomNumber(10),-rng->randomPositiveNumber(10));
   ngl::Vec3 v0(rng->randomNumber(2)+0.1,rng->randomNumber(2)+0.1,-rng->randomPositiveNumber(2)+0.1);
   ngl::Vec3 v1(rng->randomNumber(2)+0.1,rng->randomNumber(2)+0.1,-rng->randomPositiveNumber(2)+0.1);
   ngl::Vec3 v2(rng->randomNumber(2)+0.1,rng->randomNumber(2)+0.1,-rng->randomPositiveNumber(2)+0.1);
   m_triangleArray.push_back(new Triangle(c+v0,c+v1,c+v2));
  }
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglDiffuseShader"]->use();

  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;

  MVP=  m_transformStack.getCurrentTransform().getMatrix()
        *m_mouseGlobalTX*m_cam->getVPMatrix();
  normalMatrix=m_transformStack.getCurrentTransform().getMatrix()
               *m_mouseGlobalTX*m_cam->getViewMatrix();
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
}

void NGLScene::loadMatricesToColourShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglColourShader"]->use();
  ngl::Mat4 MVP;

  MVP=m_transformStack.getCurrentTransform().getMatrix()
      *m_mouseGlobalTX*m_cam->getVPMatrix() ;
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

  // grab an instance of the primitives for drawing
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  // Rotation based on the mouse position for our global
  // transform
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("nglDiffuseShader");

	// draw a cube at the ray start points
	m_transformStack.pushTransform();
	{
		shader->setRegisteredUniform4f("Colour",1,1,1,1);
		m_transformStack.setPosition(m_rayStart);
		loadMatricesToShader();
		prim->draw("cube");
	} // and before a pop
	m_transformStack.popTransform();

	// we build up a vertex array for the lines of the start and end points and draw
	m_transformStack.pushTransform();
	{
		ngl::VertexArrayObject *vao= ngl::VertexArrayObject::createVOA(GL_LINES);
		vao->bind();
		ngl::Vec3 points[2];
		points[0]=m_rayStart;
		points[1]=m_rayEnd;
		vao->setData(2*sizeof(ngl::Vec3),points[0].m_x);
		vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(ngl::Vec3),0);
		vao->setNumIndices(2);
		loadMatricesToColourShader();
		vao->draw();
		vao->removeVOA();
		delete vao;
	}
	m_transformStack.popTransform();
	// draw all the triangles
	shader->use("nglDiffuseShader");
	BOOST_FOREACH(Triangle *t, m_triangleArray)
	{
		shader->setRegisteredUniform4f("Colour",1,1,0,0);
		t->rayTriangleIntersect(m_rayStart,m_rayEnd);
		t->draw("nglDiffuseShader",m_mouseGlobalTX,m_cam);
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
  static float s_increment=0.5;
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
    // escape key to quite
    case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
    case Qt::Key_Up : m_rayEnd.m_y+=s_increment; break;
    case Qt::Key_Down : m_rayEnd.m_y-=s_increment; break;
    case Qt::Key_Left : m_rayEnd.m_x-=s_increment; break;
    case Qt::Key_Right : m_rayEnd.m_x+=s_increment; break;
    case Qt::Key_W : m_rayStart.m_y+=s_increment; break;
    case Qt::Key_Z : m_rayStart.m_y-=s_increment; break;
    case Qt::Key_A : m_rayStart.m_x-=s_increment; break;
    case Qt::Key_S : m_rayStart.m_x+=s_increment; break;


    default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
    renderLater();
}
