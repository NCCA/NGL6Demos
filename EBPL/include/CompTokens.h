#include <string>


/*! tokens used by the compiler to determine what object to parse in compiler */ 
enum COMPTOKENS {CFLOAT,CBOOL,CVECTOR,CPOINT,CFUZZY,COMMENT,WHITESPACE,VARASSIGN,FUNCTION,
				 FUNCTIONLIST,DRAWFUNCTION,UPDATEFUNCTION,INITFUNCTION,COLLIDEFUNCTION,
                 CALLLISTITEM,DEFCALLLIST
				
				};

/*! size of buffer  for data read in from file */
#define BUFFSIZE 500


/*! parses the current line of the source file and determines which token is used
\param char *buffer the current source file line
\returns COMPTOKENS which token has been read */
COMPTOKENS ParseString(char *buffer);
/*! the main compiler file loader 
\prame char *fname the name of the .bs file to load */
void ReadBrainFile(char *fname);
/*! parse a float variable declaration and load it to the globals list
\param char *buffer the current line of the source file */
extern void ParseFloat(char *buffer);
/*! check the variable lists and see if we have duplicate names */
extern void CheckForDuplicateVarNames(void);
/*! parse a bool variable declaration and load it to the globals list
\param char *buffer the current line of the source file */
extern void ParseBool(char *buffer);
/*! parse a fuzzy variable declaration and load it to the globals list
\param char *buffer the current line of the source file */
extern void ParseFuzzy(char *buffer);
/*! parse a Point variable declaration and load it to the globals list
\param char *buffer the current line of the source file */
extern void ParsePoint(char *buffer);
/*! parse a Vector variable declaration and load it to the globals list
\param char *buffer the current line of the source file */
extern void ParseVector(char *buffer);
/*! Parse a function and load it to the funciton list
\param char *buffer the current line of the source file
\COMPTOKENS type what type of function it is */
void ParseFunction(char *buffer,COMPTOKENS type);
/*! parse the function list (this is now obselete)
\param char *buffer the current source file line */
void ParseFunctionList(char *buffer);
/*! This function loads in the .bs file and removes extra spaces and comments
then saves it as a temp file to make compilation easier */
void createTempCompFile(char *fname);
/*! finds a function index by name
\param string s the name of the funciton to find
\returns unsigned int the index into the function list */
extern unsigned int FindFunction(string s);
/*! finds a global variable index by name
\param string s the name of the global variable to find
\returns unsigned int the index into the global list of the variable */
extern unsigned int FindGlobalVar(string s);
/*! Check if a name is actually a function 
\param string s the name of the function to find
\returns bool true if it is a function false if not */
extern bool CheckIfFunction(string s);
/*! checks to see if a name is a variable or not
\param string var the name of the variable to check
\returns bool true if it is a variable false if not */
extern bool CheckIfVar(string var);
/*! check to see if two global names have been used or not */
extern void CheckForDuplicateVarNames(void);
/*! build the initial function list from the 1st stage of the parse
\param char *fname the name of the .bs file */
extern void BuildInitialFunctionList(char *fname);
/*! The main opcode parsing function
\param char *buffer the current source file line 
\parame OpCodeList *o pointer to the opcode lists to save the compiled opcodes to
*/
extern void ParseOpCodes(char *buffer,OpCodeList *o);
/*! Parse the call list declarations
\param char *buffer the current source file line */
extern void ParseCallList(char *buffer);
/*! Parse the call list items and add them to the call list
\param char *buffer the current source file line */

extern void ParseCallListItem(char *buffer);
/*! check by name if a call list exists
\param string s the name of the call list
\returns bool true is valid list name else false */
extern bool CheckIfCallList(string s);
/*! find the call list index by name
\param string s the name of the call list
\return unsigned int the index into the call list structure */
extern unsigned int FindCallList(string s);



#ifndef __COMPGLOBALS_H_
#define __COMPGLOBALS_H_
/*! \class GlobalVar <BR>
	Author Jonathan Macey <BR>
    Version 1.0 <BR>
    Date Last Revision 17/08/03 <BR>
    Description : <BR>
	This is a holding structure for the parsing of the global variables

*/

class GlobalVar
{
 public :
 /*! the name of the variable */
 std::string Name;
 /*! the index into the list */
 int VarIndex;
 /*! the src file line number of this variable declaration */
 int LineNum;
 /*! constructor
 \param string n the name of the varaible
 \param int i the index of the variable
 \param int LineNum the source file line number */
 GlobalVar(std::string n, int i,int ln){Name=n; VarIndex=i;LineNum=ln;}
};
/*! \class GlobalFunc <BR>
	Author Jonathan Macey <BR>
    Version 1.0 <BR>
    Date Last Revision 17/08/03 <BR>
    Description : <BR>
	This is a holding structure for the parsing of the Functions

*/

class GlobalFunc
{
 public :
 /*! the name of the function */
 std::string Name;
 /*! The index of the function */
 int FIndex;
  /*! constructor
  \param string n the name of the funciton
  \param int i the index into the func list */
  GlobalFunc(std::string n, int i){Name=n; FIndex=i;}
};

/*! \class CallListNames <BR>
	Author Jonathan Macey <BR>
    Version 1.0 <BR>
    Date Last Revision 17/08/03 <BR>
    Description : <BR>
	This is a holding structure for the parsing of the call lists

*/

class CallListNames
{
 public :
 /*! The name of the calllist */
 std::string Name;
 /*! The index of the call list */
 int CLIndex;
 /*! constructor
 \param string n the name of the function 
 \param int i the index into the list */ 
 CallListNames(std::string n, int i){Name=n; CLIndex=i;}
};


#endif
