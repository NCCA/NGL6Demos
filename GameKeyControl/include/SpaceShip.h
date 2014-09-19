#ifndef __SPACESHIP_H__
#define __SPACESHIP_H__

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/TransformStack.h>

class SpaceShip
{
  public :
    /// @brief ctor
    /// @param _pos the initial position
    /// @param _fname the name of mesh to load
    SpaceShip(
               ngl::Vec3 _pos,
               std::string _fname
              );
   ///@brief draw method
   void draw(
              const std::string &_shader,
              ngl::Camera *_cam
             );

	 void move(
							float _x,
							float _y
						);

	 void rotate(
								float _rot
							);
  private :
    /// @brief the position of the ship
    ngl::Vec3 m_pos;
    /// @brief the ship's mesh
    ngl::Obj *m_mesh;
    /// @brief ship movement increment
    const static float s_increment;
    /// @brief a transform stack
    ngl::TransformStack m_transform;
    /// @brief the rotation of the ship
		float m_rotation;
  };

#endif
