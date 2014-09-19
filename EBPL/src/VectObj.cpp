#include "VectObj.h"
using namespace GraphicsLib; 

VectObj::VectObj(Point3 Center,GLfloat Width,GLfloat Height,GLfloat Depth,GLfloat ff,Vector vect)
: EnvObj(Center,Width,Height,Depth,ff)
{
// call the default constructor for the EnvObj then add the Vector
Vect = vect;
}

void VectObj::DrawVect(void)
{
// draw the Vector
glPushMatrix();
	glBegin(GL_LINES);
    	glVertex3f(center.x,center.y,center.z);
		glVertex3f(Vect.x,Vect.y,Vect.z);
    glEnd();
glPopMatrix();

}

