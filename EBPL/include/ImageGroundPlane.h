#ifndef _IMAGEGROUNDPLANE_H_
#define _IMAGEGROUNDPLANE_H_

#ifdef __USINGWINDOWS__
	#include <windows.h>
#endif
#include "GroundPlane.h"
/*! \class ImageGroundPlane <BR>
	Author Jonathan Macey <BR>
    Version 3.0 <BR>
    Date Last Revision 18/02/03 <BR>
    Linkages : BBox.h Point3.h Vector.h <BR>
	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>
    Description : <BR>
    Ground plane for Environment used to create a ground level for the agents to rest on
*/


class ImageGroundPlane : public GroundPlane
{

public :

/*! constructor for GroundPlane using a bmp image
	\param	BBox *ext a pointer to the world bounding box
	\param GLfloat y the ground level 
	\param char *Fname the name of the bmp file to load
	\param GLfloat YY the divisor for the image height map values
	*/
	ImageGroundPlane(ngl::BBox *ext,GLfloat y,char *Fname,GLfloat YY);
    /*! constructor for GroundPlane using a bmp image
	\param	BBox *ext a pointer to the world bounding box
	\param GLfloat y the ground level 
	\param char *Fname the name of the bmp file to load
	\param char *Fname2 the name of the bmp file to load for the texture
	\param GLfloat YY the divisor for the image height map values
	*/
	ImageGroundPlane(ngl::BBox *ext,GLfloat y,char *Fname, char *Fname2,GLfloat YY);
	
	/*! destructor */	 
	virtual ~ImageGroundPlane();
	/*! Get the height for the agent from it's x y position
	 \param GLfloat x the x position
	 \param GLfloat y the y position
	 \returns GLfloat the height at x y */
	virtual GLfloat GetHeight(GLfloat x, GLfloat z);
	
    /*! draw the GroundPlane */
	virtual void Draw(void);
private :
 	/*! the Image used for the terrain generation */
  ngl::Texture *image;
	/*! the Image used for the texture */
	ngl::Texture *Tex;
    /*! a list of pointers for the GroundPlane Points */
  ngl::Vec3 *gpPoints;
 	/*! a list of pointers for the texture co-ords */
  ngl::Vec3 *texPoints;
    /*! the number of steps in the X direction */
	int Xsteps;
    /*! the number of steps in the Z direction */
	int Zsteps;
	/*! the number of points in the groundplane */
	int nPoints;
    /*! Draw using a display list */
	void DrawDL(void);
    /*! Create the Display list */
	void CreateDisplayList(void);
    /*! Number of divisions in the x direction */
	GLfloat divx;
    /*! Number of divisions in the z direction */
	GLfloat divz;
	/*! Number of divisions in the x direction for texture mapping */
	GLfloat Tdivx;
    /*! Number of divisions in the z direction for texture mapping */
	GLfloat Tdivz;
    /*! the divisor for the height value */
	GLfloat Ydivisior;
    /*! The name of the texture for the texture generation */
	unsigned int Tname;
	
	

};

 

#endif



