#include "Agent.h"
#include "perlin.h"
using namespace std;
using namespace ngl; 

Agent::Agent(Vec3 pos,GLfloat width,GLfloat height,GLfloat depth,
         GLfloat radius,Vec3 dir,int species,char *BrainScript,Environment *e)
{
CollideFlag=false;
brain = new Brain(BrainScript,this);
env=e;
//Setup Agent Attributes
Pos=pos;
//PreviousPos=pos; 
//Dir=dir;
Extents = new BBox(pos,width,height,depth);
noise = new Noise();
useNoise=false;
noiseScale=0.0;

}

Agent::Agent(void)
{
// dummy default constructor so we can setup the AgentEmitter containers if we need to
}

Agent::~Agent()
{
//don't need anything as yet no dynamic memory allocation in the Agents
//delete brain;
}

void Agent::DrawPoints(void)
{

// // push the current transformation stack
 glPushMatrix();
// 	//set the colour of the Point based on the species
//     // set the size of the point based on the species
//     // Draw the Point (note Vec3 has a call to glVertex3f embedded in the class)
     glColor3f(1,1,1);
     glPointSize(1.0);
     glBegin(GL_POINTS);
      //Pos.Vertex();
        glVertex3fv(Pos.m_openGL);
     glEnd();
// 	//reset the point size
//     glPointSize(1.0f);
// //finally reset the transformation stack
 glPopMatrix();
}


void Agent::Draw(void)
{
// call the brain draw funcion
brain->Draw(this);
}


 



void Agent::Update(Vec3 C)
{
// get the height from the gp
GPYlevel=gp->GetHeight(Pos.m_x,Pos.m_z);//GPYlevel;
Centroid = C;
// call the brain update routine
brain->Update(this);

}


// set the Agents noise values

void Agent::SetNoise(GLfloat Scale, int Type)
{
// set the noise
noiseScale=Scale;
useNoise=true;
NoiseType=Type;
}

Vec4 Agent::GetNoise(GLfloat nx,GLfloat ny, GLfloat nz)
{
 // get the noise value
 GLfloat v[3];
 // determine which noise type and calculate the noise values.
 float X=0.0; float Y=0.0; float Z=0.0;
 Vec3 NV(nx,ny,nz);
 switch (NoiseType)
 {
 case 0:
	X=noise->turbulance(noiseScale,NV);
    Y=noise->turbulance(noiseScale,NV);
    Z=noise->turbulance(noiseScale,NV);
break;

case 1 :
  X=noise->marble(NV.m_x,noiseScale,NV);
    Y=noise->marble(NV.m_y,noiseScale,NV);
    Z=noise->marble(NV.m_z,noiseScale,NV);
 break;
case 2 :
    X=noise->undulate(nx);
    Y=noise->undulate(ny);
    Z=noise->undulate(nz);
break;
case 3 :

	v[0]=nx; v[1]=ny; v[2]=nz;
    X=noise3(v); Y=noise3(v); Z=noise3(v);
break;
case 4 :

	v[0]=nx; v[1]=ny; v[2]=nz;
    X=turbulence(v,noiseScale); Y=turbulence(v,noiseScale); Z=turbulence(v,noiseScale);
break;

}
Vec4 ret(nx,ny,nz,0.0);
return ret;
}

void Agent::CollideFunction(LatticeBin *bin,unsigned int AgentIndex)
{
	brain->Collide(this,bin,AgentIndex);
}


//End of File Agent.cpp
