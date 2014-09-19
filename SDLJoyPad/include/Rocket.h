#ifndef ROCKET_H__
#define ROCKET_H__
#include <ngl/Vec3.h>
#include <ngl/Camera.h>
#include <ngl/Obj.h>

/// @class Rocket simple rocket class container
class Rocket
{
  public :
    /// @brief ctor passing in pointers to the mesh and camera to allow drawing
    /// @param [in] _startPos where the rocket is going to start from
    /// @param [in] _speed the speed of the rocket
    /// @param [in] _mesh a pointer to the rocket mesh to draw (loaded in Game class)
    /// @param [in] _cam pointer to the camera from the game class
    Rocket(const ngl::Vec3 &_startPos,float _speed, ngl::Obj *_mesh,  ngl::Camera *_cam);
    /// @brief draw the rocket at the current location
    void draw()const;
    /// @brief add the speed to the z position of the rocket
    void update();
    /// @brief accessor for the m_active attribute, on creation this is set to true
    /// once life reaches max life this will set to false.
    /// @returns m_active
    inline bool isActive() const {return m_active;}
  private :
    /// @brief the position of the rocket
    ngl::Vec3 m_pos;
    /// @brief flag to indicate if the rocket is still alive
    bool m_active;
    /// @brief the current life of the rocket
    int m_life;
    /// @brief the max lifespan of the rocket (set in the ctor at present)
    int m_maxLife;
    /// @brief the speed of the rocket for updates
    float m_speed;
    /// @brief a pointer to the mesh to draw
    ngl::Obj *m_mesh;
    /// @brief a pointer to the system camera used for drawing
    ngl::Camera *m_camera;

};

#endif
