/*! \class VarList <BR>
	Author Jonathan Macey <BR>
    Version 3.0 <BR>
    Date Last Revision 18/02/03 <BR>
    Linkages : VarObj.h
	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>
    Description : <BR>
    Main global variable container used to store a list of all the global variables used in the 
    brain.
*/

#ifndef __VARLIST_H_
#define __VARLIST_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "VarObj.h"

typedef std::vector <VarObj> VariableList;


class VarList
{
public :
	/*! a list of VarObj */
    VariableList v;
	/*! Constructor */
    VarList();
	/*! Destructor */
    ~VarList();
	/*! Constructor with size 
    \param int size the size of the list to create */
    VarList(int size);
	/*! Add a variable to the list
    \param VarObj the variable to add */
    void Add(VarObj V);
	/*! Remove a variable from the list 
    \param int index the index of the variable to remove */
    void Remove(int index);
	/*! get the value of the variable 
    \param int index the index into the list
    \returns VarObj the object to return */
    VarObj GetValue(int index);
	/*! print out the list */
    void Print(void);
	/*! Assing the variable as a float
    \param unsigned int index the index of the variable in the list
    \param float V the value to assign
    */
    void Assign(unsigned int index,float V);
 	/*! Assing the variable as a bool
    \param unsigned int index the index of the variable in the list
    \param bool V the value to assign
    */
    void Assign(unsigned int index,bool V);
 	
    /*! Assing the variable as a fuzzy
    \param unsigned int index the index of the variable in the list
    \param fuzzy V the value to assign
    */
    void Assign(unsigned int index,fuzzy V);
 	
    /*! Assing the variable as a Vector
    \param unsigned int index the index of the variable in the list
    \param Vector V the value to assign
    */
    void Assign(unsigned int index,ngl::Vec4 V);
 	/*! Assing the variable as a Point3
    \param unsigned int index the index of the variable in the list
    \param Point3 V the value to assign
    */
    void Assign(unsigned int index,ngl::Vec3 V);
    /*! Save the Var list */
    void Save(std::ofstream *op);
    /*! Load the var list */
    void Load(std::ifstream *ip);
private :



};





#endif

