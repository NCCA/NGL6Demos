#include "LatticeBin.h"
 
using namespace std;	
using namespace GraphicsLib; 


LatticeBin::LatticeBin(void)
{
// set default values
IsEmpty = true;
nAgents = 0;
}
void LatticeBin::ResizeBin(int size)
{
// resize the storage vector
agents.resize(size); 
}
void LatticeBin::setWorldExtents(BBox *Ext,Environment *e)
{
// set the world extents
Extents=Ext; 
ext[0]=ext[1]=(Extents->height/2.0f);
ext[2]=ext[3]=(Extents->width/2.0f);
ext[4]=ext[5]=(Extents->depth/2.0f);
env=e;
}

void LatticeBin::SetExtents(Point3 P[8])
{
// set the bin ectents
for(int i=0; i<8; i++)
    p[i]=P[i];

}
 
LatticeBin::~LatticeBin()
{
agents.erase(agents.begin(),agents.end());
}

void LatticeBin::DoAgentCollideRoutines(void)
{
//cout <<"***************** doing collide *************"<<endl;
//cout <<"In this bin there are "<<nAgents<<endl;

for(unsigned int i=0; i<nAgents; i++)
	{
    //agents[i]->Pos.Print("int collide");
    agents[i]->CollideFunction(this,i);
    }
//cout <<"***************** end collide *************"<<endl;
}


void LatticeBin::InBin(Agent *a)
{
// check to see if the Agent is in the bin
GLfloat x,y,z;
x=a->Pos.x;
y=a->Pos.y;
z=a->Pos.z;

// we only need to check a few points 

if((x>p[0].x && x<p[3].x) && (y>p[0].y &&y<p[4].y) &&(z>p[0].z && z<p[1].z))
	{
    agents[nAgents]=a;
	nAgents++;
	if(nAgents>1)
		IsEmpty=false;
	}

}





void LatticeBin::Draw(void)
{
// draw the bin points
for(int i=0; i<8; i++)	
	{
	 glPushMatrix();
	 	p[i].Translate();
		glColor3f((float)i/10.0,(float)i/10.0,(float)i/10.0);
		glutWireSphere(0.5,8,8);
	 glPopMatrix();
	}	
}
void LatticeBin::Reset(void)
{
// reset the bin to have no agents in it
IsEmpty=true;
nAgents=0;
}









