#ifndef __TORUS_H__
#define __TORUS_H__


#include "Particle.h"

class Torus : public Particle
{
	public :

  /// @brief constructor must be called by the child classes
  /// @param[in] _pos the position of the particle
  /// @param[in] _dir the direction of the particle
  /// @param[in] _c the colour of the particle
  /// @param[in] _shaderName the name of the shader to use
  /// @param[in] _parent the parent (Emitter used to query global values)

    Torus(
           ngl::Vec3 _pos,
           ngl::Vec3 _dir,
           ngl::Colour _c,
           const std::string &_shaderName,
           Emitter *_parent
          );
    /// @brief dtor
    virtual ~Torus();
    /// @brief draw the particle
    virtual void draw() const;
    /// @brief update the particle position etc
    virtual void update();
    /// @brief accessor to get the particle type
    /// @returns the particle type
    virtual inline ParticleType getType()const {return m_type;}
  private :
    /// @brief rotation in the x
		float m_rotX;
    /// @brief x rotation increment value
    float m_rotUpdateX;
    /// @brief rotation in the y
		float m_rotY;
    /// @brief y rotation increment value
		float m_rotUpdateY;
    /// @brief rotation in the z
    float m_rotZ;
    /// @brief z rotation increment value
    float m_rotUpdateZ;
    /// @brief what type of particle this is
    const static ParticleType m_type;

};




#endif

