/*! \class Brain <BR>
	Author Jonathan Macey <BR>
    Version 1.0 <BR>
    Date Last Revision 17/08/03 <BR>
    Linkages : Stack.h Point3.h Vector.h fuzzy.h VarList.h OpcodeList.h AgentRender.h LatticeBin.h
    CallList.h
	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>
    Description : <BR>
	The main interpretor and opcode system for the Agents brain. This contains the stacks and
    a series of Opcodes and Global variable used for the Agent. See other headers for more info

*/
#ifndef __BRAIN_H__
#define __BRAIN_H__

#include <iostream>
#include "Stack.h"
#include <ngl/Vec4.h>
#include <ngl/Vec3.h>
#include "fuzzy.h"
#include "VarList.h"

#include "OpCodeList.h"
#include "AgentRender.h"
#include "Agent.h"
#include "LatticeBin.h"
#include "CallList.h"
// Vector of Opcodes to be stored for the functions
typedef std::vector <OpCodeList> FUNCTIONS;
// vector for the calllists 
typedef std::vector <CallList> CALLLIST;
class Brain
{
public :
	/*! These are the build in functions for the AgentBrain at present there are 4 but
    more may be added Init is called once when the brain is loaded the rest are called
    every cycle of the Brain */
    enum BUILTINFUNCTIONS{INIT,UPDATE,DRAW,COLLIDE};
    /*! The index of this agent in the global list used when looping bins */
    unsigned int MeIndex;
    /*! constructor */
    Brain();
	/*! Constructor using a brain script file 
    \param char *fname the name of the compiled brain script to load 
    \param Agent *me poiner to the Agent to load the brain for, as the agent is constructed
    befor the brain the brain must be made aware of the Agent afterwards */
    Brain(char *fname,Agent *me);
    /*! Destructor */
    ~Brain();
	/*! vstack the stack of vector classes */
	Stack <ngl::Vec3> vstack;
	/*! fstack the stack of floats */
    Stack <float> fstack;
	/*! Fuzstack the stack of fuzzy logic classes */
    Stack <fuzzy> Fuzstack;
	/*! bstack the stack of bools */
    Stack <bool> bstack;
	/*! The global variables used by the brain, these are stored in the script then
    loaded and relocated to the current Agent brains */
    VarList Globals;
	/*! The opcode functions loaded from the compiled brain script file */
    FUNCTIONS Functions;
    /*! The calllists loaded from the compiled brain file */
    CALLLIST CallLists;
    /******************************************************************************
    Methods
    *********************************************************************************/
    /*! Load in the compiled brain script file
    \param char *fname the name of the .comp file to load
    \param Agent *Me a reference to the Agent who's brain this is. The brain needs to 
    know about the Agent and the Agent needs to know about the Brain so it's quite complex
    not sure how else to do it but could be refined at a later date using inheritance */
    void Load(char *fname,Agent *Me);
    /*! save the current brain. not implimented yet but will be usefull once all the opcodes
    are done so we can run simulations and save an Agent for later use 
    \param char *fname the name of the file to save as */
    
    void Save(char *fname);
    /*! Print out the details of the brain */
    void Print(void);
    /*! executes the opcodes helf in the funciton lists
    \param unsigned int index the index into the Functions list of the opcode to execute
    \param Agent *Me pointer to the current Agent */
    void ExecuteOpCode(unsigned int index,Agent *Me);
    /*! Update is one of the built in opcode functions called every iteration of the system
    \param Agent *Me pointer to the current Agent */
    void Update(Agent *Me);
    /*! Draw is one of the built in opcode functions called every iteration of the system to
    draw the Agent
    \param Agent *Me pointer to the current Agent */
    void Draw(Agent *Me);
    /*! Collide is one of the built in opcode functions called every iteration of the system to
    do the collision detection this uses the LatticeBin structure 
     \param Agent *Me pointer to the current Agent 
     \param LatticeBin *bin the lattice structure containint the Agents spacial info 
     \param int AgentIndex the index of the Agent in the WorldAgentList */
    void Collide(Agent *Me,LatticeBin *bin,unsigned int AgentIndex);
    /*! Debug the brain */
    void Debug();
    /*! assign the current AgentRender pointer to this brain so we can access it
    \param AgentRender *ar pointer to the Global Agent render class */
    void SetAgentRender(AgentRender *ar);
    /*! Changes the current animatio cycle of the agent render
    \param int c the cycle to set
    */
    void ChangeAnimCycle(int c);
    /*! render using the AgentRender system */
    void RenderAgent(void);
    /*! Set the fame for the AgentRender system
    \param int fn the frame to use for rendering */
    void SetCurrentFrame(int fn);
  	/*! Calls the init function, this is used when we load an Arf file as the 
    brain is already constructed and the init is called when the brain is constructed
    we need to re-call init using the arf info
    \param Agent *Me point to the Agent the brain belongs to */
    void CallInitFunction(Agent *Me);
    /*! Turn on and off the agent render module */
    
    inline void ToggleAgentRender(void){UseAgentRender^=true;}
    /*! Set the AgentRender material colour 
    \param int c the material index for the STDMAT of the material */
    void SetMaterialColour(int c);
private :
	/*! Pointer to the global agent Render module */
    AgentRender *render;
	/*! flag to indicate the agent render is in use */
    bool UseAgentRender;
   
protected :


};






#endif

