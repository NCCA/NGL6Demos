#ifndef AIUTIL_H__
#define AIUTIL_H__
#include <assimp/scene.h>
#include <assimp/cimport.h>
/// @brief some useful conversion routines for assimp to ngl
// to make things cleaner pre declare the classes
namespace ngl
{
  class Mat4;
  class Mat4;
  class Vec2;
  class Vec3;
  class Vec4;
  class Quaternion;
}


namespace AIU
{
  extern ngl::Mat4 aiMatrix4x4ToNGLMat4(const aiMatrix4x4 &_m);
  extern ngl::Mat4 aiMatrix4x4ToNGLMat4Transpose(const aiMatrix4x4 &_m);
  extern ngl::Vec3 aiVector3DToNGLVec3(const aiVector3D &_v);
  extern ngl::Vec4 aiVector3DToNGLVec4(const aiVector3D &_v);
  extern ngl::Vec2 aiVector2DToNGLVec2(const aiVector2D &_v);
  extern ngl::Quaternion aiQuatToNGLQuat(const aiQuaternion &_v);
  extern void getSceneBoundingBox(const aiScene * scene,ngl::Vec3 &o_min, ngl::Vec3 &o_max);
}


#endif
