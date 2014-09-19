#ifndef COLLISIONSHAPE_H__
#define COLLISIONSHAPE_H__

/// @brief simple class to store instances of bullet collision
/// shapes this is a singleton so we only have one instance of
/// each shape.
///
#include <ngl/Singleton.h>
#include <btBulletDynamicsCommon.h>
#include <map>
#include <string>
class CollisionShape
{

  typedef struct
  {
    btCollisionShape* shape;
    float dim1;
    float dim2;
    float dim3;
  }ShapeDef;

  public :
    static CollisionShape *instance();
    void addMesh(const std::string & _name, const std::string &_objFilePath);
    void addSphere(const std::string & _name, ngl::Real _rad);
    void addBox(const std::string & _name, ngl::Real _w, ngl::Real _h, ngl::Real _d);
    void addCapsule(const std::string & _name, ngl::Real _rad, ngl::Real _height);
    void addCylinder(const std::string & _name, ngl::Real _rad, ngl::Real _height);
    void addCone(const std::string & _name, ngl::Real _rad, ngl::Real _height);

    btCollisionShape* getShape(const std::string &_name);
    float getDim1(const std::string &_name) const;
    float getDim2(const std::string &_name) const;
    float getDim3(const std::string &_name) const;

  private :
    CollisionShape(){}
    CollisionShape(const CollisionShape &_c){NGL_UNUSED(_c);}
    CollisionShape & operator=(const CollisionShape &_c){NGL_UNUSED(_c);}

    std::map <std::string,ShapeDef> m_shapes;
    static CollisionShape *s_instance;
};



#endif
