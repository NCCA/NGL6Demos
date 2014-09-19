#ifndef MESH_H__
#define MESH_H__

/// @note this is based on several demos and converted to NGL
/// http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html
/// http://zylinski.se
/// http://assimp.sourceforge.net/lib_html/data.html
/// http://gamedev.stackexchange.com/questions/26382/i-cant-figure-out-how-to-animate-my-loaded-model-with-assimp
#include <map>
#include <ngl/Mat4.h>
#include <ngl/Vec3.h>
#include <ngl/Quaternion.h>
#include <ngl/VertexArrayObject.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/cimport.h>


const static int s_bonesPerVertex=4;


class Mesh
{
public :

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor for Mesh only sets a few default values
  //----------------------------------------------------------------------------------------------------------------------
  Mesh();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  dtor this calls the clear method which will  remove all allocated data
  //----------------------------------------------------------------------------------------------------------------------
  ~Mesh();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief loads the animation / mesh data from a aiScene. It is important that this scene is
  /// static and not destroyed by the client
  /// @param[in] _scene a pre-loaded scene
  //----------------------------------------------------------------------------------------------------------------------

  bool load(const aiScene *_scene);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief draw the mesh at the current time
  //----------------------------------------------------------------------------------------------------------------------
  void render() const;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief accessor for the number of bones in the mesh
  //----------------------------------------------------------------------------------------------------------------------
  inline unsigned int numBones() const { return m_numBones;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief accessor for the time in seconds of the animation
  //----------------------------------------------------------------------------------------------------------------------
  inline double getDuration() const { return m_scene->mAnimations[0]->mDuration;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief how many tick in the animation per second
  //----------------------------------------------------------------------------------------------------------------------
  inline double getTicksPerSec() const { return m_scene->mAnimations[0]->mTicksPerSecond;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this set the bone transformation for the current time. This is then passed to the shader
  /// to do the animation of the mesh
  /// @param[in] _timeInSeconds the time to set the animation frame to
  /// @param[out] _transforms an array of transform matrices for the current frame
  //----------------------------------------------------------------------------------------------------------------------
  void boneTransform(float _timeInSeconds, std::vector<ngl::Mat4>& o_transforms);

private :

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the bone information structure to store the data
  //----------------------------------------------------------------------------------------------------------------------
  struct BoneInfo
  {
    ngl::Mat4 boneOffset;
    ngl::Mat4 finalTransformation;
  };
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Vertex bone data
  //----------------------------------------------------------------------------------------------------------------------
  struct VertexBoneData
  {
      unsigned int ids[s_bonesPerVertex];
      float weights[s_bonesPerVertex];
      void addBoneData(uint BoneID, float Weight);
  };

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Mesh data
  //----------------------------------------------------------------------------------------------------------------------
  struct MeshEntry
  {
      MeshEntry()
      {
          NumIndices    = 0;
          BaseVertex    = 0;
          BaseIndex     = 0;
      }

      unsigned int NumIndices;
      unsigned int BaseVertex;
      unsigned int BaseIndex;
  };

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief calculate the scale value between two keys
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 calcInterpolatedScaling(float _animationTime, const aiNodeAnim* _nodeAnim);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief calculate the rotation value between two keys
  //---------------------------------------------------------------------------------------------------------------------
  ngl::Quaternion calcInterpolatedRotation(float _animationTime, const aiNodeAnim* _nodeAnim);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief calculate the position value between two keys
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 calcInterpolatedPosition(float _animationTime, const aiNodeAnim* _nodeAnim);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief find the current node animation
  //----------------------------------------------------------------------------------------------------------------------
  const aiNodeAnim* findNodeAnim(const aiAnimation* _animation, const std::string &_nodeName);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief recurse the node for the next animation node
  //----------------------------------------------------------------------------------------------------------------------
  void recurseNodeHeirarchy(float _animationTime, const aiNode* _node, const ngl::Mat4& _parentTransform);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  init our data structures from the scene
  //----------------------------------------------------------------------------------------------------------------------
  void initFromScene(const aiScene* _scene);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  create our mesh
  //----------------------------------------------------------------------------------------------------------------------
  void initMesh(
                unsigned int _meshIndex,
                const aiMesh* _aiMesh,
                std::vector<ngl::Vec3>& o_positions,
                std::vector<ngl::Vec3>& o_normals,
                std::vector<ngl::Vec2>& o_texCoords,
                std::vector<VertexBoneData>& o_bones,
                std::vector<unsigned int>& o_indices
                );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  load the bone data
  //----------------------------------------------------------------------------------------------------------------------

  void loadBones(unsigned int _meshIndex, const aiMesh* _mesh, std::vector<VertexBoneData>& o_bones);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief clear all the allocated data
  //----------------------------------------------------------------------------------------------------------------------

  void clear();


  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the mesh entry values
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<MeshEntry> m_entries;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief maps a bone name to its index
  //----------------------------------------------------------------------------------------------------------------------
  std::map<std::string,unsigned int> m_boneMapping;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief number of bones in the mesh
  //----------------------------------------------------------------------------------------------------------------------
  unsigned int m_numBones;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief array of bone information
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<BoneInfo> m_boneInfo;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief global inverse transformation
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_globalInverseTransform;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief pointer to the mesh scene
  //----------------------------------------------------------------------------------------------------------------------
  const aiScene* m_scene;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the vertex array object to store the mesh data
  //----------------------------------------------------------------------------------------------------------------------
  ngl::VertexArrayObject *m_vao;
};


#endif
