#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__
/*! \class Environment.h <BR>
	Author Jonathan Macey <BR>
    Version 5.0 <BR>
    Date Last Revision 17/08/03 <BR>

	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>

    Linkages : Agent.h BBox.h CurveFollow.h Util.h Goal.h Util.h  uses namespace std
               everything comes from this class <BR>

    Description : <BR>

    This class is used to emit agents in the World based on a BBox for the world <BR>
	

    Revision History : <BR>

    a much more sparse version of the env class as most of the stuff is now handeled by the brain
    */
#ifdef __USINGWINDOWS__
	#include <windows.h>
#endif

// Global includes
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <time.h>

// local includes
#include <ngl/BBox.h>
#include "EnvObj.h"
#include "VectObj.h"
// fwd declaration of class so we can include the following
class Environment;
#include "AgentEmitter.h"
#include "Agent.h"
#include <ngl/Obj.h>
//#include "AnimatedObj.h"
#include "GroundPlane.h"
#include "ImageGroundPlane.h"
#include "ObjGroundPlane.h"
#include "Plane.h"
#include "AgentRender.h"

// fwd declaration of class so we can include the Lattice class
class Lattice;
class LatticeBin;
#include "Lattice.h"

// typdefs for iterators and containers
typedef std::vector<EnvObj>                    EList;
typedef std::vector<EnvObj>::iterator          EListItt;

typedef std::vector<VectObj>                    VOList;
typedef std::vector<VectObj>::iterator          VOListItt;

typedef std::vector<AgentEmitter>                    AEList;
typedef std::vector<AgentEmitter>::iterator          AEListItt;

// These containers are used to point to other lists so are set as pointers to
// agents
typedef std::vector<Agent *>                    WorldAgentList;
typedef std::vector<Agent *>::iterator          WorldAgentListItt;

typedef std::vector<ngl::Obj *>                    ObjList;
//typedef std::vector<AnimatedObj *>                    AObjList;


typedef std::vector<Plane>                    PlaneList;
typedef std::vector<Plane>::iterator          PlaneListItt;


class Environment
{
public :
	/*! unsigned long int nObj the number of objects in the environment */
    unsigned long int nObj;
    /*! unsigned long int nVectObj the number of Vector objects in the environment */
	unsigned long int nVectObj;
    /*! unsigned long int nEmitters the number of emitters in the environment */
    unsigned long int nEmitters;
	/*! The list of Environment Objects in the environment */
    EList EnvObjects;
    /*! The list of the VectorObjects used */
	VOList VectObjects;
	/*! The list of Emitters in the environment */
    AEList Emitters;
    /*! The list of obj files loaded for drawing of agent models */
    ObjList ObjFileList;
	/*! The list of Plane objects help in the environment */
    PlaneList Planes;
    /*! The number of planes in the environment */
    unsigned long int nPlanes;
    /*! The list of obj files loaded for drawing of agent models */
    //AObjList AObjFileList;
	/*! The GroundPlane object used for all Agents */
	GroundPlane *gp;
    /*! Pointer to the world bounding box */
    ngl::BBox *WorldBBox;
    /*! the current time between calls in seconds */
    GLdouble dtSec;
	/*! the current time between calls in m Seconds */
    GLdouble dt;
    /*! how many frames to skip per write to the output file */
	unsigned long int FrameSkip;
    /*! Flag to indicate if env details are drawn */
	bool DrawEnvDetails;
    /*! Flag to indicate if we have any goals in the system and are using them for path following */
	bool UsingGoals;
    /*! The BinLattice data structure used for the Agent Collisions */
	Lattice *lat;
	
    /******************************************************************************
	Methods
*******************************************************************************/
    /*!default constuctor */
    /*! Constructor for Environment passing in a World BBox
    \param BBox *wb the bbox to set
	\param int Xlat the Lattice subdivisions in the X
    \param int Ylat the Lattice subdivisions in the Y
	\param int Zlat the Lattice subdivisions in the Z
	\param int binsize the default size for each bin */
	Environment(ngl::BBox *wb,int Xlat, int Ylat, int Zlat,int binsize);
    /*!destructor call all the destructors for the objects in the environment */
    ~Environment();
    /*! add an object to the environment list
    \param EnvObj newObj the object to add to the environment */
    void AddObject(EnvObj newObj);
	/*! adds an emitter to the environment
    \param AgentEmitter newEmitter the emitter to add to the environment */
    void AddEmitter(AgentEmitter newEmitter);
    /*! loop throught each of the emitters in the system an call the draw Method
    \param int mode the mode to draw them in 0 = solid 1 = points */
    void DrawEmitters(int mode);
    /*! loop through all the emitters and update them */
    void UpdateEmitters(void);
    /*! return the EnvObj at index i if it exists
    \params int i the index of the object */
    EnvObj *GetObject(int i);
    /*! draws the objects in the environment */
    void DrawObjects();
    /*! do all the updates for the environment this is a single interface to call to
        do all the updates we need
        \params int mode the draw mode for the agents 0=solid 1 = points */
    void DoUpdates(int mode);
    /*! set the DoCollide flag for each of the Agents */
    void SetDoCollide(void);
    /*! SetEmitterPath sets the Curve for the Emitter at index i using the points passed in
    \param int index the index of the agent to set
    \param Point3 p[4] the points for the curve */
    void SetEmitterPath(int index,ngl::Vec3 p[4]);
    /*! set the follow curve flag for emitter index
    \param int index the index of the emitter to set */
    void SetFollowCurve(int index);
    /*! sets the curve path time to t for the emitter index
    \param int index the index of the emitter to set
    \param GLfloat t the time to set the path to */
    void SetCurvePathTime(int index, GLfloat t);
    /*! Rotates an environment object around the axis flagged
    \param int index the index into the object list for the object to rotate
    \param GLfloat r the angle of rotation in degrees
    \param bool x true = rotate around the x-axis false = no rotate
	\param bool y true = rotate around the y-axis false = no rotate
    \param bool z true = rotate around the z-axis false = no rotate */
    void RotateObject(int index,GLfloat r,bool x, bool y, bool z);
    /*! used to set the maxRunTime for the environment this allows goals to be timed in a short
    simulation  so that the system runs for t seconds then resets the time to 0
    \param GLdouble t the time for the simulation to run in Seconds */
    void SetMaxRunTime(GLdouble t);
	/*! Get the elapsed time between calls to tick
 	\return GLdouble the elapsed time between calls */
    GLdouble tick(void);
    /*! opens the flock output file for writing */
    void OpenOutFile(void);
    /*!Closes the flock output file */
    void CloseOutFile(void);
    /*! Toggle the draw extents flag for all Emitters in the system */
    void ToggleDrawExtents(void);
    /*! Add a point to all the agents in all the emitters to our World agent list so we can
    do interflock collision detection */
    void AddAgentsToWorldList(void);
    
    /*! OutputToFile writes all the current information of each of the agents to the output file
    this allows pos and rot information to be written for each frame and dumped to a textfile
    specified using the Scrip OutputFile which can then be place in maya etc */
    void OutputToFile(void);
  /*! adds an alias wavefront obj file to the environment so the agents can be
    rendered with new models saved as obj files
    \param char *objFile the name of the file to load (.obj)
    \returns int the index into the objfile list for the agent to be drawn */
    int AddObjModel(char *objFile);
    /*! adds an alias wavefront obj file to the environment so the agents can be
    rendered with new models saved as obj files the models are interpolated from
	file1 to file2 in frames number of steps
    \param char *objFile1 the name of the file to load (.obj)
    \param char *objFile2 the name of the file to load (.obj)
	\param int frames the number of frames to generate 
	\param int Mat the type of Material to use for colour
	\returns int the index into the objfile list for the agent to be drawn */
	int AddObjModel(char *objFile1,char *objFile2,int frames,int Mat);


    void AddVectObject(VectObj newObj);
    void CreateGP(GLfloat y,GLfloat r,GLfloat g,GLfloat b,GLfloat a);
    void CreateGP(GLfloat y,char *fname,GLfloat YY);
    void CreateGP(GLfloat y,char *fname,char *fname2,GLfloat YY);
    void CreateObjGP(GLfloat y,ngl::Obj *obj,GLfloat YY);
    void CreateObjGP(GLfloat x,GLfloat y,char *fname,char *fname2,GLfloat xs,GLfloat yz, GLfloat zs);
	void LoadARF(unsigned long int index,char *fname);
		void AddPlane(ngl::Vec3 p[4]);
    void UseAgentRender(void);
/****************************************************************************
 Inline Methods
*****************************************************************************/
	/*! Toggles the file write flag which indicates if the current agent data should
    be written to file for each frame, default is false */
    inline void ToggleFileWrite(void){Write^=true;}
    /*! set the frame skip for write to the output file, default is 1
    \param long int s the size of the frame skip to set */
    inline void setFrameSkip(long int s){FrameSkip=s;}
    /*! return a pointer to the output file currently in use
    \returns FILE * the pointer to the file */
    inline FILE * getOutFile(void){return OutFile;}
	/*! setOutFile set the name of the output file to open
    \param char *fname the name of the output file to open */
    inline void setOutFile(char *fname){strcpy(outfile,fname);}
    /*!set the start frame value for writing to file */
    inline void setFrameOffset(unsigned long int F){FrameCount=F;}


private :
	/*! get the elapsed time in MS
    \returns GLint the time in MS */
    GLint GetTimeMS();
	/*! get the elapsed time in seconds
    \return GLdouble the elapsed time in seconds */
    GLdouble GetTimeSeconds();
	/*! a pointer to the output file for flock data */
    FILE *OutFile;
    /*! The elapsed time between calls to the Tick method */
    GLfloat Elapsed;
    /*! The current time for calculating the diff time */
    GLdouble currtime;
    /*! The max time a simulation is to run for before the clock is set back to 0
    This is usually run in conjunction with Goal objects */
    GLdouble MaxRunTime;
	/*! a vector of pointers to agents used for inter flock collisions */
		WorldAgentList worldAgents;
    /*! the current time set to time(NULL) when the system is started */
    time_t Time;
    /*! the name of the output file to open */
    char outfile[200];
	/*! The number of agents in the world agent vector list */
    unsigned long int numWorldAgents;
	/*! The current Frame */
    unsigned long int FrameCount;
    /*! flag to indicate if write to file is active default = false; */
    bool Write;
		AgentRender *agentRender;

    /*! Number of Objfiles loaded */
    unsigned long int nObjFiles;
	/*! Number of Animated Objfiles loaded */
    unsigned long int nAnimObjFiles;

};




#endif


//end of file

