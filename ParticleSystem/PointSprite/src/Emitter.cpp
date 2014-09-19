#include "Emitter.h"
#include "ngl/Texture.h"
#include <ngl/ShaderLib.h>
#include <ngl/VertexArrayObject.h>

/// @brief ctor
/// @param _pos the position of the emitter
/// @param _numParticles the number of particles to create
Emitter::Emitter(
									ngl::Vec3 _pos,
									std::string _texture,
									int _numParticles
								 )
{
	for (int i=0; i< _numParticles; ++i)
	{
    m_particles.push_back(Particle(_pos,180,ngl::Vec3(0,1,0)));
	}
	m_numParticles=_numParticles;

	ngl::Texture t(_texture);
	m_textureID=t.setTextureGL();
	// create the array for the particles we will update later
  m_particlePositions.resize(_numParticles);

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
void Emitter::draw(const ngl::Mat4 &_mvp)
{
 int index=-1;
 ngl::Vec3 pos;
 for(int i=0; i<m_numParticles; ++i)
 {
   m_particlePositions[i]=m_particles[i].getPosition();
   m_particlePositions[i].m_w=m_particles[i].getLife()/4.0;
//   std::cout<<m_particlePositions[i]<<"\n";
 }
 ngl::ShaderLib *shader = ngl::ShaderLib::instance();
 (*shader)[m_shaderName]->use();
 shader->setShaderParamFromMat4("MVP",_mvp);
 ngl::VertexArrayObject *vao=ngl::VertexArrayObject::createVOA(GL_POINTS);
 vao->bind();
 vao->setData(m_numParticles*sizeof(ngl::Vec3),m_particlePositions[0].m_x);
 vao->setVertexAttributePointer(0,4,GL_FLOAT,sizeof(ngl::Vec3),0);
 vao->setNumIndices(m_numParticles);
 vao->draw();
 vao->unbind();
 vao->removeVOA();
 delete vao;

}
