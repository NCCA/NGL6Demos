#ifndef FIRSTPERSONCAMERA
#define FIRSTPERSONCAMERA

#include <ngl/Camera.h>
#include <ngl/Util.h>
#include <ngl/Mat4.h>

class FirstPersonCamera : public ngl::Camera
{
  public :

    FirstPersonCamera();
    FirstPersonCamera(const ngl::Vec3 &_eye, const ngl::Vec3 &_look, const ngl::Vec3 _up );
    ~FirstPersonCamera();



    /**
     * Moves the camera forward in the direction that it is pointing. (Fly mode)
     */
    void moveForward(float _amount);

    /**
     * Moves the camera in the opposite direction that it is pointing.
     */
    void moveBackward(float _amount);

    /**
     * Strafes that camera left, which is perpendicular to the direction it is facing.
     */
    void moveLeft(float _amount);

    /**
     * Strafes that camera right, which is perpendicular to the direction it is facing.
     */
    void moveRight(float _amount);

    void moveUp(float _amount);

    void moveDown(float _amount);

    /**
     * Rotates the camera in place in order to change the direction it is looking.
     *
     * @param yaw Rotates the camera around the yaw axis in radians. Positive looks right, negative looks left.
     * @param pitch Rotates the camera around the ptich axis in radians. Positive looks up, negative looks down.
     */
    void rotate(float _yaw, float _pitch);

  private :
    ngl::Mat4 m_transform;
    ngl::Mat4 m_view;
};



#endif
