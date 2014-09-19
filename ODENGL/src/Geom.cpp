#include "Geom.h"
#include "MeshData.h"

Geom::Geom(dSpaceID _space)
{
  // set the space
  m_space=_space;
}

Geom::~Geom()
{
  // nothing to do here
}

void Geom::createSphere(ngl::Real _r)
{
  // create a sphere and set the type
  m_id=dCreateSphere(m_space,_r);
  m_type=SPHERE;
}

void Geom::createBox(ngl::Vec3 _b)
{
  m_id=dCreateBox(m_space,_b.m_x,_b.m_y,_b.m_z);
  m_type=BOX;
}

void Geom::setOffsetPosition(float _x, float _y, float _z)
{
  dGeomSetOffsetPosition(m_id,_x,_y,_z);
}

void Geom::createBox(ngl::Real _lx, ngl::Real _ly, ngl::Real _lz)
{
  m_id=dCreateBox(m_space,_lx,_ly,_lz);
  m_type=BOX;
}

void Geom::createPlane(ngl::Vec4 _p)
{
  m_id=dCreatePlane(m_space,_p.m_x,_p.m_y,_p.m_z,_p.m_w);
  m_type=PLANE;
}

void Geom::createPlane(ngl::Real _a, ngl::Real _b, ngl::Real _c,ngl::Real _d)
{
  m_id=dCreatePlane(m_space,_a,_b,_c,_d);
  m_type=PLANE;

}

void Geom::createCylinder(ngl::Real _r, ngl::Real _l)
{
  m_id=dCreateCCylinder(m_space,_r,_l);
  m_type=CYLINDER;
}

void Geom::createRay(ngl::Real _l)
{
  m_id=dCreateRay(m_space,_l);
}

void Geom::createMesh(const std::string &_name)
{
  // here we create and set the mesh data
  // first create some mesh geo
  m_meshID=dGeomTriMeshDataCreate();
  m_type=TRIANGLEMESH;
  // now grab the MeshData class which holds our meshes
  // and see if we have attached the geo
  MeshData *mesh = MeshData::instance();
  collisionMesh *m=mesh->getMesh(_name);
  if(m !=0)
  {
    // if it exists set the data for this version of the geo
    // this is basically storing pointers to our mesh
    dGeomTriMeshDataBuildSimple(m_meshID,
                                &m->m_verts[0] ,m->m_verts.size()-1,
                                &m->m_index[0],m->m_index.size()-1
                               );
    // now we create the tri with this mesh, we could pass in handling
    // routines here but for now set to 0
    m_id=dCreateTriMesh(m_space,m_meshID,0,0,0);
  }
}


