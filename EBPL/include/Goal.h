#ifndef __GOAL_H__
#define __GOAL_H__

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
#include <ngl/Material.h>

/*! \class Goal.h <BR>
	Author Jonathan Macey <BR>
    Version 3.0 <BR>
    Date Last Revision 18/02/03 <BR>
    Linkages : Point3.h primitives.h <BR>
	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>
    Description : <BR>
    a simple goal object container for the Agents to approach it mimics the centroid for the agents when
    in goal follow mode <BR>
*/


class Goal
{
public :
	/*! The position of the Goal */
		ngl::Vec4 GPos;
    /*! The time when the Goal is active based on the timer in the environment */
    GLfloat Gtime;
    /*! Constructor for the goal object passing in the position and the time active
    \param GLfloat x the x position
    \param GLfloat y the y position
    \param GLfloat z the z position
    \param GLfloat t the Time when the goal becomes active */
    Goal(GLfloat x, GLfloat y, GLfloat z, GLfloat t);
	/*! draw the goal in the environment */
    void DrawGoal(void);
private :
	/*! Material for the object to be drawn in */
	ngl::Material *m;

};



#endif
//end of class

