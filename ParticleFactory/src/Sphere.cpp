#include "Sphere.h"
#include "Emitter.h"
#include <cmath>
#include <ngl/Util.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Random.h>
#include <ngl/ShaderLib.h>

// set the type of the particle
const ParticleType Sphere::m_type=SPHERE;


Sphere::Sphere(
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
  m_emitAngle=360.0;
	ngl::Random *rng=ngl::Random::instance();
  // get the angles for emission
  GLfloat theta=ngl::radians(rng->randomNumber(m_emitAngle));
  GLfloat phi=ngl::radians(rng->randomNumber(m_emitAngle));
  // set the
  m_dir.m_x=sin(theta)*cos(phi);
  m_dir.m_y=sin(theta)*sin(phi);
  m_dir.m_z=cos(theta);
  m_dir.normalize();
	m_radius=rng->randomPositiveNumber(1.2)+0.1;
}


void Sphere::draw() const
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[m_shaderName]->use();
  shader->setShaderParam4f("Colour",m_colour.m_r,m_colour.m_g,m_colour.m_b,m_colour.m_a);
  ngl::Transformation t;
  t.setPosition(m_pos);
  t.setScale(m_radius,m_radius,m_radius);
  ngl::Mat4 MVP=t.getMatrix()*m_parent->getGlobalTransform()*m_parent->getCamera()->getVPMatrix();
  shader->setShaderParamFromMat4("MVP",MVP);
  // get an instance of the VBO primitives for drawing
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  // draw the sphere
  prim->draw("sphere");

}


void Sphere::update()
{
// more the particle by adding the Direction vector to the position
	
  m_pos+=m_dir;
  ++m_life;
  if(m_life > m_maxLife)
	{
    m_life=0.0;
    m_pos=m_parent->getPos();
		ngl::Random *rng=ngl::Random::instance();

    m_maxLife=rng->randomPositiveNumber(80);
    GLfloat theta=ngl::radians(rng->randomNumber(m_emitAngle));
    GLfloat phi=ngl::radians(rng->randomNumber(m_emitAngle));
    m_dir.m_x=sin(theta)*cos(phi);
    m_dir.m_y=sin(theta)*cos(theta);
    m_dir.m_z=cos(theta);
    m_dir.normalize();
    m_radius=rng->randomPositiveNumber(1.2)+0.01;

	}
}



 Sphere::~Sphere()
{
   std::cout<<"Sphere dtor\n";
}

