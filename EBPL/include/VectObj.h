#ifndef _VECTOBJ_H_
#define _VECTOBJ_H_

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
#include <ngl/Vec3.h>
#include <ngl/BBox.h>
#include "EnvObj.h"
#include "Util.h"

/*! \class VectObj.h <BR>
	Author Jonathan Macey <BR>
    Version 3.0 <BR>
    Date Last Revision 18/02/03 <BR>
    Linkages : BBox.h Util.h Point3.h Vector.h <BR>
	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>
    Description : <BR>
    Extended Bounding Box class used for Environment objects so they can have a force feild atached
    to them for deflecting objects <BR>
*/



class VectObj : public EnvObj
{

public :
	/*! constructor
    \param Point3 Center the center of the Object
    \param GLfloat Width the width of the object
    \param GLfloat Height the height of the object
    \param GLfloat Depth the depth of the object
    \param GLfloat ff the force feild radius used to calculate if the Agent has hit the
    forcefeild */
    VectObj(ngl::Vec3 Center,GLfloat Width,GLfloat Height,GLfloat Depth,GLfloat ff,ngl::Vec4 vect);
	/*! Method to draw the vector */
    void DrawVect(void);
    /*! The direction of the Object's vector */
  ngl::Vec4 Vect;
private :

};

#endif
//end of class

