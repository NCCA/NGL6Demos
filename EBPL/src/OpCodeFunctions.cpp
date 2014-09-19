#include "OpCodeList.h"

using namespace std;
using namespace ngl;

const int HALF_RAND = (RAND_MAX / 2);
/*! returns a random number between +/- 1
\returns a random number between +/- 1 */
Real RandomNum()
{
	int rn;
	rn = rand();
	return ((Real)(rn - HALF_RAND) / (Real)HALF_RAND);
}
/*! returns a random number between +/- MaxVal
\param Real MaxVal the value to set for the max rand num
\returns a random number between +/- 1 */
Real RandomNum(Real MaxVal)
{
		return MaxVal * RandomNum();
}
/*! returns a positive random number between 0 and MaxVal
\param Real MaxVal the value to set for the max rand num
\returns a random number between 0 and MaxVal */
Real RandomPosNum(Real MaxVal)
{
 return  (MaxVal *rand()/(RAND_MAX+1.0f));
}

/*! set the random seed */

void SetSeed(void)
{
srand(time(NULL));
}



int OpCodeList::ProcessIf(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
 
bool res=false;
float a=0;
float b=0;
float c=0;
float aa=0;
float bb=0;
float cc=0;
Vec3 Pa,Pb;
 
  // get Var 1
switch(v->v[op[i].GlobVarIndex].type)
    {
    case VarObj::POINT :
//     a=v->v[op[i].GlobVarIndex].Point.m_x;
//     b=v->v[op[i].GlobVarIndex].Point.m_y;
//     c=v->v[op[i].GlobVarIndex].Point.m_z;
	Pa=v->v[op[i].GlobVarIndex].Point;
    break;

    case VarObj::VECTOR :
    a=v->v[op[i].GlobVarIndex].vect.m_x;
    b=v->v[op[i].GlobVarIndex].vect.m_y;
    c=v->v[op[i].GlobVarIndex].vect.m_z;

    break;
    case VarObj::FLOAT :
    a=b=c=v->v[op[i].GlobVarIndex].Float;
    break;
    case VarObj::BOOL :
    a=b=c=(float)v->v[op[i].GlobVarIndex].Bool;
    //cout <<"A Bool Value =" <<aa <<endl;
    break;
    case VarObj::FUZZY :
    a=b=c=0;
    break;
    } 
//get Var 2
switch(v->v[op[i].Index].type)
    {
    case VarObj::POINT :
//     aa=v->v[op[i].Index].Point.m_x;
//     bb=v->v[op[i].Index].Point.m_y;
//     cc=v->v[op[i].Index].Point.m_z;
    Pb=v->v[op[i].GlobVarIndex].Point;
    break;

    case VarObj::VECTOR :
    aa=v->v[op[i].Index].vect.m_x;
    bb=v->v[op[i].Index].vect.m_y;
    cc=v->v[op[i].Index].vect.m_z;

    break;
    case VarObj::FLOAT :
    aa=bb=cc=v->v[op[i].Index].Float;
    break;
    case VarObj::BOOL :
    aa=bb=cc=(float)v->v[op[i].Index].Bool;
    //cout <<"AA Bool Value =" <<aa <<endl;
    break;
    case VarObj::FUZZY :
    aa=bb=cc=0;
    break;
    } 
    //cout <<" a "<<a <<" aa "<< aa <<endl;
    if(v->v[op[i].GlobVarIndex].type==VarObj::POINT)
    {
    switch (op[i].ifeval)
        {
        case OpCode::IFEQUAL :
            res=Pa==Pb;   
        break;

        case OpCode::IFNOTEQUAL :
            res=Pa!=Pa;   
        break;
        case OpCode::IFGREATER : 
            //res=Pa>Pb;   
        break;
        case OpCode::IFGREATEREQ :
            //res=Pa>=Pb;   
        break;
        case OpCode::IFLESS :
            //res=Pa<Pb;   
        break;

        case OpCode::IFLESSEQ :
            //res=Pa<=Pb;   
        break;
        }
    
    }
    else
    {
    switch (op[i].ifeval)
        {
        case OpCode::IFEQUAL :
            res=a==aa;   
        break;

        case OpCode::IFNOTEQUAL :
            res=a!=aa;   
        break;
        case OpCode::IFGREATER : 
            res=a>aa;   
        break;
        case OpCode::IFGREATEREQ :
            res=a>=aa;   
        break;
        case OpCode::IFLESS :
            res=a<aa;   
        break;

        case OpCode::IFLESSEQ :
            res=a<=aa;   
        break;
        }
      }
        if(DebugMode==true)
        	cout <<"Res equals "<<res<<" index2 =" <<op[i].Index2<<endl;
    if(res!=true)
            i+=op[i].Index2; // watch this it could be dodgy
return i;
 }

 void OpCodeList::ProcessAddDir(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
 {
    switch (v->v[op[i].GlobVarIndex].type)
        {
         case  VarObj::POINT :
          v->v[op[i].GlobVarIndex].Point.m_x +=op[i].Floats[0];
            v->v[op[i].GlobVarIndex].Point.m_y +=op[i].Floats[1];
            v->v[op[i].GlobVarIndex].Point.m_z +=op[i].Floats[2];
         break;
        
         case  VarObj::VECTOR :
          v->v[op[i].GlobVarIndex].vect.m_x +=op[i].Floats[0];
            v->v[op[i].GlobVarIndex].vect.m_y +=op[i].Floats[1];
            v->v[op[i].GlobVarIndex].vect.m_z +=op[i].Floats[2];
         break;
         case  VarObj::FLOAT :
         	v->v[op[i].GlobVarIndex].Float +=op[i].Floats[0];
         break;
         case  VarObj::FUZZY :
         	v->v[op[i].GlobVarIndex].Fuzzy.m_fNum +=op[i].Floats[0];
         break;
         case VarObj::BOOL :
         break;
        
        }
 }
 
 void OpCodeList::ProcessMulDir(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
 {
    switch (v->v[op[i].GlobVarIndex].type)
        {
         case  VarObj::POINT :
          v->v[op[i].GlobVarIndex].Point.m_x *=op[i].Floats[0];
            v->v[op[i].GlobVarIndex].Point.m_y *=op[i].Floats[1];
            v->v[op[i].GlobVarIndex].Point.m_z *=op[i].Floats[2];
         break;
        
         case  VarObj::VECTOR :
          v->v[op[i].GlobVarIndex].vect.m_x *=op[i].Floats[0];
            v->v[op[i].GlobVarIndex].vect.m_y *=op[i].Floats[1];
            v->v[op[i].GlobVarIndex].vect.m_z *=op[i].Floats[2];
         break;
         case  VarObj::FLOAT :
         	v->v[op[i].GlobVarIndex].Float *=op[i].Floats[0];
         break;
         case  VarObj::FUZZY :
         //	v->v[op[i].GlobVarIndex].Fuzzy.m_fNum *=op[i].Floats[0];
         break;
         case VarObj::BOOL :
         break;
        
        }
 }
 
 
 
 void OpCodeList::ProcessDivDir(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
 {
 if(op[i].Floats[0]== 0)
 	{
    cout <<"Poss div by zero setting value to 1"<<endl;
    op[i].Floats[0]=1;
    }
if(op[i].Floats[1]== 0)
 	{
    cout <<"Poss div by zero setting value to 1"<<endl;
    op[i].Floats[1]=1;
    }
 if(op[i].Floats[2]== 0)
 	{
    cout <<"Poss div by zero setting value to 1"<<endl;
    op[i].Floats[2]=1;
    }
     
    switch (v->v[op[i].GlobVarIndex].type)
        {
         case  VarObj::POINT :
          v->v[op[i].GlobVarIndex].Point.m_x /=op[i].Floats[0];
            v->v[op[i].GlobVarIndex].Point.m_y /=op[i].Floats[1];
            v->v[op[i].GlobVarIndex].Point.m_z /=op[i].Floats[2];
         break;
        
         case  VarObj::VECTOR :
          v->v[op[i].GlobVarIndex].vect.m_x /=op[i].Floats[0];
            v->v[op[i].GlobVarIndex].vect.m_y /=op[i].Floats[1];
            v->v[op[i].GlobVarIndex].vect.m_z /=op[i].Floats[2];
         break;
         case  VarObj::FLOAT :
         	v->v[op[i].GlobVarIndex].Float /=op[i].Floats[0];
         break;
         case  VarObj::FUZZY :
         	v->v[op[i].GlobVarIndex].Fuzzy.m_fNum /=op[i].Floats[0];
         break;
         case VarObj::BOOL :
         break;
        
        }
 }
 
 void OpCodeList::ProcessReverse(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
 {
     	switch (v->v[op[i].GlobVarIndex].type)
        {
         case  VarObj::POINT :
          v->v[op[i].GlobVarIndex].Point.m_x =-v->v[op[i].GlobVarIndex].Point.m_x;
            v->v[op[i].GlobVarIndex].Point.m_y =-v->v[op[i].GlobVarIndex].Point.m_y;
            v->v[op[i].GlobVarIndex].Point.m_z =-v->v[op[i].GlobVarIndex].Point.m_z;
         break;
        
         case  VarObj::VECTOR :
          v->v[op[i].GlobVarIndex].vect.m_x =-v->v[op[i].GlobVarIndex].vect.m_x;
            v->v[op[i].GlobVarIndex].vect.m_y =-v->v[op[i].GlobVarIndex].vect.m_y;
            v->v[op[i].GlobVarIndex].vect.m_z =-v->v[op[i].GlobVarIndex].vect.m_z;
         break;
         case  VarObj::FLOAT :
         	v->v[op[i].GlobVarIndex].Float =-v->v[op[i].GlobVarIndex].Float;
         break;
         case  VarObj::FUZZY :
         	v->v[op[i].GlobVarIndex].Fuzzy.m_fNum =-v->v[op[i].GlobVarIndex].Fuzzy.m_fNum;
         break;
         case VarObj::BOOL :
         break;
        
        }
}
 
void OpCodeList::ProcessSubDir(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
switch (v->v[op[i].GlobVarIndex].type)
        {
         case  VarObj::POINT :
          v->v[op[i].GlobVarIndex].Point.m_x -=op[i].Floats[0];
            v->v[op[i].GlobVarIndex].Point.m_y -=op[i].Floats[1];
            v->v[op[i].GlobVarIndex].Point.m_z -=op[i].Floats[2];
         break;
        
         case  VarObj::VECTOR :
          v->v[op[i].GlobVarIndex].vect.m_x -=op[i].Floats[0];
            v->v[op[i].GlobVarIndex].vect.m_y -=op[i].Floats[1];
            v->v[op[i].GlobVarIndex].vect.m_z -=op[i].Floats[2];
         break;
         case  VarObj::FLOAT :
         	v->v[op[i].GlobVarIndex].Float -=op[i].Floats[0];
         break;
         case  VarObj::FUZZY :
         	v->v[op[i].GlobVarIndex].Fuzzy.m_fNum -=op[i].Floats[0];
         break;
         case VarObj::BOOL :
         break;
        
        }

}
 
void OpCodeList::ProcessSetDir(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
switch (v->v[op[i].GlobVarIndex].type)
        {
         case  VarObj::POINT :
          v->v[op[i].GlobVarIndex].Point.m_x =op[i].Floats[0];
            v->v[op[i].GlobVarIndex].Point.m_y =op[i].Floats[1];
            v->v[op[i].GlobVarIndex].Point.m_z =op[i].Floats[2];
         break;
        
         case  VarObj::VECTOR :
          v->v[op[i].GlobVarIndex].vect.m_x =op[i].Floats[0];
            v->v[op[i].GlobVarIndex].vect.m_y =op[i].Floats[1];
            v->v[op[i].GlobVarIndex].vect.m_z =op[i].Floats[2];
         break;
         case  VarObj::FLOAT :
         	v->v[op[i].GlobVarIndex].Float =op[i].Floats[0];
         break;
         case  VarObj::FUZZY :
         	v->v[op[i].GlobVarIndex].Fuzzy.m_fNum =op[i].Floats[0];
         break;
         case VarObj::BOOL :
         	v->v[op[i].GlobVarIndex].Bool =(bool)op[i].Floats[0];
         break;
        
        }

}
void OpCodeList::ProcessRandomize(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
    	if(op[i].Index2 == OpCode::GuardValue)
        {
        //cout <<"using direct float randomize "<<op[i].Floats[0]<<" "<<op[i].Floats[1]<<" "<<op[i].Floats[2]<<endl;
        switch (v->v[op[i].GlobVarIndex].type)
        {
         case  VarObj::POINT :
          v->v[op[i].GlobVarIndex].Point.m_x =RandomNum(op[i].Floats[0]);
            v->v[op[i].GlobVarIndex].Point.m_y =RandomNum(op[i].Floats[1]);
            v->v[op[i].GlobVarIndex].Point.m_z =RandomNum(op[i].Floats[2]);
         break;
        
         case  VarObj::VECTOR :
          v->v[op[i].GlobVarIndex].vect.m_x =RandomNum(op[i].Floats[0]);
            v->v[op[i].GlobVarIndex].vect.m_y =RandomNum(op[i].Floats[1]);
            v->v[op[i].GlobVarIndex].vect.m_z =RandomNum(op[i].Floats[2]);
         break;
         case  VarObj::FLOAT :
         	v->v[op[i].GlobVarIndex].Float =RandomNum(op[i].Floats[0]);
         break;
         case  VarObj::FUZZY :
         	v->v[op[i].GlobVarIndex].Fuzzy.m_fNum =RandomNum(op[i].Floats[0]);
         break;
		 case VarObj::BOOL :
         break;
        }
        }
        else
        {
        v->v[op[i].GlobVarIndex].Float =RandomNum(v->v[op[i].Index].Float);
        }

}
void OpCodeList::ProcessRandomizePos(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
    	if(op[i].Index2 == OpCode::GuardValue)
        {
        
        switch (v->v[op[i].GlobVarIndex].type)
        {
         case  VarObj::POINT :
          v->v[op[i].GlobVarIndex].Point.m_x =RandomPosNum(op[i].Floats[0]);
            v->v[op[i].GlobVarIndex].Point.m_y =RandomPosNum(op[i].Floats[1]);
            v->v[op[i].GlobVarIndex].Point.m_z =RandomPosNum(op[i].Floats[2]);
         break;
        
         case  VarObj::VECTOR :
          v->v[op[i].GlobVarIndex].vect.m_x =RandomPosNum(op[i].Floats[0]);
            v->v[op[i].GlobVarIndex].vect.m_y =RandomPosNum(op[i].Floats[1]);
            v->v[op[i].GlobVarIndex].vect.m_z =RandomPosNum(op[i].Floats[2]);
         break;
         case  VarObj::FLOAT :
         	v->v[op[i].GlobVarIndex].Float =RandomPosNum(op[i].Floats[0]);
         break;
         case  VarObj::FUZZY :
         	v->v[op[i].GlobVarIndex].Fuzzy.m_fNum =RandomPosNum(op[i].Floats[0]);
         break;
		 case VarObj::BOOL :
         break;
        }
        }
        else
        {
        v->v[op[i].GlobVarIndex].Float =RandomPosNum(v->v[op[i].Index].Float);
        }

}
 
void OpCodeList::ProcessNormalize(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
switch (v->v[op[i].GlobVarIndex].type)
        {
         case  VarObj::POINT :
         	v->v[op[i].GlobVarIndex].Point.normalize(); //x =RandomNum(op[i].Floats[0]);
            //v->v[op[i].GlobVarIndex].Point.m_y =RandomNum(op[i].Floats[1]);
            //v->v[op[i].GlobVarIndex].Point.m_z =RandomNum(op[i].Floats[2]);
         break;
        
         case  VarObj::VECTOR :
         	v->v[op[i].GlobVarIndex].vect.normalize();  //x =RandomNum(op[i].Floats[0]);
            //v->v[op[i].GlobVarIndex].vect.m_y =RandomNum(op[i].Floats[1]);
            //v->v[op[i].GlobVarIndex].vect.m_z =RandomNum(op[i].Floats[2]);
         break;
         case  VarObj::FLOAT :
         	v->v[op[i].GlobVarIndex].Float /= (v->v[op[i].GlobVarIndex].Float*v->v[op[i].GlobVarIndex].Float);
         break;
         case  VarObj::FUZZY :
         	//v->v[op[i].GlobVarIndex].Fuzzy.m_fNum =RandomNum(op[i].Floats[0]);
         break;
		 case VarObj::BOOL :
         break;
        }

}


void OpCodeList::ProcessMul(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
float a=0;
float b=0;
float c=0;
switch(v->v[op[i].Index].type)
        	{
             case VarObj::POINT :
             a=v->v[op[i].Index].Point.m_x;
             b=v->v[op[i].Index].Point.m_y;
             c=v->v[op[i].Index].Point.m_z;
             break;
             
             case VarObj::VECTOR :
             a=v->v[op[i].Index].vect.m_x;
             b=v->v[op[i].Index].vect.m_y;
             c=v->v[op[i].Index].vect.m_z;
             
             break;
             case VarObj::FLOAT :
             a=b=c=v->v[op[i].Index].Float;
             break;
             case VarObj::BOOL :
             case VarObj::FUZZY :
             a=b=c=0;
             break;
            } 
        
        switch(v->v[op[i].GlobVarIndex].type)
        	{
             case VarObj::POINT :
              v->v[op[i].GlobVarIndex].Point.m_x*=a;
              v->v[op[i].GlobVarIndex].Point.m_y*=b;
                v->v[op[i].GlobVarIndex].Point.m_z*=c;
             break;
             case VarObj::VECTOR :
              v->v[op[i].GlobVarIndex].vect.m_x*=a;
              v->v[op[i].GlobVarIndex].vect.m_y*=b;
                v->v[op[i].GlobVarIndex].vect.m_z*=c;
             
             break;
             case VarObj::FLOAT :
             	v->v[op[i].GlobVarIndex].Float*=a;
             break;
             case VarObj::BOOL :
         	 case VarObj::FUZZY :
             break;
            } 
         
}
void OpCodeList::ProcessDiv(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
float a=0;
float b=0;
float c=0;
switch(v->v[op[i].Index].type)
        	{
             case VarObj::POINT :
             a=v->v[op[i].Index].Point.m_x;
             b=v->v[op[i].Index].Point.m_y;
             c=v->v[op[i].Index].Point.m_z;
             break;
             
             case VarObj::VECTOR :
             a=v->v[op[i].Index].vect.m_x;
             b=v->v[op[i].Index].vect.m_y;
             c=v->v[op[i].Index].vect.m_z;
             
             break;
             case VarObj::FLOAT :
             a=b=c=v->v[op[i].Index].Float;
             break;
             case VarObj::BOOL :
             case VarObj::FUZZY :
             a=b=c=0;
             break;
            } 
        if(a == 0)
        	{
            cout <<"Possible div by 0 setting a to 1"<<endl; a=1;
            }
            if(b == 0)
        	{
            cout <<"Possible div by 0 setting b to 1"<<endl; b=1;
            }
            if(c == 0)
        	{
            cout <<"Possible div by 0 setting c to 1"<<endl; c=1;
            }
        switch(v->v[op[i].GlobVarIndex].type)
        	{
             case VarObj::POINT :
              v->v[op[i].GlobVarIndex].Point.m_x/=a;
              v->v[op[i].GlobVarIndex].Point.m_y/=b;
                v->v[op[i].GlobVarIndex].Point.m_z/=c;
             break;
             case VarObj::VECTOR :
              v->v[op[i].GlobVarIndex].vect.m_x/=a;
              v->v[op[i].GlobVarIndex].vect.m_y/=b;
                v->v[op[i].GlobVarIndex].vect.m_z/=c;
             
             break;
             case VarObj::FLOAT :
             	v->v[op[i].GlobVarIndex].Float*=a;
             break;
             case VarObj::BOOL :
         	 case VarObj::FUZZY :
             a=b=c=0;
             break;
            } 
         
}

void OpCodeList::ProcessAdd(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
float a=0;
float b=0;
float c=0;

switch(v->v[op[i].Index].type)
        	{
             case VarObj::POINT :
             a=v->v[op[i].Index].Point.m_x;
             b=v->v[op[i].Index].Point.m_y;
             c=v->v[op[i].Index].Point.m_z;
             break;
             
             case VarObj::VECTOR :
             a=v->v[op[i].Index].vect.m_x;
             b=v->v[op[i].Index].vect.m_y;
             c=v->v[op[i].Index].vect.m_z;
             
             break;
             case VarObj::FLOAT :
             a=b=c=v->v[op[i].Index].Float;
             break;
             case VarObj::BOOL :
             case VarObj::FUZZY :
             break;
            } 
        
        switch(v->v[op[i].GlobVarIndex].type)
        	{
             case VarObj::POINT :
              v->v[op[i].GlobVarIndex].Point.m_x+=a;
              v->v[op[i].GlobVarIndex].Point.m_y+=b;
                v->v[op[i].GlobVarIndex].Point.m_z+=c;
             break;
             case VarObj::VECTOR :
              v->v[op[i].GlobVarIndex].vect.m_x+=a;
              v->v[op[i].GlobVarIndex].vect.m_y+=b;
                v->v[op[i].GlobVarIndex].vect.m_z+=c;
             
             break;
             case VarObj::FLOAT :
             	v->v[op[i].GlobVarIndex].Float+=a;
             break;
             case VarObj::BOOL :
             case VarObj::FUZZY :
             a=b=c=0;
             break;
            } 
         
}

void OpCodeList::ProcessSub(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
float a=0;
float b=0;
float c=0;

switch(v->v[op[i].Index].type)
        	{
             case VarObj::POINT :
             a=v->v[op[i].Index].Point.m_x;
             b=v->v[op[i].Index].Point.m_y;
             c=v->v[op[i].Index].Point.m_z;
             break;
             
             case VarObj::VECTOR :
             a=v->v[op[i].Index].vect.m_x;
             b=v->v[op[i].Index].vect.m_y;
             c=v->v[op[i].Index].vect.m_z;
             
             break;
             case VarObj::FLOAT :
             a=b=c=v->v[op[i].Index].Float;
             break;
             case VarObj::BOOL :
         	 case VarObj::FUZZY :
         	 a=b=c=0;
             break;
            } 
        
        switch(v->v[op[i].GlobVarIndex].type)
        	{
             case VarObj::POINT :
              v->v[op[i].GlobVarIndex].Point.m_x-=a;
              v->v[op[i].GlobVarIndex].Point.m_y-=b;
                v->v[op[i].GlobVarIndex].Point.m_z-=c;
             break;
             case VarObj::VECTOR :
              v->v[op[i].GlobVarIndex].vect.m_x-=a;
              v->v[op[i].GlobVarIndex].vect.m_y-=b;
                v->v[op[i].GlobVarIndex].vect.m_z-=c;
             break;
             case VarObj::FLOAT :
             	v->v[op[i].GlobVarIndex].Float-=a;
             break;
             case VarObj::BOOL :
         	 case VarObj::FUZZY :
         	 break;
            } 
         
}


void OpCodeList::ProcessSet(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
float a=0;
float b=0;
float c=0;
switch(v->v[op[i].Index].type)
        	{
             case VarObj::POINT :
             a=v->v[op[i].Index].Point.m_x;
             b=v->v[op[i].Index].Point.m_y;
             c=v->v[op[i].Index].Point.m_z;
             break;
             
             case VarObj::VECTOR :
             a=v->v[op[i].Index].vect.m_x;
             b=v->v[op[i].Index].vect.m_y;
             c=v->v[op[i].Index].vect.m_z;
             
             break;
             case VarObj::FLOAT :
             a=b=c=v->v[op[i].Index].Float;
             break;
             case VarObj::BOOL :
             	a=b=c=v->v[op[i].Index].Bool;
             break;
             case VarObj::FUZZY :
             a=b=c=0;
             break;
            } 

        switch(v->v[op[i].GlobVarIndex].type)
        	{
             case VarObj::POINT :
              v->v[op[i].GlobVarIndex].Point.m_x=a;
              v->v[op[i].GlobVarIndex].Point.m_y=b;
                v->v[op[i].GlobVarIndex].Point.m_z=c;
             break;
             case VarObj::VECTOR :
              v->v[op[i].GlobVarIndex].vect.m_x=a;
              v->v[op[i].GlobVarIndex].vect.m_y=b;
              v->v[op[i].GlobVarIndex].vect.m_z=c;
             break;
             case VarObj::FLOAT :
             	v->v[op[i].GlobVarIndex].Float=a;
             break;
             case VarObj::BOOL :
             	v->v[op[i].GlobVarIndex].Bool=v->v[op[i].Index].Bool;
             break;
         	 case VarObj::FUZZY :
             	v->v[op[i].GlobVarIndex].Fuzzy=v->v[op[i].Index].Fuzzy;
             break;
            } 

}

void OpCodeList::ProcessFpush(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
//float a,b,c;
float value=op[i].Index;
float sv;
        switch(v->v[op[i].GlobVarIndex].type)
        	{
             case VarObj::POINT :
             	if(value == 0)
                  sv=v->v[op[i].GlobVarIndex].Point.m_x;
             	else if(value == 1)
                  sv=v->v[op[i].GlobVarIndex].Point.m_y;
                else if(value == 2)
                  sv=v->v[op[i].GlobVarIndex].Point.m_z;
             break;
             case VarObj::VECTOR :
             	//v->v[op[i].GlobVarIndex].vect.Print("Vect in Fpush");
             	if(value == 0)
                  sv=v->v[op[i].GlobVarIndex].vect.m_x;
             	else if(value == 1)
                  sv=v->v[op[i].GlobVarIndex].vect.m_y;
                else if(value == 2)
                  sv=v->v[op[i].GlobVarIndex].vect.m_z;
             break;
             case VarObj::FLOAT :
             	sv=v->v[op[i].GlobVarIndex].Float;
             break;
             case VarObj::BOOL :
             	sv=(float)v->v[op[i].GlobVarIndex].Bool;
             break;
         	 case VarObj::FUZZY :
             	sv=v->v[op[i].GlobVarIndex].Fuzzy.m_fNum;
             break;
            } 
            br->fstack.push(sv);
        
}

void OpCodeList::ProcessFpop(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
float a;
float value=op[i].Index;
//cout <<"value is "<<value<<endl;
        switch(v->v[op[i].GlobVarIndex].type)
        	{
             case VarObj::POINT :
             	a=br->fstack.pop();
                if(value == 0)
                  v->v[op[i].GlobVarIndex].Point.m_x=a;
             	else if(value == 1)
                  v->v[op[i].GlobVarIndex].Point.m_y=a;
                else if(value == 2)
                  v->v[op[i].GlobVarIndex].Point.m_z=a;
             break;
             case VarObj::VECTOR :
             	a=br->fstack.pop();
                if(value == 0)
                  v->v[op[i].GlobVarIndex].vect.m_x=a;
             	else if(value == 1)
                  v->v[op[i].GlobVarIndex].vect.m_y=a;
                else if(value == 2)
                  v->v[op[i].GlobVarIndex].vect.m_z=a;
                    
             break;
             case VarObj::FLOAT :
             	a=br->fstack.pop();
                v->v[op[i].GlobVarIndex].Float=a;
             break;
             case VarObj::BOOL :
             	a=br->fstack.pop();
             	v->v[op[i].GlobVarIndex].Bool=(bool)a;
             break;
         	 case VarObj::FUZZY :
             	a=br->fstack.pop();
             	v->v[op[i].GlobVarIndex].Fuzzy.m_fNum=a;
             break;
            } // end Fpop

}



void OpCodeList::ProcessSetGlobalPos(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
switch(v->v[op[i].GlobVarIndex].type)
        	{
             case VarObj::POINT :
              Me->Pos.m_x=v->v[op[i].GlobVarIndex].Point.m_x;
              Me->Pos.m_y=v->v[op[i].GlobVarIndex].Point.m_y;
                Me->Pos.m_z=v->v[op[i].GlobVarIndex].Point.m_z;
                //Me->Pos.Print("ME");
             break;
             case VarObj::VECTOR :
              Me->Pos.m_x=v->v[op[i].GlobVarIndex].vect.m_x;
              Me->Pos.m_y=v->v[op[i].GlobVarIndex].vect.m_y;
                Me->Pos.m_z=v->v[op[i].GlobVarIndex].vect.m_z;
             
             break;
             case VarObj::FLOAT :
              Me->Pos.m_x=v->v[op[i].GlobVarIndex].Float;
                Me->Pos.m_y=v->v[op[i].GlobVarIndex].Float;
                Me->Pos.m_z=v->v[op[i].GlobVarIndex].Float;
             break;
             case VarObj::BOOL :
             case VarObj::FUZZY :
             break;
            } 

}

void OpCodeList::ProcessGetGlobalPos(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
         
         switch(v->v[op[i].GlobVarIndex].type)
         	{
              case VarObj::POINT :
                v->v[op[i].GlobVarIndex].Point.m_x=Me->Pos.m_x;
                v->v[op[i].GlobVarIndex].Point.m_y=Me->Pos.m_y;
                 v->v[op[i].GlobVarIndex].Point.m_z=Me->Pos.m_z;
              break;
              case VarObj::VECTOR :
                v->v[op[i].GlobVarIndex].vect.m_x=Me->Pos.m_x;
                v->v[op[i].GlobVarIndex].vect.m_y=Me->Pos.m_y;
                 v->v[op[i].GlobVarIndex].vect.m_z=Me->Pos.m_z;
              
              break;
              case VarObj::FLOAT :
                v->v[op[i].GlobVarIndex].Float=Me->Pos.m_y;
                 
              break;
              case VarObj::BOOL :
              case VarObj::FUZZY :
              break;
             } 
          

}


void OpCodeList::ProcessSetGlobalCentroid(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
switch(v->v[op[i].GlobVarIndex].type)
        	{
             case VarObj::POINT :
              Me->Centroid.m_x=v->v[op[i].GlobVarIndex].Point.m_x;
              Me->Centroid.m_y=v->v[op[i].GlobVarIndex].Point.m_y;
                Me->Centroid.m_z=v->v[op[i].GlobVarIndex].Point.m_z;
                //Me->Pos.Print("ME");
             break;
             case VarObj::VECTOR :
              Me->Centroid.m_x=v->v[op[i].GlobVarIndex].vect.m_x;
              Me->Centroid.m_y=v->v[op[i].GlobVarIndex].vect.m_y;
                Me->Centroid.m_z=v->v[op[i].GlobVarIndex].vect.m_z;
             
             break;
             case VarObj::FLOAT :
              Me->Centroid.m_x=v->v[op[i].GlobVarIndex].Float;
                Me->Centroid.m_y=v->v[op[i].GlobVarIndex].Float;
                Me->Centroid.m_z=v->v[op[i].GlobVarIndex].Float;
             break;
             case VarObj::BOOL :
             case VarObj::FUZZY :
             break;
            } 

}
void OpCodeList::ProcessGetGlobalCentroid(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
switch(v->v[op[i].GlobVarIndex].type)
         	{
              case VarObj::POINT :
              v->v[op[i].GlobVarIndex].Point.m_x=Me->Centroid.m_x;
                v->v[op[i].GlobVarIndex].Point.m_y=Me->Centroid.m_y;
                 v->v[op[i].GlobVarIndex].Point.m_z=Me->Centroid.m_z;
              break;
              case VarObj::VECTOR :
                v->v[op[i].GlobVarIndex].vect.m_x=Me->Centroid.m_x;
                v->v[op[i].GlobVarIndex].vect.m_y=Me->Centroid.m_y;
                 v->v[op[i].GlobVarIndex].vect.m_z=Me->Centroid.m_z;
              
              break;
              case VarObj::FLOAT :
                v->v[op[i].GlobVarIndex].Float=Me->Centroid.m_y;
                
             break;
              case VarObj::BOOL :
              case VarObj::FUZZY :
             break;
             } 
       
}

void OpCodeList::ProcessSphereSphere(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
 if(Me==bin->agents[AgentIndex])
 {
 
    return;
 }
if(Me->CollideFlag == true || bin->agents[AgentIndex]->CollideFlag==true )
	return;
float Rad1,Rad2;
Vec3 Pos1,Pos2;
//bool res;
Pos1=v->v[op[i].Index].Point;
Rad1=v->v[op[i].Index2].Float;
Pos2=bin->agents[AgentIndex]->brain->Globals.v[op[i].Index3].Point;
Rad2=bin->agents[AgentIndex]->brain->Globals.v[op[i].Index4].Float;


//Pos2=bin->Agents[AgentIndex].Brain.v->v[op[i].Index3].Point;
//Rad2=v->v[op[i].Index4].Float;
//Pos1.Print("Pos 1");
//Pos2.Print("Pos 2");
v->v[op[i].GlobVarIndex].Bool=SphereSphereCollision(Pos1,Rad1,Pos2,Rad2);


}

void OpCodeList::ProcessCylinderCylinder(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
if(Me==bin->agents[AgentIndex])
 {
  //  cout <<"CC Me = "<<Me <<" bin "<<bin->agents[AgentIndex]<<" ai "<<AgentIndex<<endl;
    return;
    
 }
float Rad1,Rad2,Len1,Len2;
Vec3 Pos1,Pos2;
//bool res;
Pos1=v->v[op[i].Index].Point;
Rad1=v->v[op[i].Index2].Float;
Len1=v->v[op[i].Index3].Float;
Pos2=bin->agents[AgentIndex]->brain->Globals.v[op[i].Index4].Point;
Rad2=bin->agents[AgentIndex]->brain->Globals.v[op[i].Index5].Float;
Len2=bin->agents[AgentIndex]->brain->Globals.v[op[i].Index6].Float;

//Pos2=bin->Agents[AgentIndex].Brain.v->v[op[i].Index3].Point;
//Rad2=v->v[op[i].Index4].Float;
//Pos1.Print("Pos 1");
//Pos2.Print("Pos 2");
//cout<<"calling cylcyl funct now"<<endl;
v->v[op[i].GlobVarIndex].Bool=CylinderCylinderCollision(Pos1,Rad1,Len1,Pos2,Rad2,Len2);


}



void OpCodeList::ProcessGetAgentI(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
// GlobVarIndex == Me Agent
// Index == Agent I
//copy AgentI into Me Agent;

switch (v->v[op[i].GlobVarIndex].type)
        {
         case  VarObj::POINT :
          v->v[op[i].GlobVarIndex].Point=bin->agents[AgentIndex]->brain->Globals.v[op[i].Index].Point;
         break;
       
         case  VarObj::VECTOR :
         //cout <<"doing get Agent I Vect vi" << v->v[op[i].GlobVarIndex].vect <<" valgot "<<bin->agents[AgentIndex]->brain->Globals.v[op[i].Index].vect<<endl;
         v->v[op[i].GlobVarIndex].vect.m_x=bin->agents[AgentIndex]->brain->Globals.v[op[i].Index].vect.m_x;
         v->v[op[i].GlobVarIndex].vect.m_y=bin->agents[AgentIndex]->brain->Globals.v[op[i].Index].vect.m_y;
         v->v[op[i].GlobVarIndex].vect.m_z=bin->agents[AgentIndex]->brain->Globals.v[op[i].Index].vect.m_z;
         break;
         case  VarObj::FLOAT :
         	v->v[op[i].GlobVarIndex].Float =bin->agents[AgentIndex]->brain->Globals.v[op[i].Index].Float;
         break;
         case  VarObj::FUZZY :
         	v->v[op[i].GlobVarIndex].Fuzzy.m_fNum =bin->agents[AgentIndex]->brain->Globals.v[op[i].Index].Fuzzy.m_fNum;
         break;
         case VarObj::BOOL :
         	v->v[op[i].GlobVarIndex].Bool=bin->agents[AgentIndex]->brain->Globals.v[op[i].Index].Bool;
         break;
        
        }



}

void OpCodeList::ProcessSetAgentI(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
// GlobVarIndex == Me Agent
// Index == Agent I
//copy AgentI into Me Agent;
//cout <<"SetAgentI "<<AgentIndex<<" i="<<i<<endl;
switch (v->v[op[i].GlobVarIndex].type)
        {
         case  VarObj::POINT :
      //   cout <<"Setting Point "<<AgentIndex<<" op "<<op[i].Index<<" i= "<<i<<endl;
         bin->agents[AgentIndex]->brain->Globals.v[op[i].Index].Point.m_x =v->v[op[i].GlobVarIndex].Point.m_x;
         bin->agents[AgentIndex]->brain->Globals.v[op[i].Index].Point.m_y =v->v[op[i].GlobVarIndex].Point.m_y;
         bin->agents[AgentIndex]->brain->Globals.v[op[i].Index].Point.m_z =v->v[op[i].GlobVarIndex].Point.m_z;
         break;
       
         case  VarObj::VECTOR :
         bin->agents[AgentIndex]->brain->Globals.v[op[i].Index].vect=v->v[op[i].GlobVarIndex].vect;
         break;
         case  VarObj::FLOAT :
         	bin->agents[AgentIndex]->brain->Globals.v[op[i].Index].Float=v->v[op[i].GlobVarIndex].Float ;
         break;
         case  VarObj::FUZZY :
         	bin->agents[AgentIndex]->brain->Globals.v[op[i].Index].Fuzzy.m_fNum=v->v[op[i].GlobVarIndex].Fuzzy.m_fNum ;
         break;
         case VarObj::BOOL :
         	bin->agents[AgentIndex]->brain->Globals.v[op[i].Index].Bool=v->v[op[i].GlobVarIndex].Bool;
         break;
        
        }



}


int OpCodeList::ProcessIfElse(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
bool res=false;
float a=0;
float b=0;
float c=0;
float aa=0;
float bb=0;
float cc=0;
Vec3 Pa,Pb;
 
  // get Var 1
switch(v->v[op[i].GlobVarIndex].type)
    {
    case VarObj::POINT :
//     a=v->v[op[i].GlobVarIndex].Point.m_x;
//     b=v->v[op[i].GlobVarIndex].Point.m_y;
//     c=v->v[op[i].GlobVarIndex].Point.m_z;
	Pa=v->v[op[i].GlobVarIndex].Point;
    break;

    case VarObj::VECTOR :
    a=v->v[op[i].GlobVarIndex].vect.m_x;
    b=v->v[op[i].GlobVarIndex].vect.m_y;
    c=v->v[op[i].GlobVarIndex].vect.m_z;

    break;
    case VarObj::FLOAT :
    a=b=c=v->v[op[i].GlobVarIndex].Float;
    break;
    case VarObj::BOOL :
    a=b=c=(float)v->v[op[i].GlobVarIndex].Bool;
    //cout <<"A Bool Value =" <<aa <<endl;
    break;
    case VarObj::FUZZY :
    a=b=c=0;
    break;
    } 
//get Var 2
switch(v->v[op[i].Index].type)
    {
    case VarObj::POINT :
//     aa=v->v[op[i].Index].Point.m_x;
//     bb=v->v[op[i].Index].Point.m_y;
//     cc=v->v[op[i].Index].Point.m_z;
    Pb=v->v[op[i].Index].Point;
    break;

    case VarObj::VECTOR :
    aa=v->v[op[i].Index].vect.m_x;
    bb=v->v[op[i].Index].vect.m_y;
    cc=v->v[op[i].Index].vect.m_z;

    break;
    case VarObj::FLOAT :
    aa=bb=cc=v->v[op[i].Index].Float;
    break;
    case VarObj::BOOL :
    aa=bb=cc=(float)v->v[op[i].Index].Bool;
    //cout <<"AA Bool Value =" <<aa <<endl;
    break;
    case VarObj::FUZZY :
    aa=bb=cc=0;
    break;
    } 
    
    if(v->v[op[i].GlobVarIndex].type==VarObj::POINT)
    {
    switch (op[i].ifeval)
        {
        case OpCode::IFEQUAL :
            res=Pa==Pb;   
        break;

        case OpCode::IFNOTEQUAL :
            res=Pa!=Pa;   
        break;
        case OpCode::IFGREATER : 
          //  res=Pa>Pb;   
        break;
        case OpCode::IFGREATEREQ :
           // res=Pa>=Pb;   
        break;
        case OpCode::IFLESS :
           // res=Pa<Pb;   
        break;

        case OpCode::IFLESSEQ :
           // res=Pa<=Pb;   
        break;
        }
    
    }
    else
    {
    
    //cout <<" a "<<a <<" aa "<< aa <<endl;
    switch (op[i].ifeval)
        {
        case OpCode::IFEQUAL :
            res=a==aa;   
        break;

        case OpCode::IFNOTEQUAL :
            res=a!=aa;   
        break;
        case OpCode::IFGREATER : 
            res=a>aa;   
        break;
        case OpCode::IFGREATEREQ :
            res=a>=aa;   
        break;
        case OpCode::IFLESS :
            res=a<aa;   
        break;

        case OpCode::IFLESSEQ :
            res=a<=aa;   
        break;
        }
      } // end of else
        //cout <<"Res equals "<<res<<endl;
    // if its false jump over
    if(res==true)
       {
        //cout <<"ifelse true "<<endl;
        DoingIFELSE=true;
        ifelsejump=i+op[i].Index3; // watch this bit could be buggy
        ifelseend=i+op[i].Index2;
		//CurrIfElseIndex++;        
        //InIfElse=true;
       }
    // other wise we're doing the if part so we need to do up to the index then jump
    // the next bit of code
    else
    	{
        
        i+=op[i].Index2; // watch this it could be dodgy
        //cout <<"False doing jump"<<i<<endl;
        }

return i;

}

void OpCodeList::ProcessSpherePlane(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
float Rad;
Vec4 Pos;
float D;
float Xmin,Xmax;
//bool res;
Pos.m_x=v->v[op[i].Index].Point.m_x;
Pos.m_y=v->v[op[i].Index].Point.m_y;
Pos.m_z=v->v[op[i].Index].Point.m_z;
Rad=v->v[op[i].Index2].Float;

//cout <<"Doing Sphere Plane"<<endl;
unsigned int nPlanes=Me->env->nPlanes;
//cout << "Number of Planes = "<<nPlanes<<endl;
for(unsigned int ii=0; ii<nPlanes; ii++)
{
//cout <<"I = "<<ii<<endl;
D=Me->env->Planes[ii].Normal.dot(Pos);
        //Now Add the Radius of the sphere to the offsett
//float R=-Me->env->Planes[i].C.m_x;
D+=Rad;

//D+=Me->env->Planes[i].C.m_x/2;
//cout <<"D="<<D<<" C "<<Me->env->Planes[i].C<<endl;
// If this is greater or equal to the BBox extent /2 then there is a collision
//So we calculate the Agents new direction
 if(D<=Me->env->Planes[ii].C.m_z+Rad && D>=Me->env->Planes[ii].C.m_z-Rad)
   {
   
   Xmin=Me->env->Planes[ii].bl.m_x;
   Xmax=Me->env->Planes[ii].br.m_x;
    if(Xmin > Xmax)
    	 {
      float t=Xmax;
      Xmax=Xmin;
      Xmin=t;
      }
   //cout <<" X "<<Pos.m_x<<" Xmin "<<Xmin<<" Xmax "<<Xmax<<endl;
   if(Pos.m_x >= Xmin && Pos.m_x <= Xmax )
     {//cout <<"Hit Plane"<<endl<<endl;;
   		v->v[op[i].GlobVarIndex].Bool=true;}
        //else cout <<"Miss"<<endl;
   }

}
}

void OpCodeList::ProcessSphereEnvObj(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
float Rad;
Vec3 Pos;
Pos.m_x=v->v[op[i].Index].Point.m_x;
Pos.m_y=v->v[op[i].Index].Point.m_y;
Pos.m_z=v->v[op[i].Index].Point.m_z;
Rad=v->v[op[i].Index2].Float;

// the relative position of the agent
Vec4 relPos;
// the distance of the agent from the object
GLfloat dist;
float minDist;
 // Dot product needs a Vector so we convert The Point Temp into a Vector so we can
// do a dot product on it
Vec3 p;
// create iterators for the loop
//AListItt a = Agents.begin();
//AListItt end = Agents.end();
// Loop for each agent in the list
//while(a != end)
//check for each object in the Env list


for(unsigned  long int nEobj=0; nEobj<Me->env->nObj; nEobj++)
      {
       	// first calculate the agent to test's new position
        relPos =Pos-Me->env->EnvObjects[nEobj].center;
        dist=relPos.m_x * relPos.m_x + relPos.m_y * relPos.m_y + relPos.m_z * relPos.m_z;
		minDist =Rad +Me->env->EnvObjects[nEobj].ForceFeildR;
        //test to see if we've hit the bounding sphere
        if(dist <=(minDist * minDist))
        	{
            //hit bounding spehere now to test which face.
			
						p.set(Pos.m_x,Pos.m_y,Pos.m_z);
            //check each cube face
            for(int f=0; f<6; f++)
                {
                //flag we've hit the Object
                //agents[a]->HitEnv=true;
                v->v[op[i].GlobVarIndex].Bool=true;
                 //to calculate the distance we take the dotporduct of the Plane Normal
                 //with the new point P
                 
                 GLfloat D=Me->env->EnvObjects[nEobj].Norm[f].dot(p);
//                 //Now Add the Radius of the sphere to the offsett
                 D+=Rad;
//                 // If this is greater or equal to the BBox extent /2 then there is a collision
//                 //So we calculate the Agents new direction
                 if(D >=Me->env->EnvObjects[nEobj].EnvExt[f])
                     {
//                     // for now we just move the agent normal to the face which is clunky
//                     // later on i will improve this by adding proper force feild deflection
//                     // to the system however this works so will do for now
                     //agents[a]->EnvAvoidDir=env->EnvObjects[nEobj].Norm[f];
                     v->v[op[i].Index3].vect=Me->env->EnvObjects[nEobj].Norm[f];
                     } //end of hit if
     			} // end of each object face
        	} // we hit if
          } //end of check each Env Object loop
  	


   
}




void OpCodeList::ProcessSetGlobalCollideFlag(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
switch(v->v[op[i].GlobVarIndex].type)
        	{
             case VarObj::POINT :
             case VarObj::VECTOR :
             case VarObj::FLOAT :
             case VarObj::FUZZY :
             break;	
             case VarObj::BOOL :
             	Me->CollideFlag=v->v[op[i].GlobVarIndex].Bool;
             break;
             
            } 

}

void OpCodeList::ProcessGetGlobalCollideFlag(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
         
         switch(v->v[op[i].GlobVarIndex].type)
         	{
              case VarObj::POINT :
			  case VarObj::FUZZY :
              case VarObj::VECTOR :
              case VarObj::FLOAT :
              break;
              case VarObj::BOOL :
              	v->v[op[i].GlobVarIndex].Bool=Me->CollideFlag;
              break;
             } 
          

}



void OpCodeList::ProcessDotProduct(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{

float a=0;
float b=0;
float c=0;
float aa=0;
float bb=0;
float cc=0;
Vector A,B;
 
  // get Var 1
switch(v->v[op[i].Index].type)
    {
    case VarObj::POINT :
    a=v->v[op[i].Index].Point.m_x;
    b=v->v[op[i].Index].Point.m_y;
    c=v->v[op[i].Index].Point.m_z;
    break;

    case VarObj::VECTOR :
    a=v->v[op[i].Index].vect.m_x;
    b=v->v[op[i].Index].vect.m_y;
    c=v->v[op[i].Index].vect.m_z;

    break;
    case VarObj::FLOAT :
    a=b=c=v->v[op[i].Index].Float;
    break;
    case VarObj::BOOL :
    a=b=c=(float)v->v[op[i].Index].Bool;
    //cout <<"A Bool Value =" <<aa <<endl;
    break;
    case VarObj::FUZZY :
    a=b=c=0;
    break;
    } 
    A.m_x=a; A.m_y=b; A.m_z=c;
//get Var 2
switch(v->v[op[i].Index2].type)
    {
    case VarObj::POINT :
    aa=v->v[op[i].Index2].Point.m_x;
    bb=v->v[op[i].Index2].Point.m_y;
    cc=v->v[op[i].Index2].Point.m_z;
    break;

    case VarObj::VECTOR :
    aa=v->v[op[i].Index2].vect.m_x;
    bb=v->v[op[i].Index2].vect.m_y;
    cc=v->v[op[i].Index2].vect.m_z;

    break;
    case VarObj::FLOAT :
    aa=bb=cc=v->v[op[i].Index2].Float;
    break;
    case VarObj::BOOL :
    aa=bb=cc=(float)v->v[op[i].Index2].Bool;
    //cout <<"AA Bool Value =" <<aa <<endl;
    break;
    case VarObj::FUZZY :
    aa=bb=cc=0;
    break;
    } 
    B.m_x=aa; B.m_y=bb; B.m_z=cc;
    //cout <<" a "<<a <<" aa "<< aa <<endl;
    
    v->v[op[i].GlobVarIndex].Float=A.dot(B);
  
  }


void OpCodeList::ProcessSetGlobalDir(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
switch(v->v[op[i].GlobVarIndex].type)
        	{
             case VarObj::POINT :
              Me->Dir.m_x=v->v[op[i].GlobVarIndex].Point.m_x;
              Me->Dir.m_y=v->v[op[i].GlobVarIndex].Point.m_y;
                Me->Dir.m_z=v->v[op[i].GlobVarIndex].Point.m_z;
                //Me->Pos.Print("ME");
             break;
             case VarObj::VECTOR :
              Me->Dir.m_x=v->v[op[i].GlobVarIndex].vect.m_x;
              Me->Dir.m_y=v->v[op[i].GlobVarIndex].vect.m_y;
                Me->Dir.m_z=v->v[op[i].GlobVarIndex].vect.m_z;
             
             break;
             case VarObj::FLOAT :
              Me->Dir.m_x=v->v[op[i].GlobVarIndex].Float;
                Me->Dir.m_y=v->v[op[i].GlobVarIndex].Float;
                Me->Dir.m_z=v->v[op[i].GlobVarIndex].Float;
             break;
             case VarObj::BOOL :
             case VarObj::FUZZY :
             break;
            } 

}

void OpCodeList::ProcessGetGlobalDir(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
         
         switch(v->v[op[i].GlobVarIndex].type)
         	{
              case VarObj::POINT :
                v->v[op[i].GlobVarIndex].Point.m_x=Me->Dir.m_x;
                v->v[op[i].GlobVarIndex].Point.m_y=Me->Dir.m_y;
                 v->v[op[i].GlobVarIndex].Point.m_z=Me->Dir.m_z;
              break;
              case VarObj::VECTOR :
                v->v[op[i].GlobVarIndex].vect.m_x=Me->Dir.m_x;
                v->v[op[i].GlobVarIndex].vect.m_y=Me->Dir.m_y;
                 v->v[op[i].GlobVarIndex].vect.m_z=Me->Dir.m_z;
              
              break;
              case VarObj::FLOAT :
                v->v[op[i].GlobVarIndex].Float=Me->Dir.m_y;
                 
              break;
              case VarObj::BOOL :
              case VarObj::FUZZY :
              break;
             } 
          

}

void OpCodeList::ProcessSetGPYlevel(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
//GLfloat ObjGroundPlane::GetHeight(GLfloat x, GLfloat z)
float X,Y,Z;         
         switch(v->v[op[i].GlobVarIndex].type)
         	{
              case VarObj::POINT :
                X=v->v[op[i].GlobVarIndex].Point.m_x;
                Z=v->v[op[i].GlobVarIndex].Point.m_z;
                v->v[op[i].GlobVarIndex].Point.m_y=Me->env->gp->GetHeight(X,Z);
                //cout <<"X "<<X <<" Y "<<Y<<endl;
              break;
              case VarObj::VECTOR :
                X=v->v[op[i].GlobVarIndex].vect.m_x;
                Y=v->v[op[i].GlobVarIndex].vect.m_z;
                v->v[op[i].GlobVarIndex].vect.m_y=Me->env->gp->GetHeight(X,Z);
              break;
              case VarObj::FLOAT :
              case VarObj::BOOL :
              case VarObj::FUZZY :
              break;
             } 


}


void OpCodeList::ProcessGetNoiseValue(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
// value to set, Noise Seed Noise type
float X=0.0; float Y=0.0; float Z=0.0;         
//int NoiseType=v->v[op[i].Index1].Float;
switch(v->v[op[i].Index].type)
         	{
              case VarObj::POINT :
                X=v->v[op[i].Index].Point.m_x;
                Y=v->v[op[i].Index].Point.m_y;
                Z=v->v[op[i].Index].Point.m_z;
                
              break;
              case VarObj::VECTOR :
                X=v->v[op[i].Index].vect.m_x;
                Y=v->v[op[i].Index].vect.m_y;
                Z=v->v[op[i].Index].vect.m_z;
                
              break;
              case VarObj::FLOAT :
              case VarObj::BOOL :
              case VarObj::FUZZY :
              break;
             } 
v->v[op[i].GlobVarIndex].vect=Me->GetNoise(X,Y,Z);

}
void OpCodeList::ProcessSetNoise(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex,int i)
{
float  Scale=v->v[op[i].GlobVarIndex].Float;
float Type=v->v[op[i].Index].Float;
Me->SetNoise(Scale,(int) Type);
}
