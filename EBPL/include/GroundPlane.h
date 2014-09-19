#ifndef _GROUNDPLANE_H_
#define _GROUNDPLANE_H_

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
#include <ngl/Vec4.h>
#include <ngl/BBox.h>
#include <ngl/Texture.h>
#include <iostream>
/*! \class GroundPlane <BR>
	Author Jonathan Macey <BR>
    Version 4.0 <BR>
    Date Last Revision 18/02/03 <BR>
    Linkages : BBox.h Point3.h Vector.h <BR>
	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>
    Description : <BR>
    Ground plane for Environment used to create a ground level for the agents this is inherited
    by the other ground plane classes
*/


class GroundPlane
{
public :
	/*! constructor for simple GroundPlane using a flat value
	\param	BBox *ext a pointer to the world bounding box
	\param GLfloat y the ground level 
    \param GLfloat r the red component of the gp
	\param GLfloat g the green component of the gp
    \param GLfloat b the blue component of the gp
    \param GLfloat a the alpha component of the gp */
    GroundPlane(ngl::BBox *ext,GLfloat y,GLfloat r,GLfloat g, GLfloat b, GLfloat a);
	
	/*! destructor */
	virtual ~GroundPlane();
    /*! Get the height for the agent from it's x y position
	 \param GLfloat x the x position
	 \param GLfloat y the y position
	 \returns GLfloat the height at x y */
	virtual GLfloat GetHeight(GLfloat x, GLfloat z);
	
    /*! get a different direction for the Agent based on the current position
	\param GLfloat x the x position of the Agent
	\param GLfloat y the y position of the Agent
	\param int Mode which path follow algorithm to use 
	\returns Point3 the new direction to go */
	ngl::Vec3 GetAlternativeDir(GLfloat x, GLfloat y, int Mode);
    /*! draw the GroundPlane */
	virtual void Draw(void);
	/*! Get the MaxX extents of the gp
    \returns GLfloat the max x value */
    inline GLfloat GetMaxX(void){return MaxX;}
	/*! Get the MaxZ extents of the gp
    \returns GLfloat the max z value */
    inline GLfloat GetMaxZ(void){return MaxZ;}
	/*! Get the MinX extents of the gp
    \returns GLfloat the min x value */
    inline GLfloat GetMinX(void){return MinX;}
	/*! Get the MinZ extents of the gp
    \returns GLfloat the min z value */
    inline GLfloat GetMinZ(void){return MinZ;}
	/*! Get the MaxY extents of the gp
    \returns GLfloat the max y value */
    inline GLfloat GetMaxY(void){return MaxY;}
	/*! Get the MinY extents of the gp
    \returns GLfloat the min y value */
    inline GLfloat GetMinY(void){return MinY;}
    /*! set the gp colour
    \param GLfloat r the red value
	\param GLfloat g the green value
    \param GLfloat b the blue value
    \param GLfloat a the alpha value */
    inline void SetColour(GLfloat r,GLfloat g, GLfloat b, GLfloat a){gpRed=r; gpGreen=g; gpBlue=b; gpAlpha=a;}
    
private :
    
    /*! Create the Display list */
	void CreateDisplayList(void);
    /*! red value for gp \todo replace this with the Colour class */
    GLfloat gpRed;
    /*! green value for gp */
    GLfloat gpGreen;
    /*! blue value for gp */
    GLfloat gpBlue;
    /*! alpha value for gp */
    GLfloat gpAlpha;
  protected :
	/*! MinX value of the GroundPlane triangle strip */
	GLfloat MinX;
	/*! MaxX value of the GroundPlane triangle strip */
	GLfloat MaxX;
	/*! MinZ value of the GroundPlane triangle strip */
	GLfloat MinZ;
	/*! MaxZ value of the GroundPlane triangle strip */
	GLfloat MaxZ;
    /*! MinY value of the GroundPlane triangle strip */
	GLfloat MinY;
	/*! MaxY value of the GroundPlane triangle strip */
	GLfloat MaxY;
	/*! the index of the gp display list */
    GLuint ListIndex;
    
};

#endif

