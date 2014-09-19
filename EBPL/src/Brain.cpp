#include "Brain.h"

using namespace std;

Brain::Brain()
{
// simple constructor
UseAgentRender=false;
}

Brain::Brain(char *fname,Agent *Me)
{
// load in the compiled brain script file
UseAgentRender=false;
Load(fname,Me);
}

void Brain::SetAgentRender(AgentRender *ar)
{
// assign the global agent render pointer
render=ar;
}
Brain::~Brain()
{
// clean up the opcodes etc
Functions.erase(Functions.begin(),Functions.end());
}
void Brain::ChangeAnimCycle(int c)
{
// change the anim cycle of the agent render
render->ChangeCycle(c);
}
void Brain::SetMaterialColour(int c)
{
// set the material colour for the agent render
render->SetMaterial(c);
}

void Brain::Load(char *fname,Agent *Me)
{
// load in the compiled brain file
ifstream ip;
ip.open(fname);
if (!ip) {
       cerr << "Unable to open file " << fname<<endl;
       exit(1); // call system to stop
    }
// the globals are the first data chunk in the file
Globals.Load(&ip);
// next we have a uint to say how many functions we are loading in
unsigned int numFuncs;
ip>>numFuncs;
// make room for the funcitons
Functions.resize(numFuncs);
for(unsigned int i=0; i<numFuncs; i++)
	{
    // load them in 
    Functions[i].Load(&ip);
    // and make the functions aware of the global variables
    Functions[i].v=&Globals;
    }
// now load in the call lists
ip>>numFuncs;
CallLists.resize(numFuncs);
for(unsigned int i=0; i<numFuncs; i++)
	{
	CallLists[i].Load(&ip);
    }
// now call the init funciton
Functions[INIT].ExecOpCodes(this,Me,NULL,0);
ip.close();
}

void Brain::CallInitFunction(Agent *Me)
{
// call the init function
Functions[INIT].ExecOpCodes(this,Me,NULL,0);
}
void Brain::ExecuteOpCode(unsigned int index,Agent *Me)
{
//execute the opcode of index
Functions[index].ExecOpCodes(this,Me,NULL,0);
}

void Brain::Update(Agent *Me)
{
// call the built in update funciton
Functions[UPDATE].ExecOpCodes(this,Me,NULL,0);
}
void Brain::RenderAgent(void)
{
  // call the render agent module draw function
  render->Draw();
}
void Brain::Draw(Agent *Me)
{
	// call the built in draw funciton
    Functions[DRAW].ExecOpCodes(this,Me,NULL,0);
}

void Brain::Collide(Agent *Me,LatticeBin *bin,unsigned int AgentIndex)
{
// call the built in collide function
Functions[COLLIDE].ExecOpCodes(this,Me,bin,AgentIndex);
}
    
void Brain::Save(char *fname)
{
/*! \todo write the funciton to save the brain */
}

void Brain::Print(void)
{
// print out debug info
cout <<"Brain Dump "<<endl;
cout <<"********************************"<<endl;
Globals.Print();
cout <<"** FUNCTIONS ******************"<<endl;
for(unsigned int i=0; i<Functions.size(); i++)
	{
    cout <<"Function >>"<<i<<endl;
    Functions[i].Print();
    }
 	fstack.StackTrace();
}

void Brain::Debug(void)
{
// this has been superceeded by the built in debug function opcodes
}
void Brain::SetCurrentFrame(int fn)
{
// set the anim cycle
render->SetCurrentFrame(fn);
}

