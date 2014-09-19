#include <QMouseEvent>
#include <QGuiApplication>

#include "PointCloud.h"
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/TransformStack.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include "QKinect.h"


//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1;

PointCloud::PointCloud(QWindow *_parent) : OpenGLWindow(_parent)
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  m_depth.resize(640*480);
  startTimer(0);
  m_rgb.resize(640*480*3);

}


PointCloud::~PointCloud()
{

}

void PointCloud::resizeEvent(QResizeEvent *_event )
{
  if(isExposed())
  {
  int w=_event->size().width();
  int h=_event->size().height();
  // set the viewport for openGL
  glViewport(0,0,w,h);
  // now set the camera size values as the screen size has changed
  m_cam->setShape(35,(float)w/h,0.01,20);
  renderLater();
  }
}


void PointCloud::initialize()
{

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,0.5,-0.5);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  // now load to our new camera
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(35,(float)640.0/480.0,0.001,10);

  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());

  std::vector <pcData> pointCloud;
  float yStep=2.0/480.0;
  float xStep=2.0/640.0;
  float xPos=-1.0;
  float yPos=1.0;
  float uStep=1.0/640.0;
  float vStep=1.0/480.0;
  float u=0.0;
  float v=0.0;
  pcData p;


  for(int y=0; y<480; ++y)
  {
    for(int x=0; x<640; ++x)
    {
      p.point=ngl::Vec3(xPos,yPos,0.0);
      p.uv=ngl::Vec2(u,v);
      pointCloud.push_back(p);
      xPos+=xStep;
      u+=uStep;
    }
    xPos=-1.0;
    yPos-=yStep;
    u=0.0;
    v+=vStep;
  }

  m_vao= ngl::VertexArrayObject::createVOA(GL_POINTS);
  // next we bind it so it's active for setting data
  m_vao->bind();
  // now we have our data add it to the VAO, we need to tell the VAO the following
  // how much (in bytes) data we are copying
  // a pointer to the first element of data (in this case the address of the first element of the
  // std::vector
  m_vao->setData(pointCloud.size()*sizeof(pcData),pointCloud[0].point.m_x);
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(pcData),0);
  m_vao->setVertexAttributePointer(1,2,GL_FLOAT,sizeof(pcData),3);
  m_vao->setNumIndices(pointCloud.size());
  m_vao->unbind();
  glGenTextures(1, & m_rgbTexture);
  glActiveTextureARB(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, m_rgbTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}


void PointCloud::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["PointCloud"]->use();

  ngl::Mat4 MVP;
  MVP= m_transformStack.getCurrAndGlobal().getMatrix()*m_cam->getVPMatrix();
  shader->setShaderParamFromMat4("MVP",MVP);
}

float PointCloud::rawDepthToMeters(int depthValue)
{
  if (depthValue < 2047) {
    return (float)(1.0 / ((double)(depthValue) * -0.0030711016 + 3.3309495161));
  }
  return 0.0f;
}

void PointCloud::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // grab an instance of the shader manager

  // Rotation based on the mouse position for our global transform
  ngl::Transformation trans;
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  // multiply the rotations
  ngl::Mat4 final=rotY*rotX;
  // add the translations
  final.m_m[3][0] = m_modelPos.m_x;
  final.m_m[3][1] = m_modelPos.m_y;
  final.m_m[3][2] = -m_modelPos.m_z;
  // set this in the TX stack
  trans.setMatrix(final);
  m_transformStack.setGlobal(trans);

  loadMatricesToShader();
  QKinect *kinect=QKinect::instance();

  kinect->getDepth16Bit(m_depth);
  m_vao->bind();
  pcData *data=(pcData *)m_vao->getDataPointer(0);
  glPointSize(2.0);
  // code modified from
  // http://www.shirky.com/weblog/kinect-and-processing/
  double fx_d = 1.0 / 5.9421434211923247e+02;
  double fy_d = 1.0 / 5.9104053696870778e+02;
  double cx_d = 3.3930780975300314e+02;
  double cy_d = 2.4273913761751615e+02;

  int i=0;
  for(int y=480; y>0; --y)
  {
    for(int x=0; x<640; ++x)
    {
      float depthValue=rawDepthToMeters(m_depth[i]);
      data[i].point.m_x= (float)( (x - cx_d) * depthValue * fx_d);
      data[i].point.m_y=(float)((y - cy_d) * depthValue * fy_d);
      data[i].point.m_z=depthValue*0.4;
      ++i;
    }
  }
  m_vao->freeDataPointer();

  kinect->getRGB(m_rgb);

  glBindTexture(GL_TEXTURE_2D, m_rgbTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &m_rgb[0]);
  glGenerateMipmap(GL_TEXTURE_2D);


  m_vao->draw();
  // finally we have finished for now so time to unbind the VAO
  m_vao->unbind();

}

//----------------------------------------------------------------------------------------------------------------------
void PointCloud::mouseMoveEvent (QMouseEvent * _event)
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
void PointCloud::mousePressEvent ( QMouseEvent * _event)
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
void PointCloud::mouseReleaseEvent ( QMouseEvent * _event )
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
void PointCloud::wheelEvent(QWheelEvent *_event)
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

void PointCloud::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : this->close(); break;
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

void PointCloud::timerEvent(QTimerEvent *)
{
  renderNow();
}
