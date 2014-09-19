/// @file GLWindow.cpp
/// @brief basic implementation file for the GLWindow class
#include "GLWindow.h"
#include <iostream>
#include <ngl/Light.h>
#include <ngl/TransformStack.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>

#include <QDebug>
const static float INCREMENT=0.01;
const static float ZOOM=0.1;

const  std::string GLWindow::s_vboNames[17]=
{
  "sphere",
  "cylinder",
  "cone",
  "disk",
  "plane",
  "torus",
  "teapot",
  "octahedron",
  "dodecahedron",
  "icosahedron",
  "tetrahedron",
  "football",
  "cube",
  "troll",
  "buddah",
  "dragon",
  "bunny"
};



//----------------------------------------------------------------------------------------------------------------------
GLWindow::GLWindow(const QGLFormat _format, QWidget *_parent ) : QGLWidget( _format, _parent )
{

  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());
  // Now set the initial GLWindow attributes to default values
  // Roate is false
  m_rotateActive=false;
  m_translateActive=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  m_origX=0;
  m_origY=0;
  m_drawIndex=6;
  m_drawNormals=false;
  /// set all our matrices to the identity
  m_transform=1.0;
  m_rotate=1.0;
  m_translate=1.0;
  m_scale=1.0;
  m_normalSize=6.0;
  m_colour.set(0.5,0.5,0.5);
  m_material.setDiffuse(m_colour);
  m_material.setSpecular(ngl::Colour(0.2,0.2,0.2));
  m_material.setAmbient(ngl::Colour());
  m_material.setSpecularExponent(20);
  m_material.setRoughness(0);
  m_matrixOrder=GLWindow::RTS;
  m_euler=1.0;
  m_modelPos.set(0,0,0);
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
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,0,8);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);


  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.5,10);
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
  // as we now use glsl 400 and layout qualifiers we don't need this anymore
//  shader->bindAttribute("Phong",0,"inVert");
//  // attribute 1 is the UV data u,v (if present)
//  shader->bindAttribute("Phong",1,"inUV");
//  // attribute 2 are the normals x,y,z
//  shader->bindAttribute("Phong",2,"inNormal");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Phong");
  // and make it active ready to load values
  (*shader)["Phong"]->use();

  // now create our light this is done after the camera so we can pass the
  // transpose of the projection matrix to the light to do correct eye space
  // transformations
  ngl::Mat4 iv=m_cam->getProjectionMatrix();
  iv.transpose();
  m_light = new ngl::Light(ngl::Vec3(-2,2,-2),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT);
  m_light->setTransform(iv);
  // load these values to the shader as well
  m_light->loadToShader("light");
  shader->createShaderProgram("Colour");

   shader->attachShader("ColourVertex",ngl::VERTEX);
   shader->attachShader("ColourFragment",ngl::FRAGMENT);
   shader->loadShaderSource("ColourVertex","shaders/ColourVertex.glsl");
   shader->loadShaderSource("ColourFragment","shaders/ColourFragment.glsl");

   shader->compileShader("ColourVertex");
   shader->compileShader("ColourFragment");
   shader->attachShaderToProgram("Colour","ColourVertex");
   shader->attachShaderToProgram("Colour","ColourFragment");
   // as we now use glsl 400 and layout qualifiers we don't need this anymore
  //   shader->bindAttribute("Colour",0,"inVert");
  //   shader->bindAttribute("Colour",1,"inUV");
  //   shader->bindAttribute("Colour",2,"inNormal");


   shader->linkProgramObject("Colour");
   (*shader)["Colour"]->use();
  /// now create our primitives for drawing later

  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createSphere("sphere",1.0,40);
  prim->createCylinder("cylinder",0.5,1.4,40,40);
  prim->createCone("cone",0.5,1.4,20,20);
  prim->createDisk("disk",0.5,40);
  prim->createTrianglePlane("plane",1,1,10,10,ngl::Vec3(0,1,0));
  prim->createTorus("torus",0.15,0.4,40,40);
  // set the bg colour
  glClearColor(0.5,0.5,0.5,0.0);
  m_axis = new Axis("Colour",1.5);
  m_axis->setCam(m_cam);
  // load the normal shader
  shader->createShaderProgram("normalShader");

  shader->attachShader("normalVertex",ngl::VERTEX);
  shader->attachShader("normalFragment",ngl::FRAGMENT);
  shader->loadShaderSource("normalVertex","shaders/normalVertex.glsl");
  shader->loadShaderSource("normalFragment","shaders/normalFragment.glsl");

  shader->compileShader("normalVertex");
  shader->compileShader("normalFragment");
  shader->attachShaderToProgram("normalShader","normalVertex");
  shader->attachShaderToProgram("normalShader","normalFragment");

  shader->attachShader("normalGeo",ngl::GEOMETRY);
  shader->loadShaderSource("normalGeo","shaders/normalGeo.glsl");
  shader->compileShader("normalGeo");
  shader->attachShaderToProgram("normalShader","normalGeo");
  // as we now use glsl 400 and layout qualifiers we don't need this anymore
//  shader->("normalShader",0,"inVert");
//  shader->bindAttribute("normalShader",1,"inUV");
//  shader->bindAttribute("normalShader",2,"inNormal");

  shader->linkProgramObject("normalShader");
  shader->use("normalShader");
  // now pass the modelView and projection values to the shader
  shader->setShaderParam1f("normalSize",0.1);
  shader->setShaderParam4f("vertNormalColour",1,1,0,1);
  shader->setShaderParam4f("faceNormalColour",1,0,0,1);

  shader->setShaderParam1i("drawFaceNormals",true);
  shader->setShaderParam1i("drawVertexNormals",true);
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void GLWindow::resizeGL(int _w, int _h )
{
  glViewport(0,0,_w,_h);
  m_cam->setShape(45,(float)_w/_h,0.05,450);

}

void GLWindow::loadMatricesToShader( )
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();
  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_transform*m_mouseGlobalTX;
  MV=M*m_cam->getViewMatrix();
  MVP=  MV*m_cam->getProjectionMatrix();
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
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Rotation based on the mouse position for our global
  // transform

  m_light->enable();

  m_transform.identity();

  if (m_matrixOrder == GLWindow::RTS)
  {
    m_transform=m_rotate*m_translate*m_scale;
  }

  else if (m_matrixOrder == GLWindow::TRS)
  {
    m_transform=m_translate*m_rotate*m_scale;
  }
  else if (m_matrixOrder == GLWindow::EULERTS)
  {
    m_transform=m_translate*m_euler*m_scale;
  }
  else if (m_matrixOrder == GLWindow::TEULERS)
  {
    m_transform=m_euler*m_translate*m_scale;
  }

  else if (m_matrixOrder == GLWindow::GIMBALLOCK )
  {
    m_transform=m_translate*m_gimbal*m_scale;
  }
  emit matrixDirty(m_transform);
  // now set this value in the shader for the current ModelMatrix
  (*shader)["Phong"]->use();

  m_material.loadToShader("material");


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


       loadMatricesToShader();
      // get the VBO instance and draw the built in teapot
      ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
      prim->draw( s_vboNames[m_drawIndex]);

      if(m_drawNormals)
      {
        (*shader)["normalShader"]->use();
        ngl::Mat4 MV;
        ngl::Mat4 MVP;

        MV=m_transform*m_mouseGlobalTX* m_cam->getViewMatrix();
        MVP=MV*m_cam->getProjectionMatrix();
        shader->setShaderParamFromMat4("MVP",MVP);
        shader->setShaderParam1f("normalSize",m_normalSize/10.0);

        prim->draw( s_vboNames[m_drawIndex]);
      }




  m_axis->draw(m_mouseGlobalTX);



}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseMoveEvent (QMouseEvent * _event  )
{
  // note the method buttons() is the button state when event was called
  // this is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
  if(m_rotateActive && _event->buttons() == Qt::LeftButton)
  {
    m_spinYFace = ( m_spinYFace + (_event->x() - m_origX) ) % 360 ;
    m_spinXFace = ( m_spinXFace + (_event->y() - m_origY) ) % 360 ;
    m_origX = _event->x();
    m_origY = _event->y();

    updateGL();

  }
  // right mouse translate code
  else if(m_translateActive && _event->buttons() == Qt::RightButton)
  {
    int diffX = (int)(_event->x() - m_origXPos);
    int diffY = (int)(_event->y() - m_origYPos);
    m_origXPos=_event->x();
    m_origYPos=_event->y();
    m_modelPos.m_x += INCREMENT * diffX;
    m_modelPos.m_y -= INCREMENT * diffY;
    updateGL();

  }

}


//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent (QMouseEvent * _event )
{
  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if(_event->button() == Qt::LeftButton)
  {
    m_origX = _event->x();
    m_origY = _event->y();
    m_rotateActive =true;
  }
  // right mouse translate mode
  else if(_event->button() == Qt::RightButton)
  {
    m_origXPos = _event->x();
    m_origYPos = _event->y();
    m_translateActive=true;
  }

}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseReleaseEvent (QMouseEvent * _event )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_rotateActive=false;
  }
  // right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_translateActive=false;
  }
}

void GLWindow::wheelEvent(QWheelEvent *_event)
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
  updateGL();
}


//----------------------------------------------------------------------------------------------------------------------
void GLWindow::vboChanged( int _index  )
{
  m_drawIndex=_index;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::toggleNormals(bool _value )
{
  m_drawNormals=_value;
  updateGL();
}


//----------------------------------------------------------------------------------------------------------------------
void GLWindow::toggleWireframe( bool _value )
{
   if( _value == true)
   {
     glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
   }
   else
   {
     glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   }
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setNormalSize(int _value)
{
  m_normalSize=_value;
  updateGL();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setScale(float _x, float _y,float _z )

{
  m_scale.scale(_x,_y,_z);
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setTranslate(float _x, float _y, float _z )
{

  m_translate.translate(_x,_y,_z);
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setRotate(float _x,  float _y, float _z  )
{
  ngl::Mat4 rx=1.0;
  ngl::Mat4 ry=1.0;
  ngl::Mat4 rz=1.0;
  rx.rotateX(_x);
  ry.rotateY(_y);
  rz.rotateZ(_z);
  m_rotate=rz*ry*rx;
  // now for the incorrect gimbal 1
  m_gimbal.identity();
  m_gimbal.rotateX(_x);
  m_gimbal.rotateY(_y);
  m_gimbal.rotateZ(_z);

  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setColour( float _r,   float _g,  float _b  )
{
  m_colour.set(_r,_g,_b);
  m_material.setDiffuse(m_colour);
  m_material.loadToShader("material");
  updateGL();
}


//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setMatrixOrder(int _index  )
{
  switch(_index)
  {
    case 0 : { m_matrixOrder=GLWindow::RTS;   break; }
    case 1 : { m_matrixOrder=GLWindow::TRS;   break; }
    case 2 : { m_matrixOrder=GLWindow::GIMBALLOCK; break; }
    case 3 : { m_matrixOrder=GLWindow::EULERTS; break; }
    case 4 : { m_matrixOrder=GLWindow::TEULERS; break; }
    default : break;
  }
updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setEuler(float _angle, float _x,  float _y,  float _z )
{
  m_euler=1.0;
  m_euler.euler(_angle,_x,_y,_z);
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::resetMouse()
{
  m_spinXFace=0;
  m_spinYFace=0;
  m_origX=0;
  m_origY=0;
  updateGL();
}
//----------------------------------------------------------------------------------------------------------------------
