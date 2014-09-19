#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera() :
  ngl::Camera(ngl::Vec3(2,2,2),ngl::Vec3(0,0,0),ngl::Vec3(0,1,0))
{

}

FirstPersonCamera::FirstPersonCamera(const ngl::Vec3 &_eye, const ngl::Vec3 &_look, const ngl::Vec3 _up ) :
  ngl::Camera(_eye,_look,_up)
{

}

FirstPersonCamera::~FirstPersonCamera()
{

}


/**
 * Sets the position of the camera.
 *
 * @param position The position to move to.
 */

/**
 * Moves the camera forward in the direction that it is pointing. (Fly mode)
 */
void FirstPersonCamera::moveForward(float _amount)
{
  // get the world forward vector
  ngl::Vec3 v = m_viewMatrix.getForwardVector();
  v*=_amount;
  v.normalize();
  moveEye(v.m_x,v.m_y,v.m_z);
}

/**
 * Moves the camera in the opposite direction that it is pointing.
 */
void FirstPersonCamera::moveBackward(float _amount)
{
  moveForward(-_amount);

}

/**
 * Strafes that camera left, which is perpendicular to the direction it is facing.
 */
void FirstPersonCamera::moveLeft(float _amount)
{

     ngl::Vec3 left=m_viewMatrix.getLeftVector();
     left.normalize();
     left*=(_amount);
     slide(left.m_x,left.m_y,left.m_z);
}

/**
 * Strafes that camera right, which is perpendicular to the direction it is facing.
 */
void FirstPersonCamera::moveRight(float _amount)
{
  ngl::Vec3 left=m_viewMatrix.getLeftVector();
  left.normalize();
  left*=(-_amount);
  slide(left.m_x,left.m_y,left.m_z);

}

void FirstPersonCamera::moveUp(float _amount)
{
  ngl::Vec3 left=m_viewMatrix.getUpVector();
  left*=(_amount);
  left.normalize();
  slide(left.m_x,left.m_y,left.m_z);

}

void FirstPersonCamera::moveDown(float _amount)
{
  ngl::Vec3 left=m_viewMatrix.getUpVector();
  left*=(-_amount);
  left.normalize();
  slide(left.m_x,left.m_y,left.m_z);

}

/**
 * Rotates the camera in place in order to change the direction it is looking.
 *
 * @param yaw Rotates the camera around the yaw axis in radians. Positive looks right, negative looks left.
 * @param pitch Rotates the camera around the ptich axis in radians. Positive looks up, negative looks down.
 */
void FirstPersonCamera::rotate(float _yaw, float _pitch)
{
  //rotate(_yaw,_pitch);
  pitch(_pitch);
  yaw(_yaw);
}
