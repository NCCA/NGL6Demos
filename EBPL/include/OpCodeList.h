#ifndef __OPCODE_LIST_H_
#define __OPCODE_LIST_H_


/*! \class OpCodeList <BR>
	Author Jonathan Macey <BR>
    Version 3.0 <BR>
    Date Last Revision 18/02/03 <BR>
    Linkages : Brain.h BrainUtils.h VarList.h OpCode.h Util.h LatticeBin.h
	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>
    Description : <BR>
    This is the main interpretor class which holds a series of OpCode classes and executes them
    at present this uses a big case structure. In future version will use faster jump tables
    \todo re-engineer this class to use jump tables (or even directed goto).
*/


class Agent;
class Brain;
class OpCodeList;
class AgentEmitter;
#include "Brain.h"
#include "BrainUtils.h"
#include "VarList.h"
#include "OpCode.h"
#include <vector>
#include "Util.h"
#include "LatticeBin.h"

class OpCodeList
{

    
public :        
	/*! iterator for the opcodes */
    typedef std::vector <OpCode> OpcodesList;    
    /*! the global variable list */
    VarList *v;
	/*! List of the Opcodes for this function */
    OpcodesList op;
	/*! Constructor passing in the global variables
    \param VarList *V pointer to the globals */
    OpCodeList(VarList *V){V=v;}
	/*! default constructor */
    OpCodeList();
    /*! Add an opcode to the List used in the compiler 
    \param OpCode o the opcode class to load */
	void Add(OpCode o);
    /*! Execute the opcodes in this list
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent */
    void ExecOpCodes(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex);
    /*! Execute the current opcode based on the index
    \param OPCODES type the type of OpCode this is
    \param unsigned int i the index into the OpCodeList this is 
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param returns unsigned int the index of the opcode (changes when doing calls and ifs)*/
    unsigned int Exec(OpCode::OPCODES type,unsigned int i,Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex);
    /*! Save the Opcode list 
    \param ofstream *op the pointer to file stream
    */
    void Save(std::ofstream *op);
    /*! Load the Opcoe list 
    \param ifstream *ip the pointer to file stream */
    void Load(std::ifstream *ip);
    /*! print debug info */
    void Print(void);

    private :
    /*! flag to indicate if we're doing an ifelse */
    bool DoingIFELSE;
    /*! where we are going to jump to if the if is true */
    unsigned int ifelsejump;
    /*! where the jump ends */
    unsigned int ifelseend;
    /*! set to true for debug */
    bool DebugMode;
    /*! process the if opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    \param returns unsigned int the index of the opcode (changes when doing calls and ifs)*/
    int ProcessIf(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the add directly opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessAddDir(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);  
    /*! process the Mul directly opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
     
	void ProcessMulDir(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);    
    /*! process the reverse opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessReverse(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);    
    /*! process the Sub directly opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessSubDir(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);    
    /*! process the Set directly opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessSetDir(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the randomize opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessRandomize(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the randomizepos opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessRandomizePos(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
	/*! process the normalize opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessNormalize(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
	/*! process the multiply opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessMul(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
	/*! process the add divide opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessDiv(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the add  opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessAdd(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
	/*! process the subtract opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessSub(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
	/*! process the set directly opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessSet(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the fpush opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessFpush(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the fpop directly opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessFpop(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the set global pos
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessSetGlobalPos(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the getglobal pos
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    void ProcessGetGlobalPos(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
	/*! process the setglobalcentroid
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    void ProcessSetGlobalCentroid(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the getglobalcentroid
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    void ProcessGetGlobalCentroid(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the setGlobalDir opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    void ProcessSetGlobalDir(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the GetGlobalDir
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    void ProcessGetGlobalDir(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
	/*! process the sphere-sphere collision detection
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    void ProcessSphereSphere(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the cylinder cylinder collision detection
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    void ProcessCylinderCylinder(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the divide directly opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
        
    void ProcessDivDir(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
	/*! process the GetAgentI opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    void ProcessGetAgentI(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
	/*! process the SetAgentI opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    void ProcessSetAgentI(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the ifelse opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    int ProcessIfElse(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the sphere plane collision detection opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    void ProcessSpherePlane(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the sphere env object collision detection
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    void ProcessSphereEnvObj(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! debug the opcodes
    \param OPCODES type the opcode type
    \param unsigned int i the index of the opcode
    */
    void DebugOpCodes(OpCode::OPCODES type,unsigned int i);
    /*! process the set global collise flag
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    
    void ProcessSetGlobalCollideFlag(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the get gloal collide flag
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    
    void ProcessGetGlobalCollideFlag(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the dot product opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    
    void ProcessDotProduct(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the set gp y levet
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    
    void ProcessSetGPYlevel(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
	/*! process the noise get opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    
    void ProcessGetNoiseValue(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
    /*! process the noise set opcode
    \param Brain *br the agents brain
    \param Agent *Me the agent who's brain this is
    \param LatticeBin *bin the spatial data structure holding the agents
    \param unsigned int AgentIndex the index into the bin of this agent 
    \param unsigned int i the index into the OpCodeList this is 
    */
    
    
    void ProcessSetNoise(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i);
};



#endif

 
