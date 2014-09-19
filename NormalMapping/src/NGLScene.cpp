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
#include <ngl/Texture.h>
#include <ngl/Obj.h>


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
  setTitle("Normal mapping Demo");
  m_showNormals=false;

}


NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_key;
  delete m_fill;
  delete m_back;

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
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,0,1);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  // create our camera
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(25,(float)720.0/576.0,0.001,350);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // load a frag and vert shaders

  shader->createShaderProgram("TextureShader");

  shader->attachShader("SimpleVertex",ngl::VERTEX);
  shader->attachShader("SimpleFragment",ngl::FRAGMENT);
  shader->loadShaderSource("SimpleVertex","shaders/NormalMapVert.glsl");
  shader->loadShaderSource("SimpleFragment","shaders/NormalMapFrag.glsl");

	shader->compileShader("SimpleVertex");
	shader->compileShader("SimpleFragment");
	shader->attachShaderToProgram("TextureShader","SimpleVertex");
	shader->attachShaderToProgram("TextureShader","SimpleFragment");

	// link the shader no attributes are bound
	shader->linkProgramObject("TextureShader");
	(*shader)["TextureShader"]->use();
	// set our samplers for each of the textures this will correspond to the
	// multitexture id below
	shader->setShaderParam1i("tex",0);
	shader->setShaderParam1i("spec",1);
	shader->setShaderParam1i("normalMap",2);
	// specular power
	shader->setShaderParam1f("specPower",12.0);

	// build our VertexArrayObject from the mesh
	loadModel();

  // load and set a texture for the colour
  ngl::Texture t("textures/TrollColour.png");
  t.setMultiTexture(0);
  t.setTextureGL();
  // mip map the textures
  glGenerateMipmap(GL_TEXTURE_2D);
  // now one for the specular map
  ngl::Texture spec("textures/2k_troll_spec_map.jpg");
  spec.setMultiTexture(1);
  spec.setTextureGL();
  // mip map the textures
  glGenerateMipmap(GL_TEXTURE_2D);
  // this is our normal map
  ngl::Texture normal("textures/2k_ct_normal.png");
  normal.setMultiTexture(2);
  normal.setTextureGL();
  // mip map the textures
  glGenerateMipmap(GL_TEXTURE_2D);

  glEnable(GL_MULTISAMPLE);

  ngl::Mat4 iv;
  iv=m_cam->getViewMatrix();
  iv.transpose();

  /// now setup a basic 3 point lighting system
  m_key= new ngl::Light(ngl::Vec3(3,2,2),ngl::Colour(1,1,1,1),ngl::POINTLIGHT);
  m_key->setTransform(iv);
  m_key->enable();
  m_key->loadToShader("light[0]");
  m_fill = new ngl::Light(ngl::Vec3(-3,1.5,2),ngl::Colour(1,1,1,1),ngl::POINTLIGHT);
  m_fill->setTransform(iv);
  m_fill->enable();
  m_fill->loadToShader("light[1]");

  m_back= new ngl::Light(ngl::Vec3(0,1,-2),ngl::Colour(1,1,1,1),ngl::POINTLIGHT);
  m_back->setTransform(iv);
  m_back->enable();
  m_back->loadToShader("light[2]");


  shader->createShaderProgram("normalShader");

  shader->attachShader("normalVertex",ngl::VERTEX);
  shader->attachShader("normalFragment",ngl::FRAGMENT);
  shader->loadShaderSource("normalVertex","shaders/normalVertex.glsl");
  shader->loadShaderSource("normalFragment","shaders/normalFragment.glsl");

  shader->compileShader("normalVertex");
  shader->compileShader("normalFragment");
  shader->attachShaderToProgram("normalShader","normalVertex");
  shader->attachShaderToProgram("normalShader","normalFragment");

  shader->attachShader("normalGeo",ngl::GEOMETRY);
  shader->loadShaderSource("normalGeo","shaders/normalGeo.glsl");
  shader->compileShader("normalGeo");
  shader->attachShaderToProgram("normalShader","normalGeo");


  shader->linkProgramObject("normalShader");
  shader->use("normalShader");
  // now pass the modelView and projection values to the shader
  shader->setShaderParam1f("normalSize",0.01);

  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
}


// a simple structure to hold our vertex data
struct vertData
{
	GLfloat u; // tex cords from obj
	GLfloat v; // tex cords
	GLfloat nx; // normal from obj mesh
	GLfloat ny;
	GLfloat nz;
	GLfloat x; // position from obj
	GLfloat y;
	GLfloat z;
	GLfloat tx; // tangent calculated by us
	GLfloat ty;
	GLfloat tz;
	GLfloat bx; // binormal (bi-tangent really) calculated by us
	GLfloat by;
	GLfloat bz;
};


void NGLScene::loadModel()
{
	std::cout<<"loading mesh\n";
	// load the obj
	ngl::Obj mesh("models/troll.obj");
	std::cout<<"checking triangular\n";
	// this is only going to work for tri meshes so check
	if( ! mesh.isTriangular() )
	{
		std::cout<<"only works for tri meshes\n";
		exit(EXIT_FAILURE);
	}
	std::cout<<"getting lists\n";

  // get the obj data so we can process it locally
  std::vector <ngl::Vec3> verts=mesh.getVertexList();
  std::vector <ngl::Face> faces=mesh.getFaceList();
  std::vector <ngl::Vec3> tex=mesh.getTextureCordList();
  std::vector <ngl::Vec3> normals=mesh.getNormalList();

	std::cout<<"got mesh data\n";
	// now we are going to process and pack the mesh into an ngl::VertexArrayObject
	std::vector <vertData> vboMesh;
	vertData d;
	unsigned int nFaces=faces.size();
	unsigned int nNorm=normals.size();
	unsigned int nTex=tex.size();
	// loop for each of the faces
	for(unsigned int i=0;i<nFaces;++i)
	{
		// now for each triangle in the face (remember we ensured tri above)
		for(int j=0;j<3;++j)
		{
			// pack in the vertex data first
			d.x=verts[faces[i].m_vert[j]].m_x;
			d.y=verts[faces[i].m_vert[j]].m_y;
			d.z=verts[faces[i].m_vert[j]].m_z;
			// now if we have norms of tex (possibly could not) pack them as well
			if(nNorm >0 && nTex > 0)
			{
				d.nx=normals[faces[i].m_norm[j]].m_x;
				d.ny=normals[faces[i].m_norm[j]].m_y;
				d.nz=normals[faces[i].m_norm[j]].m_z;

				d.u=tex[faces[i].m_tex[j]].m_x;
				d.v=tex[faces[i].m_tex[j]].m_y;

      }
      // now if neither are present (only verts like Zbrush models)
      else if(nNorm ==0 && nTex==0)
      {
        d.nx=0;
        d.ny=0;
        d.nz=0;
        d.u=0;
        d.v=0;
      }
      // here we've got norms but not tex
      else if(nNorm >0 && nTex==0)
      {
        d.nx=normals[faces[i].m_norm[j]].m_x;
        d.ny=normals[faces[i].m_norm[j]].m_y;
        d.nz=normals[faces[i].m_norm[j]].m_z;
        d.u=0;
        d.v=0;

      }
      // here we've got tex but not norm least common
      else if(nNorm ==0 && nTex>0)
      {
        d.nx=0;
        d.ny=0;
        d.nz=0;
        d.u=tex[faces[i].m_tex[j]].m_x;
        d.v=tex[faces[i].m_tex[j]].m_y;
      }
    // now we calculate the tangent / bi-normal (tangent) based on the article here
    // http://www.terathon.com/code/tangent.html

    ngl::Vec3 c1 = normals[faces[i].m_norm[j]].cross(ngl::Vec3(0.0, 0.0, 1.0));
    ngl::Vec3 c2 = normals[faces[i].m_norm[j]].cross(ngl::Vec3(0.0, 1.0, 0.0));
    ngl::Vec3 tangent;
    ngl::Vec3 binormal;
    if(c1.length()>c2.length())
    {
      tangent = c1;
    }
    else
    {
      tangent = c2;
    }
    // now we normalize the tangent so we don't need to do it in the shader
    tangent.normalize();
    // now we calculate the binormal using the model normal and tangent (cross)
    binormal = normals[faces[i].m_norm[j]].cross(tangent);
    // normalize again so we don't need to in the shader
    binormal.normalize();
    d.tx=tangent.m_x;
    d.ty=tangent.m_y;
    d.tz=tangent.m_z;
    d.bx=binormal.m_x;
    d.by=binormal.m_y;
    d.bz=binormal.m_z;
    // finally add it to our mesh VAO structure
    vboMesh.push_back(d);
    }
  }

	// first we grab an instance of our VOA class as a TRIANGLE_STRIP
	m_vaoMesh= ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
	// next we bind it so it's active for setting data
	m_vaoMesh->bind();
	unsigned int meshSize=vboMesh.size();
	// now we have our data add it to the VAO, we need to tell the VAO the following
	// how much (in bytes) data we are copying
	// a pointer to the first element of data (in this case the address of the first element of the
	// std::vector
	m_vaoMesh->setData(meshSize*sizeof(vertData),vboMesh[0].u);
	// in this case we have packed our data in interleaved format as follows
	// u,v,nx,ny,nz,x,y,z
	// If you look at the shader we have the following attributes being used
	// attribute vec3 inVert; attribute 0
	// attribute vec2 inUV; attribute 1
	// attribute vec3 inNormal; attribure 2
	// so we need to set the vertexAttributePointer so the correct size and type as follows
	// vertex is attribute 0 with x,y,z(3) parts of type GL_FLOAT, our complete packed data is
	// sizeof(vertData) and the offset into the data structure for the first x component is 5 (u,v,nx,ny,nz)..x
	m_vaoMesh->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(vertData),5);
	// uv same as above but starts at 0 and is attrib 1 and only u,v so 2
	m_vaoMesh->setVertexAttributePointer(1,2,GL_FLOAT,sizeof(vertData),0);
	// normal same as vertex only starts at position 2 (u,v)-> nx
	m_vaoMesh->setVertexAttributePointer(2,3,GL_FLOAT,sizeof(vertData),2);

	// tangent same as vertex only starts at position 8 (u,v)-> nx
	m_vaoMesh->setVertexAttributePointer(3,3,GL_FLOAT,sizeof(vertData),8);

	// bi-tangent (or Binormal) same as vertex only starts at position 11 (u,v)-> nx
	m_vaoMesh->setVertexAttributePointer(4,3,GL_FLOAT,sizeof(vertData),11);


	// now we have set the vertex attributes we tell the VAO class how many indices to draw when
	// glDrawArrays is called, in this case we use buffSize (but if we wished less of the sphere to be drawn we could
	// specify less (in steps of 3))
	m_vaoMesh->setNumIndices(meshSize);
	// finally we have finished for now so time to unbind the VAO
	m_vaoMesh->unbind();

}



void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["TextureShader"]->use();
  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat4 M;

  M=m_transform.getMatrix()*m_mouseGlobalTX;
  MV=M*m_cam->getViewMatrix();
  MVP=MV*m_cam->getProjectionMatrix() ;

  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat4("MV",MV);

}


void NGLScene::loadMatricesToNormalShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["normalShader"]->use();
  ngl::Mat4 MV;
  ngl::Mat4 MVP;

  MV=m_transform.getMatrix()*m_mouseGlobalTX*m_cam->getViewMatrix();
  MVP=MV*m_cam->getProjectionMatrix();
  shader->setShaderParamFromMat4("MVP",MVP);

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
  // set this in the TX stack
  loadMatricesToShader();
  // now we bind back our vertex array object and draw
  m_vaoMesh->bind();
  // draw the VAO
  m_vaoMesh->draw();
  if(m_showNormals == true)
  {
    loadMatricesToNormalShader();
    m_vaoMesh->draw();
  }
  // now we are done so unbind
  m_vaoMesh->unbind();
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
  case Qt::Key_1 : m_key->enable(); m_key->loadToShader("light[0]"); break;
  case Qt::Key_2 : m_key->disable(); m_key->loadToShader("light[0]"); break;
  case Qt::Key_3 : m_fill->enable(); m_fill->loadToShader("light[1]"); break;
  case Qt::Key_4 : m_fill->disable(); m_fill->loadToShader("light[1]"); break;
  case Qt::Key_5 : m_back->enable(); m_back->loadToShader("light[2]"); break;
  case Qt::Key_6 : m_back->disable(); m_back->loadToShader("light[2]"); break;
  case Qt::Key_T : m_showNormals^=true; break;
  default : break;
  }
  // finally update the GLWindow and re-draw
 renderLater();
}
