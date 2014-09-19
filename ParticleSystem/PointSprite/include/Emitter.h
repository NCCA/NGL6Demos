#ifndef __EMITTER_H__
#define __EMITTER_H__
#include <vector>
#include <QTime>
#include <ngl/Camera.h>
#include <ngl/Mat4.h>
#include <ngl/Vec3.h>


#include "Particle.h"


class Emitter
{
public :

	/// @brief ctor
	/// @param _pos the position of the emitter
	/// @param _numParticles the number of particles to create
	Emitter(
           ngl::Vec3 _pos,
					 std::string _texture,
					 int _numParticles
				 );
	/// @brief a method to update each of the particles contained in the system
	void update();
	/// @brief a method to draw all the particles contained in the system
  void draw(const ngl::Mat4 &_mvp);
	inline GLuint getTextureID() const {return m_textureID;}
  inline void setCam(ngl::Camera *_cam){m_cam=_cam;}
  inline ngl::Camera * getCam()const {return m_cam;}
  inline void setShaderName(const std::string &_n){m_shaderName=_n;}
  inline const std::string getShaderName()const {return m_shaderName;}

private :
	/// @brief the position of the emitter
  ngl::Vec3 m_pos;
	/// @brief the number of particles
	int m_numParticles;
	/// @brief the container for the particles
	std::vector <Particle> m_particles;
	/// @brief the container for the points passed to GL
  std::vector <ngl::Vec4> m_particlePositions;
	/// @brief the texture ID for the emitters point sprite
	GLuint m_textureID;
  /// @brief the name of the shader to use
  std::string m_shaderName;
  /// @brief a pointer to the camera used for drawing
  ngl::Camera *m_cam;

};


#endif

