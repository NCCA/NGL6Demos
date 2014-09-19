#include "MeshData.h"


//----------------------------------------------------------------------------------------------------------------------
void MeshData::addMesh(const std::string &_name,ngl::Obj *_m)
{
  collisionMesh *m = new collisionMesh;
  // grab the mesh faces
  std::vector<ngl::Face> faces=_m->getFaceList();
  unsigned int size=faces.size();
  // store the index data (we have already ensured they are
  // triangles in the obj load
  for(unsigned int i=0; i<size; ++i)
  {
    // but still only do 3 as it is tri
    for(int f=0; f<3; ++f)
    {
      m->m_index.push_back(faces[i].m_vert[f]);
    }
  }
  // now get the verts and store them in a list
  ngl::Vec3 vv;
  size=_m->getNumVerts();
  for(unsigned int i=0; i<size; ++i)
  {
    // grad the vert at index i and store
    vv=_m->getVertexAtIndex(i);
    m->m_verts.push_back(vv.m_x);
    m->m_verts.push_back(vv.m_y);
    m->m_verts.push_back(vv.m_z);
    // ode needs a float[4] padding so add this
    m->m_verts.push_back(0);
  }

  // finally store our data
  m_collisionData[_name]=m;

}

//----------------------------------------------------------------------------------------------------------------------
collisionMesh * MeshData::getMesh(const std::string &_name)
{
  std::map <std::string, collisionMesh *>::const_iterator mesh=m_collisionData.find(_name);
  // make sure we have a valid set of geometry
  if(mesh!=m_collisionData.end())
  {
    return mesh->second;
  }
  else
  {
    std::cerr<<"Mesh not in mesh collection \n";
    return 0;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void MeshData::clear()
{
  m_collisionData.clear();
}
//----------------------------------------------------------------------------------------------------------------------

