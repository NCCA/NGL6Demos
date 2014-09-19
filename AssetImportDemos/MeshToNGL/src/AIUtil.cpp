#include "AIUtil.h"
#include <assimp/postprocess.h>
#include <ngl/Mat4.h>
#include <ngl/Quaternion.h>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <ngl/Vec2.h>

namespace AIU
{

  ngl::Mat4 aiMatrix4x4ToNGLMat4(const aiMatrix4x4 &_m)
  {
    return ngl::Mat4(
                     _m.a1,_m.a2,_m.a3,_m.a4,
                     _m.b1,_m.b2,_m.b3,_m.b4,
                     _m.c1,_m.c2,_m.c3,_m.c4,
                     _m.d1,_m.d2,_m.d3,_m.d4
                     );
  }


  ngl::Mat4 aiMatrix4x4ToNGLMat4Transpose(const aiMatrix4x4 &_m)
  {
    return ngl::Mat4(
                     _m.a1,_m.b1,_m.c1,_m.d1,
                     _m.a2,_m.b2,_m.c2,_m.d2,
                     _m.a3,_m.b3,_m.c3,_m.d3,
                     _m.a4,_m.b4,_m.c4,_m.d4
                     );
  }


  ngl::Vec3 aiVector3DToNGLVec3(const aiVector3D &_v)
  {
    return ngl::Vec3(_v.x,_v.y,_v.z);
  }

  ngl::Vec4 aiVector3DToNGLVec4(const aiVector3D &_v)
  {
    return ngl::Vec4(_v.x,_v.y,_v.z,1.0);
  }


  ngl::Quaternion aiQuatToNGLQuat(const aiQuaternion &_v)
  {
    return ngl::Quaternion(_v.w,_v.x,_v.y,_v.z);
  }

  ngl::Vec2 aiVector2DToNGLVec2(const aiVector2D &_v)
  {
    return ngl::Vec2(_v.x,_v.y);
  }


  void getBBoxForNode (
                        const aiScene *scene,
                        const  aiNode* nd,
                        aiVector3D *min,
                        aiVector3D *max,
                        aiMatrix4x4 *trafo
                      )
  {
    aiMatrix4x4 prev;
    unsigned int n = 0, t;

    prev = *trafo;
    aiMultiplyMatrix4(trafo,&nd->mTransformation);

    for (; n < nd->mNumMeshes; ++n)
    {
      const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
      for (t = 0; t < mesh->mNumVertices; ++t)
      {

        aiVector3D tmp = mesh->mVertices[t];
        aiTransformVecByMatrix4(&tmp,trafo);

        min->x = std::min(min->x,tmp.x);
        min->y = std::min(min->y,tmp.y);
        min->z = std::min(min->z,tmp.z);

        max->x = std::max(max->x,tmp.x);
        max->y = std::max(max->y,tmp.y);
        max->z = std::max(max->z,tmp.z);
      }
    }

    for (n = 0; n < nd->mNumChildren; ++n)
    {
      getBBoxForNode(scene,nd->mChildren[n],min,max,trafo);
    }
    *trafo = prev;
  }

  void getSceneBoundingBox( const aiScene * scene,ngl::Vec3 &o_min, ngl::Vec3 &o_max)
  {
    aiMatrix4x4 trafo;
    aiIdentityMatrix4(&trafo);
    aiVector3D min,max;

    min.x = min.y = min.z =  1e10f;
    max.x = max.y = max.z = -1e10f;
    getBBoxForNode(scene,scene->mRootNode,&min,&max,&trafo);
    o_min=aiVector3DToNGLVec3(min);
    o_max=aiVector3DToNGLVec3(max);

  }

} // end namespace
