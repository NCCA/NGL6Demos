#include <iostream>
#include <fstream>
using namespace std;

#include "OpCode.h"
#include "VarList.h"
#include "Brain.h"
#include "CompTokens.h"


using namespace ngl;

typedef std::vector <GlobalVar> GlobList;
typedef std::vector <OpCodeList> OpList;
typedef std::vector <GlobalFunc> GFList;

extern unsigned int GlobalIndex;
extern VarList Globals;

extern GlobList GlobalList;
extern GFList GlobalFuncList;
extern OpList Opcodes;
extern int FunctionIndex;
extern int GFunctionIndex;
extern ifstream stream;
extern int line;


void ParseFloat(char *buffer)
{
string VarName="";
char val[30];
float vf;
// skip float 
int i=6;
int s=0;
while(buffer[i]!= '=')
	VarName+=buffer[i++];
// add to the global list
GlobalVar v(VarName,GlobalIndex,line);
GlobalList.push_back(v);
CheckForDuplicateVarNames();
GlobalIndex++;
// now get the variable value up to the ; token
// skip = sign
i++;
while(buffer[i]!= ';')
	val[s++]=buffer[i++];
vf=atof(val);
VarObj Vo(vf);
Globals.Add(Vo);
}

void ParseBool(char *buffer)
{
string VarName="";
// skip boool 
int i=5;
while(buffer[i]!= '=')
	VarName+=buffer[i++];
// add to the global list
GlobalVar v(VarName,GlobalIndex,line);
GlobalList.push_back(v);
CheckForDuplicateVarNames();
GlobalIndex++;
// now get the variable value up to the ; token
// skip = sign
i++;
VarName="";
while(buffer[i]!= ';')
	VarName+=buffer[i++];
bool vf;
if(VarName == "true")
	vf=true;
else if(VarName == "false")
	vf=false;
else
	{
	cout <<"***************ERROR IN COMPILATION**********************"<<endl;
	cout <<" Error in compilation line :"<<line<<endl;
	cout <<" bool data type must be initialised as either true or false "<<endl;
	cout << "****************** COMPILATION FAILED EXITING ***********"<<endl;
	exit(0);
	}
VarObj Vo(vf);
Globals.Add(Vo);
}








void ParseFuzzy(char *buffer)
{
string VarName="";
char val[30];
fuzzy vf;
// skip float 
int i=6;
int s=0;
while(buffer[i]!= '=')
	VarName+=buffer[i++];
// add to the global list
GlobalVar v(VarName,GlobalIndex,line);
GlobalList.push_back(v);
CheckForDuplicateVarNames();
GlobalIndex++;
// now get the variable value up to the ; token
// skip = sign
i++;
while(buffer[i]!= ';')
	val[s++]=buffer[i++];
vf=atof(val);
VarObj Vo(vf);
Globals.Add(Vo);
}


void ParseVector(char *buffer)
{
string VarName="";
char val[BUFFSIZE];
//Vector vf;
// skip Vector
int i=7;
int s=0;
while(buffer[i]!= '=')
	VarName+=buffer[i++];
//VarName[s]='\0';
// add to the global list
GlobalVar v(VarName,GlobalIndex,line);
GlobalList.push_back(v);
CheckForDuplicateVarNames();
GlobalIndex++;
// skip over [
i+=2;
float x,y,z,w;
s=0;
while(buffer[i]!= ',')
	val[s++]=buffer[i++];
val[s]='\0';
x=atof(val);

i++; s=0;
while(buffer[i]!= ',')
	val[s++]=buffer[i++];
val[s]='\0';
y=atof(val);

i++; s=0;
while(buffer[i]!= ',')
	val[s++]=buffer[i++];
val[s]='\0';
z=atof(val);
i++; s=0;
while(buffer[i]!= ']')
	val[s++]=buffer[i++];
val[s]='\0';
w=atof(val);
Vec4 vf(x,y,z,w);
VarObj Vo(vf);
Globals.Add(Vo);
//cout <<"Buffer "<<buffer <<" ";
//cout <<"X "<<x<<" Y "<<y <<" z "<<z<<endl;
}


void ParsePoint(char *buffer)
{
string VarName="";
char val[30];
Vec3 vf;
// skip Vector
int i=6;
int s=0;
while(buffer[i]!= '=')
	VarName+=buffer[i++];
// add to the global list
GlobalVar v(VarName,GlobalIndex,line);
GlobalList.push_back(v);
CheckForDuplicateVarNames();
GlobalIndex++;
// skip over [
i+=2;
float x,y,z;
s=0;
while(buffer[i]!= ',')
	val[s++]=buffer[i++];
val[s]='\0';
x=atof(val);

i++; s=0;
while(buffer[i]!= ',')
	val[s++]=buffer[i++];
val[s]='\0';
y=atof(val);

i++; s=0;
while(buffer[i]!= ']')
	val[s++]=buffer[i++];
val[s]='\0';
z=atof(val);
vf.set(x,y,z);
VarObj Vo(vf);
Globals.Add(Vo);
}

