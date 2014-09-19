#ifndef MESHDATA_H__
#define MESHDATA_H__
#include <ngl/Singleton.h>
#include <ngl/Obj.h>
#include <vector>
#include <map>

//----------------------------------------------------------------------------------------------------------------------
/// @brief this stores the actual mesh data and index value
/// basically a series of verts and tri index values
//----------------------------------------------------------------------------------------------------------------------
struct collisionMesh
{
  std::vector<unsigned int> m_index;
  std::vector<float> m_verts;
};


//----------------------------------------------------------------------------------------------------------------------
/// @brief container for our mesh data for collisions.
/// this is basically a singleton class to hold all of our mesh
/// indices and verts in one place
//----------------------------------------------------------------------------------------------------------------------

class MeshData : public ngl::Singleton <MeshData>
{
  //----------------------------------------------------------------------------------------------------------------------
  /// @note allow ngl::Singleton to access the data
  //----------------------------------------------------------------------------------------------------------------------
  friend class ngl::Singleton<MeshData>;

public :

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief add a mesh to the class
  /// @param _name the name of the mesh for lookup
  /// @param [in] _m a pointer to the object to store this can be
  /// destroyed after setting as the data is stored here, the best approach is to
  /// have two sets of geo, a low-res collision mesh and a highres one for
  /// drawing (see the demo program)
  //----------------------------------------------------------------------------------------------------------------------
  void addMesh(const std::string &_name, ngl::Obj *_m);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief get the mesh data the Geo class will access this
  /// @param[in] _name the name of the mesh to find
  /// @returns the mesh data or zero if not found
  //----------------------------------------------------------------------------------------------------------------------
  collisionMesh *getMesh(const std::string &_name);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief clear the mesh data.
  //----------------------------------------------------------------------------------------------------------------------
  void clear();
protected :

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor as we inherit from ngl::Singleton we need these to be protected
  //----------------------------------------------------------------------------------------------------------------------
  MeshData(){;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief hide copy ctor as singleton
  //----------------------------------------------------------------------------------------------------------------------
  MeshData(const MeshData &){;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the map to store our structures
  //----------------------------------------------------------------------------------------------------------------------
  std::map<std::string,collisionMesh *> m_collisionData;

};


#endif
