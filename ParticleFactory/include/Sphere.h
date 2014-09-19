#ifndef __SPHERE_H__
#define __SPHERE_H__

/// @file Sphere.h
/// @brief A Sphere particle class
/// @author Jonathan Macey
/// @version 1.0
/// @date 8/2/12
/// @class Particle
/// @brief this inherits from the base particle and draws a sphere


#include "Particle.h"

class Sphere : public Particle
{
	public :

    /// @brief constructor must be called by the child classes
    /// @param[in] _pos the position of the particle
    /// @param[in] _dir the direction of the particle
    /// @param[in] _c the colour of the particle
    /// @param[in] _shaderName the name of the shader to use
    /// @param[in] _parent the parent (Emitter used to query global values)
    Sphere(
            ngl::Vec3 _pos,
            ngl::Vec3 _dir,
            ngl::Colour _c,
						const std::string &_shaderName,
            Emitter *_parent
          );
    /// @param dtor
		virtual ~Sphere();
    /// @brief set the angle for emission this is in this class
    /// @param[in] _a the emit angle to use as max emit angle
    inline void setEmitAngle(GLfloat _a){m_emitAngle=_a;}
    /// @brief draw the sphere
    virtual void draw() const;
    /// @brief update the sphere
    virtual void update();
    /// @brief accessor to get the particle type
    /// @returns the particle type
    virtual inline ParticleType getType()const {return m_type;}

  private :
    /// @brief the max emission angle for the sphere particle
    GLfloat m_emitAngle;
    /// @brief the current radius for the sphere
		GLfloat m_radius;
    /// @brief what type of particle this is
    const static ParticleType m_type;

 };


#endif
