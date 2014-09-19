#ifndef __LATTICEBIN_H__
#define __LATTICEBIN_H__
/*! \class LatticeBin.h <BR>
	Author Jonathan Macey <BR>
    Version 3.1 <BR>
    Date Last Revision 17/02/03 <BR>

	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>

    Linkages : Agent.h BBox.h CurveFollow.h Util.h Goal.h Util.h  uses namespace std
               everything comes from this class <BR>

    Description : <BR>

    

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
class LatticeBin;
#include "Agent.h"
#include <ngl/Vec3.h>

#include "Environment.h"
typedef std::vector<Agent *>                    AgentList;
typedef std::vector<Agent *>::iterator          AgentListItt;

class LatticeBin
{
public :
	/*! constructor */
	LatticeBin(void);
	/*! set the extents of the bin
	\param Point3 P[8] the vertices of the bin */
	void SetExtents(ngl::Vec3 P[8]);
	/*! destructor */
	~LatticeBin();
	/*! get the Empty status of the Lattice 
	\returns bool the state of the bin true == empty */
	inline bool Empty(){return IsEmpty;}
	/*! Method to determine is the Agent is in the bin and place it in the bin list
	\param Agent *a the Agent to test */
	void InBin(Agent *a);
	/*! draw method */
	void Draw(void);
	/*! find how many Agents are in the bin 
	\returns unsigned long in the number of Agents in the bin */
	inline unsigned long int getNagents(void){return nAgents;}
	/*! reset the bin */
	void Reset(void);
	/*! set the extents for the bin 
	\param BBox *Ext the world extents
	\param Environment *e the environment */
	void setWorldExtents(ngl::BBox *Ext,Environment *e);
	/*! change the size of the bin 
	 \param int size the new size for the bin */
	void ResizeBin(int size);
    void DoAgentCollideRoutines(void);
    /*! list of Agents in the bin */
	AgentList agents;
private :
	
	/*! flag to indicate if the bin is Empty */
	bool IsEmpty;
	/*! number of Agents in the bin */
	unsigned long int nAgents;
	/*! the verticies of the bins */
	ngl::Vec3 p[8];
	/*! the world  bbox extents */
	ngl::BBox *Extents;
	/*! GLfloat ext[6] used to calculate the collision in the system ext contains the width
    height and depth of the BBox and this is tested against to see if the Agent has collided
    with world  BBox initialised in the constructor */
	GLfloat ext[6];
	/*! check the Agent against the world bbox
	\param Agent *a */
	void WorldBBoxCollision(Agent *a);
	/*! pointer to the Environment */
	Environment *env;

};




#endif

