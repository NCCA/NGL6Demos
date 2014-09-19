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
#include <QFont>


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
  setTitle("Simple OpenGL Texture");
  m_fpsTimer =startTimer(0);
  m_fps=0;
  m_frames=0;
  m_timer.start();
  m_polyMode=GL_FILL;
}

void NGLScene::loadTexture()
{
  QImage *image = new QImage();
  bool loaded=image->load("textures/crate.bmp");
  if(loaded == true)
  {
    int width=image->width();
    int height=image->height();

    unsigned char *data = new unsigned char[ width*height*3];
    unsigned int index=0;
    QRgb colour;
    for( int y=0; y<height; ++y)
    {
      for( int x=0; x<width; ++x)
      {
        colour=image->pixel(x,y);

        data[index++]=qRed(colour);
        data[index++]=qGreen(colour);
        data[index++]=qBlue(colour);
      }
    }


  glGenTextures(1,&m_textureName);
  glBindTexture(GL_TEXTURE_2D,m_textureName);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);

  glGenerateMipmap(GL_TEXTURE_2D); //  Allocate the mipmaps

  }
}



//----------------------------------------------------------------------------------------------------------------------
/// @brief create a cube and stuff it into a VBO on the GPU
/// @param[in] _scale a scale factor for the unit vertices
void NGLScene::createCube( GLfloat _scale )
{

   // vertex coords array
  GLfloat vertices[] = {
                        -1,1,-1,1,1,-1,1,-1,-1, -1,1,-1,-1,-1,-1,1,-1,-1, //back
                        -1,1,1,1,1,1,1,-1,1, -1,-1,1, 1,-1,1,-1,1,1,  //front
                        -1,1,-1, 1,1,-1, 1,1,1, -1,1,1, 1,1,1, -1,1,-1, // top
                        -1,-1,-1, 1,-1,-1, 1,-1,1, -1,-1,1, 1,-1,1, -1,-1,-1, // bottom
                        -1,1,-1,-1,1,1,-1,-1,-1, -1,-1,-1,-1,-1,1,-1,1,1, // left
                        1,1,-1,1,1,1,1,-1,-1, 1,-1,-1,1,-1,1,1,1,1, // left

                        };
  GLfloat texture[] = {
                        0,0,0,1,1,1 ,0,0,1,0,1,1, //back
                        0,1,1,0,1,1 ,0,0,1,0,0,1, // front
                        0,0,1,0,1,1 ,0,1,1,1,0,0, //top
                        0,0,1,0,1,1 ,0,1,1,1,0,0, //bottom
                        1,0,1,1,0,0 ,0,0,0,1,1,1, // left
                        1,0,1,1,0,0 ,0,0,0,1,1,1, // right

                        };


  std::cout<<sizeof(vertices)/sizeof(GLfloat)<<"\n";
  // first we scale our vertices to _scale
  for(uint i=0; i<sizeof(vertices)/sizeof(GLfloat); ++i)
  {
    vertices[i]*=_scale;
  }

  // first we create a vertex array Object
  glGenVertexArrays(1, &m_vaoID);

  // now bind this to be the currently active one
  glBindVertexArray(m_vaoID);
  // now we create two VBO's one for each of the objects these are only used here
  // as they will be associated with the vertex array object
  GLuint vboID[2];
  glGenBuffers(2, &vboID[0]);
  // now we will bind an array buffer to the first one and load the data for the verts
  glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
  // now we bind the vertex attribute pointer for this object in this case the
  // vertex data
  glVertexAttribPointer(0,3,GL_FLOAT,false,0,0);
  glEnableVertexAttribArray(0);
  // now we repeat for the UV data using the second VBO
  glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(texture)*sizeof(GLfloat), texture, GL_STATIC_DRAW);
  glVertexAttribPointer(1,2,GL_FLOAT,false,0,0);
  glEnableVertexAttribArray(1);

}


NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  Init->NGLQuit();
  // remove the texture now we are done
  glDeleteTextures(1,&m_textureName);
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
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,1,2);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.5,150);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // load a frag and vert shaders

  shader->createShaderProgram("TextureShader");

  shader->attachShader("SimpleVertex",ngl::VERTEX);
  shader->attachShader("SimpleFragment",ngl::FRAGMENT);
  shader->loadShaderSource("SimpleVertex","shaders/TextureVert.glsl");
  shader->loadShaderSource("SimpleFragment","shaders/TextureFrag.glsl");

  shader->compileShader("SimpleVertex");
  shader->compileShader("SimpleFragment");
  shader->attachShaderToProgram("TextureShader","SimpleVertex");
  shader->attachShaderToProgram("TextureShader","SimpleFragment");


  shader->linkProgramObject("TextureShader");
  shader->use("TextureShader");
  shader->registerUniform("TextureShader","MVP");

  createCube(0.2);
  loadTexture();
  m_text = new ngl::Text(QFont("Arial",14));
  m_text->setScreenSize(width(),height());
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());


}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MVP=m_transformStack.getCurrentTransform().getMatrix()
                *m_mouseGlobalTX*m_cam->getVPMatrix();

  shader->setRegisteredUniform("MVP",MVP);
}

void NGLScene::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 // build our transform stack
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
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["TextureShader"]->use();
  // now we bind back our vertex array object and draw
  glBindVertexArray(m_vaoID);		// select first VAO

  int instances=0;
  // need to bind the active texture before drawing
  glBindTexture(GL_TEXTURE_2D,m_textureName);
  glPolygonMode(GL_FRONT_AND_BACK,m_polyMode);

	for (float z=-14; z<15; z+=0.5)
		{
		for (float x=-14; x<15; x+=0.5)
		{
		m_transformStack.pushTransform();
		{
			m_transformStack.setRotation(x*20,(x*z)*40,z*2);

       m_transformStack.setPosition(x,0.49,z);
       loadMatricesToShader();
       ++instances;
       glDrawArrays(GL_TRIANGLES, 0,36 );	// draw object
    } // and before a pop
     m_transformStack.popTransform();

	 }
	}
	// calculate and draw FPS
	++m_frames;
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	m_text->setColour(1,1,0);
	QString text=QString("Texture and Vertex Array Object %1 instances Demo %2 fps").arg(instances).arg(m_fps);
	m_text->renderText(10,20,text);
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
  case Qt::Key_W : m_polyMode=GL_LINE; break;
  // turn off wire frame
  case Qt::Key_S : m_polyMode=GL_FILL; break;
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

void NGLScene::timerEvent(QTimerEvent *_event)
{
	if(_event->timerId() == m_fpsTimer)
		{
			if( m_timer.elapsed() > 1000.0)
			{
				m_fps=m_frames;
				m_frames=0;
				m_timer.restart();
			}
		 }
			// re-draw GL
	renderNow();
}
