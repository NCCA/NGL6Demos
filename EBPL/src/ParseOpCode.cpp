#include <iostream>
#include <fstream>

using namespace std;
#include "OpCode.h"
#include "VarList.h"
#include "Brain.h"
#include "CompTokens.h"



typedef std::vector <GlobalVar> GlobList;
typedef std::vector <OpCodeList> OpList;
typedef std::vector <GlobalFunc> GFList;

extern unsigned int GlobalIndex;
extern VarList Globals;

extern GlobList GlobalList;
extern GFList GlobalFuncList;
extern OpList Opcodes;
//extern int FunctionIndex;
//extern int GFunctionIndex;
extern ifstream stream;
extern int line;


void SetOpcodeVarIndex(const char *buffer,unsigned int &i, OpCode *op,int Witch)
{
string Tempstr="";
unsigned int blength=strlen(buffer);
//cout <<" Buffer is "<<buffer<<endl;
while(isspace(buffer[i]) )
		i++; 
    while(buffer[i]!='\n' && buffer[i]!=';' && buffer[i]!=' ' &&i<blength)
    	{
        Tempstr+=buffer[i++];
        }
        
    if(CheckIfVar(Tempstr)==true)
    	 {
         if(Witch==0)
         	op->GlobVarIndex = FindGlobalVar(Tempstr);
         else if(Witch ==1)
         	op->Index = FindGlobalVar(Tempstr);
         else if(Witch ==2)
         	op->Index2 = FindGlobalVar(Tempstr);
         else if(Witch ==3)
         	op->Index3 = FindGlobalVar(Tempstr);
         else if(Witch ==4)
         	op->Index4 = FindGlobalVar(Tempstr);
          else if(Witch ==5)
         	op->Index5 = FindGlobalVar(Tempstr);
         else if(Witch ==6)
         	op->Index6 = FindGlobalVar(Tempstr);
        
         else { cout << "Must be either 0 , 1 , 2 3 4 5 6"<<endl; exit(0);}
         
         }
    else {
    	  cout <<"error var not know line "<<line<<" "<< Tempstr << endl; exit(0);
    	 }

}

bool VarOrFloat(const char *buffer, int i)
{
while(isspace(buffer[i]) )
		i++; 
if(isdigit(buffer[i]))
	return true;
else return false;
}

void SetFloatValues(const char *buffer,unsigned int &i, OpCode *op)
{
    VarObj::VARTYPE type=Globals.v[op->GlobVarIndex].type;
    char num[20];
    int p=0;
    switch (type)
    	{ 
        case VarObj::BOOL: break;
    	case VarObj::POINT :
        case VarObj::VECTOR :        
        
        p=0;
        // remove blank space	
        while(isspace(buffer[i]) )
				i++;
        while(!isspace(buffer[i]))
        	num[p++]=buffer[i++];
        num[p]='\0';
        op->Floats[0]=atof(num);
        p=0;
        while(isspace(buffer[i]) )
				i++;
        while(!isspace(buffer[i]))
        	num[p++]=buffer[i++];
        num[p]='\0';
        op->Floats[1]=atof(num);
        p=0;
        while(isspace(buffer[i]) )
				i++;
        while(buffer[i]!='\n' && buffer[i]!=';' && buffer[i]!=' ')
        //while(buffer[i]!=';')
        	num[p++]=buffer[i++];
        num[p]='\0';
        op->Floats[2]=atof(num);
        break;
        case VarObj::FLOAT :
    	case VarObj::FUZZY :
        p=0;
        // remove blank space	
        while(isspace(buffer[i]) )
				i++;
        //while(buffer[i]!=';')
        while(buffer[i]!='\n' && buffer[i]!=';' && buffer[i]!=' ')
        	num[p++]=buffer[i++];
        num[p]='\0';
        op->Floats[0]=atof(num);
	    break;
    	}//end switch
}

float GetFloat(const char *buffer,unsigned int &i)
{
 char num[20];
 int p=0;
 unsigned int blength=strlen(buffer);
 // remove leading spaces
 while(buffer[i]==' ' ) //&& buffer[i]==',' ) 
        i++; 
// now get the Float value      
  while(!isspace(buffer[i]) && i <blength)
      	num[p++]=buffer[i++];
 num[p]='\0';
return atof(num);
 
}

void ParseOpCodes(char *buffer,OpCodeList *o)
{
char temp[500];
unsigned int i=0;
int p=0;
string Tempstr="";
// copy the first word of the line upto the 1st space
//if(buffer[0]==' ' )|| buffer[0]=='\t')
	//{
for(i=0; (buffer[i] !=' ' && buffer[i]!=';') && i<strlen(buffer); i++)
	temp[p++]=buffer[i];
temp[p]='\0';

cout <<"Opcode Keyword "<<temp<<endl;
if (strcasecmp(temp,"PushMatrix")==0)
	{
    OpCode op(OpCode::PUSHMATRIX);
    o->Add(op);
    cout << "adding PushMAtrix"<<endl;
    }

else if (strcasecmp(temp,"PopMatrix")==0)
	{
    OpCode op(OpCode::POPMATRIX);
    o->Add(op);
    cout << "adding PopMatrix"<<endl;
    
    }

else if (strcasecmp(temp,"EnableLights")==0)
	{
    OpCode op(OpCode::ENABLELIGHTS);
    o->Add(op);
    cout << "adding enable light"<<endl;
    
    }
else if (strcasecmp(temp,"DisableLights")==0)
	{
    OpCode op(OpCode::DISABLELIGHTS);
    o->Add(op);
    cout << "adding Disable light"<<endl;
    
    }

else if (strcasecmp(temp,"SetGlobalPos")==0)
	{
    OpCode op(OpCode::SETGLOBALPOS);
    SetOpcodeVarIndex(buffer,i,&op,0);
    o->Add(op);
    cout << "adding SetGlobalPos"<<endl;
    }

else if (strcasecmp(temp,"GetGlobalPos")==0)
	{
    OpCode op(OpCode::GETGLOBALPOS);
    SetOpcodeVarIndex(buffer,i,&op,0);
    o->Add(op);
    cout << "adding SetGlobalPos"<<endl;
    }

else if (strcasecmp(temp,"SetGlobalDir")==0)
	{
    OpCode op(OpCode::SETGLOBALDIR);
    SetOpcodeVarIndex(buffer,i,&op,0);
    o->Add(op);
    cout << "adding SetGlobalPos"<<endl;
    }

else if (strcasecmp(temp,"GetGlobalDir")==0)
	{
    OpCode op(OpCode::GETGLOBALDIR);
    SetOpcodeVarIndex(buffer,i,&op,0);
    o->Add(op);
    cout << "adding SetGlobalPos"<<endl;
    }
    
    
    
else if (strcasecmp(temp,"SetGlobalCollideFlag")==0)
	{
    OpCode op(OpCode::SETGLOBALCOLLIDEFLAG);
    SetOpcodeVarIndex(buffer,i,&op,0);
    o->Add(op);
    cout << "adding SetGlobalCollideFlag"<<endl;
    }

else if (strcasecmp(temp,"GetGlobalCollideFlag")==0)
	{
    OpCode op(OpCode::GETGLOBALCOLLIDEFLAG);
    SetOpcodeVarIndex(buffer,i,&op,0);
    o->Add(op);
    cout << "adding GetGlobalCollideFlag"<<endl;
    }
else if (strcasecmp(temp,"Smooth")==0)
	{
    OpCode op(OpCode::SMOOTH);
    o->Add(op);
    cout << "adding smooth"<<endl;
    }

else if (strcasecmp(temp,"Flat")==0)
	{
    OpCode op(OpCode::FLAT);
    o->Add(op);
    cout << "adding Flat"<<endl;
    }


else if (strcasecmp(temp,"SetGlobalCentroid")==0)
	{
    OpCode op(OpCode::SETGLOBALCENTROID);
    SetOpcodeVarIndex(buffer,i,&op,0);
    o->Add(op);
    cout << "adding SetGlobalCentroid"<<endl;
    }

else if (strcasecmp(temp,"GetGlobalCentroid")==0)
	{
    OpCode op(OpCode::GETGLOBALCENTROID);
    SetOpcodeVarIndex(buffer,i,&op,0);
    o->Add(op);
    cout << "adding getGlobalCentroid"<<endl;
    }

else if (strcasecmp(temp,"Points")==0)
	{
    OpCode op(OpCode::POINT);
    o->Add(op);
    cout << "adding Ponts"<<endl;
    
    }

else if (strcasecmp(temp,"Translate")==0)
	{
    cout <<"Doing Translate"<<endl;
    OpCode op(OpCode::TRANSLATE);
    SetOpcodeVarIndex(buffer,i,&op,0);
    o->Add(op);
    cout << "adding Translate"<<endl;
 
    }
else if (strcasecmp(temp,"RotateX")==0)
	{
    OpCode op(OpCode::ROTATEX);
    if(VarOrFloat(buffer,i)==false)
        SetOpcodeVarIndex(buffer,i,&op,0);
	else
    	{
        op.GlobVarIndex = OpCode::GuardValue;
        op.Floats[0]=GetFloat(buffer,i);    
        }
    //SetOpcodeVarIndex(buffer,i,&op,0);
    o->Add(op);
    cout << "adding Rotate"<<endl;
    
    }
else if (strcasecmp(temp,"RotateY")==0)
	{
    OpCode op(OpCode::ROTATEY);
    //SetOpcodeVarIndex(buffer,i,&op,0);
    if(VarOrFloat(buffer,i)==false)
        SetOpcodeVarIndex(buffer,i,&op,0);
	else
    	{
        op.GlobVarIndex = OpCode::GuardValue;
        op.Floats[0]=GetFloat(buffer,i);    
        }
    o->Add(op);
    cout << "adding Rotatey"<<endl;
    
    }
else if (strcasecmp(temp,"RotateZ")==0)
	{
    OpCode op(OpCode::ROTATEZ);
    if(VarOrFloat(buffer,i)==false)
        SetOpcodeVarIndex(buffer,i,&op,0);
	else
    	{
        op.GlobVarIndex = OpCode::GuardValue;
        op.Floats[0]=GetFloat(buffer,i);    
        }
    //SetOpcodeVarIndex(buffer,i,&op,0);
    o->Add(op);
    cout << "adding RotateZ"<<endl;
    
    }
else if (strcasecmp(temp,"Vertex")==0)
	{
    
    OpCode op(OpCode::VERTEX);
    SetOpcodeVarIndex(buffer,i,&op,0);
	o->Add(op);
    cout << "adding Vertex"<<endl;
    }
else if (strcasecmp(temp,"Vertexf")==0)
	{
    
    OpCode op(OpCode::VERTEXF);
    i=7;
	op.Floats[0]=GetFloat(buffer,i);    
    op.Floats[1]=GetFloat(buffer,i);    
    op.Floats[2]=GetFloat(buffer,i);    
    o->Add(op);
    cout << "adding Vertex"<<endl;
    
    }

else if (strcasecmp(temp,"Sphere")==0)
	{
    i=7;
    OpCode op(OpCode::SPHERE);
    if(VarOrFloat(buffer,i)==false)
        SetOpcodeVarIndex(buffer,i,&op,0);
	else
    	{
        op.GlobVarIndex = OpCode::GuardValue;
        op.Floats[0]=GetFloat(buffer,i);    
        }
    op.Floats[1]=GetFloat(buffer,i);    
    op.Floats[2]=GetFloat(buffer,i);    
    o->Add(op);
    cout << "adding Sphere"<<endl;
    
    }
else if (strcasecmp(temp,"Cube")==0)
	{
    i=5;
    OpCode op(OpCode::CUBE);
    if(VarOrFloat(buffer,i)==false)
        SetOpcodeVarIndex(buffer,i,&op,0);
	else
    	{
        op.GlobVarIndex = OpCode::GuardValue;
        op.Floats[0]=GetFloat(buffer,i);    
        }
    o->Add(op);
    cout << "adding Cube"<<endl;
    
    }
else if (strcasecmp(temp,"RenderMaterial")==0)
	{
    i=14;
    OpCode op(OpCode::RENDERMATERIAL);
    if(VarOrFloat(buffer,i)==false)
        SetOpcodeVarIndex(buffer,i,&op,1);
	else
    	{
        op.GlobVarIndex = OpCode::GuardValue;
        op.Floats[0]=GetFloat(buffer,i);    
        }
    o->Add(op);
    cout << "adding Render material"<<endl;
    
    }

else if (strcasecmp(temp,"SolidSphere")==0)
	{
    i=11;
    OpCode op(OpCode::SOLIDSPHERE);
    if(VarOrFloat(buffer,i)==false)
        SetOpcodeVarIndex(buffer,i,&op,0);
	else
    	{
        op.GlobVarIndex = OpCode::GuardValue;
        op.Floats[0]=GetFloat(buffer,i);    
        }
    op.Floats[1]=GetFloat(buffer,i);    
    op.Floats[2]=GetFloat(buffer,i);    
    o->Add(op);
    cout << "adding SolidSphere"<<endl;
    
    }

else if (strcasecmp(temp,"Cylinder")==0)
	{
    i=9;
    OpCode op(OpCode::CYLINDER);
    if(VarOrFloat(buffer,i)==false)
        SetOpcodeVarIndex(buffer,i,&op,0);
	else
    	{
        op.GlobVarIndex = OpCode::GuardValue;
        op.Floats[0]=GetFloat(buffer,i);    
        }
    if(VarOrFloat(buffer,i)==false)
        SetOpcodeVarIndex(buffer,i,&op,1);
	else
    	{
        op.GlobVarIndex = OpCode::GuardValue;
        op.Floats[0]=GetFloat(buffer,i);    
        }
    
    op.Floats[1]=GetFloat(buffer,i);    
    op.Floats[2]=GetFloat(buffer,i);    
    o->Add(op);
    cout << "adding Cylinder"<<endl;
    
    }

else if (strcasecmp(temp,"addD")==0 || strcasecmp(temp,"SetD")==0
		 || strcasecmp(temp,"subD")==0 || strcasecmp(temp,"DivD")==0 
         || strcasecmp(temp,"MulD")==0 )
	{
    OpCode op;
    if (strcasecmp(temp,"addD")==0)
    	op.type=OpCode::ADDDIR;
    else if (strcasecmp(temp,"setD")==0)
    	op.type=OpCode::SETDIR;
	else if (strcasecmp(temp,"subD")==0)
    	op.type=OpCode::SUBDIR;        
    else if (strcasecmp(temp,"DivD")==0)
    	op.type=OpCode::DIVDIR;        
    else if (strcasecmp(temp,"MulD")==0)
    	op.type=OpCode::MULDIR;        
    
    SetOpcodeVarIndex(buffer,i,&op,0);
    //now find what type it is and then get the correct number of args
    SetFloatValues(buffer,i,&op);
    o->Add(op);
    cout << "adding addD etc"<<endl;
    
    }

else if (strcasecmp(temp,"Randomize")==0)
	{
    
    OpCode op(OpCode::RANDOMIZE);
    SetOpcodeVarIndex(buffer,i,&op,0);
    //now find what type it is and then get the correct number of args
    
    if(VarOrFloat(buffer,i)==false)
        {
        cout <<"using a variable value"<<endl;
        
        SetOpcodeVarIndex(buffer,i,&op,1);
        }
	else
    	{
        cout <<"using direct floats"<<endl;
        op.Index2 = OpCode::GuardValue;
        SetFloatValues(buffer,i,&op);
        //op.Floats[0]=GetFloat(buffer,i);    
        }
    
    
    o->Add(op);
    }
else if (strcasecmp(temp,"RandomizePos")==0)
	{
    
    OpCode op(OpCode::RANDOMIZEPOS);
    SetOpcodeVarIndex(buffer,i,&op,0);
    if(VarOrFloat(buffer,i)==false)
    {
    SetOpcodeVarIndex(buffer,i,&op,1);
    }
    else
    {
    //now find what type it is and then get the correct number of args
    op.Index2 = OpCode::GuardValue;
    SetFloatValues(buffer,i,&op);
    }
    o->Add(op);
    }

    
else if (strcasecmp(temp,"SetGPYlevel")==0)
	{
    OpCode op(OpCode::SETGPYLEVEL);
    SetOpcodeVarIndex(buffer,i,&op,0);
    //now find what type it is and then get the correct number of args
    o->Add(op);
    }
    
    
        
else if (strcasecmp(temp,"Normalize")==0)
	{
    
    OpCode op(OpCode::NORMALIZE);
    SetOpcodeVarIndex(buffer,i,&op,0);
    //now find what type it is and then get the correct number of args
    o->Add(op);
    cout <<" adding Normalize"<<endl;
    }
    
        
else if (strcasecmp(temp,"Colour")==0)
	{
    
    //now find what type it is and then get the correct number of args
    // skip the Colour keyword
    i=6;
    OpCode op(OpCode::COLOUR);
    if(VarOrFloat(buffer,i)==false)
    {
        
        SetOpcodeVarIndex(buffer,i,&op,1);
    }
	else
    	{
        op.GlobVarIndex = OpCode::GuardValue;
        op.Floats[0]=GetFloat(buffer,i);
    	op.Floats[1]=GetFloat(buffer,i);
    	op.Floats[2]=GetFloat(buffer,i);
    
        }
    	o->Add(op);
    }

else if (strcasecmp(temp,"PointSize")==0)
	{
    OpCode op(OpCode::POINTSIZE);
    i=10;
    op.Floats[0]=GetFloat(buffer,i);
    //now find what type it is and then get the correct number of args
    o->Add(op);
    }
    
else if (strcasecmp(temp,"LineSize")==0)
	{
    OpCode op(OpCode::LINESIZE);
	op.Floats[0]=GetFloat(buffer,i);    
    o->Add(op);
    }
else if (strcasecmp(temp,"Scale")==0)
	{
    OpCode op(OpCode::SCALE);
	op.Floats[0]=GetFloat(buffer,i);    
    op.Floats[1]=GetFloat(buffer,i);    
    op.Floats[2]=GetFloat(buffer,i);    
    o->Add(op);
    }
    
else if (strcasecmp(temp,"Polygon")==0)
	{
    OpCode op(OpCode::POLYGON);
    o->Add(op);
    cout << "adding Polygon"<<endl;
    }
else if (strcasecmp(temp,"LineLoop")==0)
	{
    OpCode op(OpCode::LINELOOP);
    o->Add(op);
    cout << "adding LineLoop"<<endl;
    }
else if (strcasecmp(temp,"Lines")==0)
	{
    OpCode op(OpCode::LINES);
    o->Add(op);
    cout << "adding Lines"<<endl;
    }

else if (strcasecmp(temp,"Quad")==0)
	{
    OpCode op(OpCode::QUAD);
    o->Add(op);
    cout << "adding Quad"<<endl;
    }
else if (strcasecmp(temp,"glEnd")==0)
	{
    OpCode op(OpCode::GLEND);
    o->Add(op);
    cout << "adding glEnd"<<endl;
    }

else if (strcasecmp(temp,"Add")==0 || strcasecmp(temp,"Sub")==0  
			|| (strcasecmp(temp,"Mul")==0) || (strcasecmp(temp,"Set")==0)
            || (strcasecmp(temp,"Div")==0)
            )
	{
    OpCode Op;
    if (strcasecmp(temp,"Add")==0 )
    	Op.SetType(OpCode::ADD);
    else if (strcasecmp(temp,"Sub")==0 )
    	Op.SetType(OpCode::SUB);
    else if (strcasecmp(temp,"Mul")==0 )
    	Op.SetType(OpCode::MUL);
    else if (strcasecmp(temp,"Set")==0 )
    	Op.SetType(OpCode::SET);
    else if (strcasecmp(temp,"Div")==0 )
    	Op.SetType(OpCode::SET);
    
	SetOpcodeVarIndex(buffer,i,&Op,0);    
    SetOpcodeVarIndex(buffer,i,&Op,1);    
    o->Add(Op);
    cout << "adding Add"<<endl;
    
    }
else if (strcasecmp(temp,"Fpushd")==0)
	{
    OpCode op(OpCode::FPUSHD);
    op.Floats[0]=GetFloat(buffer,i);    
    o->Add(op);
    cout << "adding FpushD"<<endl;
    }
else if (strcasecmp(temp,"Fpush")==0)
	{
    
    OpCode op(OpCode::FPUSH);
    SetOpcodeVarIndex(buffer,i,&op,0);    
    //now find what type it is and then get the correct number of args
    VarObj::VARTYPE type=Globals.v[op.GlobVarIndex].type;
    char num[20];
    switch (type)
    	{
    	case VarObj::POINT :
        case VarObj::VECTOR :        
        
        p=0;
        // remove blank space	
        while(isspace(buffer[i]) )
				i++;
        while(!isspace(buffer[i]))
        	num[p++]=buffer[i++];
        num[p]='\0';
        if(num[0]=='x' || num[0]== 'X')
        	op.Index=0;
        else if(num[0]=='y' || num[0]== 'Y')
        	op.Index=1;
		else if(num[0]=='z' || num[0]== 'Z')        	
            op.Index=2;
        else
        	{
            cout <<"error push index is either x,y or z"<<endl;
            exit(1);
            }
        //
        
        case VarObj::FLOAT :
    	case VarObj::FUZZY :
        case VarObj::BOOL :
        break;
    	}//end switch
   
    o->Add(op);
    cout << "adding Fpush"<<endl;
    
    }

else if (strcasecmp(temp,"Fpop")==0)
	{
    
    OpCode op(OpCode::FPOP);
    SetOpcodeVarIndex(buffer,i,&op,0);    
    //now find what type it is and then get the correct number of args
    VarObj::VARTYPE type=Globals.v[op.GlobVarIndex].type;
    char num[20];
    switch (type)
    	{
    	case VarObj::POINT :
        case VarObj::VECTOR :        
        
        p=0;
        // remove blank space	
        while(isspace(buffer[i]) )
				i++;
        while(!isspace(buffer[i]))
        	num[p++]=buffer[i++];
        num[p]='\0';
        if(num[0]=='x' || num[0]== 'X')
        	op.Index=0;
        else if(num[0]=='y' || num[0]== 'Y')
        	op.Index=1;
		else if(num[0]=='z' || num[0]== 'Z')        	
            op.Index=2;
        else
        	{
            cout <<"error push index is either x,y or z"<<endl;
            exit(1);
            }
        //
        
        case VarObj::FLOAT :
    	case VarObj::FUZZY :
        case VarObj::BOOL :
        break;
    	}//end switch
   
    o->Add(op);
    cout << "adding Fpop"<<endl;
    
    }
    
    
        
    else if (strcasecmp(temp,"Fatan")==0)
	{
    OpCode op(OpCode::FATAN);
    o->Add(op);
    cout << "adding Fatan"<<endl;
    
    }
    else if (strcasecmp(temp,"Fdup")==0)
	{
    OpCode op(OpCode::FDUP);
    o->Add(op);
    cout << "adding Fatan"<<endl;
    
    }
    else if (strcasecmp(temp,"Fcos")==0)
	{
    OpCode op(OpCode::FCOS);
    o->Add(op);
    cout << "adding Fatan"<<endl;
    }
    else if (strcasecmp(temp,"FACOS")==0)
	{
    OpCode op(OpCode::FACOS);
    o->Add(op);
    cout << "adding Fatan"<<endl;
    }
    else if (strcasecmp(temp,"FSQRT")==0)
	{
    OpCode op(OpCode::FSQRT);
    o->Add(op);
    cout << "adding fsqrt"<<endl;
    }
    else if (strcasecmp(temp,"FSin")==0)
	{
    OpCode op(OpCode::FSIN);
    o->Add(op);
    cout << "adding Fsin"<<endl;
    }
    else if (strcasecmp(temp,"FAsin")==0)
	{
    OpCode op(OpCode::FASIN);
    o->Add(op);
    cout << "adding Fasin"<<endl;
    }
    else if (strcasecmp(temp,"FDiv")==0)
	{
    OpCode op(OpCode::FDIV);
    o->Add(op);
    cout << "adding Fasin"<<endl;
    }
    else if (strcasecmp(temp,"FAdd")==0)
	{
    OpCode op(OpCode::FADD);
    o->Add(op);
    cout << "adding Fasin"<<endl;
    }
    else if (strcasecmp(temp,"FSub")==0)
	{
    OpCode op(OpCode::FSUB);
    o->Add(op);
    cout << "adding Fasin"<<endl;
    }
    else if (strcasecmp(temp,"FMul")==0)
	{
    OpCode op(OpCode::FMUL);
    o->Add(op);
    cout << "adding Fasin"<<endl;
    }
	else if (strcasecmp(temp,"Frad2deg")==0)
	{
    OpCode op(OpCode::FRAD2DEG);
    
    o->Add(op);
    cout << "adding Frad2deg"<<endl;
    }
    else if (strcasecmp(temp,"Fnegate")==0)
	{
    OpCode op(OpCode::FNEGATE);
    
    o->Add(op);
    cout << "adding Fnegate"<<endl;
    
    }
    else if (strcasecmp(temp,"FstackTrace")==0)
	{
    OpCode op(OpCode::FSTACKTRACE);
    
    o->Add(op);
    cout << "adding Fstack trace"<<endl;
    
    }
    else if (strcasecmp(temp,"Beep")==0)
	{
    OpCode op(OpCode::BEEP);
    
    o->Add(op);
    cout << "adding Beep"<<endl;
    
    }
else if (strcasecmp(temp,"Length")==0)
	{
    
    OpCode op(OpCode::LENGTH);
    SetOpcodeVarIndex(buffer,i,&op,0);    
    o->Add(op);
    cout << "adding Length"<<endl;
    
    }
    
else if (strcasecmp(temp,"Reverse")==0)
	{
    
    OpCode op(OpCode::REVERSE);
    SetOpcodeVarIndex(buffer,i,&op,0);    
    o->Add(op);
    cout << "adding Reverse"<<endl;
    
    }
    
        
else if (strcasecmp(temp,"Debug")==0)
	{
    OpCode op(OpCode::DEBUG);
    SetOpcodeVarIndex(buffer,i,&op,0);    
    o->Add(op);
    cout << "adding debug"<<endl;
    
    }
else if (strcasecmp(temp,"Dot")==0)
	{
    OpCode op(OpCode::DOT);
    SetOpcodeVarIndex(buffer,i,&op,0);    
    SetOpcodeVarIndex(buffer,i,&op,1);    
    SetOpcodeVarIndex(buffer,i,&op,2);    
    o->Add(op);
    cout << "adding dot"<<endl;
    
    }


else if (strcasecmp(temp,"UseAgentRender")==0)
	{
    OpCode op(OpCode::USEAGENTRENDER);
    o->Add(op);
    cout << "adding use agent render"<<endl;
    
    }
else if (strcasecmp(temp,"PushGPYlevel")==0)
	{
    OpCode op(OpCode::PUSHGPYLEVEL);
    o->Add(op);
    cout << "adding GPYlevel"<<endl;
    
    }

else if (strcasecmp(temp,"RenderAgent")==0)
	{
    OpCode op(OpCode::RENDERAGENT);
    o->Add(op);
    cout << "adding use agent render"<<endl;
    
    }

else if (strcasecmp(temp,"SetAnimCycle")==0)
	{
    OpCode op(OpCode::SETANIMCYCLE);
    SetOpcodeVarIndex(buffer,i,&op,0);    
    o->Add(op);
    cout << "adding use set anim cycle"<<endl;
    
    }

else if (strcasecmp(temp,"Call")==0)
	{
    OpCode op(OpCode::CALL);
    while(isspace(buffer[i]) )
		i++;
    while(buffer[i]!='\n' && buffer[i]!=';' && buffer[i]!=' ')
    	{
        Tempstr+=buffer[i++];
        }
        
    if(CheckIfFunction(Tempstr)==true)
    	 {
         op.GlobVarIndex = FindFunction(Tempstr);
         }
    else {
    	  cout <<"error Function not know line "<<line<<" "<< Tempstr << endl; exit(0);
    	 }
    o->Add(op);
    cout << "adding CALL"<<endl;
    
    }

else if (strcasecmp(temp,"CallList")==0)
	{
    OpCode op(OpCode::CALLLIST);
    while(isspace(buffer[i]) )
		i++;
    while(buffer[i]!='\n' && buffer[i]!=';' && buffer[i]!=' ')
    	{
        Tempstr+=buffer[i++];
        }
        
    if(CheckIfCallList(Tempstr)==true)
    	 {
         op.Floats[0] = FindCallList(Tempstr);
         }
    else {
    	  cout <<"error CallList not know line "<<line<<" "<< Tempstr << endl; exit(0);
    	 }
    
    SetOpcodeVarIndex(buffer,i,&op,1);
    o->Add(op);
    cout << "adding CALLLIST"<<endl;
    
    }




else if(strcasecmp(temp,"if")==0)
{
unsigned int FilePos=stream.tellg();
int RelOffset=0;
int CloseCount=1;
char lnbuffer[BUFFSIZE];
char inbuff[BUFFSIZE];

// if VAR1 [ token ] VAR 2
OpCode op(OpCode::IF);
// get var 1
SetOpcodeVarIndex(buffer,i,&op,0);    
// get evaluator
while(isspace(buffer[i]) )
		i++;
Tempstr="";		
while(buffer[i]!='\n' && buffer[i]!=';' && buffer[i]!=' ')
    	{
        Tempstr+=buffer[i++];
        
        }
if(Tempstr == "==")
	op.ifeval=OpCode::IFEQUAL;
else if(Tempstr == "!=")
	op.ifeval=OpCode::IFNOTEQUAL;
else if(Tempstr == ">")
	op.ifeval=OpCode::IFGREATER;
else if(Tempstr == ">=")
	op.ifeval=OpCode::IFGREATEREQ;
else if(Tempstr == "<")
	op.ifeval=OpCode::IFLESS;
else if(Tempstr == "<=")
	op.ifeval=OpCode::IFLESSEQ;
else {
	cout << "Evaluator "<<Tempstr <<"Not recognised in if statement Line"<<line<<endl;
	exit(1);
	}
// get var 2
SetOpcodeVarIndex(buffer,i,&op,1);    
// save the file position 
stream.getline(lnbuffer,BUFFSIZE);
line=atoi(lnbuffer); // incriment the line number of the file
stream.getline(inbuff,BUFFSIZE);
if(strncmp(inbuff,"{",1)!=0)		
	{
    cout <<"if must be followed by a {  on line "<<line<<endl;
    exit(0);
    }
// search to find the end of the IF statement and make this
// a relative offset to the current IF position
//int OpenCount=0;

do 
{
stream.getline(lnbuffer,BUFFSIZE);
stream.getline(inbuff,BUFFSIZE);
if(inbuff[0]=='{')
	{CloseCount++; /*RelOffset--;*/ cout <<"found { "<<endl;}
if(inbuff[0]=='}')
	{CloseCount--;  /*RelOffset--;*/ cout <<"found } "<<endl;}
RelOffset++;
}while(CloseCount >0); //(strncmp(inbuff,"}",1)!=0);
op.Index2=RelOffset+1;         
cout <<"Rel Index "<<op.Index2<<endl;
// now find 2nd closed }
o->Add(op);
stream.seekg(FilePos);
}


else if(strcasecmp(temp,"ifelse")==0)
{
unsigned int FilePos=stream.tellg();
int RelOffset=0;
int CloseCount=1;
char lnbuffer[BUFFSIZE];
char inbuff[BUFFSIZE];

// if VAR1 [ token ] VAR 2
OpCode op(OpCode::IFELSE);
// get var 1
SetOpcodeVarIndex(buffer,i,&op,0);    
// get evaluator
while(isspace(buffer[i]) )
		i++;
Tempstr="";		
while(buffer[i]!='\n' && buffer[i]!=';' && buffer[i]!=' ')
    	{
        Tempstr+=buffer[i++];
        
        }
if(Tempstr == "==")
	op.ifeval=OpCode::IFEQUAL;
else if(Tempstr == "!=")
	op.ifeval=OpCode::IFNOTEQUAL;
else if(Tempstr == ">")
	op.ifeval=OpCode::IFGREATER;
else if(Tempstr == ">=")
	op.ifeval=OpCode::IFGREATEREQ;
else if(Tempstr == "<")
	op.ifeval=OpCode::IFLESS;
else if(Tempstr == "<=")
	op.ifeval=OpCode::IFLESSEQ;
else {
	cout << "Evaluator "<<Tempstr <<"Not recognised in if statement Line"<<line<<endl;
	exit(1);
	}
// get var 2
SetOpcodeVarIndex(buffer,i,&op,1);    
// save the file position 
stream.getline(lnbuffer,BUFFSIZE);
line=atoi(lnbuffer); // incriment the line number of the file
stream.getline(inbuff,BUFFSIZE);
if(strncmp(inbuff,"{",1)!=0)		
	{
    cout <<"if must be followed by a {  on line "<<line<<endl;
    exit(0);
    }
// search to find the end of the IF statement and make this
// a relative offset to the current IF position
//int OpenCount=0;

do 
{
stream.getline(lnbuffer,BUFFSIZE);
stream.getline(inbuff,BUFFSIZE);
if(inbuff[0]=='{')
	{CloseCount++; /*RelOffset--;*/ cout <<"found { "<<endl;}
if(inbuff[0]=='}')
	{CloseCount--;  /*RelOffset--;*/ cout <<"found } "<<endl;}
RelOffset++;
}while(CloseCount >0); //(strncmp(inbuff,"}",1)!=0);
op.Index2=RelOffset+1;         
cout <<"Rel Offset "<<op.Index2<<endl;
//CloseCount=1;
//RelOffset+=2;
do 
{
stream.getline(lnbuffer,BUFFSIZE);
stream.getline(inbuff,BUFFSIZE);
if(inbuff[0]=='{')
	{CloseCount++; /*RelOffset--;*/ cout <<"found { "<<endl;}
if(inbuff[0]=='}')
	{CloseCount--;  /*RelOffset--;*/ cout <<"found } "<<endl;}
RelOffset++;
}while(CloseCount >0); //(strncmp(inbuff,"}",1)!=0);
op.Index3=RelOffset+1;         //was +2

cout <<"Rel Offset 2nd "<<op.Index3<<endl;


o->Add(op);
stream.seekg(FilePos);
}






else if(strcasecmp(temp,"loopbin")==0)
{
unsigned int FilePos=stream.tellg();
int RelOffset=0;
int CloseCount=1;
char lnbuffer[BUFFSIZE];
char inbuff[BUFFSIZE];

// if VAR1 [ token ] VAR 2
OpCode op(OpCode::LOOPBIN);
/*stream.getline(lnbuffer,BUFFSIZE);
line=atoi(lnbuffer); // incriment the line number of the file
stream.getline(inbuff,BUFFSIZE);*/
/*if(strncmp(inbuff,"{",1)!=0)		
	{
    cout <<"if must be followed by a {  on line "<<line<<endl;
    exit(0);
    }*/
// search to find the end of the IF statement and make this
// a relative offset to the current IF position
//int OpenCount=0;
do 
{
stream.getline(lnbuffer,BUFFSIZE);
stream.getline(inbuff,BUFFSIZE);
/*if(inbuff[0]=='{')
	{CloseCount++; RelOffset--; cout <<"found { "<<endl;}
if(inbuff[0]=='}')
	{CloseCount--;  RelOffset--; cout <<"found } "<<endl;}*/
RelOffset++;
cout <<"InBUFF is "<<inbuff<<endl;
}while(strncasecmp(inbuff,"LoopbinEnd",10));
//(CloseCount >0); //(strncmp(inbuff,"}",1)!=0);
op.Index=RelOffset-1;         
cout <<"Loopbin Rel Index "<<op.Index<<endl;

/*do 
{
stream.getline(lnbuffer,BUFFSIZE);
stream.getline(inbuff,BUFFSIZE);
if(inbuff[0]=='{')
	{CloseCount++; RelOffset--; cout <<"found { "<<endl;}
if(inbuff[0]=='}')
	{CloseCount--;  RelOffset--; cout <<"found } "<<endl;}
RelOffset++;
}while(CloseCount >0); //(strncmp(inbuff,"}",1)!=0);
op.Index=RelOffset+2;         
cout <<"LoopBin Rel Offset "<<RelOffset+2<<endl;*/
o->Add(op);
cout <<"resetting file pos to "<<FilePos<<endl;
stream.seekg(FilePos);
}


else if (strcasecmp(temp,"loopbinEnd")==0)
{
cout <<"Found endLoopBin ignoring"<<endl;
}
else if (strcasecmp(temp,"SphereSphereCollision")==0)
	{
    i=22;
    OpCode op(OpCode::SPHERESPHERE);
    SetOpcodeVarIndex(buffer,i,&op,0);
	SetOpcodeVarIndex(buffer,i,&op,1);
    SetOpcodeVarIndex(buffer,i,&op,2);
    SetOpcodeVarIndex(buffer,i,&op,3);
    SetOpcodeVarIndex(buffer,i,&op,4);
    o->Add(op);
    cout << "adding SphereSphere"<<endl;
    
    }
else if (strcasecmp(temp,"CylinderCylinderCollision")==0)
	{
    i=25;
    OpCode op(OpCode::CYLINDERCYLINDER);
    SetOpcodeVarIndex(buffer,i,&op,0);
	SetOpcodeVarIndex(buffer,i,&op,1);
    SetOpcodeVarIndex(buffer,i,&op,2);
    SetOpcodeVarIndex(buffer,i,&op,3);
    SetOpcodeVarIndex(buffer,i,&op,4);
    SetOpcodeVarIndex(buffer,i,&op,5);
    SetOpcodeVarIndex(buffer,i,&op,6);
    o->Add(op);
    cout << "adding CylinderCylinder"<<endl;
    
    }

else if (strcasecmp(temp,"SpherePlaneCollision")==0)
	{
    i=20;
    OpCode op(OpCode::SPHEREPLANE);
    SetOpcodeVarIndex(buffer,i,&op,0);
	SetOpcodeVarIndex(buffer,i,&op,1);
    SetOpcodeVarIndex(buffer,i,&op,2);
    o->Add(op);
    cout << "adding SpherePlane"<<endl;
    
    }

else if (strcasecmp(temp,"SphereEnvObjCollision")==0)
	{
    i=21;
    OpCode op(OpCode::SPHEREENVOBJ);
    SetOpcodeVarIndex(buffer,i,&op,0);
	SetOpcodeVarIndex(buffer,i,&op,1);
    SetOpcodeVarIndex(buffer,i,&op,2);
    SetOpcodeVarIndex(buffer,i,&op,3);
    o->Add(op);
    cout << "adding SphereEnvObj"<<endl;
    
    }
    
else if (strcasecmp(temp,"GetNoiseValue")==0)
	{
    i=13;
    OpCode op(OpCode::GETNOISEVALUE);
    SetOpcodeVarIndex(buffer,i,&op,0);
	SetOpcodeVarIndex(buffer,i,&op,1);
    o->Add(op);
    cout << "adding GetNoiseValue"<<endl;
    
    }
else if (strcasecmp(temp,"UseNoise")==0)
	{
    i=8;
    OpCode op(OpCode::USENOISE);
    SetOpcodeVarIndex(buffer,i,&op,0);
	SetOpcodeVarIndex(buffer,i,&op,1);
    o->Add(op);
    cout << "adding Use Noise"<<endl;
    
    }
    
else if (strcasecmp(temp,"SetAgentI")==0)
	{
	i=9;
    OpCode op(OpCode::SETAGENTI);
    SetOpcodeVarIndex(buffer,i,&op,0);
	SetOpcodeVarIndex(buffer,i,&op,1);
    o->Add(op);
    cout << "adding SetAgentI"<<endl;
    }
else if (strcasecmp(temp,"GetAgentI")==0)
	{
	i=9;
    OpCode op(OpCode::GETAGENTI);
    SetOpcodeVarIndex(buffer,i,&op,0);
	SetOpcodeVarIndex(buffer,i,&op,1);
    o->Add(op);
    cout << "adding GetAgentI"<<endl;
    }
else if(strcasecmp(temp,"RenderFrame")==0)
	{
	i=11;
    OpCode op(OpCode::RENDERFRAME);
    SetOpcodeVarIndex(buffer,i,&op,0);
	o->Add(op);
    cout << "adding Render frame"<<endl;
	}
/*else if(strcasecmp(temp,"DebugString")==0)
{
string tmpstr="";
i=11;
OpCode op(OpCode::DEBUGSTRING);
   while(buffer[i]!='\n' && buffer[i]!=';' )
   {
   tmpstr+=buffer[i];
   //cout <<buffer[i]<<endl;		
   i++;
   }
tmpstr+="\n";
op.String=tmpstr;
o->Add(op);    
}*/
else if (strcasecmp(temp,"end")==0)
{
	
    OpCode op(OpCode::END);
    o->Add(op);
    cout << "adding End"<<endl;
}

else if (strcasecmp(temp,"DebugOpOn")==0)
{
	
    OpCode op(OpCode::DEBUGOPON);
    o->Add(op);
    cout << "adding DebugOpOn"<<endl;
}

else if (strcasecmp(temp,"DebugOpOff")==0)
{
	
    OpCode op(OpCode::DEBUGOPOFF);
    o->Add(op);
    cout << "adding DebugOpOff"<<endl;
}

else if (strncmp(temp,"{",1)==0)
{
    OpCode op(OpCode::OPENBRACE);
    o->Add(op);
    cout << "adding {"<<endl;
}
else if (strncmp(temp,"}",1)==0)
{
	OpCode op(OpCode::CLOSEBRACE);
    o->Add(op);
    cout << "adding }"<<endl;
}
else {cout << "unknown opcode "<<buffer<<endl; exit(1);}
}


