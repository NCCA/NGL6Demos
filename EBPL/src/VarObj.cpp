#include "VarObj.h"
#include <string.h>
#include <stdio.h>
using namespace std;

ostream& operator<<(ostream& output, const VarObj& s)
  {
    switch (s.type)
	{
	case VarObj::BOOL : 
	     char str[6];
		 sprintf(str,"%s",s.Bool ? "true" : "false");
		 output <<"B  "<< str; //s.Bool ? "false" : "true" ;//str ; 
	break;
	case VarObj::FLOAT : output << "F  "<<s.Float; break;
	case VarObj::VECTOR : output << "V  "<<s.vect; break;
	case VarObj::POINT : output << "P  "<<s.Point; break;
	case VarObj::FUZZY : output << "FZ "<<s.Fuzzy.m_fNum; break;
	default : output <<"Not known "; break;
	
	
	}
	
	
	return output; //<<"["<<s.x<<","<<s.y<<","<<s.z<<"]";
  
   
  }
  
  
void VarObj::Translate(void)
{
#warning implement this
//if(type == VECTOR)
//	vect.Translate();
//else if(type == POINT)
//	Point.Translate();
}
void VarObj::Vertex(void)
{
#warning implement this
//  if(type == VECTOR)
//	vect.Vertex();
//else if(type == POINT)
//	Point.Vertex();
}
void VarObj::Normal(void)
 {
#warning implement this

//	if(type == VECTOR)
//	vect.Normal();
//else if(type == POINT)
//	Point.Normal();
  
 }
  
