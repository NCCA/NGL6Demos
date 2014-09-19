#ifndef TRIANGLE_H__
#define TRIANGLE_H__

/// @brief  simple class to hold a triangle
#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/ShaderLib.h>
#include <ngl/TransformStack.h>
#include <ngl/VertexArrayObject.h>
class Triangle
{

public :
	// ctor
	Triangle(  ngl::Vec3 _p0, ngl::Vec3 _p1, ngl::Vec3 _p2 );
	~Triangle();
	// method to draw the tri
	void draw(const std::string &_shaderName, const ngl::Mat4 &_globalMat,ngl::Camera *_cam );
	// method to see if ray has intercepted with triangle.
	void rayTriangleIntersect(ngl::Vec3 _rayStart, ngl::Vec3 _rayEnd);
	void loadMatricesToShader(ngl::Transformation &_tx, const ngl::Mat4 &_globalMat,ngl::Camera *_cam ) const;

private :
	// The triangles verticies
  ngl::Vec3 m_v0;
  ngl::Vec3 m_v1;
  ngl::Vec3 m_v2;
	// two edges for the triangle / ray calculation
  ngl::Vec3 m_edge1;
  ngl::Vec3 m_edge2;
	// the center of the triangle
  ngl::Vec3 m_center;
	// @brief the vertex normals of the triangle
  std::vector<ngl::Vec3> m_normals;
	// @brief the vertex points of the triangle
  std::vector<ngl::Vec3> m_points;

	// flag to indicate if tri has been intersected with ray
  bool m_hit;
	// u,v,w values for the calculation of hit point
  ngl::Real m_u;
  ngl::Real m_v;
  ngl::Real m_w;
	// the actual hit point of the tri
  ngl::Vec3 m_hitPoint;
  /// @brief our vertex array object
  ngl::VertexArrayObject *m_vao;

};

#endif
