#include <iostream>
#include <fstream>

using namespace std;

#include "OpCode.h"
#include "VarList.h"
#include "Brain.h"
#include "CompTokens.h"
#include "CallList.h"
 typedef std::vector <GlobalVar> GLobList;
 typedef std::vector <OpCodeList> OpList;
 typedef std::vector <GlobalFunc> GFList;
 typedef std::vector <CallListNames> CLNames;
 typedef std::vector <CallList> CLList;
#define BUFFSIZE 500
ifstream stream;

//FILE *stream;


unsigned int GlobalIndex=0;
unsigned int CLindex=0;
VarList Globals;

GLobList GlobalList;
GFList GlobalFuncList;
OpList Opcodes;
OpCodeList InitFunction;
OpCodeList UpdateFunction;
OpCodeList DrawFunction;
OpCodeList CollideFunction;
CLNames CL;    
CLList CallLists;
int FunctionIndex=4;
int GFunctionIndex=4;
int line=0;



int main(int argc, char **argv)
{

if(argc <3) 
	{
	cout << "Usage CompBrain [infile].bs [outfile].cbf "<<endl;
	exit(0);
	}	
createTempCompFile(argv[1]);
char newFile[200];
strcpy(newFile,argv[1]);
strcat(newFile,".tmp");
BuildInitialFunctionList(newFile);
Opcodes.resize(GFunctionIndex);
cout <<"Opcode Size "<<Opcodes.size()<<endl;
ReadBrainFile(newFile);
//Globals.Print();

stream.close();
//remove(newFile);

ofstream savefile;
savefile.open(argv[2]);
if(!savefile)
	{
	cerr<<"cannot open save file "<<argv[2]<<endl;
	exit(0);
	}
Globals.Save(&savefile);
savefile<<Opcodes.size()<<endl;
for(unsigned int i=0; i<Opcodes.size(); i++)
{
	Opcodes[i].Save(&savefile);
}
cout <<"Saving Call Lists Num Lists="<<CallLists.size()<<endl;
savefile<<CallLists.size()<<endl;
for(unsigned int i=0; i<CallLists.size(); i++)
	{
    cout <<"saving list "<<i<<endl;
    CallLists[i].Save(&savefile);
    }

savefile.close();
cout <<"**********************************************************"<<endl;
cout <<"**********************************************************"<<endl;
cout <<"****COMPILATION OK SAVED FILE*****************************"<<endl;
cout <<"**********************************************************"<<endl;
cout <<"**********************************************************"<<endl;

return 1;
}





void ReadBrainFile(char *fname)
{
// the line  buffer for each line read in (500 chars max at present)
char buffer[BUFFSIZE];
// the token type found
COMPTOKENS type;
char lnbuffer[100];

   stream.open(fname);

    if (!stream) {
       cerr << "Unable to open file " << fname<<endl;
       exit(1); // call system to stop
    }

	
	// loop through each line in the file
	do {
    	
    	stream.getline(lnbuffer,BUFFSIZE);
    	line=atoi(lnbuffer); // incriment the line number of the file
        //fgets(buffer,BUFFSIZE,stream);
        if(line == -99)
        	break;
        stream.getline(buffer,BUFFSIZE);
		type=ParseString(buffer);

		
		if(type == CFLOAT)
			ParseFloat(buffer);
		else if(type == CBOOL)
			ParseBool(buffer);
		else if(type == CPOINT)	
			ParsePoint(buffer);
		else if(type == CVECTOR)	
			ParseVector(buffer);
		else if(type == CFUZZY)		
			ParseFuzzy(buffer);
		else if (type==FUNCTION || type == INITFUNCTION || type == UPDATEFUNCTION ||
        		type==DRAWFUNCTION || type == COLLIDEFUNCTION)	
			ParseFunction(buffer,type);
		else if (type==FUNCTIONLIST)
			ParseFunctionList(buffer);
		
		else if (type==DEFCALLLIST)
			ParseCallList(buffer);
		else if (type==CALLLISTITEM)
			ParseCallListItem(buffer);
	
            
            	
		}while (!stream.eof()); //until the end of file is found

stream.close();

}






COMPTOKENS ParseString(char *buffer)
{
char temp[500];

int i=0;
for(i=0; buffer[i] !=' ' ; i++)
	temp[i]=buffer[i];
temp[i]='\0';

if (strcasecmp(temp,"float")==0)
	return CFLOAT;
else if (strcasecmp(temp,"bool")==0)
	return CBOOL;
else if (strcasecmp(temp,"Vector")==0)
	return CVECTOR;
else if (strcasecmp(temp,"Fuzzy")==0)
	return CFUZZY;
else if (strcasecmp(temp,"Point")==0)
	return CPOINT;
else if (strcasecmp(temp,"Function")==0)
	return FUNCTION;
else if (strcasecmp(temp,"FunctionList")==0)
	return FUNCTIONLIST;
else if (strcasecmp(temp,"DrawFunction")==0)
	return DRAWFUNCTION;
else if (strcasecmp(temp,"InitFunction")==0)
	return INITFUNCTION;
else if (strcasecmp(temp,"UpdateFunction")==0)
	return UPDATEFUNCTION;    
else if (strcasecmp(temp,"CollideFunction")==0)
	return COLLIDEFUNCTION;        
/*else if (strcasecmp(temp,"DefineCallList")==0)
	return DEFCALLLIST;        */
else if (strcasecmp(temp,"DefineCallList")==0)
	return DEFCALLLIST;        
else if (strcasecmp(temp,"CallListItem")==0)
	return CALLLISTITEM;        

else 
	{
	cout << "Unknown keyword on line " <<line<<endl;
	cout <<buffer<<endl;
	exit(0);
	}



}



void ParseFunction(char *buffer,COMPTOKENS type)
{
 OpCodeList Function;
 //Opcodes.push_back(Function);
 char CurrLine[BUFFSIZE];
 char lnbuffer[100];
 unsigned int FI=0;
 if (type == FUNCTION)
 	FI=FunctionIndex;
 else if(type== DRAWFUNCTION)
 	FI=Brain::DRAW;
 else if(type== INITFUNCTION)
 	FI=Brain::INIT;
 else if(type== UPDATEFUNCTION)
 	FI=Brain::UPDATE;
 else if(type== COLLIDEFUNCTION)
 	FI=Brain::COLLIDE;
 
 cout << "Function Index "<<FI<<endl;
 do
 {
  stream.getline(lnbuffer,BUFFSIZE);
  line=atoi(lnbuffer); // incriment the line number of the file
  stream.getline(CurrLine,BUFFSIZE);
  cout <<"Parsing function "<<CurrLine<<endl;
    
  ParseOpCodes(CurrLine,&Opcodes[FI]); 
  
  cout <<"End of Parse OpCode "<<CurrLine<<endl;
 }while(strncasecmp(CurrLine,"End",3)!=0); // && !stream.eof());
 if(type == FUNCTION)
 	FunctionIndex++;
}

void ParseFunctionList(char *buffer)
{
char CurrLine[BUFFSIZE];
 do
 {
 stream.getline(CurrLine,BUFFSIZE);
 }while( (strcmp(CurrLine,"EndFunctionList")!=0) || !stream.eof());
}



void ParseCallList(char *buffer)
{
string Tempstr="";
//cout <<"parsing call list "<<buffer;
unsigned int i=14;
// remove blank space;
while(isspace(buffer[i]) )
		i++;
while(buffer[i]!='\n' && buffer[i]!=';' && buffer[i]!=' ' )
    	{
        Tempstr+=buffer[i++];
        }
        
//cout <<"The function name is "<<Tempstr<<endl;
CallListNames cl(Tempstr,CLindex);

CL.push_back(cl);
CallList Ncl;
CallLists.push_back(Ncl);
CLindex++;
}


void ParseCallListItem(char *buffer)
{
//CallListItem TestList  CalcAngle;
string Tempstr="";
string Fstr="";
//cout <<"parsing call list "<<buffer;
unsigned int i=12;
// remove blank space;
while(isspace(buffer[i]) )
		i++;
while( buffer[i]!=' ' )
    	{
        Tempstr+=buffer[i++];
        }
while(isspace(buffer[i]) )
		i++;
while(buffer[i]!='\n' && buffer[i]!=';' && buffer[i]!=' ' )
    	{
        Fstr+=buffer[i++];
        }
cout << "Name "<<Tempstr<<" Function "<<Fstr<<endl;
unsigned int INDEX,CLINDEX;
if(CheckIfFunction(Fstr)==true)
    	 {
         INDEX = FindFunction(Fstr);
         }
    else {
    	  cout <<"error Function not know line "<<line<<" "<< Fstr << endl; exit(0);
    	 }

if(CheckIfCallList(Tempstr)==true)
    	 {
         CLINDEX = FindCallList(Tempstr);
         }
    else {
    	  cout <<"error Call list not know line "<<line<<" "<< Tempstr << endl; exit(0);
    	 }
         
CallLists[CLINDEX].Add(INDEX);
         
}


bool CheckIfCallList(string s)
{
	 for(unsigned int x=0; x<CL.size(); x++)
	  {
	  //cout <<"GL .name "<<GlobalList[x].Name<<endl;
      //no self check
		if(CL[x].Name == s)
			return true;
	   }
return false;

}

unsigned int FindCallList(string s)
{
	 for(unsigned int x=0; x<CL.size(); x++)
	  {
	  if(CL[x].Name == s)
			return x;
	   }
return 0; //-9999;
}
