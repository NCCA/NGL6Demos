#ifndef PARTICLE_H__
#define PARTICLE_H__

#include <ngl/Vec3.h>
#include <ngl/Colour.h>

// pre-declare emitter class
class Emitter;

class Particle
{
public :

	/// @brief ctor
	/// @param _pos the start position of the particle
	Particle(
            ngl::Vec3 _pos,
            Emitter *_emitter
					);
	/// @brief a method to update the particle position
  void update();
	/// @brief a method to draw the particle
  void draw() const;
	/// @brief mutator for the particle life
	inline void setLifeTime(int _l){m_lifetime=_l;}
private :
	/// @brief the curent particle position
	ngl::Vec3 m_pos;
	/// @brief the original particle position
	ngl::Vec3 m_origin;
	/// @brief start position
	ngl::Vec3 m_startPos;
	/// @brief the direction vector of the particle
	ngl::Vec3 m_dir;
	/// @brief the current life value of the particle
	int m_currentLife;
	/// @brief the total lifetime of the current particle
	int m_lifetime;
	/// @brief colour of the particle
	ngl::Colour m_colour;
  const Emitter *m_emitter;
};


#endif
