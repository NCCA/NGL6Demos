/*! \class OpCode <BR>
	Author Jonathan Macey <BR>
    Version 3.0 <BR>
    Date Last Revision 18/02/03 <BR>
    Linkages : VarList.h
	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>
    Description : <BR>
    This is the main opcode class used to store each of the Agent Brain instructions
*/



#ifndef __OPCODE_H_
#define __OPCODE_H_

#include "VarList.h"
#include <string>
// DON'T FORGET to update read and write methods when adding attribures to the class


class OpCode
{
public :
/*! These are the types for all of the OpCode and are used in the OpCodeList class to 
determine which opcode function to executr */
enum OPCODES { IF,TRANSLATE,ROTATEX,ROTATEY,ROTATEZ,VERTEX,GLEND,POLYGON,POINT,POINTSIZE,LINESIZE,LINELOOP,QUAD,COLOUR,
			   PUSHMATRIX,POPMATRIX,ADDDIR,RANDOMIZE,MUL,ADD,SUB,DIV,SET,FPUSH,FPOP,FATAN,FRAD2DEG,FNEGATE,
               DEBUG,LENGTH,FSQRT,FSIN,FASIN,FCOS,FACOS,FDIV,FSTACKTRACE,FSUB,FADD,FMUL,SETDIR,FDUP,
               CALL,SUBDIR,REVERSE,SPHERE,VERTEXF,CYLINDER,NORMALIZE,USEAGENTRENDER,SETANIMCYCLE,
               RENDERAGENT,SETGLOBALPOS,GETGLOBALPOS,SETGLOBALCENTROID,GETGLOBALCENTROID,SPHERESPHERE,
               LOOPBIN,BEEP,SETAGENTI,GETAGENTI,DIVDIR,RANDOMIZEPOS,IFELSE,PUSHGPYLEVEL,SCALE,
               SPHEREPLANE,SPHEREENVOBJ,RENDERFRAME,END,OPENBRACE,CLOSEBRACE,
               CALLLIST,CYLINDERCYLINDER,DEBUGOPON,DEBUGOPOFF,SETGLOBALCOLLIDEFLAG,GETGLOBALCOLLIDEFLAG,
               DOT,SOLIDSPHERE,LINES,SETGLOBALDIR,GETGLOBALDIR,MULDIR,CUBE,SETGPYLEVEL,GETNOISEVALUE,
               USENOISE,RENDERMATERIAL,FPUSHD,ENABLELIGHTS,DISABLELIGHTS,SMOOTH,FLAT

};
/*! used to tell what type of if evaluation is to be used in the if and ifelse opcodes */
enum IFEVALUATORS { IFEQUAL,IFNOTEQUAL,IFGREATER,IFGREATEREQ,IFLESS,IFLESSEQ};
/*! used to indicate is a direct float value is to be used or a global variable */
const static unsigned int GuardValue=999999;
// DON'T FORGET to update read and write methods when adding attribures to the class
	/*! what type of OpCode this one is */
    OPCODES type;
	/*! Constructor passing in Opcode type 
    \param OPCODES t the type of Opcode to create
    */
    OpCode (OPCODES t);
    /*! default constructor */
    OpCode(void);
	/*! set when using an if or ifelse to tell what type of comparision to use */
    IFEVALUATORS ifeval;
	/*! Index used to point into the Global varaible list */
    unsigned int Index;
    /*! Index used to point into the Global varaible list */
    unsigned int Index2;
    /*! Index used to point into the Global varaible list */
    unsigned int Index3;
    /*! Index used to point into the Global varaible list */
    unsigned int Index4;
	/*! Index used to point into the Global varaible list */
    unsigned int Index5;
    /*! Index used to point into the Global varaible list */
    unsigned int Index6;
    /*! Index used to point into the Global varaible list */
    unsigned int GlobVarIndex;
    /*! pointer to the global variable for the brain script */
    VariableList *Globals;
    /*! method to change the opcode type
    \param OPCODES t the type to change to */
    inline void SetType(OPCODES t){type=t;}
    /*! used to store direct floating point values used in the opcodes */
    float Floats[4];
    /*! used to read in the opcode class from file */
    friend std::ostream& operator<<(std::ostream& output, const OpCode& s);
	/*! used to save the opcode class to file */
    friend std::istream& operator>>(std::istream& output, OpCode &s);
private :
};





#endif

