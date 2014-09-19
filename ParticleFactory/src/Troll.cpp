#include "Troll.h"
#include <cmath>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Random.h>
#include "Emitter.h"

const ParticleType Troll::m_type=TROLL;


Troll::Troll(
								ngl::Vec3 _pos,
								ngl::Vec3 _dir,
								ngl::Colour _c,
								const std::string &_shaderName,
                Emitter *_parent
								) :
									 Particle(
														 _pos,
														 _dir,
														 _c,
														 _shaderName,
                             _parent
														)
{
	ngl::Random *rng=ngl::Random::instance();

	m_rotX=rng->randomNumber(360);
	m_rotY=rng->randomNumber(360);
	m_rotZ=rng->randomNumber(360);

	m_rotUpdateX=rng->randomNumber(10);
	m_rotUpdateY=rng->randomNumber(10);
	m_rotUpdateZ=rng->randomNumber(10);

}


void Troll::draw() const
{

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[m_shaderName]->use();
  shader->setShaderParam4f("Colour",m_colour.m_r,m_colour.m_g,m_colour.m_b,m_colour.m_a);

  ngl::Transformation t;
  t.setPosition(m_pos);
  t.setRotation(m_rotX,m_rotY,m_rotZ);;
  ngl::Mat4 MVP=t.getMatrix()*m_parent->getGlobalTransform()*m_parent->getCamera()->getVPMatrix();
  shader->setShaderParamFromMat4("MVP",MVP);

    // get an instance of the VBO primitives for drawing
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    // draw the sphere
    prim->draw("troll");

}


void Troll::update()
{
// more the particle by adding the Direction vector to the position
	ngl::Random *rng=ngl::Random::instance();

  m_pos+=m_dir;
  ++m_life;
	m_rotX+=m_rotUpdateX;
	m_rotY+=m_rotUpdateY;
	m_rotZ+=m_rotUpdateZ;

  if(m_life > m_maxLife)
	{
    m_life=0.0;
    m_pos=m_parent->getPos();

    m_maxLife=rng->randomPositiveNumber(80);

		m_rotX=rng->randomNumber(360);
		m_rotUpdateX=rng->randomPositiveNumber(10);
		m_rotY=rng->randomNumber(360);
		m_rotUpdateY=rng->randomPositiveNumber(10);
		m_rotZ=rng->randomNumber(360);
		m_rotUpdateZ=rng->randomPositiveNumber(10);
	}
}


Troll::~Troll()
{
	std::cout<<"Troll dtor\n";
}

