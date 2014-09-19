#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/Camera.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Random.h>
#include <ngl/Texture.h>


//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=2.1;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.5;

NGLScene::NGLScene(QWindow *_parent) : OpenGLWindow(_parent)
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  setTitle("Animated Billboard Textures");
  m_currentTexture=0;
  m_animate=true;
  m_time=0;

}

bool NGLScene::depthSort(data a, data b)
{
  return a.p.m_z<b.p.m_z;
}



NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
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

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();


  // create a billboard shader

  // we are creating a shader called Phong
  shader->createShaderProgram("Billboard");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("BillboardVert",ngl::VERTEX);
  shader->attachShader("BillboardFrag",ngl::FRAGMENT);
  shader->attachShader("BillboardGeo",ngl::GEOMETRY);

  // attach the source
  shader->loadShaderSource("BillboardVert","shaders/BillboardVert.glsl");
  shader->loadShaderSource("BillboardFrag","shaders/BillboardFrag.glsl");
  shader->loadShaderSource("BillboardGeo","shaders/BillboardGeo.glsl");
  // compile the shaders
  shader->compileShader("BillboardVert");
  shader->compileShader("BillboardFrag");
  shader->compileShader("BillboardGeo");
  // add them to the program
  shader->attachShaderToProgram("Billboard","BillboardVert");
  shader->attachShaderToProgram("Billboard","BillboardFrag");
  shader->attachShaderToProgram("Billboard","BillboardGeo");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Billboard");
  // and make it active ready to load values
  (*shader)["Billboard"]->use();
  shader->autoRegisterUniforms("Billboard");
  shader->setRegisteredUniform1i("time",2);

  // create a voa of points to draw
  m_vao= ngl::VertexArrayObject::createVOA(GL_POINTS);
  m_vao->bind();

  ngl::Random *rng=ngl::Random::instance();

  data p;
  std::vector <data> points;
  rng->setSeed();

  for(int i=0; i<40000; ++i)
  {
   float radius=8+rng->randomPositiveNumber(1);
   float x=radius*cos(float (ngl::radians(i)));
   float z=radius*sin(float (ngl::radians(i)));

   p.p.m_x=x;
   p.p.m_y=-rng->randomPositiveNumber(1);
   p.p.m_z=z;

   p.p.m_w=(int)rng->randomPositiveNumber(3);
   // time offset we have 10 frames for each texture
   p.offset=(int)rng->randomPositiveNumber(10);
   points.push_back(p);
  }
  std::sort(points.begin(),points.end(),NGLScene::depthSort);
  m_vao->setData(points.size()*sizeof(data),points[0].p.m_x,GL_DYNAMIC_DRAW);

  m_vao->setVertexAttributePointer(0,4,GL_FLOAT,sizeof(data),0);
  m_vao->setVertexAttributePointer(1,1,GL_FLOAT,sizeof(data),4);

  m_vao->setNumIndices(points.size());
  m_vao->unbind();


  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,0,10);
  ngl::Vec3 to(0,0,-2);
  ngl::Vec3 up(0,1,0);
  // now load to our new camera
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.01,350);

  ngl::Texture t;

  t.loadImage("textures/map1.png");
  t.setMultiTexture(0);
  m_maps[0]=t.setTextureGL();
  glGenerateMipmap(GL_TEXTURE_2D);


  t.loadImage("textures/map2.png");
  t.setMultiTexture(1);
  m_maps[1]=t.setTextureGL();
  glGenerateMipmap(GL_TEXTURE_2D);

  t.loadImage("textures/map3.png");
  t.setMultiTexture(2);
  m_maps[2]=t.setTextureGL();
  glGenerateMipmap(GL_TEXTURE_2D);

  shader->setShaderParam1i("tex1",0);
  shader->setShaderParam1i("tex2",1);
  shader->setShaderParam1i("tex3",2);


  //glEnable(GL_DEPTH_TEST);
  glDisable( GL_DEPTH_TEST );
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  startTimer(40);

  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MVP;
  MVP= m_cam->getVPMatrix();
  shader->setShaderParamFromMat4("MVP",MVP);
}

void NGLScene::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();


   // get the VBO instance and draw the built in teapot
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  shader->setRegisteredUniform("camerapos",m_cam->getEye().toVec3());
  shader->setRegisteredUniform("VP",m_cam->getVPMatrix());
  m_vao->bind();
  m_vao->draw();
  m_vao->unbind();

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
    m_origX = _event->x();
    m_origY = _event->y();
    m_cam->yaw(diffx);
    renderLater();

  }
    // right mouse translate code
  else if(m_translate && _event->buttons() == Qt::RightButton)
  {
    int diffY = (int)(_event->y() - m_origYPos);
    m_origXPos=_event->x();
    m_origYPos=_event->y();
    m_cam->pitch(diffY);
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
	m_cam->setEye(-m_modelPos);
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
    case Qt::Key_Space : m_animate ^=true; break;
  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
    renderLater();
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::timerEvent(QTimerEvent *)
{
  if(m_animate)
  {
    ++m_time;
  }
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->setRegisteredUniform1i("time",m_time);

  renderLater();
}

