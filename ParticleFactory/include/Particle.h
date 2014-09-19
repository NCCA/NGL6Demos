#ifndef __PARTICLE__
#define __PARTICLE__

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Colour.h>
#include <ngl/TransformStack.h>
#include "ParticleTypeinfo.h"

/// @file Particle.h
/// @brief an abstract particle class for the ParticleFactory
/// @author Jonathan Macey
/// @version 1.0
/// @date 8/2/12
/// @class Particle
/// @brief this is the base class for all particles, it contains pure virtual methods
/// required by the child classes


// predeclare the emitter for inclusion here
class Emitter;


class Particle
{
public:
  /// @brief constructor must be called by the child classes
  /// @param[in] _pos the position of the particle
  /// @param[in] _dir the direction of the particle
  /// @param[in] _c the colour of the particle
  /// @param[in] _shaderName the name of the shader to use
  /// @param[in] _parent the parent (Emitter used to query global values)
	Particle(
           ngl::Vec3 _pos,
           ngl::Vec3 _dir,
	         ngl::Colour _c,
					 const std::string &_shaderName,
           Emitter *_parent
	        );
  /// @brief  brief destructor

	virtual ~Particle();
  /// @brief method to draw the particle
  virtual void draw() const =0;
  /// @brief methods to update the particle
	virtual void update()=0;
  /// @brief method to get particle type
  virtual ParticleType getType() const =0;
  /// @brief set the maximum life of the particle
  inline void setMaxLife(int _l){m_maxLife=_l;}
protected :
  /// @brief  Position of the Particle
  ngl::Vec3 m_pos;
  /// @brief  Direction of the Particle
  ngl::Vec3 m_dir;
  /// @brief   Colour of the Particle
  ngl::Colour m_colour;
	/// @brief the current life of the particle
  GLfloat m_life;
	/// @brief the max life of the partice
  GLfloat m_maxLife;
	/// @brief the name of the shader to use
	std::string m_shaderName;
  /// @brief the parent Emitter
  Emitter *m_parent;
};

#endif

