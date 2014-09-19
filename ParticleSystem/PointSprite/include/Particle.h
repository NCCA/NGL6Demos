#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <ngl/Vec3.h>
#include <ngl/Colour.h>

class Particle
{
public :

	/// @brief ctor
	/// @param _pos the start position of the particle
	Particle(
            ngl::Vec3 _pos,
						float _var,
            ngl::Vec3 _upDir

					);
	/// @brief a method to update the particle position
	void update();
	/// @brief mutator for the particle life
	inline void setLifeTime(
													 int _l
													){m_lifetime=_l;}
  inline ngl::Vec3 getPosition()const {return m_pos;}
	inline GLfloat getLife() const {return m_currentLife;}
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
	GLfloat m_var;
  ngl::Vec3 m_upDir;
};


#endif
