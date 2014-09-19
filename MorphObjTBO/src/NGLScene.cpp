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

NGLScene::NGLScene(QWindow *_parent) : OpenGLWindow(_parent)
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  setTitle("Morph Mesh Demo");
  m_weight1=0.0;
  m_weight2=0.0;
  m_animation=true;
  m_punchLeft=false;
  m_punchRight=false;
  m_timerLeft = new QTimer();
  m_timerRight = new QTimer();
  connect(m_timerLeft,SIGNAL(timeout()),this,SLOT(updateLeft()));
  connect(m_timerRight,SIGNAL(timeout()),this,SLOT(updateRight()));

}
void NGLScene::punchLeft()
{
  if(m_punchLeft !=true)
  {
    m_weight1=0.0;
    m_timerLeft->start(1);
    m_punchLeft=true;
  }
}

void NGLScene::punchRight()
{
  if(m_punchRight !=true)
  {
    m_weight2=0.0;
    m_timerRight->start(1);
    m_punchRight=true;
  }
}

// a simple structure to hold our vertex data
struct vertData
{
  ngl::Vec3 p1;
  ngl::Vec3 n1;
  ngl::Vec3 p2;
  ngl::Vec3 n2;
  ngl::Vec3 p3;
  ngl::Vec3 n3;
};
void NGLScene::createMorphMesh()
{

  // texture buffers have to be vec4 unless using GL 4.x so mac is out for now
  // just use Vec4 and waste data see http://www.opengl.org/wiki/Buffer_Texture
  std::vector <ngl::Vec4> targets;
  // base pose is mesh 1 stored in m_meshes[0]
  // get the obj data so we can process it locally
  std::vector <ngl::Vec3> verts1=m_meshes[0]->getVertexList();
  // should really check to see if the poses match if we were doing this properly
  std::vector <ngl::Vec3> verts2=m_meshes[1]->getVertexList();
  std::vector <ngl::Vec3> verts3=m_meshes[2]->getVertexList();

  // faces will be the same for each mesh so only need one
  std::vector <ngl::Face> faces=m_meshes[0]->getFaceList();
  // now get the normals
  std::vector <ngl::Vec3> normals1=m_meshes[0]->getNormalList();
  std::vector <ngl::Vec3> normals2=m_meshes[1]->getNormalList();
  std::vector <ngl::Vec3> normals3=m_meshes[2]->getNormalList();

  // now we are going to process and pack the mesh into an ngl::VertexArrayObject
  std::vector <vertData> vboMesh;
  vertData d;
  unsigned int nFaces=faces.size();
  //unsigned int nNorm=normals1.size();
  //unsigned int nTex=tex.size();
  // loop for each of the faces
  for(unsigned int i=0;i<nFaces;++i)
  {
    // now for each triangle in the face (remember we ensured tri above)
    for(int j=0;j<3;++j)
    {
      // pack in the vertex data first

      d.p1=verts1[faces[i].m_vert[j]];
      // the blend meshes are just the differences so we subtract the base mesh
      // from the current one (could do this on GPU but this saves processing time)
      ngl::Vec3 c;
      c=verts2[faces[i].m_vert[j]]-d.p1;
      targets.push_back(ngl::Vec4(c.m_x,c.m_y,c.m_z,1.0));
      c=verts3[faces[i].m_vert[j]]-d.p1;
      targets.push_back(ngl::Vec4(c.m_x,c.m_y,c.m_z,1.0));

      // now do the normals
      d.n1=normals1[faces[i].m_norm[j]];
      // again we only need the differences so subtract base mesh value from pose values
      c=normals2[faces[i].m_norm[j]]-d.n1;
      targets.push_back(ngl::Vec4(c.m_x,c.m_y,c.m_z,1.0));
      c=normals3[faces[i].m_norm[j]]-d.n1;
      targets.push_back(ngl::Vec4(c.m_x,c.m_y,c.m_z,1.0));

    // finally add it to our mesh VAO structure
    vboMesh.push_back(d);
    }
  }




  // generate and bind our matrix buffer this is going to be fed to the feedback shader to
  // generate our model position data for later, if we update how many instances we use
  // this will need to be re-generated (done in the draw routine)


  GLuint morphTarget;
  glGenBuffers(1,&morphTarget);

  glBindBuffer(GL_TEXTURE_BUFFER, morphTarget);
  //ngl::NGLCheckGLError("bind texture",__LINE__);
  glBufferData(GL_TEXTURE_BUFFER, targets.size()*sizeof(ngl::Vec4), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_TEXTURE_BUFFER, 0, targets.size()*sizeof(ngl::Vec4),&targets[0].m_x );              // Fill

  glGenTextures(1, &m_tboID);
  glActiveTexture( GL_TEXTURE0 );
  glBindTexture(GL_TEXTURE_BUFFER,m_tboID);

  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, morphTarget);

  // first we grab an instance of our VOA class as a TRIANGLE_STRIP
  m_vaoMesh= ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
  // next we bind it so it's active for setting data
  m_vaoMesh->bind();
  unsigned int meshSize=vboMesh.size();
  // now we have our data add it to the VAO, we need to tell the VAO the following
  // how much (in bytes) data we are copying
  // a pointer to the first element of data (in this case the address of the first element of the
  // std::vector
  m_vaoMesh->setData(meshSize*sizeof(vertData),vboMesh[0].p1.m_x);

  // so data is Vert / Normal for each mesh
  m_vaoMesh->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(vertData),0);
  m_vaoMesh->setVertexAttributePointer(1,3,GL_FLOAT,sizeof(vertData),3);



  // now we have set the vertex attributes we tell the VAO class how many indices to draw when
  // glDrawArrays is called, in this case we use buffSize (but if we wished less of the sphere to be drawn we could
  // specify less (in steps of 3))
  m_vaoMesh->setNumIndices(meshSize);
  // finally we have finished for now so time to unbind the VAO
  m_vaoMesh->unbind();
}

void NGLScene::changeWeight(Weights _w,Direction _d )
{

  switch(_w)
   {
    case POSE1 :
      if(_d == UP)
        m_weight1+=0.1;
      else
        m_weight1-=0.1;
    break;

    case POSE2 :
      if(_d == UP)
        m_weight2+=0.1;
      else
        m_weight2-=0.1;
    break;
   }
  // clamp to 0.0 -> 1.0 range
  m_weight1=std::min(1.0f, std::max(0.0f, m_weight1));
  m_weight2=std::min(1.0f, std::max(0.0f, m_weight2));
}

NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  Init->NGLQuit();
  for(unsigned int i=0; i<m_meshes.size(); ++i)
  {
    delete m_meshes[i];
  }

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
  renderNow();
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
  ngl::Vec3 from(0,10,40);
  ngl::Vec3 to(0,10,0);
  ngl::Vec3 up(0,1,0);


  // first we create a mesh from an obj passing in the obj file and texture
  ngl::Obj *mesh1= new ngl::Obj("models/BrucePose1.obj");
  m_meshes.push_back(mesh1);

  ngl::Obj *mesh2= new ngl::Obj("models/BrucePose2.obj");
  m_meshes.push_back(mesh2);

  ngl::Obj *mesh3= new ngl::Obj("models/BrucePose3.obj");
  m_meshes.push_back(mesh3);
  createMorphMesh();

  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.05,350);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called PerFragADS
  shader->createShaderProgram("PerFragADS");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("PerFragADSVertex",ngl::VERTEX);
  shader->attachShader("PerFragADSFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("PerFragADSVertex","shaders/PerFragASDVert.glsl");
  shader->loadShaderSource("PerFragADSFragment","shaders/PerFragASDFrag.glsl");
  // compile the shaders
  shader->compileShader("PerFragADSVertex");
  shader->compileShader("PerFragADSFragment");
  // add them to the program
  shader->attachShaderToProgram("PerFragADS","PerFragADSVertex");
  shader->attachShaderToProgram("PerFragADS","PerFragADSFragment");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("PerFragADS");
  // and make it active ready to load values
  (*shader)["PerFragADS"]->use();
  // now we need to set the material and light values
  /*
   *struct MaterialInfo
   {
        // Ambient reflectivity
        vec3 Ka;
        // Diffuse reflectivity
        vec3 Kd;
        // Specular reflectivity
        vec3 Ks;
        // Specular shininess factor
        float shininess;
  };*/
  shader->setShaderParam3f("material.Ka",0.1,0.1,0.1);
  // red diffuse
  shader->setShaderParam3f("material.Kd",0.8,0.8,0.8);
  // white spec
  shader->setShaderParam3f("material.Ks",1.0,1.0,1.0);
  shader->setShaderParam1f("material.shininess",1000);
  // now for  the lights values (all set to white)
  /*struct LightInfo
  {
  // Light position in eye coords.
  vec4 position;
  // Ambient light intensity
  vec3 La;
  // Diffuse light intensity
  vec3 Ld;
  // Specular light intensity
  vec3 Ls;
  };*/
  shader->setUniform("light.position",ngl::Vec3(2,20,2));
  shader->setShaderParam3f("light.La",0.1,0.1,0.1);
  shader->setShaderParam3f("light.Ld",1.0,1.0,1.0);
  shader->setShaderParam3f("light.Ls",0.9,0.9,0.9);

  glEnable(GL_DEPTH_TEST); // for removal of hidden surfaces

  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
  m_text=new ngl::Text(QFont("Arial",16));
  m_text->setScreenSize(width(),height());
}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["PerFragADS"]->use();
  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  MV= m_mouseGlobalTX*m_cam->getViewMatrix();
  MVP=MV*m_cam->getProjectionMatrix() ;
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setUniform("MVP",MVP);
  shader->setUniform("MV",MV);
  shader->setUniform("normalMatrix",normalMatrix);
  shader->setUniform("weight1",m_weight1);
  shader->setUniform("weight2",m_weight2);
}

void NGLScene::render()
{
  // clear the screen and depth buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

  loadMatricesToShader();
  // draw the mesh
  m_vaoMesh->bind();
  glBindTexture(GL_TEXTURE_BUFFER, m_tboID);
  m_vaoMesh->draw();
  m_vaoMesh->unbind();
  m_text->setColour(1.0f,1.0f,1.0f);
  QString text=QString("Q-W change Pose one weight %1").arg(m_weight1);

  m_text->renderText(10,18,text);
  text=QString("A-S change Pose two weight %1").arg(m_weight2);
  m_text->renderText(10,34,text);
  m_text->renderText(10,58,"Z trigger Left Punch X trigger Right");
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
    renderNow();

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
    renderNow();

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
	renderNow();
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
  // show full screen
  case Qt::Key_F : showFullScreen(); break;
  // show windowed
  case Qt::Key_N : showNormal(); break;
  case Qt::Key_Q : changeWeight(POSE1,DOWN); break;
  case Qt::Key_W : changeWeight(POSE1,UP); break;

  case Qt::Key_A : changeWeight(POSE2,DOWN); break;
  case Qt::Key_S : changeWeight(POSE2,UP); break;
  case Qt::Key_Space : toggleAnimation(); break;
  case Qt::Key_Z : punchLeft(); break;
  case Qt::Key_X : punchRight(); break;

  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
    renderNow();
}


void NGLScene::updateLeft()
{
  static Direction left=UP;
  if(left == UP )
  {
    m_weight1+=0.2;
    if(m_weight1 >1.1)
      left=DOWN;
  }
  else if(left == DOWN )
  {
   m_weight1-=0.2;
   if(m_weight1<=0.0)
   {
     m_weight1=0.0;

     m_timerLeft->stop();
     left=UP;
     m_punchLeft=false;
   }
  }
  renderNow();
}

void NGLScene::updateRight()
{
  static Direction right=UP;
  if(right == UP )
  {
    m_weight2+=0.2;
    if(m_weight2 >1.1)
      right=DOWN;
  }
  else if(right == DOWN )
  {
   m_weight2-=0.2f;
   if(m_weight2<=0.0)
   {
     m_weight2=0.0;
     m_timerRight->stop();
     right=UP;
     m_punchRight=false;
   }
  }
  renderNow();
}

