#include "Rocket.h"
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>

Rocket::Rocket(const ngl::Vec3 &_startPos, float _speed, ngl::Obj *_mesh, ngl::Camera *_cam)
{
  m_pos=_startPos;
  m_life=0;
  m_camera=_cam;
  m_mesh=_mesh;
  m_speed=_speed;
  m_active=true;
  m_maxLife=60;
}

void Rocket::update()
{
  m_pos.m_z-=m_speed;
  if(++m_life >= m_maxLife)
  {
    m_active=false;
  }
}

void Rocket::draw() const
{
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Texture"]->use();
  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat4 M;
  ngl::Transformation t;
  t.setPosition(m_pos);
  t.setRotation(0,-90,0);
  MVP= t.getMatrix()*m_camera->getVPMatrix();
  shader->setShaderParamFromMat4("MVP",MVP);
  m_mesh->draw();
}
