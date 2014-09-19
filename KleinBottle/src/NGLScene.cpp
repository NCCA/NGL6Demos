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
  setTitle("Klein Bottle taken from http://paulbourke.net/geometry/klein/");
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
  // we must call this first before any other GL commands to load and link the
  // gl commands from the lib, if this is not done program will crash
  ngl::NGLInit::instance();
  std::cout<<glGetString(GL_VERSION)<<std::endl;
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);

  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,12,40);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
   m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.05,350);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  shader->createShaderProgram("Phong");

  shader->attachShader("SimpleVertex",ngl::VERTEX);
  shader->attachShader("SimpleFragment",ngl::FRAGMENT);
  shader->loadShaderSource("SimpleVertex","shaders/PhongVertex.glsl");
  shader->loadShaderSource("SimpleFragment","shaders/PhongFragment.glsl");

  shader->compileShader("SimpleVertex");
  shader->compileShader("SimpleFragment");
  shader->attachShaderToProgram("Phong","SimpleVertex");
  shader->attachShaderToProgram("Phong","SimpleFragment");


  shader->linkProgramObject("Phong");
  (*shader)["Phong"]->use();
  // the shader will use the currently active material and light0 so set them
  ngl::Material m(ngl::GOLD);
  m.loadToShader("material");
  m_light = new ngl::Light(ngl::Vec3(2,2,2),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT);
  m_light->loadToShader("light");
  shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);


  createKleinBottle();
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());

}


/// @brief
/// the following two function have been adapted from the following
/// website
/// http://paulbourke.net/geometry/klein/


ngl::Vec3 NGLScene::eval(double u, double v)
{
  ngl::Vec3 p;
  double r;

  r = 4 * (1 - cos(u) / 2);
  if (u < M_PI)
  {
    p.m_x =  6 * cos(u) * (1 + sin(u)) + r * cos(u) * cos (v);
    p.m_y = 16 * sin(u) + r * sin(u) * cos(v);
  }
  else
  {
    p.m_x = 6 * cos(u) * (1 + sin(u)) + r * cos(v + M_PI);
    p.m_y = 16 * sin(u);
  }
  p.m_z = r * sin(v);

  return p;
}

struct vertData
{
  GLfloat u;
  GLfloat v;
  GLfloat nx;
  GLfloat ny;
  GLfloat nz;
  GLfloat x;
  GLfloat y;
  GLfloat z;
};
void NGLScene::createKleinBottle()
{
  m_vao =  ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
  double umin = 0,umax = ngl::TWO_PI, vmin = 0, vmax = ngl::TWO_PI;
  int i,j,N;
  double u,v,dudv=0.01;
  ngl::Vec3 p[4];
  ngl::Vec3 n[4];
  // a simple structure to hold our vertex data


  std::vector <vertData> data;

  N = 40;

  for (i=0;i<N;++i)
  {
    for (j=0;j<N;++j)
    {
      u = umin + i * (umax - umin) / (double)N;
      v = vmin + j * (vmax - vmin) / (double)N;
      p[0] = eval(u,v);
      n[0] = ngl::calcNormal(p[0],eval(u+dudv,v),eval(u,v+dudv));

      u = umin + (i+1) * (umax - umin) / (double)N;
      v = vmin + j     * (vmax - vmin) / (double)N;
      p[1] = eval(u,v);
      n[1] = ngl::calcNormal(p[1],eval(u+dudv,v),eval(u,v+dudv));

      u = umin + (i+1) * (umax - umin) / (double)N;
      v = vmin + (j+1) * (vmax - vmin) / (double)N;
      p[2] = eval(u,v);
      n[2] = ngl::calcNormal(p[2],eval(u+dudv,v),eval(u,v+dudv));

      u = umin + i     * (umax - umin) / (double)N;
      v = vmin + (j+1) * (vmax - vmin) / (double)N;
      p[3] = eval(u,v);
      n[3] = ngl::calcNormal(p[3],eval(u+dudv,v),eval(u,v+dudv));

      /* Write the geometry */

      vertData d;
      // tri 1

      d.x=p[0].m_x;
      d.y=p[0].m_y;
      d.z=p[0].m_z;

      d.nx=-n[0].m_x;
      d.ny=-n[0].m_y;
      d.nz=-n[0].m_z;
      data.push_back(d);

      d.x=p[1].m_x;
      d.y=p[1].m_y;
      d.z=p[1].m_z;

      d.nx=-n[1].m_x;
      d.ny=-n[1].m_y;
      d.nz=-n[1].m_z;
      data.push_back(d);

      d.x=p[2].m_x;
      d.y=p[2].m_y;
      d.z=p[2].m_z;

      d.nx=-n[2].m_x;
      d.ny=-n[2].m_y;
      d.nz=-n[2].m_z;
      data.push_back(d);


      /// tri 2
      d.x=p[2].m_x;
      d.y=p[2].m_y;
      d.z=p[2].m_z;

      d.nx=-n[2].m_x;
      d.ny=-n[2].m_y;
      d.nz=-n[2].m_z;
      data.push_back(d);

      d.x=p[3].m_x;
      d.y=p[3].m_y;
      d.z=p[3].m_z;

      d.nx=-n[3].m_x;
      d.ny=-n[3].m_y;
      d.nz=-n[3].m_z;
      data.push_back(d);

      d.x=p[0].m_x;
      d.y=p[0].m_y;
      d.z=p[0].m_z;

      d.nx=-n[0].m_x;
      d.ny=-n[0].m_y;
      d.nz=-n[0].m_z;
      data.push_back(d);
    }
  }
  m_vao->bind();
  unsigned int buffSize=data.size();
  // now we have our data add it to the VAO, we need to tell the VAO the following
  // how much (in bytes) data we are copying
  // a pointer to the first element of data (in this case the address of the first element of the
  // std::vector
  m_vao->setData(buffSize*sizeof(vertData),data[0].u);
  // in this case we have packed our data in interleaved format as follows
  // u,v,nx,ny,nz,x,y,z
  // If you look at the shader we have the following attributes being used
  // attribute vec3 inVert; attribute 0
  // attribute vec2 inUV; attribute 1
  // attribute vec3 inNormal; attribure 2
  // so we need to set the vertexAttributePointer so the correct size and type as follows
  // vertex is attribute 0 with x,y,z(3) parts of type GL_FLOAT, our complete packed data is
  // sizeof(vertData) and the offset into the data structure for the first x component is 5 (u,v,nx,ny,nz)..x
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(vertData),5);
  // uv same as above but starts at 0 and is attrib 1 and only u,v so 2
  m_vao->setVertexAttributePointer(1,2,GL_FLOAT,sizeof(vertData),0);
  // normal same as vertex only starts at position 2 (u,v)-> nx
  m_vao->setVertexAttributePointer(2,3,GL_FLOAT,sizeof(vertData),2);
  // now we have set the vertex attributes we tell the VAO class how many indices to draw when
  // glDrawArrays is called, in this case we use buffSize (but if we wished less of the sphere to be drawn we could
  // specify less (in steps of 3))
  m_vao->setNumIndices(buffSize);
  // finally we have finished for now so time to unbind the VAO
  m_vao->unbind();
}
/// End of citation


void NGLScene::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();
  // clear the screen and depth buffer
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


  m_transformStack.pushTransform();
  {
    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    ngl::Mat4 M;
    M=m_transformStack.getCurrentTransform().getMatrix()*m_mouseGlobalTX;
    MV=  M*m_cam->getViewMatrix();
    MVP= M*m_cam->getVPMatrix();
    normalMatrix=MV;
    normalMatrix.inverse();
    shader->setShaderParamFromMat4("MV",MV);
    shader->setShaderParamFromMat4("MVP",MVP);
    shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
    shader->setShaderParamFromMat4("M",M);    m_vao->bind();
    m_vao->draw();

    // now we are done so unbind
    m_vao->unbind();
  } // and before a pop
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
