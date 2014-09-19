#ifndef VEHICLE_H__
#define VEHICLE_H__
#include <btBulletDynamicsCommon.h>
#include <ngl/Mat4.h>
class PhysicsWorld;

class Vehicle
{
  // for ease of access make the PhysicsWorld our friend (save on accesors)
  friend class PhysicsWorld;
  public :

    Vehicle(PhysicsWorld *_world);
    void draw();
    ngl::Mat4 getWheelTransform(int i);
    void left();
    void right();
    void accelerate();
    void stop();
    void update();
    void reset();
    void resetSteer(){m_VehicleSteering = 0.0;}
    ngl::Mat4 getChassisTransform();

  private :

    btRigidBody* m_carChassis;

    btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
    btRaycastVehicle::btVehicleTuning	m_tuning;
    btVehicleRaycaster*	m_vehicleRayCaster;
    btRaycastVehicle*	m_vehicle;
    btCollisionShape*	m_wheelShape;
    PhysicsWorld *m_world;
    float	m_VehicleSteering ;
    float	m_steeringIncrement ;
    float	m_steeringClamp ;
    float	m_EngineForce ;
    float	m_BreakingForce;




};



#endif
