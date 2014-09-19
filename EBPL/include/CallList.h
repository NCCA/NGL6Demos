/*! \class CallList <BR>
	Author Jonathan Macey <BR>
    Version 1.0 <BR>
    Date Last Revision 17/08/03 <BR>
    Linkages : uses namespace std
    CallList.h
	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>
    Description : <BR>
	Call lists are a way of using an ordinal value to call a function within a list of functions
    it is simmilar to a case statement

*/
#ifndef __CALLLIST_H__
#define __CALLLIST_H__


#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// Vector of Call list objects
typedef std::vector <unsigned int> CallObjList;


class CallList
{

public :
	/*! the object to store for the call list  just a list of int indexes into the funcitons*/
    CallObjList o;
	/*! Constructor */
    CallList(void);
    /*! Destructor */
    ~CallList();
    /*! Add a CallListObj to the list 
    \param unsigned int index the index of the Funciton to add to the list */
    void Add(unsigned int index);
    /*! Execute the funciton stored at index, these are really pointers to the 
    functions saved in the compiled brain file
    \param unsigned int index the index of the funciton to use */
    void Execute(unsigned int index);
    /*! Save the call list used in the compiler 
    \param std::ofstream *op pointer to the stream to save  file to */
    void Save(std::ofstream *op);
    /*! loadf the call list
    \param std::ifstream *ip pointer to the stream to load from */
    void Load(std::ifstream *ip);


};









#endif
