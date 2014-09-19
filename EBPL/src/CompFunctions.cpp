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
extern int GFunctionIndex;
extern ifstream stream;
extern int line;

void CheckForDuplicateVarNames(void)
{

for(unsigned int i=0; i<GlobalList.size(); i++)
	{
	 for(unsigned int x=0; x<GlobalList.size(); x++)
	  {
	  //no self check
	  if(i==x) continue;
	  else
	  	{
		if(GlobalList[i].Name == GlobalList[x].Name)
			{
			cout <<"***************ERROR IN COMPILATION**********************"<<endl;
			cout << "Error duplicate global names not allowed"<<endl;
			cout <<"Var 1 "<<GlobalList[i].Name <<" Line "<<GlobalList[i].LineNum<<endl;
			cout <<"Var 2 "<<GlobalList[x].Name <<" Line "<<GlobalList[x].LineNum<<endl;
			cout << "****************** COMPILATION FAILED EXITING ***********"<<endl;
			exit(0);
			}
		}
		}
	}	
}



bool CheckIfVar(string s)
{
	 for(unsigned int x=0; x<GlobalList.size(); x++)
	  {
	  //cout <<"GL .name "<<GlobalList[x].Name<<endl;
      //no self check
		if(GlobalList[x].Name == s)
			return true;
	   }
return false;

}

unsigned int FindGlobalVar(string s)
{
	 for(unsigned int x=0; x<GlobalList.size(); x++)
	  {
	  if(GlobalList[x].Name == s)
			return x;
	   }
return 0; //-9999;
}



bool CheckIfFunction(string s)
{
	 for(unsigned int x=0; x<GlobalFuncList.size(); x++)
	  {
	  //cout <<"GL .name "<<GlobalList[x].Name<<endl;
      //no self check
		if(GlobalFuncList[x].Name == s)
			return true;
	   }
return false;

}


unsigned int FindFunction(string s)
{
	 for(unsigned int x=0; x<GlobalFuncList.size(); x++)
	  {
	  if(GlobalFuncList[x].Name == s)
			return x;
	   }
return 0; //-9999;
}

bool CheckForBlankLine(char *buffer)
{
for(unsigned int i=0; i<strlen(buffer); i++)
	{
    if(!isspace(buffer[i]) )
    	return false;
       
    }
return true;
}




void createTempCompFile(char *fname)
{
   ofstream opstream;
   ifstream ipstream;
   char newFile[200];
   char buffer[BUFFSIZE];
   unsigned int ln=0;
   ipstream.open(fname);
   strcpy(newFile,fname);
   strcat(newFile,".tmp");
   opstream.open(newFile);
	if (!ipstream) {
       cerr << "Unable to open file " << fname<<endl;
       exit(1); // call system to stop
    }
	if (!opstream) {
       cerr << "Unable to open file " << newFile<<endl;
       exit(1); // call system to stop
    }

    //do
    while(!ipstream.eof())
    {
    ipstream.getline(buffer,BUFFSIZE);
    ln++;
    if(CheckForBlankLine(buffer) ==false)
    	{
        unsigned int i=0;
        while(isspace(buffer[i]) )
        		i++;
        if(buffer[i]=='/' && buffer[i+1]=='/')
        	{
                        
            }
        else if(buffer[i]=='/' && buffer[i+1]=='*')
        {
        //cout <<"found /*"<<endl;
        bool done=false;
        do{
        ipstream.getline(buffer,BUFFSIZE);
        //cout <<buffer<<" "<<strlen(buffer)<<endl;
        if(strlen(buffer)>2)
        for(unsigned int z=0; z<strlen(buffer)-1; z++)
        	{
            if(buffer[z]=='*' && buffer[z+1]=='/')
               done =true;
            }
        
        }while(done !=true && !ipstream.eof());
        
        }
        else
        {
        opstream<<ln<<endl;
        for(; i<strlen(buffer) && buffer[i]!='\n'; i++)
        	opstream<<buffer[i];
            opstream<<endl;
        }
        }
        
        
    }
  	opstream<<"-99"<<endl<<"EOF";  
    ipstream.close();
    opstream.close();
    cout <<"Created Temp file"<<endl;
    
}

void BuildInitialFunctionList(char *fname)
{
// the line  buffer for each line read in (500 chars max at present)
char buffer[BUFFSIZE];
// the token type found
char lnbuffer[100];
//cout <<"Fname = "<<fname<<endl;

 GlobalFunc initf("InitFunction",Brain::INIT);
 GlobalFuncList.push_back(initf);
 
 GlobalFunc upf("UpdateFunction",Brain::UPDATE);
 GlobalFuncList.push_back(upf);
 
 GlobalFunc df("DrawFunction",Brain::DRAW);
 GlobalFuncList.push_back(df);

 GlobalFunc cf("CollideFunction",Brain::COLLIDE);
 GlobalFuncList.push_back(cf);

ifstream ip;
ip.open(fname);
if (!ip) {
       cerr << "Unable to open file " << fname<<endl;
       exit(1); // call system to stop
    }
	do {
    	
    	ip.getline(lnbuffer,BUFFSIZE);
    	line=atoi(lnbuffer); // incriment the line number of the file
        if(line == -99)
        	break;
        ip.getline(buffer,BUFFSIZE);
		char temp[500];
		int i=0;
		for(i=0; buffer[i] !=' ' ; i++)
		temp[i]=buffer[i];
		temp[i]='\0';
		
		if (strcasecmp(temp,"Function")==0)
			{
            int i=9;
        	string Tempstr="";
            while(buffer[i]!='\n' && buffer[i]!=';' && buffer[i]!=' ')
         	{
    	    Tempstr+=buffer[i++];
            }
            //Tempstr+='\n';   
            cout <<"tmp "<<Tempstr<<endl;
            GlobalFunc gf(Tempstr,GFunctionIndex);
            GFunctionIndex++;
            GlobalFuncList.push_back(gf);
            }
		
		
		}while (!stream.eof()); //until the end of file is found




ip.close();

}

