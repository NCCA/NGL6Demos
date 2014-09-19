#include "GroundPlane.h"
#include <iostream>
using namespace GraphicsLib;
using namespace std;

GroundPlane::GroundPlane(BBox *ext,GLfloat y,GLfloat r,GLfloat g, GLfloat b, GLfloat a)
{
// calculate the extents of the gp
MinX=ext->center.x-ext->width/2;
MaxX=ext->center.x+ext->width/2;

MinZ=ext->center.z-ext->depth/2;
MaxZ=ext->center.z+ext->depth/2;
MinY=MaxY = y;
GLuint range=glGenLists(1);
gpRed=r; gpGreen=g; gpBlue=b; gpAlpha=a;
ListIndex=glGenLists(range);

CreateDisplayList();
}


GroundPlane::~GroundPlane()
{
// remove the lists
/*if(UsingImage == true)
	delete [] gpPoints;*/
}


GLfloat  GroundPlane::GetHeight(GLfloat x, GLfloat z)
{ 
return MinY;
}


 


void GroundPlane::Draw(void)
{
	glCallList(ListIndex);
}



void GroundPlane::CreateDisplayList(void)
{

// create a new list in compile mode so we only create it and not draw it
glNewList(ListIndex,GL_COMPILE);
 
glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
		//glColor4f(0,0.4,0.0,0.4);
		glColor4f(gpRed,gpGreen,gpBlue,gpAlpha);
		glBegin(GL_QUADS);
            glVertex3f(MinX,MaxY,MinZ);
            glVertex3f(MaxX,MaxY,MinZ);
            glVertex3f(MaxX,MaxY,MaxZ);
            glVertex3f(MinX,MaxY,MaxZ);
        glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
	

glEndList();
// end the list
}

/*! find the lowest value in the list of points
 \param GLfloat p[9]  the point values
 \returns int the index to the lowest */
int findLowest(GLfloat p[9])
{
// find the lowest value in the list of points
int low = 0;
for (int i=1; i<9; i++)
	 if(p[i] < p[low])
     	low = i;
return low;
}

/*! find the lowest value in the list of points
 \param GLfloat p[9]  the point values
 \returns int the index to the lowest */
int findHighest(GLfloat p[9])
{
// find the lowest value in the list of points
int low = 0;
for (int i=1; i<9; i++)
	 if(p[i] > p[low])
     	low = i;
return low;
}

Point3 GroundPlane::GetAlternativeDir(GLfloat x, GLfloat z, int Mode)
{
// create a list of the points surrounding the Agent
GLfloat HPoints[9];
// top row
HPoints[0]=GetHeight(x-1.0, z-1.0);
HPoints[1]=GetHeight(x, z-1.0);
HPoints[2]=GetHeight(x+1.0, z-1.0);
// middle row
HPoints[3]=GetHeight(x-1.0, z);
HPoints[4]=GetHeight(x, z);
HPoints[5]=GetHeight(x+1.0, z);
// bot row
HPoints[6]=GetHeight(x-1.0, z+1.0);
HPoints[7]=GetHeight(x, z+1.0);
HPoints[8]=GetHeight(x+1.0, z+1.0);
// find the index to the lowest from these points
int i=0;
switch (Mode)
{
case 0 : i=findLowest(HPoints); break;
case 1 : i=findHighest(HPoints); break;
}

// now determine the best direction to go
Point3 ret(0,HPoints[i],0);;
switch(i)
	{
    case 0 : ret.x=-1.0; ret.z=-1.0; break;
    case 1 : ret.x=0; ret.z=0.0; break;
    case 2 : ret.x=1.0; ret.z=-1.0; break;

    case 3 : ret.x=-1.0; ret.z=0; break;
    case 4 : ret.x=0; ret.z=0; break;
    case 5 : ret.x=1.0; ret.z=0; break;

    case 6 : ret.x=-1.0; ret.z=+1.0; break;
    case 7 : ret.x=0; ret.z=+1.0; break;
	case 8 : ret.x=1.0; ret.z=+1.0; break;

    }
return ret;
}


