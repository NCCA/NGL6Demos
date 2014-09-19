#include "ObjGroundPlane.h"
using namespace std;
using namespace GraphicsLib;
ObjGroundPlane::ObjGroundPlane(BBox *ext,GLfloat y, ObjLoader *obj,GLfloat yy)
: GroundPlane(ext,y,1.0,1.0,1.0,1.0)
{
// set default values
Obj=obj; 
Ytrans=y;
Modeltrans=yy;
// create the display list
CreateDisplayList();
}
 


ObjGroundPlane::ObjGroundPlane(BBox *ext,GLfloat y, GLfloat yy,char *fname,char *fname2,
				               GLfloat xs,GLfloat yz,GLfloat zs) : GroundPlane(ext,y,1.0,1.0,1.0,1.0)
{
// set the default value
Ytrans=y;
Modeltrans=yy;
Obj = new ObjLoader(fname,fname2);
Obj ->Scale(xs,yz,zs);
// create the display list
CreateDisplayList();
}


GLfloat ObjGroundPlane::GetHeight(GLfloat x, GLfloat z)
{
// get the height y value
GLfloat Yval;
Point3 p(x,z,0);
Yval=0; //Obj->GetHeight(p)+Ytrans+Modeltrans;
return Yval;
}

void ObjGroundPlane::Draw(void)
{
// draw the gp
glCallList(ListIndex);

}


void ObjGroundPlane::CreateDisplayList(void)
{
// create a new list in compile mode so we only create it and not draw it
glNewList(ListIndex,GL_COMPILE);
 glPushMatrix();
 glTranslatef(0,Ytrans,0);
 Obj->Draw();
glPopMatrix();
glEndList();

}

ObjGroundPlane::~ObjGroundPlane()
{
delete Obj;
}


               



