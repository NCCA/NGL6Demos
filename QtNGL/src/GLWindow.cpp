#include "GLWindow.h"
#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Light.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Material.h>
#include <ngl/ShaderLib.h>
#include <QColorDialog>


//----------------------------------------------------------------------------------------------------------------------
GLWindow::GLWindow(const QGLFormat _format, QWidget *_parent ) : QGLWidget( _format, _parent )
{

  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());
	m_wireframe=false;
	m_rotation=0.0;
	m_scale=1.0;
	m_position=0.0;

	m_selectedObject=0;

}

// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining display lists, etc.

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::initializeGL()
{

  ngl::NGLInit::instance();
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  /// create our camera
  ngl::Vec3 eye(2,2,2);
  ngl::Vec3 look(0,0,0);
  ngl::Vec3 up(0,1,0);

  m_camera = new ngl::Camera(eye,look,up);
  m_camera->setShape(45,float(1024/720),0.1,300);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Phong
  shader->createShaderProgram("Phong");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("PhongVertex",ngl::VERTEX);
  shader->attachShader("PhongFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("PhongVertex","shaders/Phong.vs");
  shader->loadShaderSource("PhongFragment","shaders/Phong.fs");
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
  shader->setShaderParam3f("viewerPos",m_camera->getEye().m_x,m_camera->getEye().m_y,m_camera->getEye().m_z);
  // now pass the modelView and projection values to the shader
  // the shader will use the currently active material and light0 so set them
  ngl::Material m(ngl::POLISHEDSILVER);
  m.loadToShader("material");
  // we need to set a base colour as the material isn't being used for all the params
  shader->setShaderParam4f("Colour",0.23125f,0.23125f,0.23125f,1);

  ngl::Light light(ngl::Vec3(2,2,2),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT);
  // now create our light this is done after the camera so we can pass the
  // transpose of the projection matrix to the light to do correct eye space
  // transformations
  ngl::Mat4 iv=m_camera->getViewMatrix();
  iv.transpose();
  light.setTransform(iv);
  light.setAttenuation(1,0,0);
  light.enable();
  // load these values to the shader as well
  light.loadToShader("light");

  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createSphere("sphere",1.0,40);
  m_text = new  ngl::Text(QFont("Arial",18));
  m_text->setScreenSize(this->size().width(),this->size().height());

}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void GLWindow::resizeGL( int _w, int _h )
{
  glViewport(0,0,_w,_h);
  m_camera->setShape(45,float(_w/_h),0.1,300);
  m_text->setScreenSize(this->size().width(),this->size().height());

}


void GLWindow::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_transform.getMatrix();
  MV=m_transform.getMatrix()*m_camera->getViewMatrix();
  MVP=MV*m_camera->getProjectionMatrix() ;
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void GLWindow::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
	if(m_wireframe == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	}
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();

	m_transform.setPosition(m_position);
	m_transform.setScale(m_scale);
	m_transform.setRotation(m_rotation);
	loadMatricesToShader();
	switch(m_selectedObject)
	{
		case 0 : prim->draw("teapot"); break;
		case 1 : prim->draw("sphere"); break;
		case 2 : prim->draw("cube"); break;
	}
	m_text->renderText(10,10,"Qt Gui Demo");
}





//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseMoveEvent ( QMouseEvent * _event )
{
  Q_UNUSED(_event);
}


//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent (QMouseEvent * _event  )
{
  Q_UNUSED(_event);

}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseReleaseEvent (  QMouseEvent * _event  )
{
  Q_UNUSED(_event);

}



GLWindow::~GLWindow()
{
}

void GLWindow::toggleWireframe(bool _mode	 )
{
	m_wireframe=_mode;
	updateGL();
}

void GLWindow::setXRotation( double _x		)
{
	m_rotation.m_x=_x;
	updateGL();
}

void GLWindow::setYRotation( double _y	)
{
	m_rotation.m_y=_y;
	updateGL();
}
void GLWindow::setZRotation( double _z )
{
	m_rotation.m_z=_z;
	updateGL();
}

void GLWindow::setXScale( double _x	)
{
	m_scale.m_x=_x;
	updateGL();
}

void GLWindow::setYScale(	 double _y)
{
	m_scale.m_y=_y;
	updateGL();
}
void GLWindow::setZScale( double _z )
{
	m_scale.m_z=_z;
	updateGL();
}

void GLWindow::setXPosition( double _x	)
{
	m_position.m_x=_x;
	updateGL();
}

void GLWindow::setYPosition( double _y	)
{
	m_position.m_y=_y;
	updateGL();
}
void GLWindow::setZPosition( double _z	 )
{
	m_position.m_z=_z;
	updateGL();
}

void GLWindow::setObjectMode(	int _i)
{
	m_selectedObject=_i;
	updateGL();
}
void GLWindow::setColour()
{
	QColor colour = QColorDialog::getColor();
	if( colour.isValid())
	{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();
    shader->setShaderParam4f("Colour",colour.redF(),colour.greenF(),colour.blueF(),1.0);
		updateGL();
	}
}
