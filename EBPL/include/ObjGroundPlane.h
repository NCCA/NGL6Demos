#ifndef _OBJGROUNDPLANE_H_
#define _OBJGROUNDPLANE_H_

#ifdef __USINGWINDOWS__
	#include <windows.h>
#endif
#include "GroundPlane.h"
#include <ngl/Obj.h>
/*! \class ObjGroundPlane <BR>
	Author Jonathan Macey <BR>
    Version 3.0 <BR>
    Date Last Revision 18/02/03 <BR>
    Linkages : BBox.h Point3.h Vector.h <BR>
	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>
    Description : <BR>
    Ground plane for Environment used to create a ground level for the agents to rest on
*/


class ObjGroundPlane : public GroundPlane
{

public :
	/*! constructor
    \param BBox *ext the extents of the environment bbox
    \param GLfloat y the height of the base gp
    \param ObjLoader *obj the obj file to load
    \param GLfloat yy the offset of the agent on the gp */
    ObjGroundPlane(ngl::BBox *ext,GLfloat y, ngl::Obj *obj,GLfloat yy);
	/*! constructor
    \param BBox *ext the extents of the environment bbox
    \param GLfloat y the height of the base gp
    \param GLfloat yy the offset of the agent on the gp 
    \param char *fname the name of the objfile to load
    \param char *fname2 the name of the bmp texture file
    \param GLfloat xs scale in the x 
    \param GLfloat ys the scale in the y
    \param GLfloat zs the scale in the z */
    
    
    ObjGroundPlane(ngl::BBox *ext,GLfloat y, GLfloat yy,char *fname,char *fname2,GLfloat xs,GLfloat ys,GLfloat zs);
	/*! destructor */
	virtual ~ObjGroundPlane();
	/*! Get the height for the agent from it's x y position
	 \param GLfloat x the x position
	 \param GLfloat y the y position
	 \returns GLfloat the height at x y */
	virtual GLfloat GetHeight(GLfloat x, GLfloat z);
	
    /*! draw the GroundPlane */
	virtual void Draw(void);
private :
	/*! Create the Display list */
	void CreateDisplayList(void);
    /*! how much to divide the  y scale by */
	GLfloat Ydivisior;
 	/*! the obj terrain file loaded */
    ngl::Obj *Obj;
	/*! where to place the gp in the y */
    GLfloat Ytrans;
	/*! how much to offset the agent on the terrain */
    GLfloat Modeltrans;
};



#endif



