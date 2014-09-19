#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include "AIUtil.h"
#include <iostream>
#include <ngl/NGLInit.h>
#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>
#include <ngl/Material.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <boost/foreach.hpp>
#include <QTime>
#include <boost/format.hpp>




//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.1;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=1.0;

NGLScene::NGLScene(const char *_fname, QWindow *_parent) : OpenGLWindow(_parent)
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  setTitle("Using libassimp with NGL for Animation");
  m_animate=true;
  m_frameTime=0.0;
  m_sceneName=_fname;


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

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  m_scene = m_importer.ReadFile(m_sceneName,
                                aiProcessPreset_TargetRealtime_Quality |
                                aiProcess_Triangulate
                                );
  if(m_scene == 0)
  {
    std::cerr<<"Error loading scene file\n";
    exit(EXIT_FAILURE);
  }
  std::cout<<"num animations "<<m_scene->mNumAnimations<<"\n";
  if(m_scene->mNumAnimations <1)
  {
      std::cerr<<"No animations in this scene exiting\n";
      exit(EXIT_FAILURE);
  }
  bool loaded=m_mesh.load(m_scene);
  if(loaded == false)
  {
      std::cerr<<"Assimp reports "<<m_importer.GetErrorString()<<"\n";
      exit(EXIT_FAILURE);
  }
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Skinning
  shader->createShaderProgram("Skinning");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("SkinningVertex",ngl::VERTEX);
  shader->attachShader("SkinningFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("SkinningVertex","shaders/SkinningVertex.glsl");
  shader->loadShaderSource("SkinningFragment","shaders/SkinningFragment.glsl");
  // compile the shaders
  shader->compileShader("SkinningVertex");
  shader->compileShader("SkinningFragment");
  // add them to the program
  shader->attachShaderToProgram("Skinning","SkinningVertex");
  shader->attachShaderToProgram("Skinning","SkinningFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // now we have associated this data we can link the shader
  shader->linkProgramObject("Skinning");
  shader->autoRegisterUniforms("Skinning");
  (*shader)["Skinning"]->use();

  // the shader will use the currently active material and light0 so set them
  ngl::Material m(ngl::GOLD);
  // load our material values to the shader into the structure material (see Vertex shader)
  m.loadToShader("material");
  ngl::Vec3 min,max;
  AIU::getSceneBoundingBox(m_scene,min,max);
  ngl::Vec3 center=(min+max)/2.0;
  ngl::Vec3 from;
  from.m_x=0;
  from.m_y=max.m_y*4;
  from.m_z=max.m_z*4;
  std::cout<<"from "<<from<<" center "<<center<<"\n";
  ngl::Vec3 up(0,1,0);
  // now load to our new camera
  m_cam= new ngl::Camera(from,center,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.05,350);
  shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);
  // now create our light this is done after the camera so we can pass the
  // transpose of the projection matrix to the light to do correct eye space
  // transformations
  ngl::Mat4 iv=m_cam->getViewMatrix();
  iv.transpose();
  m_light = new ngl::Light(from,ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT );
  m_light->setTransform(iv);
  // load these values to the shader as well
  m_light->loadToShader("light");
  startTimer(20);
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
}




void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat4 M;
    M=m_transform.getMatrix()*m_mouseGlobalTX;
    MV=  M*m_cam->getViewMatrix();
    MVP= M*m_cam->getVPMatrix();
    shader->setShaderParamFromMat4("MV",MV);
    shader->setRegisteredUniform("MVP",MVP);
    shader->setShaderParamFromMat4("M",M);
}

void NGLScene::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Skinning"]->use();

  // Rotation based on the mouse position for our global transform
  ngl::Transformation trans;
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
  // set this in the TX stack
  loadMatricesToShader();
  std::vector<ngl::Mat4> transforms;
  if(m_animate)
  {
    QTime t=QTime::currentTime();
    float time=float(t.msec()/1000.0)*m_mesh.getDuration()/m_mesh.getTicksPerSec();

    m_mesh.boneTransform(time, transforms);
  }
  else
  {
    m_mesh.boneTransform(m_frameTime, transforms);

  }
  unsigned int size=transforms.size();
  for (unsigned int i = 0 ; i < size ; ++i)
  {
    std::string name=boost::str(boost::format("gBones[%d]") % i );

    shader->setRegisteredUniform(name.c_str(),transforms[i]);
  }



  m_mesh.render();
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

void NGLScene::timerEvent(QTimerEvent *)
{
  renderNow();
}
