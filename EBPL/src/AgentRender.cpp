#include "AgentRender.h"
#include <iostream>
using namespace std;


AgentRender::AgentRender(void)
{
// load the obj's in 
anims[0]= new AnimatedObj("../Models/TAgentWalk1.obj","../Models/TAgentWalk2.obj", 8,AnimatedObj::LOOPANIM,5);
anims[1]= new AnimatedObj("../Models/TAgentRun1.obj","../Models/TAgentRun2.obj", 8,AnimatedObj::LOOPANIM,5);
anims[2]= new AnimatedObj("../Models/TAgentNeutral.obj","../Models/TAgentNeutral.obj", 1,AnimatedObj::LOOPANIM,5);
anims[3]= new AnimatedObj("../Models/TAgentDead.obj","../Models/TAgentDead.obj", 1,AnimatedObj::LOOPANIM,5);
anims[4]= new AnimatedObj("../Models/TAgentPunch1.obj","../Models/TAgentPunch2.obj", 8,AnimatedObj::LOOPANIM,5);
anims[5]= new AnimatedObj("../Models/TAgentSwing1.obj","../Models/TAgentSwing2.obj", 8,AnimatedObj::LOOPANIM,5);

CurrentCycle=0;

}

AgentRender::~AgentRender()
{
// tidy up
delete anims[0];
delete anims[1];
delete anims[2];
delete anims[3];
delete anims[4];
delete anims[5];
}



void AgentRender::Draw(void)
{
// turn on the lights and draw the agent 
glEnable(GL_LIGHTING);
anims[CurrentCycle]->Draw();
glDisable(GL_LIGHTING);

}

void AgentRender::SetMaterial(int mat)
{
// set the material for the agent drawing
anims[CurrentCycle]->SetMaterial(mat);
}

void AgentRender::ChangeCycle(int c)
{
// set the cycle to render
CurrentCycle=c;

}
void AgentRender::UpdateFrame(void)
{
	// Add 1 to the current frame no this is controled in the animated obj file
    anims[CurrentCycle]->UpdateFrameNo();
}

void AgentRender::SetCurrentFrame(int fn)
{
// set the frame number directly
anims[CurrentCycle]->SetFrameNo(fn);
}



