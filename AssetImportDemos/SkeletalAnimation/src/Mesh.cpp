#include "Mesh.h"
#include "AIUtil.h"
#include <ngl/Util.h>
#include <cassert>
#include <iostream>
/// @note this is based on several demos and converted to NGL
/// http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html
/// http://zylinski.se
/// http://assimp.sourceforge.net/lib_html/data.html
/// http://gamedev.stackexchange.com/questions/26382/i-cant-figure-out-how-to-animate-my-loaded-model-with-assimp



void Mesh::VertexBoneData::addBoneData(unsigned int _boneID, float _weight)
{
  // copy the data (weights) for this bone into our structure
  for (unsigned int i = 0 ; i < sizeof(ids)/sizeof(ids[0]) ; ++i)
  {
    if (weights[i] == 0.0)
    {
      ids[i]     = _boneID;
      weights[i] = _weight;
      return;
    }
  }
}



Mesh::Mesh()
{
  // ctor just does basic setup
  m_numBones = 0;
  m_scene = NULL;
}

Mesh::~Mesh()
{
  // clear the VAO the rest of the containers are ok to auto destruct
  clear();
}

bool Mesh::load(const aiScene *_scene)
{
  bool success=false;
  m_scene=_scene;
  // we have already forced the load to be trinagles so no need to check
  m_vao = ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
  // if we have a valid scene load and init
  if (m_scene)
  {
    // grab the inverse global transform
    m_globalInverseTransform = AIU::aiMatrix4x4ToNGLMat4(m_scene->mRootNode->mTransformation);
    m_globalInverseTransform.inverse();
    // now load the bones etc
    initFromScene(m_scene);
    success=true;
  }
  else
  {
    std::cerr<<"Error parsing "<<_scene<<"\n";
  }
  return success;
}

void Mesh::render() const
{
  m_vao->bind();
  unsigned int size=m_entries.size();
  for (unsigned int i = 0 ; i < size; ++i)
  {
  // whist we have the data stored in our VAO structure we only need to bind to re-activate the
  // attribute data, then we draw using ElemetsBaseVertex. This is similar to how the
  // sponza demo works, really if you were dealing with different model textures etc this would
  // also be switched here for each of the different mesh entries.
  glDrawElementsBaseVertex(GL_TRIANGLES,
                           m_entries[i].NumIndices,
                           GL_UNSIGNED_INT,
                           (void*)(sizeof(unsigned int) * m_entries[i].BaseIndex),
                           m_entries[i].BaseVertex
                           );

  // seems that BaseVertex isn't under linux (not sure why) this works as well but doesn't
  // cope with embedded meshes so well
//  glDrawElements(GL_TRIANGLES,
//                           m_entries[i].NumIndices,
//                           GL_UNSIGNED_INT,
//                           (void*)(sizeof(unsigned int) * m_entries[i].BaseIndex)
//                           );

  }
  m_vao->unbind();
}


void Mesh::boneTransform(float _timeInSeconds, std::vector<ngl::Mat4>& o_transforms)
{
  ngl::Mat4 identity(1.0);
  // calculate the current animation time at present this is set to only one animation in the scene and
  // hard coded to animaiton 0 but if we have more we would set it to the proper animation data
  float ticksPerSecond = m_scene->mAnimations[0]->mTicksPerSecond != 0 ? m_scene->mAnimations[0]->mTicksPerSecond : 25.0f;
  float timeInTicks = _timeInSeconds * ticksPerSecond;
  float animationTime = fmod(timeInTicks, m_scene->mAnimations[0]->mDuration);
  // now traverse the animaiton heirarchy and get the transforms for the bones
  recurseNodeHeirarchy(animationTime, m_scene->mRootNode, identity);
  o_transforms.resize(m_numBones);

  for (unsigned int i = 0 ; i < m_numBones ; ++i)
  {
    // now copy this data note that we need to transpose for NGL useage
    // this data will be copied to the shader and used in the animation skinning
    // process
    o_transforms[i] = m_boneInfo[i].finalTransformation.transpose();
  }
}


ngl::Vec3 Mesh::calcInterpolatedScaling(float _animationTime, const aiNodeAnim* _nodeAnim)
{
  // this grabs the scale from this frame and the next and returns the interpolated version
  if (_nodeAnim->mNumScalingKeys == 1)
  {
    return AIU::aiVector3DToNGLVec3(_nodeAnim->mScalingKeys[0].mValue);
  }

  unsigned int scalingIndex=0;
  for (unsigned int i = 0 ; i < _nodeAnim->mNumScalingKeys - 1 ; ++i)
  {
    if (_animationTime < (float)_nodeAnim->mScalingKeys[i + 1].mTime)
    {
      // once we find the data break out of the loop
      scalingIndex=i;
      break;
    }
  }
  unsigned int nextScalingIndex = (scalingIndex + 1);
  assert(nextScalingIndex < _nodeAnim->mNumScalingKeys);
  float deltaTime = _nodeAnim->mScalingKeys[nextScalingIndex].mTime - _nodeAnim->mScalingKeys[scalingIndex].mTime;
  float factor = (_animationTime - (float)_nodeAnim->mScalingKeys[scalingIndex].mTime) / deltaTime;
  //assert(factor >= 0.0f && factor <= 1.0f);
  ngl::Vec3 start = AIU::aiVector3DToNGLVec3(_nodeAnim->mScalingKeys[scalingIndex].mValue);
  ngl::Vec3 end   = AIU::aiVector3DToNGLVec3(_nodeAnim->mScalingKeys[nextScalingIndex].mValue);
  ngl::Vec3 delta = end - start;
  return (start + factor * delta);
}

ngl::Quaternion Mesh::calcInterpolatedRotation( float _animationTime, const aiNodeAnim* _nodeAnim)
{
  // we need at least two values to interpolate...
  if (_nodeAnim->mNumRotationKeys == 1)
  {
     return AIU::aiQuatToNGLQuat(_nodeAnim->mRotationKeys[0].mValue);
  }

  unsigned int rotationIndex =0;

  for (unsigned int i = 0 ; i < _nodeAnim->mNumRotationKeys - 1 ; ++i)
  {
    if (_animationTime < (float)_nodeAnim->mRotationKeys[i + 1].mTime)
    {
      // search for index and break out of loop
      rotationIndex=i;
      break;
    }
  }

  unsigned int nextRotationIndex = (rotationIndex + 1);
  float deltaTime = _nodeAnim->mRotationKeys[nextRotationIndex].mTime - _nodeAnim->mRotationKeys[rotationIndex].mTime;
  float factor = (_animationTime - (float)_nodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;
  ngl::Quaternion startRotation = AIU::aiQuatToNGLQuat(_nodeAnim->mRotationKeys[rotationIndex].mValue);
  ngl::Quaternion endRotation   = AIU::aiQuatToNGLQuat(_nodeAnim->mRotationKeys[nextRotationIndex].mValue);
  ngl::Quaternion out=ngl::Quaternion::slerp(startRotation,endRotation,factor);
  out.normalise();
  return out;
}

ngl::Vec3 Mesh::calcInterpolatedPosition(float _animationTime, const aiNodeAnim* _nodeAnim)
{
  if (_nodeAnim->mNumPositionKeys == 1)
  {
    return AIU::aiVector3DToNGLVec3(_nodeAnim->mPositionKeys[0].mValue);
  }

  unsigned int positionIndex =0;
  for (unsigned int i = 0 ; i < _nodeAnim->mNumPositionKeys - 1 ; ++i)
  {
    if (_animationTime < (float)_nodeAnim->mPositionKeys[i + 1].mTime)
    {
      // if we find the key assign and exit search
      positionIndex=i;
      break;
    }
  }
  unsigned int nextPositionIndex = (positionIndex + 1);
  assert(nextPositionIndex < _nodeAnim->mNumPositionKeys);
  float deltaTime = _nodeAnim->mPositionKeys[nextPositionIndex].mTime - _nodeAnim->mPositionKeys[positionIndex].mTime;
  float factor = (_animationTime - (float)_nodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;
  ngl::Vec3 start = AIU::aiVector3DToNGLVec3(_nodeAnim->mPositionKeys[positionIndex].mValue);
  ngl::Vec3 end = AIU::aiVector3DToNGLVec3(_nodeAnim->mPositionKeys[nextPositionIndex].mValue);

  return ngl::lerp(start,end,factor);
}




const aiNodeAnim* Mesh::findNodeAnim(const aiAnimation* _animation, const std::string &_nodeName)
{
  for (unsigned int i = 0 ; i < _animation->mNumChannels ; ++i)
  {
    const aiNodeAnim* nodeAnim = _animation->mChannels[i];

    if (std::string(nodeAnim->mNodeName.data) == _nodeName)
    {
      return nodeAnim;
    }
  }

  return NULL;
}

void Mesh::recurseNodeHeirarchy(float _animationTime, const aiNode* _node, const ngl::Mat4& _parentTransform)
{
  std::string name(_node->mName.data);

  const aiAnimation* animation = m_scene->mAnimations[0];

  ngl::Mat4 nodeTransform=AIU::aiMatrix4x4ToNGLMat4(_node->mTransformation);

  const aiNodeAnim* nodeAnim = findNodeAnim(animation, name);

  if (nodeAnim)
  {
    // Interpolate scaling and generate scaling transformation matrix
    ngl::Vec3 scale=calcInterpolatedScaling(_animationTime, nodeAnim);
    ngl::Mat4 scaleMatrix;
    scaleMatrix.scale(scale.m_x, scale.m_y, scale.m_z);
    // Interpolate rotation and generate rotation transformation matrix
    ngl::Quaternion rotation=calcInterpolatedRotation( _animationTime, nodeAnim);
    ngl::Mat4 rotationMatrix = rotation.toMat4();

    // Interpolate translation and generate translation transformation matrix
    ngl::Vec3 translation=calcInterpolatedPosition(_animationTime, nodeAnim);
    // Combine the above transformations
    nodeTransform = rotationMatrix*scaleMatrix;
    nodeTransform.m_30=translation.m_x;
    nodeTransform.m_31=translation.m_y;
    nodeTransform.m_32=translation.m_z;
    nodeTransform.transpose();

  }


  ngl::Mat4 globalTransform = _parentTransform * nodeTransform;

  if (m_boneMapping.find(name) != m_boneMapping.end())
  {
    unsigned int boneIndex = m_boneMapping[name];
    m_boneInfo[boneIndex].finalTransformation = m_globalInverseTransform * globalTransform * m_boneInfo[boneIndex].boneOffset;
  }

  for (unsigned int i = 0 ; i < _node->mNumChildren ; ++i)
  {
    recurseNodeHeirarchy(_animationTime, _node->mChildren[i], globalTransform);
  }
}

void Mesh::initFromScene(const aiScene* _scene)
{
  std::cout<<"init from scene\n";
  m_entries.resize(_scene->mNumMeshes);

  std::vector<ngl::Vec3> positions;
  std::vector<ngl::Vec3> normals;
  std::vector<ngl::Vec2> texCords;
  std::vector<VertexBoneData> bones;
  std::vector<GLuint > indices;

  unsigned int NumVertices = 0;
  unsigned int NumIndices = 0;

  // Count the number of vertices and indices
  unsigned int size=m_entries.size();
  for (unsigned int i = 0 ; i < size; ++i)
  {
    m_entries[i].NumIndices    = _scene->mMeshes[i]->mNumFaces * 3;
    m_entries[i].BaseVertex    = NumVertices;
    m_entries[i].BaseIndex     = NumIndices;

    NumVertices += _scene->mMeshes[i]->mNumVertices;
    NumIndices  += m_entries[i].NumIndices;
  }

  // Reserve space in the vectors for the vertex attributes and indices
  positions.reserve(NumVertices);
  normals.reserve(NumVertices);
  texCords.reserve(NumVertices);
  bones.resize(NumVertices);
  indices.reserve(NumIndices);

  // Initialize the meshes in the scene one by one
  for (unsigned int i = 0 ; i < size ; ++i)
  {
    const aiMesh* paiMesh = _scene->mMeshes[i];
    initMesh(i, paiMesh, positions, normals, texCords, bones, indices);
  }

  m_vao->bind();
  m_vao->setIndexedData(positions.size()*sizeof(ngl::Vec3),positions[0].m_x,indices.size(),&indices[0],GL_UNSIGNED_INT,GL_STATIC_DRAW);
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  m_vao->setIndexedData(texCords.size()*sizeof(ngl::Vec2),texCords[0].m_x,indices.size(),&indices[0],GL_UNSIGNED_INT,GL_STATIC_DRAW);
  m_vao->setVertexAttributePointer(1,2,GL_FLOAT,0,0);

  m_vao->setIndexedData(normals.size()*sizeof(ngl::Vec3),normals[0].m_x,indices.size(),&indices[0],GL_UNSIGNED_INT,GL_STATIC_DRAW);
  m_vao->setVertexAttributePointer(2,3,GL_FLOAT,0,0);

  m_vao->setRawIndexedData(sizeof(VertexBoneData) * bones.size(),&bones[0],indices.size(),&indices[0],GL_UNSIGNED_INT,GL_STATIC_DRAW);
  m_vao->setVertexAttributeIPointer(3,4,GL_INT,sizeof(VertexBoneData),0);
  m_vao->setVertexAttributePointer(4,4,GL_FLOAT,sizeof(VertexBoneData),4);
  m_vao->setNumIndices(indices.size());
  m_vao->unbind();
}

void Mesh::initMesh(
                    unsigned int _meshIndex,
                    const aiMesh* _aiMesh,
                    std::vector<ngl::Vec3>& o_positions,
                    std::vector<ngl::Vec3>& o_normals,
                    std::vector<ngl::Vec2>& o_texCoords,
                    std::vector<VertexBoneData>& o_bones,
                    std::vector<unsigned int>& o_indices
                    )
{
  ngl::Vec3 Zero3D(0.0f, 0.0f, 0.0f);

  // Populate the vertex attribute vectors
  for (unsigned int i = 0 ; i < _aiMesh->mNumVertices ; ++i)
  {
  ngl::Vec3 pos  = AIU::aiVector3DToNGLVec3(_aiMesh->mVertices[i]);
  ngl::Vec3 normal = AIU::aiVector3DToNGLVec3(_aiMesh->mNormals[i]);
  ngl::Vec3 tex = _aiMesh->HasTextureCoords(0) ? AIU::aiVector3DToNGLVec3(_aiMesh->mTextureCoords[0][i]) : Zero3D;

  o_positions.push_back(pos);
  o_normals.push_back(normal);
  o_texCoords.push_back(ngl::Vec2(tex.m_x,tex.m_y) );
  }

  loadBones(_meshIndex, _aiMesh, o_bones);

  // Populate the index buffer
  for (unsigned int i = 0 ; i < _aiMesh->mNumFaces ; ++i)
  {
    const aiFace& Face = _aiMesh->mFaces[i];
    o_indices.push_back(Face.mIndices[0]);
    o_indices.push_back(Face.mIndices[1]);
    o_indices.push_back(Face.mIndices[2]);
  }

}

void Mesh::loadBones(unsigned int _meshIndex, const aiMesh* _mesh, std::vector<VertexBoneData>& o_bones)
{
  for (unsigned int i = 0 ; i < _mesh->mNumBones ; ++i)
  {
    unsigned int BoneIndex = 0;
    std::string boneName(_mesh->mBones[i]->mName.data);

    if (m_boneMapping.find(boneName) == m_boneMapping.end())
    {
      // Allocate an index for a new bone
      BoneIndex = m_numBones;
      m_numBones++;
      BoneInfo bi;
      m_boneInfo.push_back(bi);
      // this is the Matrix that transforms from mesh space to bone space in bind pose.
      m_boneInfo[BoneIndex].boneOffset = AIU::aiMatrix4x4ToNGLMat4(_mesh->mBones[i]->mOffsetMatrix);
      m_boneMapping[boneName] =BoneIndex;
    }
    else
    {
      BoneIndex = m_boneMapping[boneName];
    }

    for (unsigned int j = 0 ; j < _mesh->mBones[i]->mNumWeights ; ++j)
    {
      unsigned int VertexID = m_entries[_meshIndex].BaseVertex + _mesh->mBones[i]->mWeights[j].mVertexId;
      float Weight  = _mesh->mBones[i]->mWeights[j].mWeight;
      o_bones[VertexID].addBoneData(BoneIndex, Weight);
    }
  }
}


void Mesh::clear()
{
  m_vao->removeVOA();

}
