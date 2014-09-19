#include <QMouseEvent>
#include <QGuiApplication>

#include "VideoWindow.h"
#include "QKinect.h"
#include "Context.h"
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

VideoWindow::VideoWindow(DrawModes _mode, QWindow *_parent) : OpenGLWindow(_parent)
{
  m_mode=_mode;
  m_rgb.resize(640*480*3);
  startTimer(0);
}


VideoWindow::~VideoWindow()
{

}

void VideoWindow::resizeEvent(QResizeEvent *_event )
{
  if(isExposed())
  {
  int w=_event->size().width();
  int h=_event->size().height();
  // set the viewport for openGL
  glViewport(0,0,w,h);
  renderLater();
  }
}


void VideoWindow::initialize()
{

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
  glGenTextures(1, & m_rgbTexture);
  glActiveTextureARB(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, m_rgbTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void VideoWindow::loadMatricesToShader()
{

}

void VideoWindow::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QKinect *kinect=QKinect::instance();

  if(m_mode == RGB)
  {
    kinect->getRGB(m_rgb);
  }
  else if(m_mode == DEPTH)
  {
    kinect->getDepth (m_rgb);
  }

  glBindTexture(GL_TEXTURE_2D, m_rgbTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &m_rgb[0]);
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("Colour");
  glGenerateMipmap(GL_TEXTURE_2D);

  Context::instance(this)->drawScreenQuad();

}

//----------------------------------------------------------------------------------------------------------------------
void VideoWindow::mouseMoveEvent (QMouseEvent * _event)
{
}


//----------------------------------------------------------------------------------------------------------------------
void VideoWindow::mousePressEvent ( QMouseEvent * _event)
{

}

//----------------------------------------------------------------------------------------------------------------------
void VideoWindow::mouseReleaseEvent ( QMouseEvent * _event )
{

}

//----------------------------------------------------------------------------------------------------------------------
void VideoWindow::wheelEvent(QWheelEvent *_event)
{


}
//----------------------------------------------------------------------------------------------------------------------

void VideoWindow::keyPressEvent(QKeyEvent *_event)
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

void VideoWindow::timerEvent(QTimerEvent *_event)
{
  renderNow();
}
