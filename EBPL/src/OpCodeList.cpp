#include "OpCodeList.h"
#include <math.h>
#ifdef LINUX
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif
#ifdef DARWIN
	#include <unistd.h>
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#endif
#include "Util.h"
#include "Agent.h"
#include <ngl/Util.h>
using namespace std;
using namespace ngl;
/*! draw a cylinder as glut doesn't have one built in
 \param float base the base radius
 \param float height the height of the cylinder
 \param int slices the number of slices in the width
 \param the number of stacks in the height */
void Cylinder(float base,float height,int slices,int stacks)
{
static GLUquadricObj *quadObj;
quadObj = gluNewQuadric();
 gluQuadricDrawStyle(quadObj, GLU_LINE);
 gluQuadricNormals(quadObj, GLU_SMOOTH);
  /* If we ever changed/used the texture or orientation state
	  of quadObj, we'd need to change it to the defaults here
	  with gluQuadricTexture and/or gluQuadricOrientation. */
  gluCylinder(quadObj, base, base, height, slices, stacks);
 
}
 
 OpCodeList::OpCodeList(void)
{
// set default
DoingIFELSE=false;
ifelsejump=0;
ifelseend=0;
DebugMode=false;
}


void OpCodeList::Add(OpCode o)
{
// add the opcode to the list
op.push_back(o);
//cout <<"pushing back opcode "<<endl;
}

void OpCodeList::ExecOpCodes(Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex)
{
// loop through and execute all the opcodes in the list
unsigned int i=0;
// i is the current index which can change if we doing a call or a if / ifelse
while(i<op.size()){
  unsigned int ret=Exec(op[i].type,i,br,Me,bin,AgentIndex);
  if(ret == i)
  	i++;
  else i=ret;

};

}

unsigned int OpCodeList::Exec(OpCode::OPCODES type,unsigned int i,Brain *br,Agent *Me,LatticeBin *bin,unsigned int AgentIndex)
{
float a,b;
unsigned int LoopPos;
unsigned int LoopEnd;

 //for(unsigned int i=0; i<op.size(); i++)
 {
 OpCode::OPCODES type;
 type = op[i].type;
 if(DoingIFELSE==true)
 {
 
 
 if(i==ifelseend)
 	{
    i+=ifelsejump;
    DoingIFELSE = false;
    }
 }
//cout <<"Executing Opcode "<<i<<" type "<<op[i].type <<endl;
if(DebugMode==true)
	{
	DebugOpCodes(type,i);
	}
    
  // big case statement to do the opcodes this is not nice and needs work
switch(type)
	{
	case  OpCode::LINELOOP :
		glBegin(GL_LINE_LOOP);
	break;
	case  OpCode::LINES :
		glBegin(GL_LINES);
	break;
	
    case  OpCode::POINT :
		glBegin(GL_POINTS);
	break;
	case OpCode::POINTSIZE :
    	glPointSize(op[i].Floats[0]);
    break;
    case OpCode::ENABLELIGHTS :
    	glEnable(GL_LIGHTING);
    break;
    case OpCode::DISABLELIGHTS :
    	glDisable(GL_LIGHTING);
    break;
    case OpCode::LINESIZE :
    	glLineWidth(op[i].Floats[0]);
    break;
    case OpCode::SCALE :
    	glScalef(op[i].Floats[0],op[i].Floats[1],op[i].Floats[2]);
    break;
    case  OpCode::POLYGON :
		glBegin(GL_POLYGON);
	break;
	
	case  OpCode::QUAD :
		glBegin(GL_QUADS);
	break;
	
	case  OpCode::GLEND :
		glEnd();
	break;
	case OpCode::POPMATRIX :
		glPopMatrix(); //cout <<"PopMatrix"<<endl;
	break;
	case OpCode::PUSHMATRIX :
		glPushMatrix(); //cout <<"PushMatrix"<<endl;
	break;
	case OpCode::SPHERE :
    
    	if(op[i].GlobVarIndex == OpCode::GuardValue)
        	glutWireSphere(op[i].Floats[0],(int)op[i].Floats[1],(int)op[i].Floats[2]);
        else
            glutWireSphere(v->v[op[i].GlobVarIndex].Float,(int)op[i].Floats[1],(int)op[i].Floats[2]);
    break;	
    case OpCode::CUBE :
    
    	if(op[i].GlobVarIndex == OpCode::GuardValue)
        	glutWireCube(op[i].Floats[0]);
        else
            glutWireCube(v->v[op[i].GlobVarIndex].Float);
    break;	
    
    case OpCode::SOLIDSPHERE :
    
    	if(op[i].GlobVarIndex == OpCode::GuardValue)
        	glutSolidSphere(op[i].Floats[0],(int)op[i].Floats[1],(int)op[i].Floats[2]);
        else
            glutSolidSphere(v->v[op[i].GlobVarIndex].Float,(int)op[i].Floats[1],(int)op[i].Floats[2]);
    break;	
    case OpCode::CYLINDER :
    	if(op[i].GlobVarIndex == OpCode::GuardValue)
        	Cylinder(op[i].Floats[0],op[i].Floats[0],(int)op[i].Floats[1],(int)op[i].Floats[2]);
        else
            Cylinder(v->v[op[i].GlobVarIndex].Float,v->v[op[i].GlobVarIndex].Float,(int)op[i].Floats[1],(int)op[i].Floats[2]);
    break;	
	case OpCode::ROTATEX :
    	if(op[i].GlobVarIndex == OpCode::GuardValue)
        	glRotatef(op[i].Floats[0],1,0,0);
        else
            glRotatef(v->v[op[i].GlobVarIndex].Float,1,0,0);
            
    break;	
	case OpCode::ROTATEY :
    	if(op[i].GlobVarIndex == OpCode::GuardValue)
        	glRotatef(op[i].Floats[0],0,1,0);
        else
            glRotatef(v->v[op[i].GlobVarIndex].Float,0,1,0);
    break;	
    case OpCode::ROTATEZ :
    	if(op[i].GlobVarIndex == OpCode::GuardValue)
        	glRotatef(op[i].Floats[0],0,0,1);
        else
            glRotatef(v->v[op[i].GlobVarIndex].Float,0,0,1);
    	
    break;	
	case OpCode::TRANSLATE :
    	v->v[op[i].GlobVarIndex].Translate();
    
    break;
    case OpCode::VERTEX :
    	v->v[op[i].GlobVarIndex].Vertex();
    break;
    case OpCode::SMOOTH :
    	glShadeModel(GL_SMOOTH);
    break;
    case OpCode::FLAT :
    	glShadeModel(GL_FLAT);
    break;
    case OpCode::VERTEXF :
         glVertex3f(op[i].Floats[0],op[i].Floats[1],op[i].Floats[2]);
    break;
    case OpCode::RENDERMATERIAL :
    if(op[i].GlobVarIndex == OpCode::GuardValue)
    	br->SetMaterialColour((int)op[i].Floats[0]);
    else
        br->SetMaterialColour((int)v->v[op[i].		Index].Float);
        
    break; 
    case OpCode::COLOUR :
     
    if(op[i].GlobVarIndex == OpCode::GuardValue)
    	glColor3f(op[i].Floats[0],op[i].Floats[1],op[i].Floats[2]);
    else
        {
        glColor3f(v->v[op[i].Index].Point.m_x,v->v[op[i].Index].Point.m_y,v->v[op[i].Index].Point.m_z);
        }
    break;
    
    case OpCode::ADDDIR :
    	ProcessAddDir(br,Me,bin,AgentIndex,i);
    break;
    case OpCode::MULDIR :
    	ProcessAddDir(br,Me,bin,AgentIndex,i);
    break;
    
    case OpCode::REVERSE :
    	ProcessReverse(br,Me,bin,AgentIndex,i);    
    break;
    
    case OpCode::SUBDIR :
    	ProcessSubDir(br,Me,bin,AgentIndex,i);    
    break;
    case OpCode::SETDIR :
    	ProcessSetDir(br,Me,bin,AgentIndex,i);
    break;
    case OpCode::RANDOMIZE :
    	ProcessRandomize(br,Me,bin,AgentIndex,i);
    break;
    case OpCode::RANDOMIZEPOS :
    	ProcessRandomizePos(br,Me,bin,AgentIndex,i);
    break;
    case OpCode::NORMALIZE :
    	ProcessNormalize(br,Me,bin,AgentIndex,i);
    break;
    
    case OpCode::MUL :
    	ProcessMul(br,Me,bin,AgentIndex,i);
    break;
    case OpCode::DIV :
    	ProcessDiv(br,Me,bin,AgentIndex,i);
    break;
    
    case OpCode::ADD :
    	   ProcessAdd(br,Me,bin,AgentIndex,i);
    break; // end ADD

    case OpCode::SUB :
    	ProcessSub(br,Me,bin,AgentIndex,i);
    break; // end sub
    
    case OpCode::SET :
          ProcessSet(br,Me,bin,AgentIndex,i);
    break; // end set
    
    
    case OpCode::FPUSH :
    	ProcessFpush(br,Me,bin,AgentIndex,i);
    break; //end fpush
    case OpCode::FPUSHD :
    	br->fstack.push(op[i].Floats[0]);
        
    break; //end fpush
    
    case OpCode::DEBUG :
    	cout <<v->v[op[i].GlobVarIndex]<<endl;
    break; //fatan
    case OpCode::FSTACKTRACE :
    	cout <<"OpCode Number "<<i <<endl;
        br->fstack.StackTrace();
    break;
    
    case OpCode::LENGTH :
    	a=v->v[op[i].GlobVarIndex].vect.length();
        br->fstack.push(a);
    break; //fatan
    case OpCode::FATAN :
    	a=br->fstack.pop();
    	b=br->fstack.pop();
       	br->fstack.push(atan2(a,b));
    break; //fatan
    case OpCode::FSQRT :
    	a=br->fstack.pop();
        br->fstack.push(sqrt(a));
    break; //sqrt
    case OpCode::FCOS :
    	a=br->fstack.pop();
    	br->fstack.push(cos(a));
    break; //fcos
    case OpCode::FACOS :
    	a=br->fstack.pop();
    	br->fstack.push(acos(a));
    break; //facos
    case OpCode::FSIN :
    	a=br->fstack.pop();
    	br->fstack.push(sin(a));
    break; //fcos
    case OpCode::FASIN :
    	a=br->fstack.pop();
    	br->fstack.push(asin(a));
    break; //facos
    case OpCode::FRAD2DEG : 
    	a=br->fstack.pop();
        br->fstack.push(ngl::degrees(a));
    break; //frad2deg
    case OpCode::FNEGATE :
    	a=br->fstack.pop();
    	br->fstack.push(-a);
    break; //fnegate
    case OpCode::FDIV :
    	a=br->fstack.pop();
        b=br->fstack.pop();
        if(a !=0 || b !=0)
        	br->fstack.push(a/b);
        else {
        //	  cout << "POSS DIV by 0 Error putting 0 on stack"<<endl;
         	  br->fstack.push(1);
         }
    break; //FDIV
    case OpCode::FADD :
    	a=br->fstack.pop();
        b=br->fstack.pop();
        br->fstack.push(a+b);
    break;
    case OpCode::FSUB :
    	a=br->fstack.pop();
        b=br->fstack.pop();
        br->fstack.push(a-b);
    break;
    case OpCode::FMUL :
    	a=br->fstack.pop();
        b=br->fstack.pop();
        br->fstack.push(a*b);
    break;
    case OpCode::FDUP :
     	a=br->fstack.pop();
        br->fstack.push(a);
    	br->fstack.push(a);
    break;
    case OpCode::FPOP :
      	ProcessFpop(br,Me,bin,AgentIndex,i);
    break; // end set
	case OpCode::CALL :
    	br->ExecuteOpCode(op[i].GlobVarIndex,Me);
    break; // end call
    
	case OpCode::IF :
    //cout <<"I before "<<i;
    	i=ProcessIf(br,Me,bin,AgentIndex,i);
        //cout <<"I after "<<i<<endl;
    break; //end if        
    case OpCode::USEAGENTRENDER :
    	br->ToggleAgentRender();
    break;
    case OpCode::SETANIMCYCLE :
    	//cout <<"Anim Cycle "<<(int )v->v[op[i].GlobVarIndex].Float<<endl;
        br->ChangeAnimCycle((int )v->v[op[i].GlobVarIndex].Float);
    break;
    case OpCode::RENDERAGENT :
    	br->RenderAgent();
    break;
    
    case OpCode::SETGLOBALPOS :
    	ProcessSetGlobalPos(br,Me,bin,AgentIndex,i);
    break; // end SetGlobalPos
    case OpCode::GETGLOBALPOS :
    	ProcessGetGlobalPos(br,Me,bin,AgentIndex,i);
      
    break; // end SetGlobalPos
	case OpCode::SETGLOBALDIR :
    	ProcessSetGlobalDir(br,Me,bin,AgentIndex,i);
    break; // end SetGlobalPos
    case OpCode::GETGLOBALDIR :
    	ProcessGetGlobalDir(br,Me,bin,AgentIndex,i);
      
    break; // end SetGlobalPos

     case OpCode::SETGLOBALCENTROID :
        ProcessSetGlobalCentroid(br,Me,bin,AgentIndex,i);
    break; // end SetGlobalPos
    case OpCode::GETGLOBALCENTROID :
    	ProcessGetGlobalCentroid(br,Me,bin,AgentIndex,i);
    break; // end SetGlobalPos
    case OpCode::LOOPBIN :
	// DO THIS BIT NEXT !!
    // store place to jump back to
    LoopPos=i+1;    
    LoopEnd=op[i].Index+1;
    //cout <<"doing loopbin"<< LoopPos<<" LoopEnd "<<LoopEnd<<endl;
    
    for(unsigned int ii=0; ii<bin->getNagents(); ii++)
    	{
        //cout <<"AgentIndex "<<AgentIndex <<" ii "<<ii<<endl;
        //cout <<"Me "<<Me <<" " << bin->agents[ii]<<endl;
        /*if(Me==bin->agents[ii])
          continue;
        else
        *//*    {
            cout <<"doing opcodes"<<endl;*/
        	for(unsigned int OpEx=LoopPos; OpEx<LoopEnd; OpEx++)
        		{
		        OpEx=Exec(op[OpEx].type,OpEx,br,Me,bin,ii);
        	    }
        	}
         
        //}
        //cout << "End loop Bin"<<endl;
    break;
    
    case OpCode::SPHERESPHERE :
        ProcessSphereSphere(br,Me,bin,AgentIndex,i);
    break;
    case OpCode::CYLINDERCYLINDER :
        //cout <<"doing cylcyl"<<endl;
        ProcessCylinderCylinder(br,Me,bin,AgentIndex,i);
    break;
    
    case OpCode::BEEP :
      printf("\a");
    break;
    case OpCode::GETAGENTI :
    // GlobVarIndex == Me Agent
    // Index == Agent I
    //copy AgentI into Me Agent;
    ProcessGetAgentI(br,Me,bin,AgentIndex,i);
    break;
    
    case OpCode::SETAGENTI :
    ProcessSetAgentI(br,Me,bin,AgentIndex,i);
    break;
    
    case OpCode::DIVDIR :
    	ProcessDivDir(br,Me,bin,AgentIndex,i);
    break;
    
 	case OpCode::IFELSE :
	i=ProcessIfElse(br,Me,bin,AgentIndex,i);
    break;  
    
    case OpCode::PUSHGPYLEVEL :
    	br->fstack.push(Me->GPYlevel);
    break ;
    case OpCode::SPHEREPLANE :
     ProcessSpherePlane(br,Me,bin,AgentIndex,i);
    break;
    case OpCode::SPHEREENVOBJ :
     ProcessSphereEnvObj(br,Me,bin,AgentIndex,i);
    break;

    case OpCode :: RENDERFRAME :
    	br->SetCurrentFrame((int )v->v[op[i].GlobVarIndex].Float);
    break;
    
    case OpCode::END :
    break;
    case OpCode::OPENBRACE : break;
    //cout <<"Opcode {"<<endl; break;
    case OpCode::CLOSEBRACE : break;
    case OpCode::CALLLIST :
      br->ExecuteOpCode(br->CallLists[(unsigned int)op[i].Floats[0]].o[(unsigned int)v->v[op[i].Index].Float],Me);
    
    break;
    case OpCode::DEBUGOPON :
     DebugMode=true;
    break;
    
    case OpCode::DEBUGOPOFF :
     DebugMode=false;
    break;
    case OpCode::SETGLOBALCOLLIDEFLAG :
    
    break;
    
    case OpCode::GETGLOBALCOLLIDEFLAG :
    
    break;
    case OpCode::DOT :
      ProcessDotProduct(br,Me,bin,AgentIndex,i);
    break;
    case OpCode::SETGPYLEVEL :
     ProcessSetGPYlevel(br,Me,bin,AgentIndex,i);
    break;
    case OpCode::GETNOISEVALUE :
      ProcessGetNoiseValue(br,Me,bin,AgentIndex,i);
    break;
    case OpCode::USENOISE :
     ProcessSetNoise(br,Me,bin,AgentIndex,i);
    break;
    
    }// end switch
    
    return i;
}

}


void OpCodeList::Save(ofstream *Op)
{
cout <<"saving "<< op.size()<<endl;
*Op<<op.size()<<endl;
for(unsigned int ii=0; ii<op.size(); ii++)
	{
    *Op<<op[ii];
    }

}

void OpCodeList::Print(void)
{
for(unsigned int ii=0; ii<op.size(); ii++)
	cout<<op[ii];
}
void OpCodeList::Load(std::ifstream *ip)
{
unsigned int size;
*ip>>size;
for(unsigned int ii=0; ii<size; ii++)
	{
    OpCode in;
    *ip>>in;
    op.push_back(in);
    }


} 




