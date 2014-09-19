#ifndef __AGENTRENDER_H__
#define __AGENTRENDER_H__
#include "AnimatedObj.h"
/*! \class AgentRender.h<BR>
	Author Jonathan Macey<BR>
    Version 1.0 <BR>
    Date Last Revision 17/08/03 <BR>
    Linkages : AnimatedObj.h uses namespace std <BR>
    PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>

    Description : <BR>

    Class to render a series of .obj files as key framed sequences. This class is a simple
    container for a series of AnimatedObj file. 
    \todo make this class fully scriptable so it can  be configure from the .fl file.
*/
// If were using windows we need to include the very bloated windows.h
#ifdef __USINGWINDOWS__
	#include <windows.h>
#endif

class AgentRender
{
	public :
    	/*! array of animated obj's this will be replaced with a vector eventually
        This version is just a test to prove that it works */
    	AnimatedObj *anims[6];
        /*! Draw method to draw the Agent */
        void Draw(void);
        /*! Constructor */
        AgentRender();
        /*! Destructor */
        ~AgentRender();
        /*! Changes the animation cycle to use
        \param int c at present the cycles are 0 Walk 1 Run 2 Neutral 3 Dead 4 Swing 5 Punch */
        void ChangeCycle(int c);
        /*! update the frame number */
        void UpdateFrame(void);
        /*! set the current frame number
        \param int FrameN the frame number to use */
        void SetCurrentFrame(int FrameN);
        /*! Set the material to use for rendering the Agent */
        void SetMaterial(int mat);
    private :
	    /*! The current animation cycle to use */
        int CurrentCycle;
        

};


#endif

