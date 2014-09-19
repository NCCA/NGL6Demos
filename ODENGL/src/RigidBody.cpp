#include "RigidBody.h"

RigidBody::RigidBody(const dWorldID &_world)
{
  m_world=_world;
  m_id=dBodyCreate(m_world);
  enable();
}


RigidBody::RigidBody(const dWorldID &_world, const ngl::Vec3 &_pos)
{
  m_world=_world;
  m_id=dBodyCreate(m_world);
  dBodySetPosition(m_id,_pos.m_x,_pos.m_y,_pos.m_z);
  enable();
}

void RigidBody::setPosition(const ngl::Vec3 &_p)
{
  dBodySetPosition(m_id,_p.m_x,_p.m_y,_p.m_z);
}

void RigidBody::setForce(const ngl::Vec3 &_t)
{
  dBodySetForce(m_id,_t.m_x,_t.m_y,_t.m_z);
}

void RigidBody::setTorque(const ngl::Vec3 &_p)
{
  dBodySetTorque(m_id,_p.m_x,_p.m_y,_p.m_z);
}





void RigidBody::setRotation(const ngl::Mat4 &_r)
{
  // this takes a matrix which is an array of 4*4 floats
  dBodySetRotation(m_id,_r.m_openGL);
}

void RigidBody::setQuaternion(const ngl::Quaternion &_q)
{
  // according to the ODE headers this is in the format
  // quaternion (qs,qx,qy,qz) but stored as an array of 4 floats
  dQuaternion q={_q.getS(),_q.getX(),_q.getY(),_q.getZ()};
  dBodySetQuaternion(m_id,q);
}

void RigidBody::setLinearVelocity(const ngl::Vec3 &_v)
{
  dBodySetLinearVel(m_id,_v.m_x,_v.m_y,_v.m_z);
}
void RigidBody::setLinearVelocity(float _x, float _y, float _z)
{
  dBodySetLinearVel(m_id,_x,_y,_z);
}

void RigidBody::setAngularVelocity(const ngl::Vec3 &_v)
{
  dBodySetAngularVel(m_id,_v.m_x,_v.m_y,_v.m_z);
}

void RigidBody::setMass(const dMass &_m)
{
  dBodySetMass(m_id,&_m);
}

dMass RigidBody::getMass() const
{
  dMass m;
  dBodyGetMass(m_id,&m);
  return m;
}

ngl::Vec3 RigidBody::getPosition()const
{
  // When getting, the returned values are pointers to internal data structures,
  //  so the vectors are valid until any changes are made to the rigid body
  //  system structure.
  const dReal *pos=dBodyGetPosition(m_id);
  return ngl::Vec3(*pos,*(pos+1),*(pos+2));
}

ngl::Vec3 RigidBody::getForce() const
{
  // When getting, the returned values are pointers to internal data structures,
  //  so the vectors are valid until any changes are made to the rigid body
  //  system structure.
  const dReal *pos=dBodyGetForce(m_id);
  return ngl::Vec3(*pos,*(pos+1),*(pos+2));
}

ngl::Vec3 RigidBody::getTorque() const
{
  // When getting, the returned values are pointers to internal data structures,
  //  so the vectors are valid until any changes are made to the rigid body
  //  system structure.
  const dReal *pos=dBodyGetTorque(m_id);
  return ngl::Vec3(*pos,*(pos+1),*(pos+2));
}

ngl::Mat4 RigidBody::getTransformMatrix()const
{
  const dReal *rot=dBodyGetRotation(m_id);
  const dReal *pos=dBodyGetPosition(m_id);
  return ngl::Mat4(
                    rot[0],rot[4],rot[8],0.0f,
                    rot[1],rot[5],rot[9],0.0f,
                    rot[2],rot[6],rot[10],0.0f,
                    pos[0],pos[1],pos[2],1.0f
                  );
}

ngl::Mat4 RigidBody::getRotation()const
{
  // When getting, the returned values are pointers to internal data structures,
  //  so the vectors are valid until any changes are made to the rigid body
  //  system structure.
  const dReal *rot=dBodyGetRotation(m_id);
  ngl::Mat4 m;

  m.m_openGL[0]=  rot[0];
   m.m_openGL[1]=  rot[4];
   m.m_openGL[2]=  rot[8];
   m.m_openGL[3]=  0;
   m.m_openGL[4]=  rot[1];
    m.m_openGL[5]=  rot[5];
    m.m_openGL[6]=  rot[9];
    m.m_openGL[7]=  0;
    m.m_openGL[8]=  rot[2];
    m.m_openGL[9]=  rot[6];
    m.m_openGL[10]= rot[10];
    m.m_openGL[11]= 0;
    m.m_openGL[12]= rot[0];
    m.m_openGL[13]= rot[1];
    m.m_openGL[14]= rot[2];
    m.m_openGL[15]= 1;


  return m;
}
ngl::Quaternion RigidBody::getQuaternion()const
{
  // When getting, the returned values are pointers to internal data structures,
  //  so the vectors are valid until any changes are made to the rigid body
  //  system structure.
  const dReal *pos=dBodyGetQuaternion(m_id);
  return ngl::Quaternion(*pos,*(pos+1),*(pos+2),*(pos+3));
}
ngl::Vec3 RigidBody::getLinearVelocity()const
{
  // When getting, the returned values are pointers to internal data structures,
  //  so the vectors are valid until any changes are made to the rigid body
  //  system structure.
  const dReal *pos=dBodyGetLinearVel(m_id);
  return ngl::Vec3(*pos,*(pos+1),*(pos+2));
}
ngl::Vec3 RigidBody::getAngularVelocity()const
{
  // When getting, the returned values are pointers to internal data structures,
  //  so the vectors are valid until any changes are made to the rigid body
  //  system structure.
  const dReal *pos=dBodyGetAngularVel(m_id);
  return ngl::Vec3(*pos,*(pos+1),*(pos+2));
}


void RigidBody::addForce(const ngl::Vec3 &_f)
{
  dBodyAddForce(m_id,_f.m_x,_f.m_y,_f.m_z);
}
void RigidBody::addTorque(const ngl::Vec3 &_t)
{
  dBodyAddTorque(m_id,_t.m_x,_t.m_y,_t.m_z);
}
void RigidBody::addRelativeForce(const ngl::Vec3 &_f)
{
  dBodyAddRelForce(m_id,_f.m_x,_f.m_y,_f.m_z);
}
void RigidBody::addRelativeTorque(const ngl::Vec3 &_t)
{
  dBodyAddRelTorque(m_id,_t.m_x,_t.m_y,_t.m_z);
}
void RigidBody::addForceAtPosition(const ngl::Vec3 &_f,const ngl::Vec3 &_p)
{
  dBodyAddForceAtPos(m_id,_f.m_x,_f.m_y,_f.m_z,_p.m_x,_p.m_y,_p.m_z);
}
void RigidBody::addForceAtRelativePosition(const ngl::Vec3 &_f,const ngl::Vec3 &_p)
{
  dBodyAddForceAtRelPos(m_id,_f.m_x,_f.m_y,_f.m_z,_p.m_x,_p.m_y,_p.m_z);
}
void RigidBody::addRelativeForceAtPosition(const ngl::Vec3 &_f,const ngl::Vec3 &_p)
{
  dBodyAddRelForceAtPos(m_id,_f.m_x,_f.m_y,_f.m_z,_p.m_x,_p.m_y,_p.m_z);
}
void RigidBody::addRelativeForceAtRelativePosition(const ngl::Vec3 &_f,const ngl::Vec3 &_p)
{
  dBodyAddRelForceAtRelPos(m_id,_f.m_x,_f.m_y,_f.m_z,_p.m_x,_p.m_y,_p.m_z);
}

void RigidBody::addGeom(Geom &_g)
{

  m_geom=_g;
  dGeomSetBody(m_geom.getID(),m_id);
  m_type=m_geom.getType();
}

void RigidBody::setMeshTransfomation()
{
  if(m_type !=TRIANGLEMESH)
  {
    std::cerr<<"trying to set transform on non mesh\n";
    return;
  }

  const dReal *rot=dBodyGetRotation(m_id);
  const dReal *pos=dBodyGetPosition(m_id);
  dReal tx[16];
  tx[0]=rot[0];
  tx[1]=rot[1];
  tx[2]=rot[2];
  tx[3]=pos[0];

  tx[4]=rot[4];
  tx[5]=rot[5];
  tx[6]=rot[6];
  tx[7]=pos[1];

  tx[8]=rot[8];
  tx[9]=rot[9];
  tx[10]=rot[10];
  tx[11]=pos[2];

  tx[12]=rot[12];
  tx[13]=rot[13];
  tx[14]=rot[14];
  tx[15]=1.0;

  dGeomTriMeshSetLastTransform( m_geom.getID() ,tx );
  dGeomTriMeshClearTCCache (m_geom.getID());

}



RigidBody::~RigidBody()
{

}
