/*! EnvObj.h
	Author Jonathan Macey
    Version 3.0
    Date Last Revision 18/02/03
    Linkages : BBox.h Util.h Point3.h Vector.h
	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one
    Description :
    Extended Bounding Box class used for Environment objects so they can have a force feild atached
    to them for deflecting objects
*/


#include "EnvObj.h"

using namespace std;
using namespace GraphicsLib; 


static const int Faces[6][4]=
{
	{0,3,7,4},{0,1,5,4},{1,2,6,5},{2,3,7,6},{0,1,2,3},{4,5,6,7}
};
// constructor inherited from the Base class
EnvObj::EnvObj(Point3 Center,GLfloat Width,GLfloat Height,GLfloat Depth,GLfloat ff) :
BBox(Center,Width,Height,Depth)
{
//set the force field radius
ForceFeildR = ff;
// calculate the environment extents
EnvExt[0]=EnvExt[1]=(Height/2.0f);
EnvExt[2]=EnvExt[3]=(Width/2.0f);
EnvExt[4]=EnvExt[5]=(Depth/2.0f);
//set the drawflag to false
drawExtents=false;
CreateDisplayList();
}


void EnvObj::DrawFF(void)
{
//call the base flag draw method
Draw();
for(int i=0; i<6; i++)
    {
    #ifdef __DEBUG_
    glBegin(DrawMode[i]);
    #else
	glBegin(GL_LINE_LOOP);
    #endif
// draw the force feild normals
        glColor3f(0.0f,0.0f,0.0f);
        glPushMatrix();
        center.Translate();
        center.Vertex();
        glColor3f(1.0f,1.0f,1.0f);
        Point3 s((center.x+Norm[i].x),(center.y+Norm[i].y),(center.z+Norm[i].z));
        s.Vertex();
        glPopMatrix();
    glEnd();
    }
// draw the force feild sphere
if(drawExtents == true)
{
glPushMatrix();
 center.Translate();
 glColor3f(0.4f,0.4f,0.4f);
 glutWireSphere(ForceFeildR,12,12);
glPopMatrix();
}
}

void EnvObj::Rotate(GLfloat Angle, bool x, bool y, bool z)
{
//rotate the object based on the angle passed in
//first need to convert degrees to radians for sin and cos
GLfloat theta=DegtoRad(Angle);
// as were going to use cos and sin theta we calculate it once and then re use it
// this is a lot quicker
GLfloat c=cos(theta);
GLfloat s=sin(theta);
//now for each of the face points rotate around the active origins
for(int i=0; i<8; i++)
	{
	//Translate to Origin
    Vert[i].x-=center.x;
    Vert[i].y-=center.y;
    Vert[i].z-=center.z;
    //do rotations
    if(x==true)
    	{
        Vert[i].y=Vert[i].y*c+Vert[i].z*-s;
		Vert[i].z=Vert[i].y*s+Vert[i].z*c;
        }
	if(y==true)
    	{
        Vert[i].x=Vert[i].x*c+Vert[i].z*s;
		Vert[i].z=Vert[i].x*-s+Vert[i].z*c;
        }
	if(z==true)
    	{
        Vert[i].x=Vert[i].x*c+Vert[i].y*-s;
		Vert[i].y=Vert[i].x*s+Vert[i].y*c;
        }
    //translate back to original center
    Vert[i].x+=center.x;
    Vert[i].y+=center.y;
    Vert[i].z+=center.z;

    }//end for
// now recalculate the new normals
for(int f=0; f<6; f++)
	Norm[f]=CalcNormal(Vert[Faces[f][0]],Vert[Faces[f][1]],Vert[Faces[f][2]]);
}


void EnvObj::Draw(void)
{
//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glCallList(ListIndex);


}


void EnvObj::CreateDisplayList(void)
{
// get a free display list
GLuint range=glGenLists(1);
ListIndex=glGenLists(range);
// create a new list in compile mode so we only create it and not draw it
glNewList(ListIndex,GL_COMPILE);
 // just use the standard draw method as this calls the correct commands
 
glPushMatrix();
//glPointSize(2.0);
glColor3f(0.0,0.0,0.0);
for(int i=0; i<6; i++)
	{
	glBegin(GL_LINE_LOOP); //LINE_LOOP);
	//now loop throught for each of the verts and draw
    for(int x=0; x<4; x++)
    	{
        Vert[Faces[i][x]].Vertex(); // Vertex is a method of Point class to call glVertex

        }
	glEnd();
	}


glEnable(GL_BLEND);
//loop through all the faces
glColor4f(0.8f,0.8f,0.8f,0.2);
for(int i=0; i<6; i++)
	{
	glBegin(GL_QUADS); //LINE_LOOP);
	//now loop throught for each of the verts and draw
    for(int x=0; x<4; x++)
    	{
        Vert[Faces[i][x]].Vertex(); // Vertex is a method of Point class to call glVertex

        }
	glEnd();
	}
glDisable(GL_BLEND);
//loop through all the faces
glColor3f(0,0,0);
glPopMatrix();
glEndList();
// end the list
}


//end of file
