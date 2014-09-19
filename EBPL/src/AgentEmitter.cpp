/*! AgentEmitter.cpp
	Author Jonathan Macey
    Version 5.0
    Date Last Revision 20/03/03
    PreProcessor : If using windows define __USINGWINDOWS__ flag
                   For debug version define __DEBUG_ so it doesn't conflict with the windows one

    Description :

    This class is used to emit agents in the World based on a BBox for the world
	It handels all Agent collision logic and flocking algoriths
	
	Revision History :
	
	This Module has had a major re-write to remove most of the AgentLogic to the Bin Lattice
	spacial data structures. 
	
 */

#include "Environment.h"
#include "AgentEmitter.h"
#include <unistd.h>
#include <time.h>
using namespace std;
using namespace GraphicsLib; 


//Constructor code
AgentEmitter::AgentEmitter(Point3 pos, unsigned long int num,GLfloat w,GLfloat h,GLfloat d,GLfloat r,
  	 					   BBox *extents,int Species,Environment *e,bool RandPos,char *BrainScript)
{
//Set Positon of Emittor
Position = pos;

//Now loop through and add a new agent to the list upto num agents
for(unsigned long int i=0; i<num; i++)
   {
    //create a random direction for the Agent to start with
    //Vector dir(RandomNum()/10.0,RandomNum()/10.0,RandomNum()/10.0);
    Vector dir(RandomNum(),RandomNum(),RandomNum());
	// If RandPos flag set create a random distribution of agents based on the world
    // BBox size
    if(RandPos == true)
    	pos.set(RandomNum((extents->width-2)/2.0f),
        		RandomNum((extents->height-2)/2.0f),
                RandomNum((extents->depth-2)/2.0f));
	//Construct a new agent to add to the list
    Agent a(pos,w,h,d,r,dir,Species,BrainScript,e);
    a.brain->MeIndex=i;
    //give the Agentsome Random Velocity greater than 0
//    a.SetVelocity(RandomPosNum(1.0f)+0.1f,RandomPosNum(1.0f)+0.1f,RandomPosNum(1.0f)+0.1f);
    // now use push_back to add it to the Agents vector
	Agents.push_back(a);
	//a.SetBrainAgentPointer(Agents[i]);
    }// end for loop
// Save a copy of the BBox
Extents=extents;
/* fill ext array used for the BBox Agent collision detection. For each Plane of the
   BBox we use either width height or depth /2 to determine if the Agent has hit the
   plane or not (see WorldBBoxColission method for usage) */
ext[0]=ext[1]=(Extents->height/2.0f);
ext[2]=ext[3]=(Extents->width/2.0f);
ext[4]=ext[5]=(Extents->depth/2.0f);
// Store the number of agents
nAgents=num;
//set the default values for flags
doCollide=false;
FollowCurve=false;
FollowGoals=false;
//DrawExtents=false;
CurvePathTime=0.0;
nGoals=0;
ActiveGoal=0;

// set the Centroid to be the center of the world (0,0,0) for now
Centroid.set(0.0f,0.0f,0.0f);
// set the mincentroid effect distance to r*2 can be overidden in script file
//MinCentroidDist=r*2.0f;
// Save a copy of the Environment
env=e;
LockedCentroid.set(0,0,0);
//LockCentroid=false;

// for(unsigned long int i=0; i<num; i++)
// 	Agents[i].SetBrainAgentPointer(&Agents[i]);

}//end method

// width height depth radius
static GLfloat SpeciesArray[8][4]=
{
//species 1 White tip shark
{1.5f,1.1f,1.28f,4.2f},
//s 2 spanish sardine
{0.940f,0.14f,0.14f,0.8f},
//s 3 Herring
{1.2f,0.4f,0.4f,0.9f},
//s4 angel fish ?
{0.70f,0.44f,0.14f,0.6f},
//s5 Sardine 2
{0.940f,0.14f,0.14f,0.8f},
//s 6 ? // nother shark
{1.5f,1.1f,1.28f,1.0f},
//s 7 ?
{0.70f,0.44f,0.14f,0.8f},
//big shark
{.5f,.1f,.28f,4.2f}

};


AgentEmitter::AgentEmitter(Point3 pos,unsigned long int num,GLfloat w,GLfloat h,GLfloat d,GLfloat r,
 			  BBox *extents,int StartSpecies,int EndSpecies,Environment *e, bool RandPos,char *BrainScript)

{
//Set Positon of Emittor
Position = pos;

//Now loop through and add a new agent to the list upto num agents
for(unsigned long int i=0; i<num; i++)
   {
    //create a random direction for the Agent to start with
    //Vector dir(RandomNum()/10.0,RandomNum()/10.0,RandomNum()/10.0);
    Vector dir(RandomNum(),RandomNum(),RandomNum());
	// If RandPos flag set create a random distribution of agents based on the world
    // BBox size
    if(RandPos == true)
    	pos.set(RandomNum(extents->width/2.0f),
        		RandomNum(extents->height/2.0f),
                RandomNum(extents->depth/2.0f));
	//Construct a new agent to add to the list
    int CS =(int) RandomPosNum((EndSpecies-1)-(StartSpecies-1))+(StartSpecies-1);
    Agent a(pos,SpeciesArray[CS][0],SpeciesArray[CS][1],SpeciesArray[CS][2],SpeciesArray[CS][3]
    		,dir,CS,BrainScript,e);
    //give the Agentsome Random Velocity greater than 0
//    a.SetVelocity(RandomPosNum(1.0f)+0.1f,RandomPosNum(1.0f)+0.1f,RandomPosNum(1.0f)+0.1f);
    // now use push_back to add it to the Agents vector
	Agents.push_back(a);
    }// end for loop
// Save a copy of the BBox
Extents=extents;
/* fill ext array used for the BBox Agent collision detection. For each Plane of the
   BBox we use either width height or depth /2 to determine if the Agent has hit the
   plane or not (see WorldBBoxColission method for usage) */
ext[0]=ext[1]=(Extents->height/2.0f);
ext[2]=ext[3]=(Extents->width/2.0f);
ext[4]=ext[5]=(Extents->depth/2.0f);
// Store the number of agents
nAgents=num;
//set the default values for flags
doCollide=false;
FollowCurve=false;
FollowGoals=false;
DrawExtents=false;
CurvePathTime=0.0f;
nGoals=0;
ActiveGoal=0;

// set the Centroid to be the center of the world (0,0,0) for now
Centroid.set(0.0f,0.0f,0.0f);

// set the mincentroid effect distance to r*2 can be overidden in script file

// Save a copy of the Environment
env=e;
// set a default locked centroid for the AgentEmitter and set the flag to false
LockedCentroid.set(0,0,0);


}//end method






AgentEmitter::~AgentEmitter()
{
// clean up the Memory allocated to each of the vectors by calling the erase
// method for each of the Lists
Agents.erase(Agents.begin(),Agents.end());
Goals.erase(Goals.begin(),Goals.end());
}




// draw the agents as points called from the RenderPoints method
void DrawP(Agent &a)
{
a.DrawPoints();
}
// Render each of the Agents as Points uses the for_each method from algorithm
void AgentEmitter::RenderPoints(void)
{
//for_each(Agents.begin(),Agents.end(),DrawP);
for (unsigned long int i=0; i<nAgents; i++)
	Agents[i].DrawPoints();
//cout <<"Render points "<<this <<" Agent i "<<&Agents[i]<<endl;}
}

/*Draw is used to render the Agent and is passed to the for_each method */
void Draw(Agent &a)
{
// call the agent draw method called from RenderGL
a.Draw();
}



void AgentEmitter::RenderGL(void)
{
// uses the for_each method from algorithm as its quick and easy
	for_each(Agents.begin(),Agents.end(),Draw);

// Now draw the global vector

//and any goals added to the system
for(unsigned long int i=0; i<nGoals; i++)
	  	Goals[ActiveGoal].DrawGoal();
		
// now draw the Centroid
// preserve the current transformation matrix
glPushMatrix();
	// move to the centroid position
    Centroid.Translate();
    glColor3f(1.0f,1.0f,1.0f);
    //draw a small spere at the centroid
    glutSolidSphere(0.2f,4,4);
glPopMatrix();
}// end of method




/*void AgentEmitter:: UpdateAgainstCentroid(void)
{
// the distance of the agent from the centroid
GLfloat dist;
// loop for each of the agents
for (unsigned long int i=0; i<nAgents; i++)
	{
	// calculate the distance of the Agent from the vector
    Vector v(Centroid.x-Agents[i].Pos.x,Centroid.y-Agents[i].Pos.y,
    		 Centroid.z-Agents[i].Pos.z);
    //now use the magnitude to find the length
    dist=v.length();
	// if were too far away we need to update the agents direction
    if(dist > MinCentroidDist )
    	{
        // set a new direction vector to 0
		Vector NewDir(0.0f,0.0f,0.0f,0.0f);
        NewDir.x = (Centroid.x-Agents[i].Pos.x);
        NewDir.y = (Centroid.y-Agents[i].Pos.y);
        NewDir.z = (Centroid.z-Agents[i].Pos.z);
		Agents[i].CentroidDir=NewDir;
        }//end to far away if
    }// end loop for each agent
} // end method
*/
//update the agents
// void AgentUpdate(Agent &a)
// {
// a.Update(Centroid);
// }



void AgentEmitter::CalcCentroid(void)
{
// if were following a path get the current centroid direciton
if(FollowCurve == true )
    {
    Centroid=path->GetPointOnCurve(CurvePathTime);
    }
// else set it to the locked centroid
else if ( LockCentroid == true)
	{
    Centroid.set(LockedCentroid);
    }
// if were following goals set the centroid to the current active goal
else if(FollowGoals == true)
	{
	Centroid=Goals[ActiveGoal].GPos;
    }
else
{
Centroid.set(0.0f,0.0f,0.0f);
// Calculate the Center of the flock based on the average position of the
// Agents in the flock (works well but not entirly accurate)
 for (unsigned long int i=0; i<nAgents; i++)
        {
        Centroid.x+=Agents[i].Pos.x;
        Centroid.y+=Agents[i].Pos.y;
        Centroid.z+=Agents[i].Pos.z;
        }
Centroid.x/=nAgents; Centroid.y/=nAgents; Centroid.z/=nAgents;
}
	
// for (unsigned long int i=0; i<nAgents; i++)
// 	{
//     Agents[i].Centroid=Centroid;
//     }
}

void AgentEmitter::ResetAgents(void)
{

CalcCentroid();
//for_each(Agents.begin(),Agents.end(),AgentUpdate);
for (unsigned long int i=0; i<nAgents; i++)
	Agents[i].Update(Centroid);
}







void AgentEmitter::AddGoal(GLfloat x, GLfloat y, GLfloat z, GLfloat t)
{
//create a new goal
Goal g(x,y,z,t);
// add it to the global goal list
Goals.push_back(g);
// increment the number of goals in the list
nGoals++;
}





//End of Class

