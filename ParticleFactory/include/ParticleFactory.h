#ifndef __PARTICLE_FACTORY_H__
#define __PARTICLE_FACTORY_H__

#include "Particle.h"

/// @file ParticleFactory.h
/// @brief a factory class to create the particles
/// @author Jonathan Macey
/// @version 1.0
/// @date 8/2/12
/// @class ParticleFactory
/// @brief the job of this is to create the particles and return it to the
/// emitter class


// pre declare the Emitter class as we need this as the parent
class Emitter;

class ParticleFactory
{
  public :
    /// @brief create a particle and return
    /// @brief[in] _type the type of particle to create
    /// @brief[in] _pos the position of the particle start
    /// @brief[in] _dir the direction of the particle
    /// @brief[in] _c the colour of the particle
    /// @brief[in] _shaderName the name of the shader to use when rendering

    Particle *CreateParticle(
                              ParticleType _type,
                              ngl::Vec3 _pos,
                              ngl::Vec3 _dir,
                              ngl::Colour _c,
                              const std::string &_shaderName,
                              Emitter *_parent
                            );

  private :


};


#endif
