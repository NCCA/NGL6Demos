#ifdef LINUX
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif
#ifdef DARWIN
	#include <unistd.h>
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#endif
#include "OpCode.h"
 
using namespace std;
OpCode::OpCode(void)
{
;
}
OpCode::OpCode (OPCODES t)
{
    // set everything to 0
    type=t; Index=0; Index2=0; Index3=0; ifeval=IFEQUAL; GlobVarIndex=0;
    Floats[0]=Floats[1]=Floats[2]=Floats[3]=0.0;

}


ostream& operator<<(ostream& output, const OpCode & s)
{
// save to file
return output <<s.type<<" "<<s.Index<<" "<<s.Index2<<" "<< s.Index3<<" "<<
			  s.Index4<<" "<< s.Index5<<" "<<s.Index6<<" "<<s.GlobVarIndex<<" "
			  <<s.Floats[0]<< " "<< s.Floats[1]<<" "<< s.Floats[2]<<
              " "<<s.Floats[3]<<" "<<s.ifeval<<endl;
}

istream& operator>>(istream& input, OpCode &s)
{
int a;

input >> a;
// read in from file
input >> s.Index  >>s.Index2>>s.Index3>>s.Index4>>s.Index5>>s.Index6
		 >>s.GlobVarIndex >>s.Floats[0]>>s.Floats[1]>>s.Floats[2]>>s.Floats[3];
         
s.type=(OpCode::OPCODES)a;
input>>a;
s.ifeval=(OpCode::IFEVALUATORS)a;
return input;
}



 
