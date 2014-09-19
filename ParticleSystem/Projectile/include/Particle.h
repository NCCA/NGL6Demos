#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <ngl/Vec3.h>
#include <ngl/Colour.h>

class Emitter;

class Particle
{
public :

	/// @brief ctor
	/// @param _pos the start position of the particle
	Particle(
						ngl::Vec3 _pos,
            ngl::Vec3 *_wind,
            Emitter *_emitter
					);
	/// @brief a method to update the particle position
	void update();
	/// @brief a method to draw the particle
	void draw();

private :
	/// @brief the curent particle position
	ngl::Vec3 m_pos;
	/// @brief the original particle position
	ngl::Vec3 m_origin;
	/// @brief the direction vector of the particle
	ngl::Vec3 m_dir;
	/// @brief the current life value of the particle
	float m_currentLife;
	/// @brief gravity
	GLfloat m_gravity;
	/// @brief the wind vector
	ngl::Vec3 *m_wind;
  /// @brief a pointer to our emitter
  const Emitter *m_emitter;
};


#endif
