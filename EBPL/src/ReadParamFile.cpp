/*! ReadParamFile.cpp
	Author Jonathan Macey <BR>
    Version 3.0 <BR>
    Date Last Revision 18/02/03 <BR>
    Linkages : BBox.h .h Environmnent.h EnvObj.h FlockMain.cpp <BR>
	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one <BR>
    Description : <BR>
    This file contains the script file parser for the scripting language to setup the flocking system.
    It is a bit clunky but it works so hey ;-) <BR> Also we are using a few global variables
    as the FlockMain and GlutCallback modules need them for the system
*/



#ifdef __USINGWINDOWS__
	#include <windows.h>
	#define strcasecmp stricmp //linux uses strcasecmp windows doesn't have it so just do the define
#endif

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h> // for isspace
#include "AgentEmitter.h"
#include "BBox.h"
#include "Environment.h"
#include "EnvObj.h"
//#include "Rover.h"
using namespace std;
using namespace GraphicsLib; 

/**************************************************************************************8
  External global variable
****************************************************************************************/
// the step of the path for the curve follow routines
extern GLfloat PathStep;
// A pointer to the global Environment created in flock main, all of the flocking system
// stems from this one Class
extern Environment *Env;
// The world Bounding Box
extern BBox *bb;
// the rate that the system is updated
extern int updateRate;
// flag to indicate if a random seed should be used or not
extern bool RandomSeed;
// Function to setup the Camera, this is declared in GlutCallback.cpp
extern void SetCamera(Point3 Eye, Point3 Look, Vector Up, int WIDTH,
				int HEIGHT,GLfloat ViewAngle, GLfloat aspect,
				GLfloat n, GLfloat f);

// flag to indicate if the camera is set by the user or if a default one should be used
bool CameraSet=false;
// flag to indicate if the camera should follow the centroid or not
bool CamFollowCentroid=false;
// and which centroid it should follow
bool CamFollowCentroidIndex=0;

// the offset of the Camera's eye position from the Centroid
GLfloat cEoffx=10.0f;
GLfloat cEoffy=10.0f;
GLfloat cEoffz=10.0f;
// the number of scatter vectors that should be create if we are using scatter mode
int NumScatterVect=4;
/*! \enum KeywordTokens the keyword tokens avaliable for the flocking system scripting language */
// If adding a new keyword to the script language we add a new token to this list first
enum KeywordToken{WORLDBBOX,AGENTEMITTER,AGENTEMITTERR,PATHFOLLOW,ENVOBJ,ROTATEOBJ,COMMENT,UNKNOWN, WHITESPACE,
				  MAXRUNTIME,GOAL,OUTFILE,FRAMESKIP,UPDATERATE,CAMERA,
                  RANDOMSEED,PATHSTEP,FRAMEOFFSET,CAMFOLLOWCENTROID,
                  OBJFILE,ANIMOBJ,LOCKEDCENTROID,VECTOBJ,GROUNDPLANE,IMAGEGROUNDPLANE,LOADARF,GROUNDPLANETEX,
                  OBJTERRAIN,PLANE,USEAGENTRENDER};
#define BUFFSIZE 500
// pointer to the main script file
FILE *stream;
// the current line number of the file
int line=0;
/*! This function parses the line from the file and determines which keyword it is
when adding a new keyword copy one of the if statements and check for the keyword required
then return the token defined in the enum KeywordToken above
\param char * buffer the line to parse
\param KeywordToken the token found */
KeywordToken ParseString(char *buffer)
{
char temp[40];
char B[200];
int i;
// copy the first word of the line upto the 1st space
for(i=0; buffer[i] !=' '; i++)
	temp[i]=buffer[i];
temp[i]='\0';
/* Ignore white space in file */
for(i=0; i<(int)strlen(temp); i++)
	{
    if(!isspace(temp[i]))
    	continue;
    else return WHITESPACE;

    }
// we found a comment ignore this line
if(strncmp(temp,"//",2)==0)
	return COMMENT;
// we found a comment loop until we find the end of the comment block
if(strncmp(temp,"/*",2)==0)
	{
	do
    {
    fgets(B,BUFFSIZE,stream);
	line++;
    }while(strncmp(B,"*/",2)!=0);
	return COMMENT;
    }
else if (strcasecmp(temp,"WORLDBBOX")==0)
	return WORLDBBOX;
else if (strcasecmp(temp,"AGENTEMITTER")==0)
	return AGENTEMITTER;
else if (strcasecmp(temp,"MultiAGENTEMITTER")==0)
	return AGENTEMITTERR;
else if (strcasecmp(temp,"PATHFOLLOW")==0)
	return PATHFOLLOW;
else if (strcasecmp(temp,"ENVOBJ")==0)
	return ENVOBJ;
else if (strcasecmp(temp,"ROTATEOBJ")==0)
	return ROTATEOBJ;
else if (strcasecmp(temp,"Goal")==0)
	return GOAL;
else if (strcasecmp(temp,"outputfile")==0)
	return OUTFILE;
else if (strcasecmp(temp,"OutFileFrameSkip")==0)
	return FRAMESKIP;
else if (strcasecmp(temp,"UpdateRate")==0)
	return UPDATERATE;
else if (strcasecmp(temp,"Camera")==0)
	return CAMERA;
else if (strcasecmp(temp,"RandomSeed")==0)
	{
	RandomSeed=true;
    return RANDOMSEED;
    }
else if (strcasecmp(temp,"UseAgentRender")==0)
	{
	return USEAGENTRENDER;
    }

else if (strcasecmp(temp,"PathStep")==0)
	{
	return PATHSTEP;
    }
else if (strcasecmp(temp,"FrameOffset")==0)
	{
	return FRAMEOFFSET;
    }
else if (strcasecmp(temp,"CamFollowCentroid")==0)
	{
	return CAMFOLLOWCENTROID;
    }
else if (strcasecmp(temp,"ObjFile")==0)
	{
	return OBJFILE;
    }
else if (strcasecmp(temp,"AnimObj")==0)
	{
	return ANIMOBJ;
    }
else if (strcasecmp(temp,"LockedCentroid")==0)
	{
	return LOCKEDCENTROID;
    }
else if (strcasecmp(temp,"VectObj")==0)
	{
	return VECTOBJ;
    }
else if (strcasecmp(temp,"GroundPlane")==0)
	{
	return GROUNDPLANE;
    }
else if (strcasecmp(temp,"ImageGroundPlane")==0)
	{
	return IMAGEGROUNDPLANE;
    }
else if (strcasecmp(temp,"LoadARF")==0)
	{
	return LOADARF;
    }

else if (strcasecmp(temp,"GroundPlaneTex")==0)
	{
	return GROUNDPLANETEX;
    }
else if (strcasecmp(temp,"ObjTerrain")==0)
	{
	return OBJTERRAIN;
    }
else if (strcasecmp(temp,"Plane")==0)
	{
	return PLANE;
    }

else // we don't know what it is return unknown
return UNKNOWN;
}

/*! This function reads the script file and passes each line to the parser to determine what keyword
it is then parses each keyword
\param char * fname the name of the file to parse
*/
void ReadParamFile(char *fname)
{
// the line  buffer for each line read in (500 chars max at present)
char buffer[BUFFSIZE];
// the token type found
KeywordToken type;
// general variables used in the reading of the files
Point3 pos;
GLfloat x,y,z,w,h,d,r;
GLfloat x1,x2,x3;
GLfloat y1,y2,y3;
GLfloat z1,z2,z3;
int sp,na;
Point3 p[4];
int NAgents=0;
int NEnvObj=0;
int NVectObj=0;
char Text[40];
int sret;
// if we are re-reading a file delete the env first so we can re-allocate
/*if(Env !=NULL)
	{
    delete Env;
    delete bb;
    }*/
    /* Open file and test for successful operation */
 //   cout <<"Doing read Param File"<<endl;
    if ((stream = fopen(fname, "rt")) == NULL)
	{
        cout << "Failed to open file for reading : "<< fname<<endl;
        exit(0);
    }
	// loop through each line in the file
	do {
    	line ++; // incriment the line number of the file
		// read the file and find which keyword token it is
        buffer[0]='\0';
        fgets(buffer,BUFFSIZE,stream);
		type=ParseString(buffer);
		//cout <<"B : "<<buffer<<endl;
        if(type== COMMENT) ; //do nothing
		// we found a world bbox
		else if(type== WORLDBBOX)
    	{
         // read in the parameters
         int xlat,ylat,zlat,binsize;
		 sret=sscanf(buffer,"%s %f %f %f %f %f %f %d %d %d %d",Text,
    			      &x,&y,&z,&w,&h,&d,&xlat,&ylat,&zlat,&binsize);
		 // if we don't get the correct number report a problem
         if(sret != 11)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
         // else create the world bbox
         pos.set(x,y,z);
         bb = new BBox(pos,w,h,d);
         Env = new Environment(bb,xlat,ylat,zlat,binsize);
        }
        else if(type == AGENTEMITTER )
        {
        // we can't attach an Agent Emitter if we haven't got a bbox and env
        if(bb == NULL)
        	{cout <<"Script Error :Must declare a world BBox first !" <<endl;
            exit(0);
            }
		int rp;
        unsigned long int una;
        char Brain[100];
        sret=sscanf(buffer,"%s %f %f %f %ld %f %f %f %f %d %d %s",Text,&x,&y,&z,&una,&w,&h,&d,&r,&sp,&rp,Brain);
		// if we don't get the correct number report a problem
        if(sret != 12)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        // create the emitter and attach it to the environmnet
        pos.set(x,y,z);
		AgentEmitter a(pos,una,w,h,d,r,bb,sp,Env,(bool)rp,Brain);
        Env->AddEmitter(a);
        //cout << "NUM_OF_PARTICLES "<<na<<endl;
        NAgents++;
        }
        else if(type == AGENTEMITTERR )
        {
        // different type of agent emitter for multi species
        if(bb == NULL)
        	{cout <<"Script Error :Must declare a world BBox first !" <<endl;
            exit(0);
            }
		int rp;
        unsigned long int una;
        int sp2;
        char Brain2[100];
        sret=sscanf(buffer,"%s %f %f %f %ld %f %f %f %f %d %d %d %s",Text,&x,&y,&z,&una,&w,&h,&d,&r,&sp,&sp2,&rp,Brain2);
		// if we don't get the correct number report a problem
        if(sret != 13)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        pos.set(x,y,z);
		AgentEmitter a(pos,una,w,h,d,r,bb,sp,sp2,Env,(bool)rp,Brain2);
        Env->AddEmitter(a);
        NAgents++;
        }
	else if(type == GROUNDPLANE )
        {
        // we can't attach an Agent Emitter if we haven't got a bbox and env
        if(bb == NULL)
        	{cout <<"Script Error :Must declare a world BBox first !" <<endl;
            exit(0);
            }
		GLfloat r,g,b,a;
        sret=sscanf(buffer,"%s %f %f %f %f %f",Text,&y,&r,&g,&b,&a);
		// if we don't get the correct number report a problem
        if(sret != 6)
         	{
			cout <<"Script Error : problem with params on line "<< line<<" "<< buffer<<endl;
            exit(0);
            }
        // create the emitter and attach it to the environmnet
        Env->CreateGP(y,r,g,b,a);
        //Env->gp->SetColour(r,g,b,a);
        }
	else if(type == IMAGEGROUNDPLANE )
        {
        // we can't attach an Agent Emitter if we haven't got a bbox and env
        if(bb == NULL)
        	{cout <<"Script Error :Must declare a world BBox first !" <<endl;
            exit(0);
            }
		char fname[80];
        sret=sscanf(buffer,"%s %f %s %f",Text,&x,fname,&y);
		// if we don't get the correct number report a problem
        if(sret != 4)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        // create the emitter and attach it to the environmnet
        assert(y >0.0);
        Env->CreateGP(x,fname,y);
        }

	else if(type == OBJTERRAIN )
        {
        // we can't attach an Agent Emitter if we haven't got a bbox and env
        if(bb == NULL)
        	{cout <<"Script Error :Must declare a world BBox first !" <<endl;
            exit(0);
            }
		char fname[80];
		char fname2[80];
		GLfloat xs,ys,zs;
        sret=sscanf(buffer,"%s %f %s %s %f %f %f %f",Text,&x,fname,fname2,&y,&xs,&ys,&zs);
		// if we don't get the correct number report a problem
        if(sret != 8)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        cout <<"in read param creating GP"<<endl;
        Env->CreateObjGP(x,y,fname,fname2,xs,ys,zs);
        }
		
		
		
		
	else if(type == GROUNDPLANETEX )
        {
        // we can't attach an Agent Emitter if we haven't got a bbox and env
        if(bb == NULL)
        	{cout <<"Script Error :Must declare a world BBox first !" <<endl;
            exit(0);
            }
		char fname[80];
        char fname2[80];
        sret=sscanf(buffer,"%s %f %s %s %f",Text,&x,fname,fname2,&y);
		// if we don't get the correct number report a problem
        if(sret != 5)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        // create the emitter and attach it to the environmnet
        assert(y >0.0);
        Env->CreateGP(x,fname,fname2,y);
        }

        else if(type==PATHFOLLOW)
        {
		sret=sscanf(buffer,"%s %d %f %f %f %f %f %f %f %f %f %f %f %f",Text,&na,&x,&y,&z
        			  ,&x1,&y1,&z1,&x2,&y2,&z2,&x3,&y3,&z3);
        // if we don't get the correct number report a problem
        if(sret != 14)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        // can;t attach a path if emitter doesn't exist
        if(NAgents <= na)
        	{
            cout <<"Script Error : Cannot attach path as No emitter exists! Line "<< line<<endl;
            exit(0);
            }
        // create the points and create the path
        p[0].set(x,y,z);
		p[1].set(x1,y1,z1);
		p[2].set(x2,y2,z2);
        p[3].set(x3,y3,z3);
        Env->SetEmitterPath(na,p);
        }

        else if(type==ENVOBJ)
        {
		// cant attach an object if env doesn't exist
        if(Env == NULL)
            {
            cout <<"Script Error : Cannot attach object as No Environment exists! Line "<< line<<endl;
            exit(0);
            }
		sret=sscanf(buffer,"%s %f %f %f %f %f %f %f ",Text,&x,&y,&z,&w,&h,&d,&x1);
        // if we don't get the correct number report a problem
        if(sret !=8)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        // crate the object
        pos.set(x,y,z);
        EnvObj o (pos,w,h,d,x1);
        // add it to the environment and incriment the object count
        Env->AddObject(o);
        NEnvObj++;
		}

      else if(type==VECTOBJ)
        {
		// cant attach an object if env doesn't exist
        if(Env == NULL)
            {
            cout <<"Script Error : Cannot attach object as No Environment exists! Line "<< line<<endl;
            exit(0);
            }
		GLfloat vx,vy,vz;
        sret=sscanf(buffer,"%s %f %f %f %f %f %f %f %f %f %f",Text,&x,&y,&z,&w,&h,&d,&x1,&vx,&vy,&vz);
        // if we don't get the correct number report a problem
        if(sret !=11)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        // crate the object
        pos.set(x,y,z);
        Vector vect(vx,vy,vz);
        VectObj o (pos,w,h,d,x1,vect);
        // add it to the environment and incriment the object count
        Env->AddVectObject(o);
        NVectObj++;
		}

        else if(type == ROTATEOBJ)
        {
        sret=sscanf(buffer,"%s %d %f %f %f %f ",Text,&na,&r,&x,&y,&z);
	    // if we don't get the correct number report a problem
        if(sret !=6)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        // can't rotate an object if it doesn't exist
        if( NEnvObj<=na)
            {
            cout <<"Script Error : Cannot rotate obj as it doesn't exist Line "<< line<<endl;
            exit(0);
            }
        // rotate the object
        Env->RotateObject(na,r,(bool)x,(bool)y,(bool)z);
        }
        else if(type== UNKNOWN)
        {
        	cout <<"Ignoring Unknown keyword on line : "<< line<< endl;
        	cout <<buffer<<endl;
        }

	else if(type==MAXRUNTIME)
        {
		GLdouble t;
        sret=sscanf(buffer,"%s %lf ",Text,&t);
        // if we don't get the correct number report a problem
        if(sret != 2)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        if(Env == NULL)
            {
            cout <<"Script Error : Cannot set time as No Environment exists! Line "<< line<<endl;
            exit(0);
            }
            //set the runtime
        Env->SetMaxRunTime(t);
        }

	else if(type==GOAL)
        {
		sret=sscanf(buffer,"%s %d %f %f %f %f ",Text,&na,&x,&y,&z,&x1);
        // if we don't get the correct number report a problem
        if(sret != 6)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        if(NAgents <= na)
        	{
            cout <<"Script Error : Cannot attach goal as No emitter exists! Line "<< line<<endl;
            exit(0);
            }
            //create the goal
         Env->Emitters[na].AddGoal(x,y,z,x1*100);
         Env->UsingGoals=true;
        }


	else if(type==OUTFILE)
        {
		char fname[30];
        sret=sscanf(buffer,"%s %s ",Text,fname);
        // if we don't get the correct number report a problem
        if(sret != 2)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        if(Env == NULL)
        	{
            cout <<"Script Error : Cannot set file as no env exists! Line "<< line<<endl;
            exit(0);
            }
            // set the file name
         Env->setOutFile(fname);
        }

	else if(type==FRAMESKIP)
        {
		sret=sscanf(buffer,"%s %d ",Text,&na);
        // if we don't get the correct number report a problem
        if(sret != 2)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        if(Env == NULL)
        	{
            cout <<"Script Error : Cannot set frameskip as no env exists! Line "<< line<<endl;
            exit(0);
            }
            //set the frame skip
         Env->setFrameSkip(na);
        }

		else if(type==FRAMEOFFSET)
        {
		unsigned long int fo;
        sret=sscanf(buffer,"%s %ld ",Text,&fo);
        // if we don't get the correct number report a problem
        if(sret != 2)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        if(Env == NULL)
        	{
            cout <<"Script Error : Cannot set frameoffset as no env exists! Line "<< line<<endl;
            exit(0);
            }
            // set the offset of the frames to start from
         Env->setFrameOffset(fo);
        }



	else if(type==PATHSTEP)
        {
		sret=sscanf(buffer,"%s %f ",Text,&PathStep);
        // if we don't get the correct number report a problem
        if(sret != 2)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }


        }
    else if(type==CAMERA)
        {
		Point3 e(0.0f,0.0f,0.0f);
        Point3 l(0.0f,0.0f,0.0f);
        Vector u(0.0f,0.0f,0.0f);
        int W,H;
        GLfloat va,as,n,f;
        sret=sscanf(buffer,"%s %f %f %f %f %f %f %f %f %f %d %d %f %f %f %f",Text,
        &e.x,&e.y,&e.z,&l.x,&l.y,&l.z,&u.x,&u.y,&u.z,&W,&H,&va,&as,&n,&f);
	// if we don't get the correct number report a problem
        if(sret != 16)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            cout << buffer<<endl;
            exit(0);
            }
		CameraSet=true;
        glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(W*64/48.0f,H*64/48.0f, -W,H, n, f);
	    SetCamera(e,l,u, W,H,va,as,n,f);

        }
    else if(type==CAMFOLLOWCENTROID)
        {
		sret=sscanf(buffer,"%s %d %f %f %f",Text,&na,&cEoffx,&cEoffy,&cEoffz);
        // if we don't get the correct number report a problem
        if(sret != 5 )
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        if(NAgents <= na)
        	{
            cout <<"Script Error : Cannot set CamFollowCentroid as No emitter exists! Line "<< line<<endl;
            exit(0);
            }

            CamFollowCentroid=true; CamFollowCentroidIndex=na;

        }
// 	else if(type==OBJFILE)
//         {
//         char objFile[100];
// 		sret=sscanf(buffer,"%s %d %s",Text,&na,objFile);
//         // if we don't get the correct number report a problem
//         if(sret != 3)
//          	{
// 			cout <<"Script Error : problem with params on line "<< line<<endl;
//             exit(0);
//             }
//         int offset = Env->AddObjModel(objFile);
// 		Env->Emitters[na].SetAgentObjModel(offset);
//         }
// 	else if(type==ANIMOBJ)
//         {
//         char objFile1[100];
// 		char objFile2[100];
//         int frames;
//         int mat;
//         sret=sscanf(buffer,"%s %d %s %s %d %d",Text,&na,objFile1,objFile2,&frames,&mat);
//         // if we don't get the correct number report a problem
//         if(sret != 6)
//          	{
// 			cout <<"Script Error : problem with params on line "<< line<<endl;
//             exit(0);
//             }
//         int offset = Env->AddObjModel(objFile1,objFile2,frames,mat);
//         Env->Emitters[na].SetAgentAnimObjModel(offset);
//         }
	else if(type==LOCKEDCENTROID)
        {
		sret=sscanf(buffer,"%s %d %f %f %f",Text,&na,&x,&y,&z);
        // if we don't get the correct number report a problem
        if(sret != 5)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        if(NAgents <= na)
        	{
            cout <<"Script Error : Cannot set LockedCentroid as No emitter exists! Line "<< line<<endl;
            exit(0);
            }
		Point3 p(x,y,z);
        Env->Emitters[na].SetLockedCentroid(p);
        }
	else if(type==LOADARF)
        {
		char arf[200];
        sret=sscanf(buffer,"%s %d %s ",Text,&na,arf);
        // if we don't get the correct number report a problem
        if(sret != 3)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        if(NAgents <= na)
        	{
            cout <<"Script Error : Cannot set weight as No emitter exists! Line "<< line<<endl;
            exit(0);
            }
            //set the weight
            Env->LoadARF(na,arf);

        }
	else if(type==PLANE)
        {
        sret=sscanf(buffer,"%s %f %f %f %f %f %f %f %f %f %f %f %f",Text,&x,&y,&z
        			  ,&x1,&y1,&z1,&x2,&y2,&z2,&x3,&y3,&z3);
        // if we don't get the correct number report a problem
        if(sret != 13)
         	{
			cout <<"Script Error : problem with params on line "<< line<<endl;
            exit(0);
            }
        // can;t attach a path if emitter doesn't exist
        // create the points and create the path
        p[0].set(x,y,z);
		p[1].set(x1,y1,z1);
		p[2].set(x2,y2,z2);
        p[3].set(x3,y3,z3);
        Env->AddPlane(p);
		
        }
	else if(type==USEAGENTRENDER)
    {
    Env->UseAgentRender();
    cout <<"using Agent Render "<<endl;
    }

}while (!feof(stream)); //until the end of file is found



}

