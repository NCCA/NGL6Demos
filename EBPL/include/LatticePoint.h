#ifndef __LATTICEPOINT_H_
#define __LATTICEPOINT_H_

/*! \class LatticePoint LatticePoint.h "include/LatticePoint.h" <BR>
	Author Jonathan Macey <BR>
    Version 1.0 <BR>
    Date Last Revision 26/03/03 <BR>
    Linkages :  uses namespace std <BR>

    PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>

    Description : Simple class to hold a lattice grid point
    Revision History : <BR>

*/
#ifdef __USINGWINDOWS__
	#include <windows.h>
#endif

#include <iostream>


#ifdef LINUX
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif
#ifdef DARWIN
	#include <unistd.h>
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#endif

#include <vector>

#include <ngl/Vec4.h>
#include <ngl/Vec3.h>
#include "Util.h"

class LatticePoint
{
 public :
	/*! the position of the lattice point */
		ngl::Vec3 Pos;
    /*! indicates if the point is selected or not */
    bool Active;
    /*! the weight of the lattice point, not used at present but will be used to set who much of
    the FFD lattice affects the point when being deformed */
    GLfloat Weight;
    /*! The name of the current ponit used in the selection process */
    GLuint Name;
    /*! Constructor
    \param Point3 pos the position of the Lattice Point 
    \param GLfloat weight the weight of the point (set to 1.0 for now) */
    LatticePoint(ngl::Vec3 pos, GLfloat weight);
    /*! Method to draw the lattice point */
    void Draw(void);

};

#endif

