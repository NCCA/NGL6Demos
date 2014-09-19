#ifndef __AGENTEMITTER_H_
#define __AGENTEMITTER_H_

/*! \class AgentEmitter.h
	Author Jonathan Macey <BR>
    Version 4.0 <BR>
    Date Last Revision 17/08/03 <BR>
    PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>

    Linkages : Agent.h BBox.h CurveFollow.h Util.h Goal.h Util.h  uses namespace std <BR>

    Description :

    This class is used to emit agents in the World based on a BBox for the world
    
	
    Revision History : <BR>

    Complet re-write of the original AgentEmitter class used in the previous flock example
	
*/
#ifdef __USINGWINDOWS__
	#include <windows.h>
#endif
// global includes

#ifdef LINUX
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif
#ifdef DARWIN
	#include <unistd.h>
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#endif
#include <vector>
#include <algorithm>
//local includes
#include "Agent.h"
#include <ngl/BBox.h>
#include <ngl/Vec4.h>
//#include "CurveFollow.h"
#include "Util.h"
#include "Goal.h"
#include "Noise.h"
#include <ngl/BezierCurve.h>

/*! these typedefs are just to make things easier */
//Agent iterator typedefs
typedef std::vector<Agent>                    AList;
//AgentList itterator typeded
typedef std::vector<Agent>::iterator          AListItt;
// GoalList
typedef std::vector<Goal>                    GList;
//GoalList ittrator
typedef std::vector<Goal>::iterator          GListItt;



class Environment;

class AgentEmitter
{
 public :
    /*! Point3 Position the position of the Emitter */
  ngl::Vec3    Position;
 	/*! AList Agents a vector of Agents */
    AList      Agents;          // vector of Agents
 	/*! BBox *Extents a pointer to the world bounding box */
    ngl::BBox *Extents;
    /*! unsigned long nAgents the number of agents in system */
    unsigned long int nAgents;
	/*! GList Goals a vector containing the Goal objects */
    GList Goals;
    /*! bool doCollide flag to set collision on or off */
    bool doCollide;
    /*! bool LockCentroid flag to lock the flock center */
    bool LockCentroid;
    
 /******************************************************************************************
 Methods : code in ../src/AgentEmitter.cpp
 ******************************************************************************************/
 /*!AgentEmitter Constructor
  \param Point3 pos the position of the emitter in the world
  \param unsigned long int num the number of Agents to emit
  \param GLfloat w width of the Agent
  \param GLfloat h height of the Agent
  \param GLfloat d depth of the agent
  \param GLfloat r Radius of the agent used for collision and vision
  \param  BBox * extents the extents of the world
  \param int Species the Agent species type
  \param Environment *e a pointer to the environment
  \param bool RandPos set to true for random dispersal of Agents or false for Point emitter
  \param char * BrainScript the name of the Agent Brain file to load
    */
AgentEmitter(ngl::Vec3 pos,unsigned long int num,GLfloat w,GLfloat h,GLfloat d,GLfloat r,
        ngl::BBox *extents,int Species,Environment *e, bool RandPos,char *BrainScript);
 /*!AgentEmitter Constructor
  \param Point3 pos the position of the emitter in the world
  \param unsigned long int num the number of Agents to emit
  \param GLfloat w width of the Agent
  \param GLfloat h height of the Agent
  \param GLfloat d depth of the agent
  \param GLfloat r Radius of the agent used for collision and vision
  \param  BBox * extents the extents of the world
  \param int StartSpecies the Agent Start species type
  \param int EndSpecies the Agent End species type
  \param Environment *e a pointer to the environment
  \param bool RandPos set to true for random dispersal of Agents or false for Point emitter
  \param char *BrainScript the name of the brain script file to load for the agents
  */

AgentEmitter(ngl::Vec3 pos,unsigned long int num,GLfloat w,GLfloat h,GLfloat d,GLfloat r,
        ngl::BBox *extents,int StartSpecies,int EndSpecies,Environment *e, bool RandPos,char *BrainScript);
/*! Destructor */
  ~AgentEmitter();
  /*! RenderGL renders the Agents in the Scene (calls the Agents Render method) */
  void RenderGL(void);
  /*! RenderAll Agents in scene as points (calls the Agent RenderPoints method) */
  void RenderPoints(void);
  /*! AddGoal adds a Goal to the Goal list vector
  \param GLfloat x the X position of the goal
  \param GLfloat y the Y position of the goal
  \param GLfloat z the Z position of the goal */
  void AddGoal(GLfloat x, GLfloat y, GLfloat z, GLfloat t);

/*************************************************************************************
   Inline Methods
***************************************************************************************/
  /*! ToggleCollide toggles the doCollide flag use to turn flocking on and off */
  inline void ToggleCollide(){doCollide ^=true;}
  /*! GetCentroid gets the centroid for the Emitter
  \returns Point3 the current Centroid */
  inline ngl::Vec3 GetCentroid(){return Centroid;}
  /*! FollowCurvePath toggles the follow curve for the Agents */
  inline void FollowCurvePath(void){FollowCurve^=true;}
  /*! SetCurvePathTime set the current time on the curve path when following the path
  \param GLfloat t the time on the path to set */
  inline void SetCurvePathTime(GLfloat t){CurvePathTime=t;}
  /*! CreatePath creates a new path for the Emitter and thus the agents to follow
  \param Point3 p[4] the array of points for the Path */
  inline void CreatePath(ngl::Vec3 p[4]){ /*path = new ngl::BezierCurve(p);*/}
  /*! SetEnv passes a pointer to the Environment to the Emitter so it can access all the global
  Environment variables
  \param Environment *E the pointer to the Environmnet */
  inline void SetEnv(Environment *E){env=E;}
  /*! DrawCurve draws the curve with a colour set to the colour s
  \param int s the species colour passed in */
  inline void DrawCurve(int s){path->draw();}
  /*! ToggleFollowGoals toggle the follow goals mode for the Emitter */
  inline void ToggleFollowGoals(){FollowGoals^=true;}
  /*! ToggleDrawExtents toggle the draw extents mode for the Emitter */
  //inline void ToggleDrawExtents(){DrawExtents^=true;}
  /*! Toggle if the Agents are to flock against a Locked Centroid */
  inline void ToggleLockedCentroid(){LockCentroid^=true;}
  /*! Sets the position of the locked centroid
  \param Point3 c The position of the Centroid */
  inline void SetLockedCentroid(ngl::Vec3 c){LockedCentroid=c;}
  /*! unsigned long int nGoals the number of Goals in the system */
  unsigned long int nGoals;
  /*! The current active goal in the system */
  unsigned long int ActiveGoal;
  /*! Reset the Agent values called each frame */
  void ResetAgents(void);
  
private :
/*! GLfloat ext[6] used to calculate the collision in the system ext contains the width
    height and depth of the BBox and this is tested against to see if the Agent has collided
    with world  BBox initialised in the constructor */
  GLfloat ext[6];
/*! Point3 Centroid The Center of the flock used for flock centering this is calculated as the
    Average position of all the Agents if doing normal flocking
    or set to the Curve Path at time t if following a path
    or set to a random position if using the follow food mode
    or set to an Agents Position if doing follow the leader mode
    It will have new meaning as the system evolves (can be passed to other species if they are hunting
    for example
    */
  ngl::Vec3 Centroid;
  /*! bool FollowCurve flag to indicate if the agents are to follow a curve */
  bool FollowCurve;
    /*! bool FollowGoals flag to indicate if the Agents should follow goals */
  bool FollowGoals;
  /*! bool DrawExtents flag to indicate if the AgentExtents should be drawn */
  bool DrawExtents;
  /*! CurveFollow *path pointer to the path for the agents to follow in follow path mode */
  ngl::BezierCurve *path;
  /*! GLfloat CurvePathTime the time on the curve path used to indicate where on the path
  the agents centroid should be */
  GLfloat CurvePathTime;
  /*! Enviroment *env a pointer to the Environment */
  Environment *env;
  /*! The position of the LockedCentroid in the Environment */
  ngl::Vec3 LockedCentroid;

// Methods
  /*! CalcCentroid calculate the current Centroid */
  void CalcCentroid(void);
  /*! The offset into the Environment Obj file list for the agent to be drawn as. This only gets set
  if an ObjFile is loaded in the script */
  int ObjListOffset;
  };


#endif

// End of class

