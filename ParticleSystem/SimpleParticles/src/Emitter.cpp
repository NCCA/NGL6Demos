#include "Emitter.h"

/// @brief ctor
/// @param _pos the position of the emitter
/// @param _numParticles the number of particles to create
Emitter::Emitter( ngl::Vec3 _pos, int _numParticles )
{
	for (int i=0; i< _numParticles; ++i)
	{
    m_particles.push_back(Particle(_pos,this));
	}
	m_numParticles=_numParticles;
}
/// @brief a method to update each of the particles contained in the system
void Emitter::update()
{
	for(int i=0; i<m_numParticles; ++i)
	{
    m_particles[i].update();
	}
}
/// @brief a method to draw all the particles contained in the system
void Emitter::draw()
{
	for(int i=0; i<m_numParticles; ++i)
	{
    m_particles[i].draw();
	}


}
