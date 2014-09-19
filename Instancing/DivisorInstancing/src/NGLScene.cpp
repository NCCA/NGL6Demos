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
#include <ngl/Random.h>


//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=5.0;
//----------------------------------------------------------------------------------------------------------------------
/// num instances
//----------------------------------------------------------------------------------------------------------------------
const static unsigned int maxinstances=1000000;



//----------------------------------------------------------------------------------------------------------------------
void NGLScene::incInstances()
{
  m_instances+=10000;
  if(m_instances>maxinstances)
    m_instances=maxinstances;
  m_updateBuffer=true;
}
//----------------------------------------------------------------------------------------------------------------------
void NGLScene::decInstances()
{
  m_instances-=10000;
  if(m_instances<10000)
    m_instances=10000;
  m_updateBuffer=true;
}

NGLScene::NGLScene(QWindow *_parent) : OpenGLWindow(_parent)
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  setTitle("Divisor Instancing");
  m_fpsTimer =startTimer(0);
  m_fps=0;
  m_frames=0;
  m_timer.start();
  m_polyMode=GL_FILL;
  m_instances=1000;
  m_updateBuffer=true;

}


NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  glDeleteVertexArrays(1,&m_dataID);
  glDeleteVertexArrays(1,&m_matrixID);
  glDeleteVertexArrays(1,&m_vaoID);
  Init->NGLQuit();
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
void NGLScene::createDataPoints()
{
  GLuint dataArray;
  // first create a Vertex array for out data points, we will create max instances
  // size of data but only use a certain amount of them when we draw
  glGenVertexArrays(1,&m_dataID);
  // bind the array
  glBindVertexArray(m_dataID);
  // generate a buffer ready to store our data
  glGenBuffers(1, &dataArray );
  glBindBuffer(GL_ARRAY_BUFFER, dataArray);
  // allocate space for the vec3 for each point
  ngl::Vec3 *data = new ngl::Vec3[maxinstances];
  // in this case create a sort of supertorus distribution of points
  // based on a random point
  ngl::Random *rng=ngl::Random::instance();
  ngl::Vec3 p;
  for(unsigned int i=0; i<maxinstances; ++i)
  {
    float angle = rng->randomNumber(M_PI);
    float radius = rng->randomNumber(1.0);
    float ca = cosf(angle);
    float sa = sinf(angle);
    float cs = ca < 0 ? -1 : 1;
    float ss = sa < 0 ? -1 : 1;
    float x = radius * cs * pow(fabsf(ca), 1.2);
    float y = radius * ss * pow(fabsf(sa), 1.2);

    p=rng->getRandomPoint(x*80,y,x+y*80);
    data[i].set(p.m_x,p.m_y,p.m_z);
  }
  // now store this buffer data for later.
  glBufferData(GL_ARRAY_BUFFER, maxinstances * sizeof(ngl::Vec3), data, GL_STATIC_DRAW);
  // attribute 0 is the inPos in our shader
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  // finally we clear the point data as it is no longer used
  delete [] data;

}
//----------------------------------------------------------------------------------------------------------------------
void NGLScene::createCube(GLfloat _scale )
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
  for(unsigned int i=0; i<sizeof(vertices)/sizeof(GLfloat); ++i)
  {
    vertices[i]*=_scale;
  }

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
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
  glEnableVertexAttribArray(0);
  // now we repeat for the UV data using the second VBO
  glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(texture)*sizeof(GLfloat), texture, GL_STATIC_DRAW);
  glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
  glEnableVertexAttribArray(1);
  // generate and bind our matrix buffer this is going to be fed to the feedback shader to
  // generate our model position data for later, if we update how many instances we use
  // this will need to be re-generated (done in the draw routine)
  glGenBuffers(1,&m_matrixID);
  glBindBuffer(GL_ARRAY_BUFFER, m_matrixID);

  glBufferData(GL_ARRAY_BUFFER, m_instances*sizeof(ngl::Mat4), NULL, GL_STATIC_DRAW);
  // bind a buffer object to an indexed buffer target in this case we are setting out matrix data
  // to the transform feedback
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_matrixID);

  glEnableVertexAttribArray(2);
  glVertexAttribPointer    (2, 4, GL_FLOAT, GL_FALSE, sizeof(ngl::Mat4), BUFFER_OFFSET(0));
  glVertexAttribDivisor (2, 1);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer    (3, 4, GL_FLOAT, GL_FALSE, sizeof(ngl::Mat4), BUFFER_OFFSET(16));
  glVertexAttribDivisor (3, 1);
  glEnableVertexAttribArray(4);
  glVertexAttribPointer    (4, 4, GL_FLOAT, GL_FALSE, sizeof(ngl::Mat4), BUFFER_OFFSET(32));
  glVertexAttribDivisor (4, 1);
  glEnableVertexAttribArray(5);
  glVertexAttribPointer    (5, 4, GL_FLOAT, GL_FALSE, sizeof(ngl::Mat4), BUFFER_OFFSET(48));
  glVertexAttribDivisor (5, 1);
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
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,1,220);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);

  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.5,150);

  // here we see what the max size of a uniform block can be, this is going
  // to be the GL_MAX_UNIFORM_BLOCK_SIZE / the size of the data we want to pass
  // into the uniform block which in this case is just a series of ngl::Mat4
  // in the case of the mac it's 1024
  GLint maxUniformBlockSize;
  glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
  m_instancesPerBlock = maxUniformBlockSize / sizeof(ngl::Mat4);
  std::cout<<"Number of instances per block is "<<m_instancesPerBlock<<"\n";

  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // This is for our transform shader and it will load a series of matrics into a uniform
  // block ready for drawing later
  shader->createShaderProgram("TransformFeedback");
  shader->attachShader("TransVertex",ngl::VERTEX);
  shader->loadShaderSource("TransVertex","shaders/feedback.glsl");
  shader->compileShader("TransVertex");
  shader->attachShaderToProgram("TransformFeedback","TransVertex");
  // bind our attribute
  shader->bindAttribute("TransformFeedback",0,"inPos");
  // now we want to get the TransformFeedback variables and set them
  // first get the shader ID
  GLuint id=shader->getProgramID("TransformFeedback");
  // create a list of the varyings we want to attach to (this is the out in our shader)
  const char *varyings[1] = { "ModelView" };
  // The names of the vertex or geometry shader outputs to be recorded in
  // transform feedback mode are specified using glTransformFeedbackVaryings
  // in this case we are storing 1 (ModelView) and the attribs are
  glTransformFeedbackVaryings(id, 1, varyings, GL_INTERLEAVED_ATTRIBS);
  // now link the shader
  shader->linkProgramObject("TransformFeedback");
  shader->use("TransformFeedback");
  // register the uniforms
  shader->autoRegisterUniforms("TransformFeedback");
  // now we are going to create our texture shader for drawing the cube
  shader->createShaderProgram("TextureShader");

  shader->attachShader("TextureVertex",ngl::VERTEX);
  shader->attachShader("TextureFragment",ngl::FRAGMENT);
  shader->loadShaderSource("TextureVertex","shaders/Vertex.glsl");
  shader->loadShaderSource("TextureFragment","shaders/Fragment.glsl");

  shader->compileShader("TextureVertex");
  shader->compileShader("TextureFragment");
  shader->attachShaderToProgram("TextureShader","TextureVertex");
  shader->attachShaderToProgram("TextureShader","TextureFragment");

  // link
  shader->linkProgramObject("TextureShader");
  shader->use("TextureShader");
  // register the uniforms for later uses
  shader->autoRegisterUniforms("TextureShader");
  // create our cube

  createCube(0.2);
  loadTexture();
  glEnable(GL_DEPTH_TEST); // for removal of hidden surfaces
  m_text = new ngl::Text(QFont("Arial",14));
  m_text->setScreenSize(width(),height());
  // create the data points
  createDataPoints();
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_transform.getMatrix()*m_mouseGlobalTX;
  MV=  M*m_cam->getViewMatrix();
  MVP= M*m_cam->getVPMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);
}

void NGLScene::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
  (*shader)["TransformFeedback"]->use();
  // if the number of instances have changed re-bind the buffer to the correct size
  if(m_updateBuffer==true)
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_matrixID);
    glBufferData(GL_ARRAY_BUFFER, m_instances*sizeof(ngl::Mat4), NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_matrixID);
    m_updateBuffer=false;
  }

  //----------------------------------------------------------------------------------------------------------------------
  // SETUP DATA
  //----------------------------------------------------------------------------------------------------------------------

  // activate our vertex array for the points so we can fill in our matrix buffer
  glBindVertexArray(m_dataID);
  // set the view for the camera
  shader->setRegisteredUniformFromMat4("View",m_cam->getViewMatrix());
  // this sets some per-vertex data values for the Matrix shader
  shader->setRegisteredUniform4f("data",0.3,0.6,0.5,1.2);
  // pass in the mouse rotation
  shader->setRegisteredUniformFromMat4("mouseRotation",m_mouseGlobalTX);
  // this flag tells OpenGL to discard the data once it has passed the transform stage, this means
  // that none of it wil be drawn (RASTERIZED) remember to turn this back on once we have done this
  glEnable(GL_RASTERIZER_DISCARD);
  // redirect all draw output to the transform feedback buffer which is our buffer object matrix
  glBeginTransformFeedback(GL_POINTS);
  // now draw our array of points (now is a good time to check out the feedback.vs shader to see what
  // happens here)
  glDrawArrays(GL_POINTS, 0, m_instances);
  // now signal that we have done with the feedback buffer
  glEndTransformFeedback();
  // and re-enable rasterisation
  glDisable(GL_RASTERIZER_DISCARD);

  //----------------------------------------------------------------------------------------------------------------------
  // DRAW INSTANCES
  //----------------------------------------------------------------------------------------------------------------------
  // now we are going to switch to our texture shader and render our boxes
  (*shader)["TextureShader"]->use();
  // set the projection matrix for our camera
  shader->setRegisteredUniformFromMat4("Projection",m_cam->getProjectionMatrix());
  // activate the texture
  glBindTexture(GL_TEXTURE_2D,m_textureName);
  // activate our vertex array object for the box
  glBindVertexArray(m_vaoID);

  glPolygonMode(GL_FRONT_AND_BACK,m_polyMode);
  glDrawArraysInstanced(GL_TRIANGLES, 0,36, m_instances);
  glBindVertexArray(0);
  ++m_frames;
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  m_text->setColour(1,1,0);
  QString text=QString("Texture and Vertex Array Object %1 instances Demo %2 fps").arg(m_instances).arg(m_fps);
  m_text->renderText(10,20,text);
  text=QString("Num vertices = %1 num triangles = %2").arg(m_instances*36).arg(m_instances*12);
  m_text->renderText(10,40,text);
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
  case Qt::Key_Equal : incInstances(); break;
  case Qt::Key_Minus : decInstances(); break;

  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
    renderLater();
}

void NGLScene::timerEvent( QTimerEvent *_event )
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

