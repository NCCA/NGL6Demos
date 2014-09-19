#include "LatticePoint.h"
#ifdef LINUX
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif
#ifdef DARWIN
	#include <unistd.h>
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#endif
#include "Material.h"
using namespace GraphicsLib; 

LatticePoint::LatticePoint(Point3 pos, GLfloat weight)
{
// set initial values for the point
Pos=pos;
Weight=weight;
Active=false;
}



void LatticePoint::Draw(void)
{

}





