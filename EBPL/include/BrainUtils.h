/*! SphereSpherCollision helper function to do sphere - sphere colliison detection
\param Point3 Pos1 the first agents position
\param GLfloat Radius1 the radius of the first agent
\param Point3 Pos2 the second agents position
\param GLfloat Radius2 the radius of the second agent
 \returns bool true = Hit false = no Hit
*/

extern bool SphereSphereCollision(ngl::Vec3 Pos1, GLfloat Radius1, ngl::Vec3 Pos2, GLfloat Radius2);

/*! helper function to do cylinder - cylinder colliison detection
\param Point3 Pos1 the first agents position
\param GLfloat rad1 the radius of the first agent
\param GLfloat height1 the height of the cylinder
\param Point3 Pos2 the second agents position
\param GLfloat rad2 the radius of the second agent
\param GLfloat height2 the height of the cylinder
 \returns bool true = Hit false = no Hit
*/

extern bool CylinderCylinderCollision(ngl::Vec3 Pos1, float rad1, float height1, ngl::Vec3 Pos2, float rad2, float height2);

