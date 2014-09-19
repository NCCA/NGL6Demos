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


//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1;

NGLScene::NGLScene(std::string _fname, QWindow *_parent) : OpenGLWindow(_parent)
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  setTitle("Qt5 Simple NGL Demo");
  m_imageName=_fname;
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


void NGLScene::genGridPoints(ngl::Real _width, ngl::Real _depth )
{
  // load our image and get size
  QImage image(m_imageName.c_str());
  int imageWidth=image.size().width()-1;
  int imageHeight=image.size().height()-1;
  std::cout<<"image size "<<imageWidth<<" "<<imageHeight<<"\n";

  // calculate the deltas for the x,z values of our point
  float wStep=_width/(float)imageWidth;
  float dStep=_depth/(float)imageHeight;
  // now we assume that the grid is centered at 0,0,0 so we make
  // it flow from -w/2 -d/2
  float xPos=-(_width/2.0);
  float zPos=-(_depth/2.0);
  // now loop from top left to bottom right and generate points
  std::vector <ngl::Vec3> gridPoints;

  for(int z=0; z<=imageHeight; ++z)
  {
    for(int x=0; x<=imageWidth; ++x)
    {
      // grab the colour and use for the Y (height) only use the red channel
      QColor c(image.pixel(x,z));
      gridPoints.push_back(ngl::Vec3(xPos,c.redF()*4,zPos));
      // now store the colour as well
      gridPoints.push_back(ngl::Vec3(c.redF(),c.greenF(),c.blueF()));
      // calculate the new position
      xPos+=wStep;
    }
    // now increment to next z row
    zPos+=dStep;
    // we need to re-set the xpos for new row
    xPos=-(_width/2.0);
  }

  std::vector <GLuint> indices;
  // some unique index value to indicate we have finished with a row and
  // want to draw a new one
  GLuint restartFlag=imageWidth*imageHeight+9999;


  for(int z=0; z<imageHeight; ++z)
  {
    for(int x=0; x<imageWidth; ++x)
    {
      // Vertex in actual row
      indices.push_back(z  * (imageWidth+1) + x);
      // Vertex row below
      indices.push_back((z + 1) * (imageWidth+1) + x);

    }
    // now we have a row of tri strips signal a re-start
    indices.push_back(restartFlag);
  }

  // we could use an ngl::VertexArrayObject but in this case this will show how to
  // create our own as a demo / reminder
  // so first create a vertex array
  glGenVertexArrays(1, &m_vaoID);
  glBindVertexArray(m_vaoID);

  // now a VBO for the grid point data
  GLuint vboID;
  glGenBuffers(1, &vboID);
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  glBufferData(GL_ARRAY_BUFFER,gridPoints.size()*sizeof(ngl::Vec3),&gridPoints[0].m_x,GL_STATIC_DRAW);

  // and one for the index values
  GLuint iboID;
  glGenBuffers(1, &iboID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint),&indices[0], GL_STATIC_DRAW);

  // setup our attribute pointers, we are using 0 for the verts (note the step is going to
  // be 2*Vec3
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(ngl::Vec3)*2,0);
  // this once is the colour pointer and we need to offset by 3 floats
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(ngl::Vec3)*2,((float *)NULL + (3)));
  // enable the pointers
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  // now we tell gl to restart the primitives when restartFlag is encountered
  // don't forget we may need to disable this for other prims so may have to set the
  // state when required
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(restartFlag);
  glBindVertexArray(0);
  // store the number of indices for re-drawing later
  m_vertSize=indices.size();


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
  ngl::Vec3 from(0,10,54);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);

  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.001,150);

  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // load a frag and vert shaders

  shader->createShaderProgram("ColourShader");

  shader->attachShader("ColourVertex",ngl::VERTEX);
  shader->attachShader("ColourFragment",ngl::FRAGMENT);
  shader->loadShaderSource("ColourVertex","shaders/ColourVert.glsl");
  shader->loadShaderSource("ColourFragment","shaders/ColourFrag.glsl");

  shader->compileShader("ColourVertex");
  shader->compileShader("ColourFragment");
  shader->attachShaderToProgram("ColourShader","ColourVertex");
  shader->attachShaderToProgram("ColourShader","ColourFragment");

  shader->linkProgramObject("ColourShader");
  shader->autoRegisterUniforms("ColourShader");
  (*shader)["ColourShader"]->use();


  genGridPoints(40,40);
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());


}


void NGLScene::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["ColourShader"]->use();

  ngl::Mat4 MVP;
  MVP=m_transformStack.getCurrentTransform().getMatrix()
      *m_mouseGlobalTX*m_cam->getVPMatrix();

  shader->setRegisteredUniform("MVP",MVP);

  // bind our VAO
  glBindVertexArray(m_vaoID);
  // draw as triangle strips
  glDrawElements(GL_TRIANGLE_STRIP, m_vertSize,GL_UNSIGNED_INT,0);	// draw first object
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
