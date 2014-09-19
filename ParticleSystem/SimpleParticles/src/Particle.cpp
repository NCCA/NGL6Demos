#include "Particle.h"
#include <ngl/Random.h>
#include <ngl/TransformStack.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include "Emitter.h"
/// @brief ctor
/// @param _pos the start position of the particle
Particle::Particle(ngl::Vec3 _pos, Emitter *_emitter  )
{
	m_pos=_pos;
	m_origin=_pos;
  ngl::Random *rand=ngl::Random::instance();
  m_dir=rand->getRandomNormalizedVec3();
	m_colour=rand->getRandomColour();
	m_lifetime=rand->randomPositiveNumber(200);
	m_currentLife=0;
  m_emitter = _emitter;
}
/// @brief a method to update the particle position
void Particle::update()
{
	m_pos+=m_dir;
	++m_currentLife;
	if(m_currentLife > m_lifetime)
	{
		m_pos=m_origin;
		m_currentLife=0;
		ngl::Random *rand=ngl::Random::instance();
    m_dir=rand->getRandomNormalizedVec3();
		m_lifetime=rand->randomPositiveNumber(200);

	}
}
/// @brief a method to draw the particle
void Particle::draw() const
{
  // get the VBO instance and draw the built in teapot
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  ngl::Transformation trans;
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use(m_emitter->getShaderName());
	trans.setPosition(m_pos);

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=trans.getMatrix();
  MV=M*m_emitter->getCam()->getViewMatrix();
  MVP=MV*m_emitter->getCam()->getProjectionMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);

  prim->draw("sphere");

}
