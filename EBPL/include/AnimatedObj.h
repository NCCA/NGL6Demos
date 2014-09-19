/*! \class AnimatedObj <BR>
	Author Jonathan Macey <BR>
    Version 2.0 <BR>
    Date Last Revision 17/08/03 <BR>
    Linkages : ObjectLoader.h Util.h <BR>
	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>
    Description : <BR>

    	Loads in two .obj files and creates a series of display lists by linear interpolation of the
        objects S1.obj -> S2.obj. Note the objects must have the same number of verts norms and faces
        else it will not work !
*/

#ifndef __ANIMATEDOBJ_H_
#define __ANIMATEDOBJ_H_

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

#include <ngl/Obj.h>
#include <vector>
#include "Util.h"
#include <ngl/Material.h>
//iterator typedef for the display list offsets

typedef std::vector<GLuint>   FrameList;


class AnimatedObj
{
public :
	/*! AnimateModes the different Animation Modes avaliable */
    enum AnimateModes{FORWARDANIM,BACKWARDANIM,LOOPANIM};
    /*! constructor 
    \param char *obj1 the name of the first obj file
    \param char *obj2 the name of the second obj file
    \param int nframes the number of frames to interpolate over
    \param AnimateModes the mode for the animation keyframes 
    \param int Mat the material to render the agent with */
    AnimatedObj(char *obj1, char *obj2, int nframes,AnimateModes amode,int Mat);
    /*! Draw method using the display lists created at construction */
    void Draw();
	/*! destructor */
    ~AnimatedObj();
	/*! Update the current frame to be drawn */
    void UpdateFrameNo(void);
    /*! Set the current frame number
    \param int fn the frame number to set */
    void SetFrameNo(int fn);
    /*! set the current render material
    \param int mat index into the STDMAT array for built in materials see Material.h for details */
    void SetMaterial(int mat);

private :
	/*! a list of display list indexs for the frames */
	FrameList Frames;
	/*! the current frame number */
    int FrameNo;
	/*! the default animation mode */
    AnimateModes AnimateMode;
	/*! the number of frames of animation stored */
    int nFrames;
	/*! a list of frame indexs created if we are doing loop animation */
    int *lframes;
    /*! Material *M the material used to colour the Objects */
  ngl::Material *M;
    /*! bool UseMaterial flag to say the Material is being used */
	bool UseMaterial;
};

// end of class

#endif
