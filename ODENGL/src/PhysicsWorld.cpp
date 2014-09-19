#include "PhysicsWorld.h"
#include <ngl/Vec4.h>
#include <algorithm>
#include <boost/function.hpp>
#include <boost/bind.hpp>

dWorldID PhysicsWorld::m_world;
dSpaceID PhysicsWorld::m_space;
int PhysicsWorld::m_maxContacts;
dContact * PhysicsWorld::m_contact;

dJointGroupID PhysicsWorld::m_contactgroup;

PhysicsWorld::PhysicsWorld(SPACE _s)
{
  dInitODE2(0);
  m_world=dWorldCreate();
  //argument is now unused and should be set to 0.
  m_contactgroup = dJointGroupCreate (0);
  m_spaceType=_s;
  switch(_s)
  {
  case SIMPLE : m_space=dSimpleSpaceCreate(0); break;
  case HASH :   m_space=dHashSpaceCreate(0); break;
  case QUAD :   std::cout <<"can't create quad space with simple ctor\n"; break;
  }
  // set some useful initial params

  m_linearDamping=0.0001;
  m_angularDamping=0.005;
  m_maxAngularSpeed=200;
  m_maxCorrectVelocity=0.1;
  m_contactSurfaceLayer=0.001;

  dWorldSetLinearDamping(m_world, m_linearDamping);
  dWorldSetAngularDamping(m_world, m_angularDamping);
  dWorldSetMaxAngularSpeed(m_world, m_maxAngularSpeed);
  dWorldSetCFM (m_world,1e-5);
  dWorldSetAutoDisableFlag (m_world,1);

  dWorldSetContactMaxCorrectingVel (m_world,m_maxCorrectVelocity);
  dWorldSetContactSurfaceLayer (m_world,m_contactSurfaceLayer);
  m_contactgroup = dJointGroupCreate (0);

  m_maxContacts=8;

  m_contact = new dContact[m_maxContacts];
}

PhysicsWorld::PhysicsWorld(ngl::Vec3 _center,ngl::Vec3 _extents, int _depth)
{
  dInitODE2(0);
  m_world=dWorldCreate();
  // a dVector3 is just a float [4] so we can convert easily
  m_space=dQuadTreeSpaceCreate(0,_center.m_openGL,_extents.m_openGL,_depth);
  // set some useful initial params
  m_linearDamping=0.0001;
  m_angularDamping=0.005;
  m_maxAngularSpeed=200;
  m_maxCorrectVelocity=0.1;
  m_contactSurfaceLayer=0.001;

  dWorldSetLinearDamping(m_world, m_linearDamping);
  dWorldSetAngularDamping(m_world, m_angularDamping);
  dWorldSetMaxAngularSpeed(m_world, m_maxAngularSpeed);

  dWorldSetContactMaxCorrectingVel (m_world,m_maxCorrectVelocity);
  dWorldSetContactSurfaceLayer (m_world,m_contactSurfaceLayer);
  m_maxContacts=8;
  m_contact = new dContact[m_maxContacts];

}
PhysicsWorld::~PhysicsWorld()
{
  dSpaceDestroy(m_space);
  dWorldDestroy(m_world);
  delete m_contact;

}

void PhysicsWorld::createGroundPlane(ngl::Vec4 _normal)
{
  ngl::Real d=_normal.m_w;
  _normal.normalize();
  m_gpNormal=_normal;
  m_gpNormal.m_w=d;
  // this will automatically add the plane to the space so we don't have to do
  // it
  m_groundPlaneID=dCreatePlane(m_space,_normal.m_x,_normal.m_y,_normal.m_z,d);
}


void PhysicsWorld::setGravity(const ngl::Vec3 &_g)
{
  m_gravity=_g;
  dWorldSetGravity(m_world,_g.m_x,_g.m_y,_g.m_z);
}

void PhysicsWorld::step(ngl::Real _size)
{
  dWorldStep(m_world,_size);
  dJointGroupEmpty (m_contactgroup);

}

void PhysicsWorld::quickStep(ngl::Real _size)
{
  dWorldQuickStep(m_world,_size);
  dJointGroupEmpty (m_contactgroup);

}





void PhysicsWorld::doCollisions()
{

  dSpaceCollide(m_space,0,collide);
}

void PhysicsWorld::reset()
{
  dSpaceDestroy(m_space);
  dWorldDestroy(m_world);
  dJointGroupDestroy(m_contactgroup);
  m_bodies.clear();
  m_world=dWorldCreate();
  //argument is now unused and should be set to 0.
  m_linearDamping=0.0001;
  m_angularDamping=0.005;
  m_maxAngularSpeed=200;
  m_maxCorrectVelocity=0.1;
  m_contactSurfaceLayer=0.001;
  dWorldSetGravity(m_world,m_gravity.m_x,m_gravity.m_y,m_gravity.m_z);

  dWorldSetLinearDamping(m_world, m_linearDamping);
  dWorldSetAngularDamping(m_world, m_angularDamping);
  dWorldSetMaxAngularSpeed(m_world, m_maxAngularSpeed);

  dWorldSetContactMaxCorrectingVel (m_world,m_maxCorrectVelocity);
  dWorldSetContactSurfaceLayer (m_world,m_contactSurfaceLayer);

  m_contactgroup = dJointGroupCreate (0);
  switch(m_spaceType)
  {
  case SIMPLE : m_space=dSimpleSpaceCreate(0); break;
  case HASH :   m_space=dHashSpaceCreate(0); break;
  }
  m_groundPlaneID=dCreatePlane(m_space,m_gpNormal.m_x,m_gpNormal.m_y,m_gpNormal.m_z,m_gpNormal.m_w);
}

void PhysicsWorld::setMaxContacts(int _c)
{
  m_maxContacts=_c;
  if(m_contact !=0)
  {
    delete m_contact;
  }

  m_contact = new dContact[m_maxContacts];

}

void PhysicsWorld::setWorldLinearDamping(ngl::Real _v)
{
  dWorldSetLinearDamping(m_world, _v);
}

void PhysicsWorld::setWorldAngularDamping(ngl::Real _v)
{
  dWorldSetAngularDamping(m_world, _v);
}

void PhysicsWorld::setWorldMaxAngularSpeed(ngl::Real _v)
{
  dWorldSetMaxAngularSpeed(m_world, _v);

}

void PhysicsWorld::setWorldContactMaxCorrectingVelocity(ngl::Real _v)
{
  dWorldSetContactMaxCorrectingVel (m_world,_v);

}

void PhysicsWorld::setWorldContactSurfaceLayer(ngl::Real _v)
{
  dWorldSetContactSurfaceLayer (m_world,_v);

}


void PhysicsWorld::addBody(const RigidBody b)
{
  m_bodies.push_back(b);
}


void PhysicsWorld::addForceToAllBodies(ngl::Vec3 _f)
{
  unsigned int size=m_bodies.size();
  for(unsigned int i=0; i<size; ++i)
  {
    // the body may have been auto disabled so enable first
    m_bodies[i].enable();
    m_bodies[i].addForce(_f);
  }
  step(0.05);
}


