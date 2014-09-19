#include "PhysicsWorld.h"


void PhysicsWorld::collide(void *_data, dGeomID _o1, dGeomID _o2)
{
  int i;
  //std::cout<<"collide\n";
  // exit without doing anything if the two bodies are connected by a joint
  dBodyID b1 = dGeomGetBody(_o1);
  dBodyID b2 = dGeomGetBody(_o2);
  if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)) return;

  for (i=0; i<m_maxContacts; i++)
  {
    m_contact[i].surface.mode = dContactBounce;// | dContactSoftCFM;
    m_contact[i].surface.mu = dInfinity;
    m_contact[i].surface.mu2 = 0;
    m_contact[i].surface.bounce = 0.3;
    m_contact[i].surface.bounce_vel = 0.1;
    m_contact[i].surface.soft_cfm = 0.01;
  }
  if (int numc = dCollide (_o1,_o2,m_maxContacts,&m_contact[0].geom,sizeof(dContact)))
  {
    dMatrix3 RI;
    dRSetIdentity (RI);
    const dReal ss[3] = {0.02,0.02,0.02};
    for (i=0; i<numc; i++)
    {
      dJointID c = dJointCreateContact (m_world,m_contactgroup,m_contact+i);
      dJointAttach (c,b1,b2);
    }
  }
}
