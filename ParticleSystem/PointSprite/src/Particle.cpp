#include "Particle.h"
#include <ngl/Random.h>
#include <ngl/TransformStack.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
/// @brief ctor
/// @param _pos the start position of the particle
Particle::Particle(
									 ngl::Vec3 _pos,
                   float _var,
                   ngl::Vec3 _upDir
  )
{
	m_pos=_pos;
	m_origin=_pos;
  ngl::Random *rand=ngl::Random::instance();
	m_var=_var;
	m_upDir=_upDir;
	GLfloat theta=(rand->randomNumber(_var));
	GLfloat phi=(rand->randomNumber(_var));
	m_dir.m_x=(_upDir.m_x+sin(theta)*cos(phi))/_var;
	m_dir.m_y=(_upDir.m_y+sin(theta)*cos(theta))/_var;
	m_dir.m_z=(_upDir.m_z+cos(theta))/_var;

	m_lifetime=rand->randomPositiveNumber(250);
	m_currentLife=0;
}
/// @brief a method to update the particle position
void Particle::update()
{
	ngl::Random *rand=ngl::Random::instance();

	m_pos+=m_dir;

	++m_currentLife;
	if(m_currentLife > m_lifetime)
	{
		m_pos=m_origin;
		m_currentLife=0;
		GLfloat theta=(rand->randomNumber(m_var));
		GLfloat phi=(rand->randomNumber(m_var));
		m_dir.m_x=(m_upDir.m_x+sin(theta)*cos(phi))/m_var;
		m_dir.m_y=(m_upDir.m_y+sin(theta)*cos(theta))/m_var;
		m_dir.m_z=(m_upDir.m_z+cos(theta))/m_var;
	}
}
