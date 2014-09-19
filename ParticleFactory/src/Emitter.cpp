#include "Emitter.h"
#include "ParticleFactory.h"
#include <ngl/Random.h>
#include <typeinfo>
#include <Sphere.h>

Emitter::Emitter(ngl::Vec3 _pos, unsigned int _numParticles,const ngl::Camera *_cam )
{
  m_pos=_pos;
  m_numParticles=_numParticles;
  m_cam=_cam;
  // create our render factory
  ParticleFactory *p = new  ParticleFactory;
  // grab an instance of the random generator
  ngl::Random *rng=ngl::Random::instance();

  ngl::Vec3 dir;
  ngl::Colour c;
  int which;
  // resize the particle container for speed
  m_particles.resize(m_numParticles);
  // loop for each of the particles and create
  for(unsigned int i=0; i<m_numParticles; ++i)
  {
    // get a random direction and colour
    dir=rng->getRandomVec3();
    c=rng->getRandomColour();
    // choose which input we want
    which=(int)rng->randomPositiveNumber(4);
    switch(which)
    {
    // use the factory to create a new particle
    case 0 : m_particles[i]=p->CreateParticle(SPHERE,_pos,dir,c,"Phong",this); break;
    case 1 : m_particles[i]=p->CreateParticle(TEAPOT,_pos,dir,c,"Phong",this); break;
    case 2 : m_particles[i]=p->CreateParticle(TORUS,_pos,dir,c,"Phong",this); break;

    case 3 : m_particles[i]=p->CreateParticle(TROLL,_pos,dir,c,"Phong",this); break;
    }
  }
// we've done with the factory so delete it
  delete p;

}


Emitter::Emitter( ngl::Vec3 _pos,unsigned int _numParticles, const ngl::Camera *_cam, const ParticleType _type  )
{
  m_cam=_cam;
  m_pos=_pos;
  m_numParticles=_numParticles;
  // create our render factory
  ParticleFactory *p = new  ParticleFactory;

  ngl::Random *rng=ngl::Random::instance();

  ngl::Vec3 dir;
  ngl::Colour c;

  for(unsigned int i=0; i<_numParticles; ++i)
  {
    dir=rng->getRandomVec3();
    dir.normalize();
    c=rng->getRandomColour();
    m_particles.push_back(p->CreateParticle(_type,_pos,dir,c,"Phong",this));
  }
// we've done with the factory so delete it
  delete p;

}


void Emitter::addParticle(
                            ParticleType _type
                          )
{
  // create our render factory
  ParticleFactory *p = new  ParticleFactory;

  ngl::Random *rng=ngl::Random::instance();

  ngl::Vec3 dir;
  ngl::Colour c;
  dir=rng->getRandomVec3();
  c=rng->getRandomColour();
  m_particles.push_back(p->CreateParticle(_type,m_pos,dir,c,"Phong",this));
// we've done with the factory so delete it
  delete p;
  ++m_numParticles;
}

void Emitter::removeParticle()
{
  /// when we remove a particle we need to first check to see if we have any
  if(m_numParticles>0)
  {
    // as we store a pointer we need to explicitly call the dtor
    delete m_particles[m_numParticles-1];
    // now remove from the end of the std::vector
    m_particles.pop_back();
    // decrement the number of particles
    --m_numParticles;
  }
}
Emitter::~Emitter()
{
  // we need to call the dtor for each of the particles as pointer
  // however the std::vector will clear it's self at the end
  for(unsigned int i=0; i<m_numParticles; ++i)
  {
    delete m_particles[i];
  }

}


void Emitter::draw() const
{
  // call the draw method for each of the particles
  for(unsigned int i=0; i<m_numParticles; ++i)
  {
    m_particles[i]->draw();
  }
}

void Emitter::update()
{
  // call the update method for each particle
  for(unsigned int i=0; i<m_numParticles; ++i)
  {
    m_particles[i]->update();
  }
}
void Emitter::updateEmitAngle(GLfloat _a)
{
  /*
  // in this case only sphere has a set emit angle
  for(unsigned int i=0; i<m_numParticles; ++i)
  {
    // so if it is a sphere
    if(m_particles[i]->getType()==SPHERE)
    {
      // now we have to cast to a Sphere to call the setEmitAngle method
      dynamic_cast<Sphere *>(m_particles[i])->setEmitAngle(_a);
    }
  }
*/

  // in this case only sphere has a set emit angle
  for(unsigned int i=0; i<m_numParticles; ++i)
  {
    // so if it is a sphere
    if(Sphere *s = dynamic_cast<Sphere *>(m_particles[i]))
    {
      // now we have to cast to a Sphere to call the setEmitAngle method
      s->setEmitAngle(_a);
    }
  }


}

void Emitter::clearParticles()
{
  // however the std::vector will clear it's self at the end
  for(unsigned int i=0; i<m_numParticles; ++i)
  {
    delete m_particles[i];
  }
  // must remember to re-size the std::vector back to 0
  m_particles.clear();
  m_numParticles=0;
}
