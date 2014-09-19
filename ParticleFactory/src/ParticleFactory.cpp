#include "ParticleFactory.h"
#include "Sphere.h"
#include "Teapot.h"
#include "Torus.h"
#include "Troll.h"

Particle * ParticleFactory::CreateParticle(
                                            ParticleType _type,
                                            ngl::Vec3 _pos,
                                            ngl::Vec3 _dir,
                                            ngl::Colour _c,
                                            const std::string &_shaderName,
                                            Emitter *_parent
                                          )
{

  if(_type==TEAPOT)
  {
    return new Teapot(_pos,_dir,_c,_shaderName,_parent);
  }
  else if(_type==SPHERE)
  {
    return new Sphere(_pos,_dir,_c,_shaderName,_parent);
  }
  if(_type==TORUS)
  {
    return new Torus(_pos,_dir,_c,_shaderName,_parent);
  }
  if(_type==TROLL)
  {
    return new Troll(_pos,_dir,_c,_shaderName,_parent);
  }

}
