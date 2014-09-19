#include "Context.h"
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <QtGui/QOpenGLContext>
#include <ngl/NGLInit.h>
#include <ngl/Material.h>
#include <QWindow>
#include "OpenGLWindow.h"
#include <ngl/NGLInit.h>

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
  if(s_instance ==0)
  {
    s_instance = new Context(_parent);
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

}

void Context::createVOAs()
{
  ngl::VAOPrimitives::instance()->createSphere("sphere",0.8,40);
  ngl::VAOPrimitives::instance()->createTorus("torus",0.2,0.8,40,40);

}

//----------------------------------------------------------------------------------------------------------------------
void Context::makeCurrent(OpenGLWindow *_window)
{
  m_context->makeCurrent(_window);
}

//----------------------------------------------------------------------------------------------------------------------
void Context::swapBuffers(OpenGLWindow *_window)
{
  m_context->swapBuffers(_window);
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
 shader->bindAttribute("Phong",0,"inVert");
 // attribute 1 is the UV data u,v (if present)
 shader->bindAttribute("Phong",1,"inUV");
 // attribute 2 are the normals x,y,z
 shader->bindAttribute("Phong",2,"inNormal");

 // now we have associated this data we can link the shader
 shader->linkProgramObject("Phong");
 // and make it active ready to load values
 (*shader)["Phong"]->use();
 // the shader will use the currently active material and light0 so set them
 ngl::Material m(ngl::GOLD);
 // load our material values to the shader into the structure material (see Vertex shader)
 m.loadToShader("material");

}
//----------------------------------------------------------------------------------------------------------------------

