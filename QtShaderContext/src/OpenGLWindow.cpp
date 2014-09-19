/*
 * Basic GL Window modified from the example here
 * http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
 * adapted to use NGL
 */
#include "OpenGLWindow.h"
#include <QtCore/QCoreApplication>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QSurfaceFormat>
#include <iostream>
#include <QWindow>

OpenGLWindow::OpenGLWindow(QWidget *_parent)
    : QWidget(_parent)
    , m_updatePending(false)
    , m_context(0)
    , m_device(0)
{
  // ensure we render to OpenGL and not a QPainter by setting the surface type
  //setSurfaceType(QWindow::OpenGLSurface);
}

OpenGLWindow::~OpenGLWindow()
{
  // now we have finished clear the device
  delete m_device;
}


void OpenGLWindow::renderLater()
{
  // this method allows us to post an event and render once the system is ready see the event processing
  // code below
  if (!m_updatePending)
  {
    m_updatePending = true;
    // signal and update request
    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
  }
}

bool OpenGLWindow::event(QEvent *event)
{
  switch (event->type())
  {
  case QEvent::UpdateRequest:
    renderNow();
    return true;

  //default:
    //return QWindow::event(event);
  }
}

//void OpenGLWindow::exposeEvent(QExposeEvent *event)
//{
//  // don't use the event
//  Q_UNUSED(event);
//  // if the window is exposed (visible) render
//  if (isExposed())
//  {
//    renderNow();
//  }
//}


void OpenGLWindow::renderNow()
{
  // no need to draw if window is hidden
  //if (!isExposed())
  //{
   // return;
  //}

  m_updatePending = false;

  bool needsInitialize = false;
  // if this is the first time round init the GL context this will only be called once
  if (!m_context)
  {
    // create an OpenGL format specifier
    QSurfaceFormat format;
    // set the number of samples for multisampling
    // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
    format.setSamples(4);
    #if defined( DARWIN)
      // at present mac osx Mountain Lion only supports GL3.2
      // the new mavericks will have GL 4.x so can change
      format.setMajorVersion(3);
      format.setMinorVersion(2);
    #else
      // with luck we have the latest GL version so set to this
      format.setMajorVersion(4);
      format.setMinorVersion(3);
    #endif
    // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
    format.setProfile(QSurfaceFormat::CoreProfile);
    // now set the depth buffer to 24 bits
    format.setDepthBufferSize(24);

    m_context = new QOpenGLContext(this);
    m_context->setFormat(format);
    m_context->create();

    needsInitialize = true;
    //m_context->makeCurrent(this);

    // now call the int method in our child class to do all the one time GL init stuff
    initialize();

  }
  // usually we will make this context current and render
 // m_context->makeCurrent(this);
  // call the render in the child class (NGLScene)
  render();
  // finally swap the buffers to make visible
  //m_context->swapBuffers(this);
}


