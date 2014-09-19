#ifndef __LATTICE_H_
#define __LATTICE_H_

/*! \class Lattice Lattice.h "include/Lattice.h" <BR>
	Author Jonathan Macey <BR>
    Version 1.0 <BR>
    Date Last Revision 26/03/03 <BR>
    Linkages :  uses namespace std <BR>

    PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>

    Description : Lattice class for FFD
    Revision History : <BR>

*/
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
#include <iostream>
#include <cmath>
#include "LatticePoint.h"

#include <ngl/Vec4.h>
#include "Util.h"
#include <ngl/BBox.h>
#include "LatticeBin.h"
#include "Environment.h"
/*! these typedefs are just to make things easier */
//Agent iterator typedefs
typedef std::vector<LatticePoint>             PList;
//AgentList itterator typeded
typedef std::vector<LatticePoint>::iterator          PListItt;

typedef std::vector<Agent *>                    AgentList;
typedef std::vector<Agent *>::iterator          AgentListItt;

class Lattice
{
 public :
 	 /*! the center of the lattice defaults to 0,0,0 */
     ngl::Vec3 center;
	 /*! the number of divisions in the X direction */
     int Xdiv;
     /*! the number of divisions in the Y direction */
     int Ydiv;
     /*! the number of divisions in the Z direction */
     int Zdiv;
     /*! the incriment in the X direction for lattice points calculated from 0 - 1 */
     GLfloat Xstep;
     /*! the incriment in the Y direction for lattice points calculated from 0 - 1 */
     GLfloat Ystep;
     /*! the incriment in the Z direction for lattice points calculated from 0 - 1 */
     GLfloat Zstep;
     /*! the direction of the local co-ordinate system x axis */
     ngl::Vec4 s;
     /*! the direction of the local co-ordinate system y axis */
     ngl::Vec4 t;
     /*! the direction of the local co-ordinate system z axis */
     ngl::Vec4 u;
     /*! the extents of the original obj file when loaded in */
     ngl::BBox *ext;
     /*! a pointer to the object loaded */
     //ObjLoader *Obj;
     /*! the number of points in the lattice */
     int Npoints;
     /*! the list of the lattice points */
     PList Points;
	 /*! constructor for the lattice
     \param int x divisions in the x direction
     \param int y divisions in the y direction
     \param int z divisions in the z direction
	 \param int BinSize the size for the bins
     */
     Lattice(int x, int y, int z,ngl::BBox *bb,Environment *e,int BinSize);
     /*! destructor */
     ~Lattice();
 	 /*! method to draw the lattice */
     void Draw(void);
     /*! method to draw the lattice point vertex from the Points array used to
     draw the lattice points
     \param int X the position of the lattice point
     \param int Y the position of the lattice point
     \param int Z the position of the lattice point */
     void DrawVertex(int X, int Y, int Z);
    /*! method to update each of the bins in the structure */
	void Update( void);
	
	/*! returns the point at position x,y,z
    \param int X the x offset
    \param int Y the y offset
    \param int Z the z offset
    \returns Vec3 the lattice point at x,y,z */
    ngl::Vec3 GetVertex(int X, int Y, int Z);
	/*! Method to fill the bins based on the WorldAgent list from the Environment
	\param AgentList *a the list of Agents from the Environment class */
	void FillBins(AgentList *a);
	/*! toggle the collide flag for the bins */
    inline void ToggleDoCollide(void){DoCollide^=true;}
    private :
		/*! pointer to all the bins in the structure */
		LatticeBin *bins;
		/*! the number of bins in the lattice */
		unsigned long int nBins;
		/*! flag to see if were doing the flocking or not */
		bool DoCollide;
		/*! pointer to the Environment */
		Environment *Env;
};


#endif

