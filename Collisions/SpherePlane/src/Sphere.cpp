#include "Sphere.h"
#include <ngl/VAOPrimitives.h>


Sphere::Sphere(const ngl::Vec3 &_pos,const ngl::Vec3 &_dir, GLfloat _rad)
{
  // set values from params
  m_pos=_pos;
  m_dir=_dir;
  m_radius=_rad;
  m_hit=false;
}

Sphere::Sphere()
{
  m_hit=false;
}

void Sphere::loadMatricesToShader(ngl::Transformation &_tx, const ngl::Mat4 &_globalTx, ngl::Camera *_cam ) const
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=_tx.getMatrix()*_globalTx;
  MV=M*_cam->getViewMatrix() ;
  MVP=MV*_cam->getProjectionMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
}


void Sphere::draw( const std::string &_shaderName, const ngl::Mat4 &_globalTx,  ngl::Camera *_cam )const
{

  // draw wireframe if hit
  if(m_hit)
  {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  }
  else
  {
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }


  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use(_shaderName);
  // grab an instance of the primitives for drawing
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  ngl::Transformation t;
  t.setPosition(m_pos);
  t.setScale(m_radius,m_radius,m_radius);
  loadMatricesToShader(t,_globalTx,_cam);
  prim->draw("sphere");

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}



void Sphere :: set(const ngl::Vec3 &_pos,   const ngl::Vec3 &_dir,  GLfloat _rad )
{
  m_pos=_pos;
  m_dir=_dir;
  m_radius=_rad;
}

void Sphere::move()
{
  // store the last position
  m_lastPos=m_pos;
  // update the current position
  m_pos+=m_dir;
  // get the next position
  m_nextPos=m_pos+m_dir;
  m_hit=false;
}












