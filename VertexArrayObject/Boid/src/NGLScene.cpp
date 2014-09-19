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
}


NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  Init->NGLQuit();
  m_vao->removeVOA();
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
	glEnable(GL_MULTISAMPLE);
	// Now we will create a basic Camera from the graphics library
	// This is a static camera so it only needs to be set once
	// First create Values for the camera position
	ngl::Vec3 from(0,1,2);
	ngl::Vec3 to(0,0,0);
	ngl::Vec3 up(0,1,0);

	m_cam= new ngl::Camera(from,to,up);
	// set the shape using FOV 45 Aspect Ratio based on Width and Height
	// The final two are near and far clipping planes of 0.5 and 10
	m_cam->setShape(45,(float)720.0/576.0,0.001,150);

// now to load the shader and set the values
	// grab an instance of shader manager
	ngl::ShaderLib *shader=ngl::ShaderLib::instance();
	shader->use("nglColourShader");
	shader->setRegisteredUniform4f("Colour",1,1,1,1);
	buildVAO();
	glViewport(0,0,width(),height());
}


void NGLScene::buildVAO()
{
  ngl::Vec3 verts[]=
  {
    ngl::Vec3(0,1,1),
    ngl::Vec3(0,0,-1),
    ngl::Vec3(-0.5,0,1),
    ngl::Vec3(0,1,1),
    ngl::Vec3(0,0,-1),
    ngl::Vec3(0.5,0,1),
    ngl::Vec3(0,1,1),
    ngl::Vec3(0,0,1.5),
    ngl::Vec3(-0.5,0,1),
    ngl::Vec3(0,1,1),
    ngl::Vec3(0,0,1.5),
    ngl::Vec3(0.5,0,1)

  };
  std::cout<<"sizeof(verts) "<<sizeof(verts)<<" sizeof(ngl::Vec3) "<<sizeof(ngl::Vec3)<<"\n";
  // create a vao as a series of GL_TRIANGLES
  m_vao= ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
  m_vao->bind();

   // in this case we are going to set our data as the vertices above

   m_vao->setData(sizeof(verts),verts[0].m_x);
   // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)

   m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);


   m_vao->setNumIndices(sizeof(verts)/sizeof(ngl::Vec3));

 // now unbind
  m_vao->unbind();

}



void NGLScene::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Rotation based on the mouse position for our global transform
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
  (*shader)["nglColourShader"]->use();

  ngl::Mat4 MVP;
  MVP=m_transformStack.getCurrentTransform().getMatrix()
      *m_mouseGlobalTX*m_cam->getVPMatrix();

  shader->setShaderParamFromMat4("MVP",MVP);


  m_vao->bind();
  m_vao->draw();
  m_vao->unbind();
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
