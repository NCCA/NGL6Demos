#include "OpCodeList.h"
using namespace std;
void OpCodeList::DebugOpCodes(OpCode::OPCODES type,unsigned int i)
{
switch(type)
	{
	case  OpCode::LINELOOP :
    case  OpCode::LINES :		
        cout <<"OP: LineLoop"<<endl;
	break;
	case  OpCode::POINT :
    	cout <<"OP: Point"<<endl;
	break;
	case OpCode::POINTSIZE :
		cout <<"OP: PointSize"<<endl;
    break;
    case OpCode::LINESIZE :
    	cout <<"OP: LineSize"<<endl;
    break;
    case OpCode::SCALE :
    	cout <<"OP: Scale"<<endl; //op[i].Floats[0]<<","<<op[i].Floats[1]<<","<<op[i].Floats[2]<<endl;
    break;
    case  OpCode::POLYGON :
		cout <<"OP: Polygon"<<endl;
	break;
	
	case  OpCode::QUAD :
		cout <<"OP: Quad"<<endl;
	break;
	
	case  OpCode::GLEND :
		cout <<"OP: glEnd"<<endl;
	break;
	case OpCode::POPMATRIX :
		cout <<"OP: PopMatrix"<<endl;
	break;
	case OpCode::PUSHMATRIX :
		cout <<"OP: PushMatrix"<<endl;
	break;
	case OpCode::SPHERE :
    case OpCode::SOLIDSPHERE :
    	cout <<"OP: Sphere"<<endl;
    break;	
    case OpCode::CYLINDER :
    	cout <<"OP: Cylinder"<<endl;
    break;	
	case OpCode::ROTATEX :
    	cout <<"OP: RotateX"<<endl;
    break;	
	case OpCode::ROTATEY :
    	cout <<"OP: RotateY"<<endl;
    break;	
    case OpCode::ROTATEZ :
    	cout <<"OP: RotateZ"<<endl;
    break;	
	case OpCode::TRANSLATE :
    	cout <<"OP: Translate"<<endl;
    break;
    case OpCode::VERTEX :
    	cout <<"OP: Vertex"<<endl;
    break;
    case OpCode::VERTEXF :
         cout <<"OP: VertexF"<<endl;
    break;
    
    case OpCode::COLOUR :
        cout <<"OP: Colour"<<endl;
    break;
    
    case OpCode::ADDDIR :
        cout <<"AddDir"<<endl;
    break;
    case OpCode::REVERSE :
        cout <<"OP: Reverse"<<endl;
    break;
    
    case OpCode::SUBDIR :
    	cout <<"OP: SubDir"<<endl;
    break;
    case OpCode::SETDIR :
    	cout <<"OP: SetDir"<<endl;
    break;
    case OpCode::RANDOMIZE :
        cout <<"OP: Randomize"<<endl;
    break;
    case OpCode::RANDOMIZEPOS :
        cout <<"OP: RandomizePos"<<endl;
    break;
    case OpCode::NORMALIZE :
        cout <<"OP: Normalize"<<endl;
    break;
    
    case OpCode::MUL :
        cout <<"OP: Mul"<<endl;
    break;
    case OpCode::DIV :
        cout <<"OP: Div"<<endl;
    break;
    
    case OpCode::ADD :
           cout <<"OP: Add"<<endl;
    break; // end ADD

    case OpCode::SUB :
        cout <<"OP: Sub"<<endl;
    break; // end sub
    
    case OpCode::SET :
          cout <<"OP: Set"<<endl;
    break; // end set
    case OpCode::FPUSH :
        cout <<"OP: FPush"<<endl;
    break; //end fpush
    
    case OpCode::DEBUG :
        cout <<"OP: Debug"<<endl;
    break; //fatan
    case OpCode::FSTACKTRACE :
        cout <<"OP: FstackTrace"<<endl;
    break;
    
    case OpCode::LENGTH :
        cout <<"OP: Length"<<endl;
    break; //fatan
    case OpCode::FATAN :
	cout <<"OP: Fatan"<<endl;
    break; //fatan
    case OpCode::FSQRT :
	cout <<"OP: Fsqrt"<<endl;
    break; //sqrt
    case OpCode::FCOS :
		cout <<"OP: Fcos"<<endl;
    break; //fcos
    case OpCode::FACOS :
    	cout <<"OP: Facos"<<endl;
    break; //facos
    case OpCode::FSIN :
		cout <<"OP: Fsin"<<endl;
    break; //fcos
    case OpCode::FASIN :
		cout <<"OP: FAsin"<<endl;
    break; //facos
    case OpCode::FRAD2DEG : 
        cout <<"OP: Frad2deg"<<endl;
    break; //frad2deg
    case OpCode::FNEGATE :
        cout <<"OP: Fnegate"<<endl;
    break; //fnegate
    case OpCode::FDIV :
	cout <<"OP: Fdiv"<<endl;
    break; //FDIV
    case OpCode::FADD :
	cout <<"OP: Fadd"<<endl;
    break;
    case OpCode::FSUB :
	cout <<"OP: Fsub"<<endl;
    break;
    case OpCode::FMUL :
	cout <<"OP: Fmul"<<endl;
    break;
    case OpCode::FDUP :
	    cout <<"OP: Fdup"<<endl;
    break;
    case OpCode::FPOP :
      	cout <<"OP: Fpop"<<endl;
    break; // end set
	case OpCode::CALL :
        cout <<"OP: Call"<<endl;
    break; // end call
    
	case OpCode::IF :
        cout <<"OP: If"<<endl;
    break; //end if        
    case OpCode::USEAGENTRENDER :
        cout <<"OP: UseAgentRender"<<endl;
    break;
    case OpCode::SETANIMCYCLE :
        cout <<"OP: SetAnimCycle"<<endl;
    break;
    case OpCode::RENDERAGENT :
        cout <<"OP: RenderAgent"<<endl;
    break;
    
    case OpCode::SETGLOBALPOS :
        cout <<"OP: SetGlobalPos"<<endl;
    break; // end SetGlobalPos
    case OpCode::GETGLOBALPOS :
        cout <<"OP: GetGlobalPos"<<endl;
      
    break; // end SetGlobalPos

     case OpCode::SETGLOBALCENTROID :
        cout <<"OP: SetGlobalCentroid"<<endl;
    break; // end SetGlobalPos
    case OpCode::GETGLOBALCENTROID :
        cout <<"OP: GetGlobalCentroid"<<endl;
    break; // end SetGlobalPos
    case OpCode::LOOPBIN :
    cout <<"OP: LoopBin"<<endl;
    break;
    
    case OpCode::SPHERESPHERE :
        cout <<"OP: SphereSphereCollision"<<endl;
    break;
    case OpCode::CYLINDERCYLINDER :
        cout <<"OP: CylinderCylinderCollision"<<endl;
    break;
    
    case OpCode::BEEP :
      cout <<"OP: Beep"<<endl;
    break;
    case OpCode::GETAGENTI :
	    cout <<"OP: GetAgentI"<<endl;
    break;
    
    case OpCode::SETAGENTI :
    cout <<"OP: SetAgentI"<<endl;
    break;
    
    case OpCode::DIVDIR :
        cout <<"OP: DIVDIR"<<endl;
    break;
    
 	case OpCode::IFELSE :
    cout <<"OP: IfElse"<<endl;
    break;  
    
    case OpCode::PUSHGPYLEVEL :
        cout <<"OP: PushGPYLevel"<<endl;
    break ;
    case OpCode::SPHEREPLANE :
     cout <<"OP: SpherePlaneCollision"<<endl;
    break;
    case OpCode::SPHEREENVOBJ :
     cout <<"OP: SphereEnvObj"<<endl;
    break;

    case OpCode :: RENDERFRAME :
        cout <<"OP: RenderFrame"<<endl;
    break;
    
    case OpCode::END :
    cout <<"OP: End"<<endl;
    break;
    case OpCode::OPENBRACE : break;
    cout <<"OP: {"<<endl;
    case OpCode::CLOSEBRACE : 
    cout <<"OP: }"<<endl;
    break;
    case OpCode::CALLLIST :
    cout <<"OP: CallList"<<endl;
    break;
    case OpCode::DEBUGOPON :
    break;
    
    case OpCode::DEBUGOPOFF :
    break;
    case OpCode::SETGLOBALCOLLIDEFLAG :
    	cout <<"OP: SetGlobalCollideFlag"<<endl;
    break;
    
    case OpCode::GETGLOBALCOLLIDEFLAG :
    	cout <<"OP: GetGlobalCollideFlag"<<endl;
    break;
    case OpCode::DOT :
    	cout <<"OP: Dot"<<endl;
    break;
    }// end switch

}
