#include "CollisionShape.h"
#include <ngl/Obj.h>
// set the instance to 0
CollisionShape *CollisionShape::s_instance=0;

CollisionShape *CollisionShape::instance()
{
  // not going to thread this so only need single check
  // should use double lock if threading
  if(s_instance ==0 )
  {
    s_instance = new CollisionShape;
  }
  return s_instance;
}

void CollisionShape::addMesh(const std::string & _name, const std::string &_objFilePath)
{
  ngl::Obj mesh(_objFilePath);
  //create a dynamic rigidbody

	btConvexHullShape*  shape =  new btConvexHullShape();
	unsigned int n=mesh.getNumVerts();
	std::vector <ngl::Vec3> points=mesh.getVertexList();
	for(unsigned int i=0; i<n; ++i)
	{
		shape->addPoint(btVector3(points[i].m_x,points[i].m_y,points[i].m_z));
	}
	m_shapes[_name]=shape;
}

void CollisionShape::addSphere(const std::string & _name, ngl::Real _rad)
{
	btCollisionShape* shape = new btSphereShape(btScalar(_rad/2.0));
	m_shapes[_name]=shape;

}

void CollisionShape::addBox(const std::string & _name, ngl::Real _w, ngl::Real _h, ngl::Real _d)
{
	btCollisionShape* shape = new btBoxShape(btVector3(_w/2.0,_h/2.0,_d/2.0));
	m_shapes[_name]=shape;

}

void CollisionShape::addCapsule(const std::string & _name, ngl::Real _rad, ngl::Real _height)
{
	btCollisionShape* shape = new  btCapsuleShape(_rad,_height);//  btBoxShape(btVector3(_rad/2.0,_rad/2.0,_rad/2.0));
	m_shapes[_name]=shape;

}

void CollisionShape::addCylinder(const std::string & _name, ngl::Real _rad, ngl::Real _height)
{
	btCollisionShape* shape = new  btCylinderShapeZ(btVector3(_rad,_height/2.0,_rad));//  btBoxShape(btVector3(_rad/2.0,_rad/2.0,_rad/2.0));
	m_shapes[_name]=shape;

}

void CollisionShape::addCone(const std::string & _name, ngl::Real _rad, ngl::Real _height)
{
	btCollisionShape* shape = new  btConeShapeZ(_rad,_height);//  btBoxShape(btVector3(_rad/2.0,_rad/2.0,_rad/2.0));
	m_shapes[_name]=shape;

}

btCollisionShape* CollisionShape::getShape(const std::string &_name)
{
	btCollisionShape *shape=0;
	std::map <std::string, btCollisionShape * >::const_iterator shapeIt=m_shapes.find(_name);
	// make sure we have a valid shader
	if(shapeIt!=m_shapes.end())
	{
		shape=shapeIt->second;
	}
	return shape;
}

