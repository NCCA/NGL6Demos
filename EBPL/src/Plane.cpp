#include "Plane.h"
using namespace GraphicsLib;
Plane::Plane()
{

}
Plane::~Plane()
{

}

Plane::Plane(Point3 TL,Point3 TR, Point3 BL, Point3 BR)
{
// configure the plane
tl=TL;
tr=TR;
bl=BL;
br=BR;

C.x=(tr.x+tl.x+br.x+bl.x)/4;
C.y=(tr.y+tl.y+br.y+bl.y)/4;
C.z=(tr.z+tl.z+br.z+bl.z)/4;
Normal = CalcNormal ( tr,bl,tl );

Normal.normalize();
}

void Plane::Draw(void)
{
glBegin(GL_QUADS);
	glColor3f(1,1,1);
    bl.Vertex();
    br.Vertex();
    
    tl.Vertex();
    
    tr.Vertex();
    
glEnd();

}



