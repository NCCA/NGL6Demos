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



//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1;

NGLScene::NGLScene(const std::string &_fname, QWindow *_parent) : OpenGLWindow(_parent)
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  setTitle("Using libassimp with NGL simple Mesh");
  // the following code is modified from this
  // http://assimp.svn.sourceforge.net/viewvc/assimp/trunk/samples/SimpleOpenGL/
  // we are taking one of the postprocessing presets to avoid
  // spelling out 20+ single postprocessing flags here.

	m_scene = aiImportFile(_fname.c_str(),
												 aiProcessPreset_TargetRealtime_MaxQuality |
												 aiProcess_Triangulate |
												 aiProcess_PreTransformVertices |
												 aiProcess_FixInfacingNormals
												 );
	if(m_scene == 0)
	{
			std::cout<<"error opening file "<<_fname<<"\n";
			exit(EXIT_FAILURE);
	}

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
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
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
  m_cam->setShape(45,(float)720.0/576.0,0.05,3500);
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
  buildVAOFromScene();
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
}

void NGLScene::buildVAOFromScene()
{

recurseScene(m_scene,m_scene->mRootNode,ngl::Mat4(1.0));

}

// a simple structure to hold our vertex data
struct vertData
{
  GLfloat u;
  GLfloat v;
  GLfloat nx;
  GLfloat ny;
  GLfloat nz;
  GLfloat x;
  GLfloat y;
  GLfloat z;
};

void NGLScene::recurseScene(const aiScene *sc, const aiNode *nd,const ngl::Mat4 &_parentTx)
{
  ngl::Mat4 m=AIU::aiMatrix4x4ToNGLMat4Transpose(nd->mTransformation);

  unsigned int n = 0, t;
  std::vector <vertData> verts;
  vertData v;
  meshItem thisMesh;
  // the transform is relative to the parent node so we accumulate
  thisMesh.tx=m*_parentTx;

  for (; n < nd->mNumMeshes; ++n)
  {
    verts.clear();
    const aiMesh* mesh = m_scene->mMeshes[nd->mMeshes[n]];

    thisMesh.vao=ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
    for (t = 0; t < mesh->mNumFaces; ++t)
    {
      const aiFace* face = &mesh->mFaces[t];
      // only deal with triangles for ease
      if(face->mNumIndices !=3){std::cout<<"mesh size not tri"<<face->mNumIndices<<"\n"; break;}
      for(unsigned int i = 0; i < face->mNumIndices; i++)
      {
        int index = face->mIndices[i];


        if(mesh->mNormals != NULL)
        {
          v.nx= mesh->mNormals[index].x;
          v.ny= mesh->mNormals[index].y;
          v.nz= mesh->mNormals[index].z;
        }
        if(mesh->mTextureCoords !=NULL && mesh->HasTextureCoords(0))
        {
          v.u=mesh->mTextureCoords[0]->x;
          v.v=mesh->mTextureCoords[0]->y;

        }
        v.x=mesh->mVertices[index].x;
        v.y=mesh->mVertices[index].y;
        v.z=mesh->mVertices[index].z;
        verts.push_back(v);
      }

    }
    thisMesh.vao->bind();
    // now we have our data add it to the VAO, we need to tell the VAO the following
    // how much (in bytes) data we are copying
    // a pointer to the first element of data (in this case the address of the first element of the
    // std::vector
    thisMesh.vao->setData(verts.size()*sizeof(vertData),verts[0].u);
    // in this case we have packed our data in interleaved format as follows
    // u,v,nx,ny,nz,x,y,z
    // If you look at the shader we have the following attributes being used
    // attribute vec3 inVert; attribute 0
    // attribute vec2 inUV; attribute 1
    // attribute vec3 inNormal; attribure 2
    // so we need to set the vertexAttributePointer so the correct size and type as follows
    // vertex is attribute 0 with x,y,z(3) parts of type GL_FLOAT, our complete packed data is
    // sizeof(vertData) and the offset into the data structure for the first x component is 5 (u,v,nx,ny,nz)..x
    thisMesh.vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(vertData),5);
    // uv same as above but starts at 0 and is attrib 1 and only u,v so 2
    thisMesh.vao->setVertexAttributePointer(1,2,GL_FLOAT,sizeof(vertData),0);
    // normal same as vertex only starts at position 2 (u,v)-> nx
    thisMesh.vao->setVertexAttributePointer(2,3,GL_FLOAT,sizeof(vertData),2);
    // now we have set the vertex attributes we tell the VAO class how many indices to draw when
    // glDrawArrays is called, in this case we use buffSize (but if we wished less of the sphere to be drawn we could
    // specify less (in steps of 3))
    thisMesh.vao->setNumIndices(verts.size());
    // finally we have finished for now so time to unbind the VAO
    thisMesh.vao->unbind();
    m_meshes.push_back(thisMesh);
  }

  // draw all children
  for (n = 0; n < nd->mNumChildren; ++n)
  {
    recurseScene(sc, nd->mChildren[n],thisMesh.tx);
  }

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
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();

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
  BOOST_FOREACH(meshItem m, m_meshes)
  {
    ngl::Transformation t;
    t.setMatrix(m.tx);
    loadMatricesToShader();
    m.vao->bind();
    m.vao->draw();
    m.vao->unbind();
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
