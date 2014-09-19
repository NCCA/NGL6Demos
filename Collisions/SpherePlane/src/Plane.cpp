#include "Plane.h"
#include <ngl/ShaderLib.h>
#include <ngl/Util.h>
#include <ngl/Vec3.h>
#include <ngl/VertexArrayObject.h>
#include <ngl/VAOPrimitives.h>
Plane::Plane(const ngl::Vec3 &_center, GLfloat _w, GLfloat _d )
{
  //store the values
  m_center=_center;
  m_width=_w;
  m_depth=_d;
  // now create the vertices based on the values passed in
  m_oVerts[0].m_x=m_center.m_x-(m_width/2.0f);
  m_oVerts[0].m_y=m_center.m_y;
  m_oVerts[0].m_z=m_center.m_z+(m_depth/2.0f);

  m_oVerts[1].m_x=m_center.m_x+(m_width/2.0f);
  m_oVerts[1].m_y=m_center.m_y;
  m_oVerts[1].m_z=m_center.m_z+(m_depth/2.0f);

  m_oVerts[2].m_x=m_center.m_x+(m_width/2.0f);
  m_oVerts[2].m_y=m_center.m_y;
  m_oVerts[2].m_z=m_center.m_z-(m_depth/2.0f);


  m_oVerts[3].m_x=m_center.m_x-(m_width/2.0f);
  m_oVerts[3].m_y=m_center.m_y;
  m_oVerts[3].m_z=m_center.m_z-(m_depth/2.0f);
  // finally calculate the surface m_normal
  m_normal=calcNormal(m_oVerts[3],m_oVerts[2],m_oVerts[1]);
  m_xrot=m_zrot=0;
}

Plane::Plane()
{
  // default ctor set 0.0 as origin and unit size
  m_center.set(0,0,0);
  m_width=1.0;
  m_depth=1.0;

  m_oVerts[0].m_x=m_center.m_x-(m_width/2.0f); m_oVerts[0].m_y=m_center.m_y; m_oVerts[0].m_z=m_center.m_z+(m_depth/2.0f);
  m_oVerts[1].m_x=m_center.m_x+(m_width/2.0f); m_oVerts[1].m_y=m_center.m_y; m_oVerts[1].m_z=m_center.m_z+(m_depth/2.0f);
  m_oVerts[2].m_x=m_center.m_x+(m_width/2.0f); m_oVerts[2].m_y=m_center.m_y; m_oVerts[2].m_z=m_center.m_z-(m_depth/2.0f);
  m_oVerts[3].m_x=m_center.m_x-(m_width/2.0f); m_oVerts[3].m_y=m_center.m_y; m_oVerts[3].m_z=m_center.m_z-(m_depth/2.0f);
  m_normal=ngl::calcNormal(m_oVerts[3],m_oVerts[2],m_oVerts[1]);
}

Plane::~Plane()
{
// do nothing for now
}

void Plane::loadMatricesToShader( ngl::TransformStack &_tx, ngl::Camera *_cam ) const
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=_tx.getCurrentTransform().getMatrix();
  MV=_tx.getCurrAndGlobal().getMatrix()*_cam->getViewMatrix() ;
  MVP=MV*_cam->getProjectionMatrix() ;
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
}



void Plane::draw(  const std::string &_shaderName,  ngl::TransformStack &_transformStack,ngl::Camera *_cam )
{
  // create the m_points array for drawing the quad as a tri
  std::vector <ngl::Vec3> normals(4);

  ngl::Vec3 normal=ngl::calcNormal(m_verts[0],m_verts[2],m_verts[1]);

	for(int i=0; i<4; ++i)
	{
		normals[i]=normal;
	}
	GLubyte indices[]={0,1,3,3,2,1};
	ngl::ShaderLib *shader=ngl::ShaderLib::instance();
	(*shader)[_shaderName]->use();
	shader->setRegisteredUniform4f("Colour",1,1,0,0);
	// first we draw the triangle
	// we build up a vertex array for the lines of the start and end points and draw
	_transformStack.pushTransform();
	{

    ngl::VertexArrayObject *vao= ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
    vao->bind();
    vao->setIndexedData(4*sizeof(ngl::Vec3),m_verts[0].m_x,sizeof(indices),&indices[0],GL_UNSIGNED_BYTE);
    vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(ngl::Vec3),0);
    vao->setIndexedData(4*sizeof(ngl::Vec3),normals[0].m_x,sizeof(indices),&indices[0],GL_UNSIGNED_BYTE);
    vao->setVertexAttributePointer(2,3,GL_FLOAT,sizeof(ngl::Vec3),0);
    vao->setNumIndices(6);
    loadMatricesToShader(_transformStack,_cam);
    vao->draw();
    vao->removeVOA();

	}
	_transformStack.popTransform();


	_transformStack.pushTransform();
	{
		std::vector<ngl::Vec3> lines(2);
		lines[0]=m_center;
		lines[1]=(m_normal*4.0);
		ngl::VertexArrayObject *vao= ngl::VertexArrayObject::createVOA(GL_LINES);
		vao->bind();
		vao->setData(2*sizeof(ngl::Vec3),lines[0].m_x);
		vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(ngl::Vec3),0);
		vao->setNumIndices(2);
		vao->draw();
		vao->removeVOA();

	}
	_transformStack.popTransform();
}



// modify the verts based on the dt and a flag to indicate which to tilt
void Plane::tilt(GLfloat dt,bool X,  bool Z)
{

  // create a matrix for the m_xrotation and set to ident
  ngl::Mat4 m_xrotMatrix=1;
  // create a m_zrotation matrix and set to ident
  ngl::Mat4 m_zrotMatrix=1;

  if(X)
  {
    m_xrot+=dt;
  }

  if (Z)
  {
    m_zrot+=dt;
  }

  m_xrotMatrix.rotateX(m_xrot);
  m_zrotMatrix.rotateZ(m_zrot);

  // combine them together for the final rotation
  ngl::Mat4 rotMatrix=m_zrotMatrix*m_xrotMatrix;
  // now modify the verts by multiplying the points with the matrix
  for(int i=0; i<4; ++i)
  {
    m_verts[i]=rotMatrix*m_oVerts[i];
  }
  m_normal=calcNormal(m_verts[3],m_verts[2],m_verts[1]);
}
