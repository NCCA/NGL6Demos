#include "GLWindow.h"
#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Quaternion.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Util.h>


//----------------------------------------------------------------------------------------------------------------------
GLWindow::GLWindow(const QGLFormat _format, QWidget *_parent ) : QGLWidget( _format, _parent )

{

  // set this widget to have the initial keyboard focus
  setFocus();
  // set some initial values for the gui, this really should be harvested from
  // the GUI to avoid things getting out of sync
  m_interp=0.0;
  m_srotation.set(45,90,80);
  m_erotation.set(-300,270,360);
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
  /// create our camera in this case we are generating it from
  /// the util lookAt function as the camera doesn't move
  ngl::Vec3 eye(0,0,3);
  ngl::Vec3 look(0,0,0);
  ngl::Vec3 up(0,1,0);
  m_lookAt=ngl::lookAt(eye,look,up);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are going to use the built in nglDiffuse shader
  (*shader)["nglDiffuseShader"]->use();
  // set the directional light position colour and diffuse
  shader->setRegisteredUniform("lightPos",1.0f,1.0f,1.0f);
  shader->setRegisteredUniform("Colour",1.0f,0.0f,0.0f,1.0f);
  shader->setRegisteredUniform("lightDiffuse",1.0f,1.0f,1.0f,1.0f);
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void GLWindow::resizeGL(int _w,int _h)
{
  glViewport(0,0,_w,_h);
  m_projection=ngl::infinitePerspective(60.0f,1024.0/720.0,0.2f);
}


void GLWindow::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglDiffuseShader"]->use();

  ngl::Mat4 MVP;
  ngl::Mat4 MV;
  ngl::Mat3 normalMatrix;
  MV=m_transformStack.getCurrentTransform().getMatrix()*m_lookAt;
  MVP=MV*m_projection;
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setRegisteredUniform("MVP",MVP);
  shader->setRegisteredUniform("normalMatrix",normalMatrix);
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void GLWindow::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  // grab the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglDiffuseShader"]->use();


  // create some matrices to help with our rotations
  ngl::Mat4 start;
  ngl::Mat4 sx,sy,sz;
  ngl::Mat4 end;
  ngl::Mat4 ex,ey,ez;
  // we are going to create 3 rotation matrices and combine them
  // using x*y*z for our start rotation
  sx.rotateX(m_srotation.m_x);
  sy.rotateY(m_srotation.m_y);
  sz.rotateZ(m_srotation.m_z);
  start=sx*sy*sz;
  // same for the end rotation
  ex.rotateX(m_erotation.m_x);
  ey.rotateY(m_erotation.m_y);
  ez.rotateZ(m_erotation.m_z);
  end=ex*ey*ez;
  // just to demonstrate we can construct from a vec3 with x,y,z
  ngl::Quaternion startQuat(m_srotation);
  // or a matrix the ends are the same
  ngl::Quaternion endQuat(end);

  // Now we interpolate between the start and end quat using a value
  // from 0-1 this will return a quat
  ngl::Quaternion i=ngl::Quaternion::slerp(startQuat,endQuat,m_interp);
  // convert this Quat into a matrix and use this for the final
  // rotation
  ngl::Mat4 tx=i.toMat4();
  // now set this matrix to be the current transform for the tx stack
  // we now load this to the shaders
  m_transformStack.setCurrent(tx);
  shader->setRegisteredUniform("Colour",1.0f,1.0f,0.0f,1.0f);
  loadMatricesToShader();
  prim->draw("teapot");

  // in this case I will convert to a rotation matrix from the quat
  tx=startQuat.toMat4();
  // and set the position in the x to the teapot is on the left
  tx.m_30=-2.0;
  m_transformStack.setCurrent(tx);
  shader->setRegisteredUniform("Colour",1.0f,0.0f,0.0f,1.0f);
  loadMatricesToShader();
  prim->draw("teapot");
  // in this case we use the normal tx stack version instead
  m_transformStack.setPosition(2.0f,0.0f,0.0f);
  m_transformStack.setRotation(m_erotation);
  shader->setRegisteredUniform("Colour",0.0f,1.0f,0.0f,1.0f);

  loadMatricesToShader();
  prim->draw("teapot");


  //now do all the text ui stuff
  QString s;
  s.sprintf("%f [%f,%f,%f]",startQuat.getS(),startQuat.getX(),startQuat.getY(),startQuat.getZ());
  // once we set the text emit it to the MainWindow where things are attatched
  emit changeStartQuat(s);
  s.sprintf("%f [%f,%f,%f]",endQuat.getS(),endQuat.getX(),endQuat.getY(),endQuat.getZ());
  emit changeEndQuat(s);
  s.sprintf("%f [%f,%f,%f]",i.getS(),i.getX(),i.getY(),i.getZ());
  emit changeInterpQuat(s);
  emit clearMatrixText();
  s.sprintf("[%+0.5f %+0.5f,%+0.5f,%+0.5f]",tx.m_00,tx.m_01,tx.m_02,tx.m_03);
  emit appendMatrixText(s);
  s.sprintf("[%+0.5f %+0.5f,%+0.5f,%+0.5f]",tx.m_10,tx.m_11,tx.m_12,tx.m_13);
  emit appendMatrixText(s);
  s.sprintf("[%+0.5f %+0.5f,%+0.5f,%+0.5f]",tx.m_20,tx.m_21,tx.m_22,tx.m_23);
  emit appendMatrixText(s);
  s.sprintf("[%+0.5f %+0.5f,%+0.5f,%+0.5f]",tx.m_30,tx.m_31,tx.m_32,tx.m_33);
  emit appendMatrixText(s);

  s.sprintf("Interpolate value = %0.2f",m_interp);
  emit updateInterpText(s);


}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseMoveEvent (
                               QMouseEvent * _event
                              )
{
  Q_UNUSED(_event);
}


//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent (
                                QMouseEvent * _event
                               )
{
  Q_UNUSED(_event);
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseReleaseEvent (
                                  QMouseEvent * _event
                                 )
{
  Q_UNUSED(_event);
}


GLWindow::~GLWindow()
{
  ngl::NGLInit *init = ngl::NGLInit::instance();
  init->NGLQuit();
}

