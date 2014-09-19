/*! \class VarObj <BR>
	Author Jonathan Macey <BR>
    Version 3.0 <BR>
    Date Last Revision 18/02/03 <BR>
    Linkages : VarObj.h
	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>
    Description : <BR>
    The actual variable itself
*/

#ifndef _VAROBJ_H_
#define _VAROBJ_H_
#include <iostream>

#include "fuzzy.h"
#include <ngl/Vec4.h>
#include <ngl/Vec3.h>




class VarObj
{
 friend std::ostream& operator<<(std::ostream& output, const VarObj& s);
 public :
  /*! the variable type */
  enum VARTYPE{BOOL,FLOAT,VECTOR,POINT,FUZZY};
  /*! the boolean component */
  bool Bool;
  /*! The floating point component */
  float Float;
  /*! The Vec4 component */
  ngl::Vec3 vect;
  /*! The point component */
  ngl::Vec3 Point;
  /*! The fuzzy component */
  fuzzy Fuzzy;
  /*! Constructor */
  VarObj();
  /*! Constructor 
  \param bool v set as a bool */
  inline VarObj(bool v){Bool=v;type=BOOL;}
  /*! Constructor 
  \param float v set as a float */
  inline VarObj(float v){Float=v;type=FLOAT;}
  /*! Constructor 
  \param Vec4 v set as a Vec4 */
  inline VarObj(ngl::Vec4 v){vect=v;type=VECTOR;}
  /*! Constructor 
  \param Point3 v set as a point */
  inline VarObj(ngl::Vec3 v){Point=v;type=POINT;}
  /*! Constructor 
  \param fuzzy v set as a fuzzy */
  inline VarObj(fuzzy v){Fuzzy=v;type=FUZZY;}
  /*! Destructor */
  ~VarObj(){;}
  /*! what type the variable is */
  VARTYPE type;
  /*! translate a point or Vec4 class */
  void Translate(void);
  /*! draw a point or Vec4 class */
  void Vertex(void);
  /*! use as a normal  a point or Vec4 class */
  void Normal(void);
 private :
	
}; 

#endif

