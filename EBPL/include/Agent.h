#ifndef __AGENT_H__
#define __AGENT_H__

/*! \class Agent.h<BR>
	Author Jonathan Macey<BR>
    Version 4.0 <BR>
    Date Last Revision 17/08/03 <BR>
    Linkages : AgentEmitter.h BBox.h Point3.h Vector.h primitives.h Util.h  Brain.h uses namespace std <BR>
    PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>

    Description : <BR>

    This is the main class for the Agents. It is a simple container for the more complex
    Brain class which contains the OpCode interpretor
    \todo tidy up the constructors and remove the un-used parameters.
*/
// If were using windows we need to include the very bloated windows.h
#ifdef __USINGWINDOWS__
	#include <windows.h>
#endif
//global includes


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

// local includes

#include <ngl/Vec3.h>
#include <ngl/BBox.h>

#include "Util.h"
#include "GroundPlane.h"
#include <ngl/Camera.h>
#include "Noise.h"
#include "Brain.h"
// pre- declare the classes so they can be included
class Agent;
class AgentEmitter;
#include "Environment.h"
#include "LatticeBin.h"
class Agent
{
public :
	/*! Point3 Pos the position of the Agent this value may be accesed via the brain */
		ngl::Vec3 Pos;
    /*! GLfloat GPYlevel the ground plane level of the Agent */
	  GLfloat GPYlevel;
    
    
    /*! BBox *Extents the extents of the Agent used for collision detection */
    ngl::BBox *Extents;
    /*! GLint DrawMode specifies how to draw the agent */
    GLint DrawMode;
    /*! bool FLOCK a flag to indicate if the Agent is flocking */
    bool FLOCK;
    /*! GroundPlane *gp the ground plane for the Agents to respond to */
	  GroundPlane *gp;
    /*! Environment *env the Global environment of the system */
    Environment *env;
    /*! Vector Dir the Agents Direction */
    ngl::Vec4 Dir;
    /*! Point3 Centroid the global flock center */
    ngl::Vec3 Centroid;
    
    /*! bool CollideFlag used to indicate if the Agent is to do the collision deteciton or not */
    bool CollideFlag;
    /*! GLfloat noiseScale the scale of the noise */
    GLfloat noiseScale;
  	/*! flag to indicate were using the noise function */
    bool useNoise;
  	/*! the index into the noise value we're using */
    int NoiseType;
    /*! The brain we are using for the Agent */
    Brain *brain;
/******************************************************************************************
 Methods : code in ../src/Agent.cpp
 ******************************************************************************************/
	/*! Agent Constructor
     \param Point3 pos the position of the Agent in the world
     \param GLfloat width width of the Agent BBox
	 \param GLfloat height height of the Agent BBox
	 \param GLfloat depth depth of the Agent BBox
     \param GLfloat radius radius of the BBox fov
     \param Vector dir the initial direction of the Agent
     \param int Species the Agent species id
     \param char *BrainScript the compiled brain script for the Agent brain
     \param Environment *e the global environment 
     */
  Agent(ngl::Vec3 pos,GLfloat width,GLfloat height,GLfloat depth,
        GLfloat radius,ngl::Vec3 dir,int species,char *BrainScript,Environment *e);
    /*! constructor with no params */
    Agent(void);
    /*! Destructor */
    ~Agent();
    /*! Draw used to draw the Agent */
    void Draw(void);
    /*! DrawPoints draws the Agent as a series of Points used to speed up rendering */
    void DrawPoints(void);
    /*! used to calculate the new dir vector and position of the Agent */
    void Update(ngl::Vec3 C);
    /*! CollideFunction for the Agent
    \param LatticeBin *bin the bin the Agent lives in
    \parame unsigned int AgentIndex the index of the Agent in the WorldList
    */
	void CollideFunction(LatticeBin *bin,unsigned int AgentIndex);
/*****************************************************************************************
		Inline Methods
******************************************************************************************/
   /*! SetDrawMode sets the drawmode for the Agents
    \param GLint m the draw mode to set (using the standard GL draw modes i.e GL_POINTS etc)
    */
    inline void SetDrawMode(GLint m){DrawMode=m;}
    /*! Get the noise vector for the Agent
    \param GLfloat nx the X value to get noise for
    \param GLfloat ny the Y value to get noise for
    \param GLfloat nz the Z value to get noise for
    \returns Vector a noise value */
    ngl::Vec4 GetNoise(GLfloat nx,GLfloat ny, GLfloat nz);
	/*! Set the noise type to use
    \param GLfloat Scale the seed scale to use for the Noise
    \param int Type the noise type to use */
    void SetNoise(GLfloat Scale, int Type);
    
    
 private :
	/*! Noise *noise the noise function used for the Agents */
	Noise *noise;
    
    
};



#endif
