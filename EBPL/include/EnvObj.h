#ifndef _ENVOBJ_H_
#define _ENVOBJ_H_

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
#include "Vector.h"
#include <ngl/BBox.h>
#include "Util.h"

/*! \class EnvObj.h <BR>
	Author Jonathan Macey <BR>
    Version 4.0 <BR>
    Date Last Revision 17/08/03 <BR>
    Linkages : BBox.h Util.h Point3.h Vector.h <BR>
	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>
    Description : <BR>
    Extended Bounding Box class used for Environment objects so they can have a force feild atached
    to them for deflecting objects <BR>
*/



class EnvObj : public ngl::BBox
{

public :
	/*! the radius of the deflction force feild */
    GLfloat ForceFeildR;
	/*! The extents of the Environment box bbox */
    GLfloat EnvExt[6];
    /* draws the force feild and object normals */
    void DrawFF(void);
	/*! constructor
    \param Point3 Center the center of the Object 
    \param GLfloat Width the width of the object
    \param GLfloat Height the height of the object
    \param GLfloat Depth the depth of the object
    \param GLfloat ff the force feild radius used to calculate if the Agent has hit the 
    forcefeild */
    EnvObj(ngl::Vec3 Center,GLfloat Width,GLfloat Height,GLfloat Depth,GLfloat ff);
	/*! Rotates the object around the axis flagged
    \param GLfloat Angle the angle of rotation in degrees
    \param bool x true = rotate around the x-axis false = no rotate
	\param bool y true = rotate around the y-axis false = no rotate
    \param bool z true = rotate around the z-axis false = no rotate */
	void Rotate(GLfloat Angle, bool x, bool y, bool z);
    /*! toggles the draw mode for the object */
    inline void ToggleDrawExtents(){drawExtents^=true;}
	/*! Draw Method to draw the EnvObj */
	void Draw(void);
	/*! Create a display list for the Objects to speed things up */
	void CreateDisplayList(void);
private :
	/*! flag to indicate if the extents (ff and normals) are to be drawn */
    bool drawExtents;
	/*! The index for the display list created automatically by OpenGL */
	GLuint ListIndex;
};

#endif
//end of class

