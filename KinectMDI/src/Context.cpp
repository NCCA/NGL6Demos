#include "Context.h"
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <QtGui/QOpenGLContext>
#include <ngl/NGLInit.h>
#include <ngl/Material.h>
#include <QWindow>
#include "OpenGLWindow.h"
#include <ngl/NGLInit.h>
#include "ScreenQuad.h"

//----------------------------------------------------------------------------------------------------------------------
// set the static variables to default values
//----------------------------------------------------------------------------------------------------------------------
Context *Context::s_instance=0;
QOpenGLContext *Context::m_context;
QSurfaceFormat Context::m_format;
bool Context::m_formatSet=false;

//----------------------------------------------------------------------------------------------------------------------
Context *Context::instance(OpenGLWindow *_parent)
{
  // using the Double Check locking pattern to make thread safe
  // http://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf
  if (s_instance == 0)
  {
    // create a mutux to stop other threads accessing
    QMutex m;
    // the locker will auto unlock when out of scope
    QMutexLocker locker(&m);

    if(s_instance ==0)
    {
      s_instance = new Context(_parent);
    }
  }
 return s_instance;
}


//----------------------------------------------------------------------------------------------------------------------
void Context::defaultFormat()
{
  m_format.setSamples(4);
  m_format.setMajorVersion(4);
  m_format.setMinorVersion(1);
  // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
  m_format.setProfile(QSurfaceFormat::CoreProfile);
  // now set the depth buffer to 24 bits
  m_format.setDepthBufferSize(24);
  m_format.setSwapBehavior(QSurfaceFormat::TripleBuffer);

  m_formatSet=true;
}

//----------------------------------------------------------------------------------------------------------------------
void Context::setFormat(QSurfaceFormat &_format)
{
  m_format=_format;
  m_formatSet=true;
}


//----------------------------------------------------------------------------------------------------------------------
Context::Context(OpenGLWindow *_window)
{
  if(!m_formatSet)
  {
    std::cerr<<"using default format\n";
    defaultFormat();
  }
  std::cout<<"creating context\n";
  m_context = new QOpenGLContext(_window);
  m_context->setFormat(m_format);
  m_context->create();
  m_context->makeCurrent(_window);
  // we must call this first before any other GL commands to load and link the
  // gl commands from the lib, if this is not done program will crash
  ngl::NGLInit::instance();

  loadShaders();
  createVOAs();
  m_context->doneCurrent();
}

void Context::drawScreenQuad()
{
  QMutexLocker locker( &m_mutex );

  m_quad->draw();
}
void Context::createVOAs()
{
  m_quad = new ScreenQuad();
}

//----------------------------------------------------------------------------------------------------------------------
void Context::makeCurrent(OpenGLWindow *_window)
{
  QMutexLocker locker( &m_mutex );

  m_context->makeCurrent(_window);

}

//----------------------------------------------------------------------------------------------------------------------
void Context::swapBuffers(OpenGLWindow *_window)
{
  QMutexLocker locker( &m_mutex );

  if(_window->isExposed())
  {

    m_context->swapBuffers(_window);
  }
  m_context->doneCurrent();

}
//----------------------------------------------------------------------------------------------------------------------
void Context::close()
{
  std::cerr<<"closing down\n";
  ngl::NGLInit::instance()->NGLQuit();
}

//----------------------------------------------------------------------------------------------------------------------
void Context::loadShaders()
{
	ngl::ShaderLib *shader=ngl::ShaderLib::instance();
	shader->createShaderProgram("Colour");

  shader->attachShader("ColourVertex",ngl::VERTEX);
  shader->attachShader("ColourFragment",ngl::FRAGMENT);
  shader->loadShaderSource("ColourVertex","shaders/ColourTextureVert.glsl");
  shader->loadShaderSource("ColourFragment","shaders/ColourTextureFrag.glsl");

  shader->compileShader("ColourVertex");
  shader->compileShader("ColourFragment");
  shader->attachShaderToProgram("Colour","ColourVertex");
  shader->attachShaderToProgram("Colour","ColourFragment");

  shader->linkProgramObject("Colour");
  (*shader)["Colour"]->use();

  shader->createShaderProgram("PointCloud");

  shader->attachShader("PCVertex",ngl::VERTEX);
  shader->loadShaderSource("PCVertex","shaders/PointCloudVert.glsl");

  shader->compileShader("PCVertex");
  shader->attachShaderToProgram("PointCloud","PCVertex");
  shader->attachShaderToProgram("PointCloud","ColourFragment");

  shader->linkProgramObject("PointCloud");
  (*shader)["PointCloud"]->use();


}
//----------------------------------------------------------------------------------------------------------------------

