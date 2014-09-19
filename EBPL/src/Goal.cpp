#include "Goal.h"

using namespace GraphicsLib; 

Goal::Goal(GLfloat x, GLfloat y, GLfloat z, GLfloat t)
{
// set the goal parameters
GPos.x=x;
GPos.y=y;
GPos.z=z;
Gtime= t;
m= new Material(GOLD);

}

void Goal::DrawGoal(void)
{
glEnable(GL_LIGHTING);
m->Use(1.0);

// store the current transformation matrix
glPushMatrix();
	//set the goal colour
    glColor3f(1.0f,0.0f,0.0f);
    //move to the right position
    GPos.Translate();
	//draw the goal
    glutSolidSphere(0.5f,8,8);
//restore the matrix
glPopMatrix();
glDisable(GL_LIGHTING);
}


//end of class

