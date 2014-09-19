#ifndef __PLANE_H__
#define __PLANE_H__

/*! \class Plane.h<BR>
	Author Jonathan Macey<BR>
    Version 3.0 <BR>
    Date Last Revision 17/02/03 <BR>
    Linkages : 
    PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>

    Description : <BR>

    Simple 3d Plane class
*/
// If were using windows we need to include the very bloated windows.h
#ifdef __USINGWINDOWS__
	#include <windows.h>
#endif

#ifdef LINUX
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif
#ifdef DARWIN
	#include <unistd.h>
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#endif

// local includes
#include <ngl/Vec4.h>
#include <ngl/Vec3.h>
#include "Util.h"
class Plane 
{
public :
	Plane();
    ~Plane();
    Plane(ngl::Vec3 TL,ngl::Vec3 TR, ngl::Vec3 BL, ngl::Vec3 BR);
    void Draw(void);
    ngl::Vec3 tl;
    ngl::Vec3 tr;
    ngl::Vec3 bl;
    ngl::Vec3 br;
    ngl::Vec3 C;
    ngl::Vec4 Normal;
private :


};

#endif

