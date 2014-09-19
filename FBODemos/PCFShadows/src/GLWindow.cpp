#include "GLWindow.h"
#include <iostream>
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Mat4.h>
#include <ngl/Transformation.h>
#include <ngl/TransformStack.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <QColorDialog>
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1;
//----------------------------------------------------------------------------------------------------------------------
// in this ctor we need to call the CreateCoreGLContext class, this is mainly for the MacOS Lion version as
// we need to init the OpenGL 3.2 sub-system which is different than other platforms
//----------------------------------------------------------------------------------------------------------------------
GLWindow::GLWindow(const QGLFormat _format, QWidget *_parent ): QGLWidget( _format, _parent )

{

  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());
  // Now set the initial GLWindow attributes to default values
  // Roate is false
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  // this timer is going to trigger an event every 40ms which will be processed in the
  //
  m_lightTimer =startTimer(40);
  m_animate=true;
  m_lightPosition.set(8,4,8);
  m_lightYPos=4.0;
  m_lightXoffset=8.0;
  m_lightZoffset=8.0;
  m_drawDebugQuad=true;
  m_textureSize=512;
  m_zNear=0.01;
  m_zfar=100;
  m_fov=45.0;
  m_polyOffsetFactor=0.0;
  m_polyOffsetScale=0.0;
  m_textureMinFilter= GL_NEAREST;
  m_textureMagFilter=  GL_NEAREST;
  m_colour.set(1,1,1,1);
}

GLWindow::~GLWindow()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  Init->NGLQuit();
  glDeleteTextures(1,&m_textureID);
  glDeleteFramebuffers(1,&m_fboID);
}

//----------------------------------------------------------------------------------------------------------------------
// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining VBOs etc.
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::initializeGL()
{

  // we must call this first before any other GL commands to load and link the
  // gl commands from the lib, if this is not done program will crash
  ngl::NGLInit::instance();

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);


  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,2,6);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  // now load to our new camera
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,m_zNear,m_zfar);

  // now load to our light POV camera

  m_lightCamera= new ngl::Camera(m_lightPosition,to,up);
  // here we set the light POV camera shape, the aspect is 1 as our
  // texture is square.
  // use the same clip plane as before but set the FOV a bit larger
  // to get slightly better shadows and the clip planes will help
  // to get rid of some of the artefacts
  m_lightCamera->setShape(m_fov,1.0,m_zNear,m_zfar);


  // in this case I'm only using the light to hold the position
  // it is not passed to the shader directly
  m_lightAngle=0.0;

  // we are creating a shader called Texture
  shader->createShaderProgram("Texture");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("TextureVertex",ngl::VERTEX);
  shader->attachShader("TextureFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("TextureVertex","shaders/TextureVert.glsl");
  shader->loadShaderSource("TextureFragment","shaders/TextureFrag.glsl");
  // compile the shaders
  shader->compileShader("TextureVertex");
  shader->compileShader("TextureFragment");
  // add them to the program
  shader->attachShaderToProgram("Texture","TextureVertex");
  shader->attachShaderToProgram("Texture","TextureFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("Texture",0,"inVert");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Texture");
  shader->use("Texture");
  shader->autoRegisterUniforms("Texture");
  // we are creating a shader called Colour
  shader->createShaderProgram("Colour");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("ColourVertex",ngl::VERTEX);
  shader->attachShader("ColourFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("ColourVertex","shaders/ColourVert.glsl");
  shader->loadShaderSource("ColourFragment","shaders/ColourFrag.glsl");
  // compile the shaders
  shader->compileShader("ColourVertex");
  shader->compileShader("ColourFragment");
  // add them to the program
  shader->attachShaderToProgram("Colour","ColourVertex");
  shader->attachShaderToProgram("Colour","ColourFragment");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Colour");
  shader->use("Colour");
  shader->setShaderParam4f("Colour",1,0,0,1);
  shader->autoRegisterUniforms("Colour");
  // we are creating a shader called Shadow
  shader->createShaderProgram("Shadow");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("ShadowVertex",ngl::VERTEX);
  shader->attachShader("ShadowFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("ShadowVertex","shaders/ShadowVert.glsl");
  shader->loadShaderSource("ShadowFragment","shaders/ShadowFrag.glsl");
  // compile the shaders
  shader->compileShader("ShadowVertex");
  shader->compileShader("ShadowFragment");
  // add them to the program
  shader->attachShaderToProgram("Shadow","ShadowVertex");
  shader->attachShaderToProgram("Shadow","ShadowFragment");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Shadow");
  shader->use("Shadow");
  shader->autoRegisterUniforms("Shadow");
  // create the primitives to draw
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createSphere("sphere",0.5,50);
  prim->createTorus("torus",0.15,0.4,40,40);

  prim->createTrianglePlane("plane",14,14,80,80,ngl::Vec3(0,1,0));
  // now create our FBO and texture
  createFramebufferObject();
  // we need to enable depth testing
  glEnable(GL_DEPTH_TEST);
  // set the depth comparison mode
  glDepthFunc(GL_LEQUAL);
  // set the bg to black
  glClearColor(0,0,0,1.0f);
  // enable face culling this will be switch to front and back when
  // rendering shadow or scene
  glEnable(GL_CULL_FACE);
  m_text = new ngl::Text(QFont("Ariel",14));
  m_text->setColour(1,1,1);
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void GLWindow::resizeGL(
                        int _w,
                        int _h
                       )
{
  glViewport(0,0,_w,_h);
  m_cam->setShape(45,(float)_w/_h,3,1000);
  m_width=_w;
  m_height=_h;
  m_text->setScreenSize(_w,_h);
}
//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void GLWindow::paintGL()
{
  //----------------------------------------------------------------------------------------------------------------------
  // Pass 1 render our Depth texture to the FBO
  //----------------------------------------------------------------------------------------------------------------------
  // enable culling
  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE_ARB);

  // bind the FBO and render offscreen to the texture
  glBindFramebuffer(GL_FRAMEBUFFER,m_fboID);
  // bind the texture object to 0 (off )
  glBindTexture(GL_TEXTURE_2D,0);
  // we need to render to the same size as the texture to avoid
  // distortions
  glViewport(0,0,m_textureSize,m_textureSize);

  // Clear previous frame values
  glClear( GL_DEPTH_BUFFER_BIT);
  // as we are only rendering depth turn off the colour / alpha
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

  // render only the back faces so we don't get too much self shadowing
  glCullFace(GL_FRONT);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(m_polyOffsetFactor,m_polyOffsetScale );
  // set the shape of the camera before rendering
  m_lightCamera->setShape(m_fov,1.0,m_zNear,m_zfar);
  // draw the scene from the POV of the light
  drawScene(&GLWindow::loadToLightPOVShader);
  //----------------------------------------------------------------------------------------------------------------------
  // Pass two use the texture
  // now we have created the texture for shadows render the scene properly
  //----------------------------------------------------------------------------------------------------------------------
  // go back to our normal framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  // set the viewport to the screen dimensions
  glViewport(0,0,m_width,m_height);
  // enable colour rendering again (as we turned it off earlier)
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // bind the shadow texture
  glBindTexture(GL_TEXTURE_2D,m_textureID);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

  glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE );
  // we need to generate the mip maps each time we bind
  glGenerateMipmap(GL_TEXTURE_2D);
  // now only cull back faces

  glDisable(GL_CULL_FACE);

  // render our scene with the shadow shader
  drawScene(&GLWindow::loadMatricesToShadowShader);
  //----------------------------------------------------------------------------------------------------------------------
  // this draws the debug texture on the quad
  //----------------------------------------------------------------------------------------------------------------------
  if(m_drawDebugQuad)
  {
    glBindTexture(GL_TEXTURE_2D,m_textureID);
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE );
//    glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE );
    debugTexture(-0.6,-1,0.6,1);
  }
  //----------------------------------------------------------------------------------------------------------------------
  // now we draw a cube to visualise the light
  //----------------------------------------------------------------------------------------------------------------------
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();

  shader->use("Colour");

  m_transform.reset();
    m_transform.setPosition(m_lightPosition);
    ngl::Mat4 MVP=m_transform.getMatrix() * m_mouseGlobalTX *
                    m_cam->getVPMatrix();
    shader->setShaderParamFromMat4("MVP",MVP);
    prim->draw("cube");



}


void GLWindow::createFramebufferObject()
{

  // Try to use a texture depth component
  glGenTextures(1, &m_textureID);
  glBindTexture(GL_TEXTURE_2D, m_textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_textureMinFilter );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_textureMagFilter );
  glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

  glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_textureSize, m_textureSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

  glBindTexture(GL_TEXTURE_2D, 0);

  // create our FBO
  glGenFramebuffers(1, &m_fboID);
  glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
  // disable the colour and read buffers as we only want depth
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  // attach our texture to the FBO
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, m_textureID, 0);

  // switch back to window-system-provided framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void GLWindow::loadMatricesToShadowShader()
{

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("Shadow");
  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_transform.getMatrix()*m_mouseGlobalTX;
  MV=  M*m_cam->getViewMatrix();
  MVP= M*m_cam->getVPMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setRegisteredUniformFromMat4("MV",MV);
  shader->setRegisteredUniformFromMat4("MVP",MVP);
  shader->setRegisteredUniformFromMat3("normalMatrix",normalMatrix);
  shader->setRegisteredUniform3f("lightPosition",m_lightPosition.m_x,m_lightPosition.m_y,m_lightPosition.m_z);
  shader->setRegisteredUniformFromColour("inColour",m_colour);
  // x = x* 0.5 + 0.5
  // y = y* 0.5 + 0.5
  // z = z* 0.5 + 0.5
  // Moving from unit cube [-1,1] to [0,1]
  ngl::Mat4 bias;
  bias.scale(0.5,0.5,0.5);
  bias.translate(0.5,0.5,0.5);

  ngl::Mat4 model=m_transform.getMatrix();
  // calculate MVP then multiply by the bias
  ngl::Mat4 textureMatrix= model * m_lightCamera->getVPMatrix() * bias;
  shader->setRegisteredUniformFromMat4("textureMatrix",textureMatrix);

}

void GLWindow::loadToLightPOVShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("Colour");
  ngl::Mat4 MVP=m_transform.getMatrix()* m_lightCamera->getVPMatrix();
  shader->setRegisteredUniformFromMat4("MVP",MVP);
}

void GLWindow::drawScene(
                          funcPointer _shaderFunc
                      )
{
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
   // get the VBO instance and draw the built in teapot
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

  m_transform.reset();
    // this is the same as calling
    //  ((*this).*(_shaderFunc))(m_transformStack);
    // but a lot more readable as to the intent
    // see the c++ faq link in header for more details
    m_transform.setScale(0.1,0.1,0.1);
    m_transform.setPosition(0,-0.5,0);
    CALLMEMBERFUNC(*this,_shaderFunc)();
    prim->draw("dragon");
    m_transform.reset();
    m_transform.setPosition(-3,0.0,0.0);
    CALLMEMBERFUNC(*this,_shaderFunc)();
    prim->draw("sphere");

  m_transform.reset();

    m_transform.setPosition(3,0.0,0.0);
    CALLMEMBERFUNC(*this,_shaderFunc)();
    prim->draw("cube");

  m_transform.reset();
    m_transform.setPosition(0,0.0,2.0);
    CALLMEMBERFUNC(*this,_shaderFunc)();
    prim->draw("teapot");

  m_transform.reset();
    m_transform.setScale(0.1,0.1,0.1);
    m_transform.setPosition(0,-0.5,-2.0);
    CALLMEMBERFUNC(*this,_shaderFunc)();
    prim->draw("buddah");

  m_transform.reset();
    m_transform.setPosition(2,0,-2.0);
    CALLMEMBERFUNC(*this,_shaderFunc)();
    prim->draw("torus");

  m_transform.reset();
    m_transform.setPosition(0.0,-0.5,0.0);
    CALLMEMBERFUNC(*this,_shaderFunc)();
    prim->draw("plane");


}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseMoveEvent (
                               QMouseEvent * _event
                              )
{
  // note the method buttons() is the button state when event was called
  // this is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
  if(m_rotate && _event->buttons() == Qt::LeftButton)
  {
    m_spinYFace = ( m_spinYFace + (_event->x() - m_origX) ) % 360 ;
    m_spinXFace = ( m_spinXFace + (_event->y() - m_origY) ) % 360 ;
    m_origX = _event->x();
    m_origY = _event->y();
    updateGL();

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
    updateGL();

  }

}


//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent (
                                QMouseEvent * _event
                               )
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
void GLWindow::mouseReleaseEvent (
                                  QMouseEvent * _event
                                 )
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
void GLWindow::updateLight()
{

}

void GLWindow::timerEvent(
                          QTimerEvent *_event
                         )
{
// if the timer is the light timer call the update light method
  if(_event->timerId() == m_lightTimer && m_animate==true)
  {
    // change the light angle
    m_lightAngle+=0.05;
  }


m_lightPosition.set(m_lightXoffset*cos(m_lightAngle),m_lightYPos,m_lightXoffset*sin(m_lightAngle));
// now set this value and load to the shader
m_lightCamera->set(m_lightPosition,ngl::Vec3(0,0,0),ngl::Vec3(0,1,0));

    // re-draw GL
updateGL();
}

void GLWindow::debugTexture(float _t, float _b, float _l, float _r)
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("Texture");
  ngl::Mat4 MVP;
  MVP.identity();
  shader->setShaderParamFromMat4("MVP",MVP);
  glBindTexture(GL_TEXTURE_2D,m_textureID);

  ngl::VertexArrayObject *quad=ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
  float* vert = new float[18];	// vertex array
  float* uv = new float[12];	// uv array
  vert[0] =_t; vert[1] =  _l; vert[2] =0.0;
  vert[3] = _t; vert[4] =  _r; vert[5] =0.0;
  vert[6] = _b; vert[7] = _l; vert[8]= 0.0;

  vert[9] =_b; vert[10]= _l; vert[11]=0.0;
  vert[12] =_t; vert[13]=_r; vert[14]=0.0;
  vert[15] =_b; vert[16]= _r; vert[17]=0.0;

  uv[0] =0.0; uv[1] =  1.0;
  uv[2] = 1.0; uv[3] =  1.0;
  uv[4] = 0.0; uv[5] = 0.0;

  uv[6] =0.0; uv[7]= 0.0;
  uv[8] =1.0; uv[9]= 1.0;
  uv[10] =1.0; uv[11]= 0.0;


  quad->bind();

  quad->setData(18*sizeof(GLfloat),vert[0]);
  quad->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  quad->setData(12*sizeof(GLfloat),uv[0]);
  quad->setVertexAttributePointer(1,2,GL_FLOAT,0,0);
  quad->setNumIndices(6);
  quad->draw();
  quad->unbind();
  quad->removeVOA();
  delete quad;
  delete uv;
}

void GLWindow::changeTextureSize(int _i)
{
  switch(_i)
  {
    case 0 : m_textureSize=64; break;
    case 1 : m_textureSize=128; break;
    case 2 : m_textureSize=256; break;
    case 3 : m_textureSize=512; break;
    case 4 : m_textureSize=1024; break;
    case 5 : m_textureSize=2048; break;
  }

  glDeleteTextures(1,&m_textureID);
  glDeleteFramebuffers(1,&m_fboID);
  createFramebufferObject();
  updateGL();


}


void GLWindow::changeTextureMinFilter(int _i)
{
  switch(_i)
  {
    case 0 : m_textureMagFilter = GL_NEAREST; break;
    case 1 : m_textureMagFilter = GL_LINEAR; break;
    case 2 : m_textureMagFilter = GL_NEAREST_MIPMAP_NEAREST; break;
    case 3 : m_textureMagFilter = GL_LINEAR_MIPMAP_NEAREST; break;
    case 4 : m_textureMagFilter = GL_NEAREST_MIPMAP_LINEAR; break;
    case 5 : m_textureMagFilter = GL_LINEAR_MIPMAP_LINEAR; break;
  }
  glDeleteTextures(1,&m_textureID);
  glDeleteFramebuffers(1,&m_fboID);
  createFramebufferObject();
  updateGL();
}


void GLWindow::changeTextureMagFilter(int _i)
{

  switch(_i)
  {
    case 0 : m_textureMagFilter = GL_NEAREST; break;
    case 1 : m_textureMagFilter = GL_LINEAR; break;
  }
  glDeleteTextures(1,&m_textureID);
  glDeleteFramebuffers(1,&m_fboID);
  createFramebufferObject();
  updateGL();
}

void GLWindow::setColour()
{
  QColor colour = QColorDialog::getColor();
  if( colour.isValid())
  {
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();
    m_colour.m_r=colour.redF();
    m_colour.m_g=colour.greenF();
    m_colour.m_b=colour.blueF();
    updateGL();
  }
}

