#ifndef PHYSICSWORLD_H__
#define PHYSICSWORLD_H__

//----------------------------------------------------------------------------------------------------------------------
/// @brief this is a minimal Facade wrapper for the code here
//----------------------------------------------------------------------------------------------------------------------

#include <vector>
#include <btBulletDynamicsCommon.h>
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Obj.h>
#include "NGLScene.h"


class PhysicsWorld
{
  friend class Vehicle;
  public :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor, this should really be a singleton as we have quite a few static members and only one world
    //----------------------------------------------------------------------------------------------------------------------
    PhysicsWorld();
    //----------------------------------------------------------------------------------------------------------------------
    // @brief dtor
    //----------------------------------------------------------------------------------------------------------------------
    ~PhysicsWorld();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief reset the simulation (remove all geo etc)
    //----------------------------------------------------------------------------------------------------------------------
    void reset();

    inline void setGravity(float _x, float _y, float _z)
    {
      m_dynamicsWorld->setGravity(btVector3(_x,_y,_z));
    }
    inline void setGravity(ngl::Vec3 _g)
    {
      m_dynamicsWorld->setGravity(btVector3(_g.m_x,_g.m_y,_g.m_z));
    }
    inline std::string getBodyNameAtIndex(unsigned int i) const{return m_bodies[i].name;}
    inline btRigidBody*  getBodyAtIndex(unsigned int i) const{return m_bodies[i].body;}

    void addGroundPlane(const ngl::Vec3 &_pos,const ngl::Vec3 &_g);

    void addSphere(std::string _shapeName,const ngl::Vec3 &_pos,ngl::Real _mass,const ngl::Vec3 &_inertia);
    void addBox(std::string _shapeName,const ngl::Vec3 &_pos);
    void addCapsule(std::string _shapeName,const ngl::Vec3 &_pos);
    void addCylinder(std::string _shapeName,const ngl::Vec3 &_pos);
    void addCone(std::string _shapeName,const ngl::Vec3 &_pos);
    void addMesh(std::string _shapeName,const ngl::Vec3 &_pos);
    void step(float _time, float _step);
    void addImpulse(const ngl::Vec3 &_i);
    void * getUserData(unsigned int _index);

    inline unsigned int getNumCollisionObjects()const
    {
      return m_dynamicsWorld->getNumCollisionObjects();
    }

    ngl::Vec3 getPosition(unsigned int _index);
    ngl::Mat4 getTransformMatrix(unsigned int _index);
    int getCollisionShape(unsigned int _index) const;
  private :
    // if we need to access the bodies we need to store our own version of the pointer
    // as there is no real way of accessing it from bullet once it is added,
    // with luck the index will be the same as the one in the
    // bullet sim
    typedef struct
    {
      std::string name;
      btRigidBody* body;
    }Body;


    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_dispatcher;
    btBroadphaseInterface* m_overlappingPairCache ;
    btSequentialImpulseConstraintSolver* m_solver;
    btDiscreteDynamicsWorld* m_dynamicsWorld;
    btCollisionShape* m_groundShape;
    std::vector <Body> m_bodies;
};

#endif

