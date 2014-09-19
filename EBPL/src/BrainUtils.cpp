
#ifdef LINUX
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif
#ifdef DARWIN
	#include <unistd.h>
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#endif

#include <ngl/Vec3.h>
  
  
  using namespace std;
using namespace ngl;  
  bool SphereSphereCollision(Vec3 Pos1, GLfloat Radius1, Vec3 Pos2, GLfloat Radius2)
  {
  // the relative position of the agent
 Vec3 relPos;
 //min an max distances of the agent
 GLfloat dist;
 float minDist;
 relPos =Pos1-Pos2;
 // and the distance
 dist=relPos.m_x * relPos.m_x + relPos.m_y * relPos.m_y + relPos.m_z * relPos.m_z;
 minDist =Radius1 +Radius2;
 // if it is a hit
 if(dist <=(minDist * minDist))
   return true;
 else 
   return false;
}

bool CylinderCylinderCollision(Vec3 Pos1, float rad1, float height1, Vec3 Pos2, float rad2, float height2)
{
        float dist, len_x, len_y, real_rad=rad1+rad2;
        height1 += Pos1.m_z;
        height2 += Pos2.m_z;
       if ((height1 - height2) < 0 || (height2 - height1) < 0) 
        	{
                len_x = Pos1.m_x - Pos2.m_x;
                len_y = Pos1.m_y - Pos2.m_y;

                if (rad1 <= rad2) 
                		real_rad = rad2 - rad1;
                if (rad2 < rad1) 
                		real_rad = rad1 - rad2;
	            dist = ((len_x * len_x) + (len_y * len_y));
                if (dist <= rad1+rad2) //((real_rad) * (real_rad))) 
                    return true;
        		else
                	return false;
            }
        
        return false;
}



