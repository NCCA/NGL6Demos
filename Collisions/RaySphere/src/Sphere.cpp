#include "Sphere.h"
#include <ngl/VAOPrimitives.h>


Sphere::Sphere(ngl::Vec3 _pos,  GLfloat _rad)
{
  // set values from params
  m_pos=_pos;
  m_radius=_rad;
  m_hit=false;
}

Sphere::Sphere()
{
  m_hit=false;
}

void Sphere::loadMatricesToShader( ngl::Transformation &_tx,  const ngl::Mat4 &_globalMat, ngl::Camera *_cam  ) const
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  MV=_tx.getMatrix()  *_globalMat*_cam->getViewMatrix() ;
  MVP=MV*_cam->getProjectionMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
}


void Sphere::draw(const std::string &_shaderName,const ngl::Mat4 &_globalMat, ngl::Camera *_cam )const

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
  loadMatricesToShader(t,_globalMat,_cam);
  prim->draw("sphere");

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}














