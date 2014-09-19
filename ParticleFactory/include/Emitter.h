#ifndef __EMITTER_H__
#define __EMITTER_H__

#include "ParticleFactory.h"
#include "ParticleTypeinfo.h"
#include <ngl/Camera.h>

/// @file Emitter.h
/// @brief a simple emitter class
/// @author Jonathan Macey
/// @version 1.0
/// @date 8/2/12
/// @class Emitter
/// @brief this class acts as an emitter for the particles
/// it uses a factory method to construct the different
/// particles

class Emitter
{
  public :
    /// @brief ctor will create a random selection of particle types
    /// @param[in] _pos the default position of the emitter
    /// @param[in] _numParticles the number of particles to emit
    /// @param[in] _cam a pointer to the global camera
    Emitter(ngl::Vec3 _pos, unsigned int _numParticles, const ngl::Camera *_cam  );
    /// @brief ctor will create all particles of the same type based on _type
    /// @param[in] _pos the default position of the emitter
    /// @param[in] _numParticles the number of particles to emit
    /// @param[in] _cam a pointer to the global camera
    /// @param[in] _type the type of particle to create
    Emitter(ngl::Vec3 _pos, unsigned int _numParticles,const ngl::Camera *_cam, ParticleType _type  );

    /// @dtor will explicitly call the Particle dtors as they are stored as pointers
    ~Emitter();
    /// @brief update all the particles stored
    void update();
    /// @brief draw all the particles using the particle draw method
    void draw() const;
    /// @brief add a new particle to the end of the list
    /// @param[in] _type the type of particle to add
    void addParticle(
                      ParticleType _type
                    );
    /// @brief remove the last particle from the list
    void removeParticle();
    /// @brief this will update the emit angle of the SPHERE type particle only
    /// @param[in] _a the new emit angle for sphere, will only update once particle is re-born
    void updateEmitAngle(
                          GLfloat _a
                         );
    /// @brief accessor for the number of particles stored in the class
    /// @returns the number of particles
    inline unsigned int getNumParticles()const {return m_numParticles;}
    /// @brief get the global camera
    inline const ngl::Camera *getCamera() const {return m_cam;}
    /// @brief get the global transform stack
    /// @brief a method to clear all particles
    void clearParticles();
    /// @brief get the position
    inline ngl::Vec3 getPos()const {return m_pos;}
    /// @brief set the position
    inline void setPos(const ngl::Vec3 &_v){m_pos=_v;}
    inline void setGlobalTransform(const ngl::Mat4 &_t){m_globalMouseTX=_t;}
    inline const ngl::Mat4 & getGlobalTransform() const { return m_globalMouseTX;}


  private :
    /// @brief the particle list stores a pointer to each particle, these are created by the
    /// particle factory
    std::vector <Particle *> m_particles;
    /// @brief a pointer to the current camera, this is used for accesing the VP matrix to load to the
    /// shader
    const ngl::Camera *m_cam;
    /// @brief the number of particles
    unsigned int m_numParticles;
    /// @brief the position of the emitter this is passed to the particle for creation
    ngl::Vec3 m_pos;
    /// @brief a global mouse transform for drawing
    ngl::Mat4 m_globalMouseTX;
};


#endif
