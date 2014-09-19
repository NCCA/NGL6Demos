/*! Environment.h
	Author Jonathan Macey
    Version 3.1
    Date Last Revision 17/02/03

	PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one

    Linkages : Agent.h BBox.h CurveFollow.h Util.h Goal.h Util.h  uses namespace std
               everything comes from this class

    Description :

    This class is used to emit agents in the World based on a BBox for the world
	It handels all Agent collision logic and flocking algoriths

    Revision History :

    14/02/03 Removed redundant data types and asociated methods

    inline void RotateObj(int o,GLfloat a,bool x,bool y, bool z)
    					 {EnvObjects[o].Rotate(a,x,y,z);} //duplicated
    16/02/03 Added destructor
    17/02/03 Added Comments to source using doxygen tags
	18/02/03 Updated revision to 3.0
	27/02/03 Added Obj and Animated Obj draw options for the Agents update Revision to 3.1
	02/04/03 Added a bin lattice spacial data structure for all Agents
	27/05/03 Removed default constructor as paramaterisd one now needed for all versions
    */
 
#include "Environment.h"

using namespace std;
using namespace GraphicsLib; 

/* constructor passing in a World BBox */
Environment::Environment(BBox *wb,int Xlat,int Ylat,int Zlat,int binsize)
{
// copy world bbox
WorldBBox=wb;
//set default values
Elapsed = dt=currtime=0.0f;
nObj=0;
nVectObj=0;
FrameSkip=0;
nEmitters=0;
numWorldAgents=0;
FrameCount=0;
nObjFiles=0;
nAnimObjFiles=0;
Write=false;
Time=time(NULL);
DrawEnvDetails=false;
UsingGoals=false;
lat = new Lattice(Xlat,Ylat,Zlat,wb,this,binsize);
nPlanes=0;
agentRender = NULL;
}

Environment::~Environment()
{
//remove all of the vectors used in the environment
Emitters.erase(Emitters.begin(),Emitters.end());
EnvObjects.erase(EnvObjects.begin(), EnvObjects.end());
worldAgents.erase(worldAgents.begin(),worldAgents.end());
ObjFileList.erase(ObjFileList.begin(),ObjFileList.end());
AObjFileList.erase(AObjFileList.begin(),AObjFileList.end());
delete lat;
//close the output file
if (agentRender != NULL)
	delete agentRender;
CloseOutFile();
}

// Add an emitter to the environment
void Environment::AddEmitter(AgentEmitter newEmitter)
{
//add the emitter to the list
Emitters.push_back(newEmitter);
//increment the emitter counter
nEmitters++;
cout <<"Nemitters "<<nEmitters<<endl;
}
// this method does the main environment updeate depending of the mode (which indicates how the
// agents should be drawn
void Environment::DoUpdates(int mode)
{
//update the system clock
Elapsed+=tick();
time_t now=time(NULL);
dtSec=difftime(now,Time);
// check to see if we're using goals and determine which goal is active for this time 
if(UsingGoals==true)
	{
    for(unsigned long int i=0; i<nEmitters; i++)
    	{
    	if(Elapsed >=Emitters[i].Goals[Emitters[i].ActiveGoal].Gtime)
			{
			Elapsed=0.0f;
	    	Time=time(NULL);
    		Emitters[i].ActiveGoal ++;
 			if(Emitters[i].ActiveGoal >= Emitters[i].nGoals)
	    		Emitters[i].ActiveGoal =0;
    		}// end if time active
		}// end for each emitter
    }// end if using goals
//draw the environment objects
if(DrawEnvDetails==true)
	{
    WorldBBox->Draw();
    for (unsigned long int i=0; i<nEmitters; i++)
    	Emitters[i].DrawCurve(0);
	}
// call the draw update for the emitters
DrawEmitters(mode);
DrawObjects();
if(gp !=NULL)
	gp->Draw();
}

// update all of the emitters in the environment
void Environment::UpdateEmitters()
{
// write the output data to the file
OutputToFile();

// do the interflock collision detection first so that we can then do inter flock collision
// afterwards

lat->FillBins(&worldAgents);
//finally update all of the emitters
// first we reset the Agent values to reduce the round off errors
for (unsigned long int i=0; i<nEmitters; i++)
	{
	Emitters[i].ResetAgents();
    }

// Update the bin lattice data structure

lat->Update();
}

void Environment::DrawEmitters(int mode)
{
// for each of the emitters draw the agents if mode is 0 we render using either the
// Agents draw method (Triangles or OBj or Animated OBj depending on setup)
//else we draw using points (used for the ortho projections )
for ( unsigned long int i=0; i<nEmitters; i++)
	{
    if(mode == 0)
    	Emitters[i].RenderGL();
	else
    	Emitters[i].RenderPoints();
    }
}


void Environment::AddObject(EnvObj newObj)
{
// add the object passed to the environment object list
EnvObjects.push_back(newObj);
nObj++;
}

void Environment::AddVectObject(VectObj newObj)
{
// add the object passed to the environment object list
VectObjects.push_back(newObj);
nVectObj++;
}



void Environment::RotateObject(int index,GLfloat r,bool x, bool y, bool z)
{
// rotate the index object
EnvObjects[index].Rotate(r,x,y,z);
}

// at present this method is not used.
EnvObj *Environment::GetObject(int i)
{
// return the indexed objec
return &EnvObjects[i];
}

void Environment::DrawObjects()
{
//loop through all the EnvObj s and draw them
for (unsigned long int i=0; i<nObj; i++)
	{
     EnvObjects[i].DrawFF();

    }
for (unsigned long int i=0; i<nVectObj; i++)
  	{
    //VectObjects[i].DrawFF();
    VectObjects[i].DrawVect();
    }
for(unsigned long int i=0; i<nPlanes; i++)
	Planes[i].Draw();
}

void Environment::SetDoCollide(void)
{
// toggle the collision flag for all the emitters
for (unsigned long int i=0; i<nEmitters; i++)
	Emitters[i].ToggleCollide();
lat->ToggleDoCollide();
}

void Environment::ToggleDrawExtents(void)
{
//toggle the draw extents flag for all the emitters and envobjets
for(unsigned long int i=0; i<nObj; i++)
	EnvObjects[i].ToggleDrawExtents();
}

void Environment::SetEmitterPath(int index,Point3 p[4])
{
// attach a path to an Emitter
Emitters[index].CreatePath(p);
}

void Environment::SetFollowCurve(int index)
{
//toggle the follow curve path mode for the emitter
Emitters[index].FollowCurvePath();
}

void Environment::SetCurvePathTime(int index, GLfloat t)
{
//set the emitter curve path time so if were following a curve we can attach this
// as the centroid for the agent
Emitters[index].SetCurvePathTime(t);
}




GLint Environment::GetTimeMS()
{
	//return the time in mili seconds
    clock_t time = clock();
	GLdouble elapsed = (GLdouble)time / (GLdouble)CLOCKS_PER_SEC;
    elapsed *= 1000.0f;
	return (GLint) elapsed;
}

GLdouble Environment::GetTimeSeconds()
{
//return the time in seconds
clock_t time = clock();
GLdouble elapsed = (GLdouble)time / (GLdouble)CLOCKS_PER_SEC;
return elapsed;
}

GLdouble Environment::tick(void)
{
 //update the system time tick
 GLdouble systime=GetTimeMS();
 dt=systime - currtime;
 currtime = systime;
 return dt;
}

void Environment::SetMaxRunTime(GLdouble t)
{
// set the max runtime
MaxRunTime =t;
}


void Environment::OpenOutFile(void)
{
/* Open file and test for successful operation */
    if ((OutFile = fopen(outfile, "wt")) == NULL)
	{
        cout << "Failed to open file for writing : "<< outfile<<endl;
    }

}
void Environment::CloseOutFile(void)
{
//close the outfile
fclose(OutFile);
}

void Environment::AddAgentsToWorldList(void)
{
	if(gp == NULL)
    	{
         gp = new GroundPlane(WorldBBox,WorldBBox->center.y-WorldBBox->height/2,0,0.4,0,0.7);
         
        }

// We keep two lists of agents on is in each emitter and the other is the Environment list.
//This contaions a pointer to all of the Agents that exist and we use this for inter species
//collision detection
//This list only contains pointers to the objects
for(unsigned long int i=0; i<nEmitters; i++)
	{
    for(unsigned long int x=0; x<Emitters[i].nAgents; x++)
    	{
        worldAgents.push_back(&Emitters[i].Agents[x]);
        

/*		if(Interactive == true)
        	Emitters[i].Agents[x].Interactive = true;*/
		Emitters[i].Agents[x].gp = gp;
        Emitters[i].Agents[x].brain->SetAgentRender(agentRender);
        numWorldAgents++;
        }
    }
cout <<"Num World Agents="<<numWorldAgents<<endl;
}
// This method tests for inter species / inter agent collision. If we have hit we have a
// runaway time and a runaway vector. This means that the agent will run in a particular direction
//for a fixed length of time before it attempts to rejoin the flock which gives us a more
// natural looking simulation. These times are set in the script file see script documents
// for more details




// The next two methods are for use with the mel script to read in the output of the
//flocking system into maya

void Environment::OutputToFile(void)
{
// if we are writing to file we print the x,y,z position and x,y,z rotation of each
// agent to the file.
/*if(Write==true)
  {
  if((FrameCount % FrameSkip)==0)
  {
  fprintf(OutFile,"FRAME %ld\n",FrameCount);
   for (unsigned long i=0; i<numWorldAgents; i++)
	{
	fprintf(OutFile,"BOID: %ld %f %f %f %f %f %f\n",i+1,
    worldAgents[i]->Pos.x,worldAgents[i]->Pos.y,worldAgents[i]->Pos.z,
	worldAgents[i]->xrot,worldAgents[i]->yrot,worldAgents[i]->zrot);
    }
  }
FrameCount++;
}*/


}



int Environment::AddObjModel(char *objFile)
{
//add an Obj model to the list so it can be used to draw the agents
ObjLoader *o = new ObjLoader(objFile,true);
ObjFileList.push_back(o);
return nObjFiles++;
}

int Environment::AddObjModel(char *objFile1,char *objFile2,int frames,int Mat)
{
//add an AnimatedObj model to the list so it can be used to draw the agents
AnimatedObj *a= new AnimatedObj(objFile1,objFile2,frames,AnimatedObj::LOOPANIM,Mat);
AObjFileList.push_back(a);
return nAnimObjFiles++;
}



// Create a new Ground plane for the Agents
void Environment::CreateGP(GLfloat y,GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
gp=new GroundPlane(WorldBBox,y,r,g,b,a);

}

void Environment::CreateGP(GLfloat y,char *fname,GLfloat YY)
{
// gp using a bmp file
gp=new ImageGroundPlane(WorldBBox,y,fname,YY);

}

void Environment::CreateGP(GLfloat y,char *fname,char *fname2,GLfloat YY)
{
// textured gp using two bmps
gp=new ImageGroundPlane(WorldBBox,y,fname,fname2,YY);

}

void Environment::CreateObjGP(GLfloat y,ObjLoader *obj,GLfloat YY)
{
gp = new ObjGroundPlane(WorldBBox,y,obj,YY);
}

void Environment::CreateObjGP(GLfloat x,GLfloat y,char *fname,char *fname2,GLfloat xs,GLfloat yz, GLfloat zs)
{
gp = new ObjGroundPlane(WorldBBox,x,y,fname,fname2,xs,yz,zs);

}




void Environment::LoadARF(unsigned long int index,char *fname)
{
// load an Agent Resource file and set the values for each of the Agents
FILE *stream;
unsigned long int numAgents;
if ((stream = fopen(fname, "rt")) == NULL)
	{
        cout << "Failed to open file for writing : "<< fname<<endl;
        exit(0);
    }
	fscanf(stream,"%ld\n",&numAgents);
    for(unsigned long int i=0; i<numAgents; i++)
    	{
        fscanf(stream,"%f %f %f %f %f %f\n",&Emitters[index].Agents[i].Pos.x,
        									&Emitters[index].Agents[i].Pos.y,
                                            &Emitters[index].Agents[i].Pos.z,
        									&Emitters[index].Agents[i].Dir.x,
                                            &Emitters[index].Agents[i].Dir.y,
                                            &Emitters[index].Agents[i].Dir.z);
        Emitters[index].Agents[i].brain->CallInitFunction(&Emitters[index].Agents[i]);


        }
	fclose(stream);




}


void Environment::AddPlane(Point3 p[4])
{
Plane pl(p[0],p[1],p[2],p[3]);
Planes.push_back(pl);
nPlanes++;

}

void Environment::UseAgentRender(void)
{

agentRender = new AgentRender();

}





