#include <QMouseEvent>
#include <QGuiApplication>
#include <QApplication>

#include "NGLScene.h"
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/TransformStack.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <QFileDialog>
#include "GameControls.h"


const static float s_shipUpdate=0.2;


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
  setTitle("Game style key control in Qt");
  m_recording=false;
  m_playbackActive=false;
  m_currentPlaybackFrame=0;
  m_keysPressed=0;

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

  glClearColor(0.0f,0.0f,0.0f,1.0f);			   // black Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
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
  // if we are using glsl #version 400 we don't need to do this if we use
  // the layout ( location = x ) elements in our shader
  shader->bindAttribute("Phong",0,"inVert");
  // attribute 1 is the UV data u,v (if present)
  shader->bindAttribute("Phong",1,"inUV");
  // attribute 2 are the normals x,y,z
  shader->bindAttribute("Phong",2,"inNormal");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Phong");
  // and make it active ready to load values
  (*shader)["Phong"]->use();

  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,0,80);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  // now load to our new camera
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.05,350);
  shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);
  // now create our light this is done after the camera so we can pass the
  // transpose of the projection matrix to the light to do correct eye space
  // transformations
  ngl::Mat4 iv=m_cam->getViewMatrix();
  iv.transpose();
  ngl::Light light(ngl::Vec3(0,0,-2),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::DIRECTIONALLIGHT);
  light.setTransform(iv);
  // load these values to the shader as well
  light.loadToShader("light");
  ngl::Material m(ngl::GOLD);
  m.loadToShader("material");
  // create our spaceship
  m_ship= new SpaceShip(ngl::Vec3(0,0,0),"models/SpaceShip.obj");
  m_text = new ngl::Text(QFont("Arial",12));
  m_text->setScreenSize(width(),height());
  m_text->setColour(1,1,1);
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
  // now we start a timer and assign it to the m_updateShipTimer variable
  // this will be triggered at an interval of every 5ms
  // to stop this timer we call killTimer(m_animationTimer) we can have
  // multiple timers but each must have it's own ID
  m_updateShipTimer=startTimer(15);
  m_redrawTimer=startTimer(30);

}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MVP=m_cam->getVPMatrix() * m_transformStack.getCurrAndGlobal().getMatrix();

  shader->setShaderParamFromMat4("MVP",MVP);
 }

void NGLScene::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // now load these values to the shader
  m_ship->draw("Phong",m_cam);


	if (m_recording==true)
	{
	 QString text=QString("Recording");
	 m_text->setColour(1,0,0);
	 m_text->renderText(10,18,text);
	}
	if (m_playbackActive==true)
	{
	 // now render the text using the QT renderText helper function
	QString text=QString("Playback doing frame %1").arg(m_currentPlaybackFrame);
	m_text->setColour(1,1,0);
	m_text->renderText(10,18,text);
	}
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

void NGLScene::timerEvent( QTimerEvent *_event)
{
	// the usual process is to check the event timerID and compare it to
	// any timers we have started with startTimer
	if (_event->timerId() == m_updateShipTimer)
	{

		if(m_playbackActive == true)
		{
			if(m_currentPlaybackFrame <= 0)
			{
				m_ship->setPos(m_keyRecorder.getStartPosition());
			}
			if(m_currentPlaybackFrame < m_keyRecorder.size())
			{
				m_keysPressed=m_keyRecorder[m_currentPlaybackFrame];
				++m_currentPlaybackFrame;
			}
			else
			{
				m_playbackActive=false;
				m_currentPlaybackFrame=0;
			}

		}
		else if (m_recording== true)
		{
			m_keyRecorder.addFrame(m_keysPressed);
		}
	 moveShip();
	}
	if (_event->timerId() == m_redrawTimer)
	{
		renderNow();
	}
}

void NGLScene::moveShip()
{
	m_ship->move(m_keysPressed);

}
void NGLScene::keyPressEvent(QKeyEvent *_event	)
{
	switch(_event->key())
	{
		case Qt::Key_Up: m_keysPressed |= kUp; break;
		case Qt::Key_Down: m_keysPressed |= kDown; break;
		case Qt::Key_Left: m_keysPressed |= kLeft; break;
		case Qt::Key_Right: m_keysPressed |= kRight; break;
		case Qt::Key_R: m_keysPressed |= kRotate; break;
		case Qt::Key_Space :
		{

			m_keyRecorder.setStartPosition(m_ship->getPos());
			m_recording^=true; break;
		}
		case Qt::Key_P : m_playbackActive^=true; break;

		case Qt::Key_S :
		{
		QString filename = QFileDialog::getSaveFileName(
						0,
						tr("Save Keypresses"),
						QDir::currentPath(),
						tr("*.kp") );
				if( !filename.isNull() )
				{
					m_keyRecorder.save( filename.toStdString() );
				}
		break;
	}
	case Qt::Key_L :
	{
	QString filename = QFileDialog::getOpenFileName(
					0,
					tr("load Keypresses"),
					QDir::currentPath(),
					tr("*.kp") );
			if( !filename.isNull() )
			{
				m_keyRecorder.load( filename.toStdString() );
			}
	}
	break;
	case Qt::Key_Escape: QApplication::exit(EXIT_SUCCESS); break;

	}
}


//----------------------------------------------------------------------------------------------------------------------
// in this case this method is called from the MainWindow class and passed
// the key event from that, if we didn't need to process the events in main
// window we could just overload this as the keyPressEvent and only process here.
//----------------------------------------------------------------------------------------------------------------------
void NGLScene::keyReleaseEvent( QKeyEvent *_event	)
{
	switch(_event->key())
	{
		case Qt::Key_Up: m_keysPressed &= ~kUp; break;
		case Qt::Key_Down: m_keysPressed &= ~kDown; break;
		case Qt::Key_Left: m_keysPressed &= ~kLeft; break;
		case Qt::Key_Right: m_keysPressed &= ~kRight; break;
		case Qt::Key_R: m_keysPressed &= ~kRotate; break;
	}
}

