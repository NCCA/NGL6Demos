#include "VarList.h"

using namespace std;
using namespace ngl;

VarList::VarList()
{

}
VarList::~VarList()
{

}
VarList::VarList(int size)
{
//v.resize(size);
 
}
void VarList::Add(VarObj V)
{
v.push_back(V);
}
void VarList::Remove(int index)
{

}
VarObj VarList::GetValue(int index)
{
return v[index]; 
}

void VarList::Print(void)
{
cout << "Variable List "<<endl;
for(unsigned int i=0; i<v.size(); i++)
	{
		
	cout << i <<" "<<v[i]<<endl;
	}


}

void VarList::Assign(unsigned int index,float V)
{
v[index].Float=V;
}

void VarList::Assign(unsigned int index,bool V)
{
v[index].Bool=V;
}

void VarList::Assign(unsigned int index,fuzzy V)
{
v[index].Fuzzy=V;
}

void VarList::Assign(unsigned int index, ngl::Vec4 V)
{
v[index].vect=V;
}

void VarList::Assign(unsigned int index, ngl::Vec3 V)
{
v[index].Point=V;
}

void VarList::Save(ofstream *op)
{
*op << v.size()<<endl;
for(unsigned int i=0; i<v.size(); i++)
	{
	*op << v[i].type << " ";
    switch (v[i].type)
    {
      case VarObj::BOOL :
       *op << (bool)v[i].Bool <<endl;
      break;
      case VarObj::FLOAT :
      *op << v[i].Float <<endl;
  	  break;
  	  case VarObj::VECTOR :
      *op << v[i].vect.m_x <<" " << v[i].vect.m_y <<" "<<v[i].vect.m_z  <<endl;
  	  break;
  	  case VarObj::POINT :
      *op << v[i].Point.m_x <<" " << v[i].Point.m_y <<" "<<v[i].Point.m_z  <<endl;
  	  break;
  	  case VarObj::FUZZY :
      *op << v[i].Fuzzy.m_fNum <<endl;
  	  break;
    }

	}

}


void VarList::Load(ifstream *ip)
{
unsigned int size;
*ip >>size;

//VarObj::VARTYPE 
int type;

for(unsigned int i=0; i<size; i++)
	{
    *ip >> type;
//enum VARTYPE{BOOL,FLOAT,VECTOR,POINT,FUZZY};
    
       if(type ==0)
       {
       bool Bool;
       *ip >> Bool;
       VarObj vb(Bool);
       v.push_back(vb);
      }
      else if(type == 1)
      {
       float Float;
       *ip >> Float;
       VarObj vf(Float);
       v.push_back(vf);
  	  }
  	  else if(type == 2) //case 2: //VarObj::VECTOR :
      {
      //*op << v[i].vect.m_x <<" " << v[i].vect.m_y <<" "<<v[i].vect.m_z <<" "<<v[i].vect.w <<endl;
      Vec4 vect;
      *ip>>vect.m_x>>vect.m_y>>vect.m_z>>vect.m_w;
      VarObj vv(vect);
       v.push_back(vv);
      }
  	  else if(type == 3)
      {
      //case 3 : //VarObj::POINT :
      Vec3 Point;
      *ip>>Point.m_x>>Point.m_y>>Point.m_z;
      VarObj vp(Point);
       v.push_back(vp);
  	  }
      else if(type == 4)
      {
  	  //case 4 : //VarObj::FUZZY :
      //*op << v[i].Fuzzy.m_fNum <<endl;
  	  fuzzy Fuzzy;
      *ip>>Fuzzy.m_fNum;
      VarObj vff(Fuzzy);
      v.push_back(vff);
      }
    
  
    
    
    } // end for
  


}


