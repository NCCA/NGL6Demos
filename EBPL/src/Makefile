#first we find what OS were on and define OSVER to allow the different versions of the code
OSVER := $(shell uname)
# on a mac so we define DARWIN
ifeq "$(OSVER)" "Darwin"
	ARCH=_DARWIN_
endif
#Linux box so we define _LINUX_
ifeq "$(OSVER)" "Linux"
	ARCH=_LINUX_
endif
#########################################################################################
# INCDIR is the flag to set the default include paths for the system
#########################################################################################
INCDIR = -I../include -I/usr/include -I$(HOME)/GraphicsLib/include

ifeq "$(OSVER)" "Linux"
	INCDIR+=-I/usr/X11R6/include
endif
ifeq "$(OSVER)" "Darwin"
	INCDIR+=-I/usr/local/
endif
#########################################################################################
# CCFLAGS are the compile flags for compiling the code
# -g include debug info (for use with ddd or gdb) can be removed from exe using strip
# -Wall display all warnings
# -DUNIX - GraphicsLib define to set some UNIX options
# -funroll-loops unroll static loops (i.e) if loop has hard coded values these will be
# 				 replaced with more optimised procedural sections increases code size
#				 and MAY increase speed
# -O3 turn on all optimisations
# Note :- for more options and flags type man g++ (there are a lot of arch specific options)
#########################################################################################
CCFLAGS = -g -Wall -DUNIX -funroll-loops -O3 -DUNIX
LIBS=
ifeq "$(OSVER)" "Linux"
	CCFLAGS+=-DLINUX
endif
ifeq "$(OSVER)" "Darwin"
	CCFLAGS+=-DDARWIN  -bind_at_load
endif

OBJECTS = FlockMain.o  GlutCallback.o Agent.o AgentEmitter.o Environment.o  \
   	  EnvObj.o ReadParamFile.o Goal.o  \
          AnimatedObj.o VectObj.o GroundPlane.o    \
	  Lattice.o LatticePoint.o LatticeBin.o ImageGroundPlane.o ObjGroundPlane.o \
      VarObj.o VarList.o  OpCode.o OpCodeList.o  Brain.o fuzzy.o AgentRender.o BrainUtils.o \
      OpCodeFunctions.o Plane.o CallList.o DebugOpcode.o perlin.o Noise.o
COMPOBJ = CompMain.o VarObj.o VarList.o OpCode.o \
		  OpCodeList.o Brain.o fuzzy.o CompFunctions.o ParseOpCode.o \
          ParseVariables.o AgentRender.o  AnimatedObj.o \
           Agent.o Noise.o BrainUtils.o OpCodeFunctions.o \
          CallList.o DebugOpcode.o perlin.o 

LAYOUT = LayoutMain.o  Agent.o \
		  GroundPlane.o Image.o LayoutCallback.o \
          Noise.o perlin.o

#########################################################################################
# XLIBS a list of the standard libs to include in the linking process
# to add libs use -l[LIBNAME] where LIBNAME is the name of the lib minus lib[LIBNAME].so
# e.g. to add libX11.so use -lX11
#########################################################################################
XLIBS =  -L$(HOME)/GraphicsLib/lib -lGraphics
ifeq "$(OSVER)" "Darwin"
	XLIBS +=  -framework Cocoa -framework OpenGL -framework GLUT -framework Foundation -lstdc++ -ltiff 
endif

ifeq "$(OSVER)" "Linux"
	XLIBS +=  -lGL -lGLU -lglut -lstdc++ -ltiff -lMagick++ 
endif




MATHS = -L/usr/lib -lm

ebpl : $(OBJECTS)
	g++ -o  ebpl $(FLAGS) $(OBJECTS) $(LIBS) $(INCDIR) $(XLIBS) $(MATHS) $(GRAPHICSLIB)


FlockMain.o : FlockMain.cpp
	g++ $(CCFLAGS) $(INCDIR)   -c FlockMain.cpp
GlutCallback.o : GlutCallback.cpp
	g++ $(CCFLAGS) $(INCDIR) -c GlutCallback.cpp
BBox.o : BBox.cpp
	g++ $(CCFLAGS) $(INCDIR) -c BBox.cpp
Plane.o : Plane.cpp
	g++ $(CCFLAGS) $(INCDIR)   -c Plane.cpp

Agent.o : Agent.cpp
	g++ $(CCFLAGS) $(INCDIR) -c Agent.cpp
AgentEmitter.o : AgentEmitter.cpp
	g++ $(CCFLAGS) $(INCDIR) -c AgentEmitter.cpp
Environment.o : Environment.cpp
	g++ $(CCFLAGS) $(INCDIR) -c Environment.cpp
CurveFollow.o : CurveFollow.cpp
	g++ $(CCFLAGS) $(INCDIR) -c CurveFollow.cpp
EnvObj.o : EnvObj.cpp
	g++ $(CCFLAGS) $(INCDIR) -c EnvObj.cpp
ReadParamFile.o : ReadParamFile.cpp
	g++ $(CCFLAGS) $(INCDIR) -c ReadParamFile.cpp
Goal.o : Goal.cpp
	g++ $(CCFLAGS) $(INCDIR) -c Goal.cpp
Vector.o  : Vector.cpp
	g++ $(CCFLAGS) $(INCDIR) -c Vector.cpp
Point3.o  : Point3.cpp
	g++ $(CCFLAGS) $(INCDIR) -c Point3.cpp
Camera.o  : Camera.cpp
	g++ $(CCFLAGS) $(INCDIR) -c Camera.cpp
Util.o  : Util.cpp
	g++ $(CCFLAGS) $(INCDIR) -c  Util.cpp
ObjLoader.o  : ObjLoader.cpp
	g++ $(CCFLAGS) $(INCDIR) -c  ObjLoader.cpp
AnimatedObj.o : AnimatedObj.cpp
	g++ $(CCFLAGS) $(INCDIR) -c AnimatedObj.cpp
VectObj.o : VectObj.cpp
	g++ $(CCFLAGS) $(INCDIR) -c VectObj.cpp
GroundPlane.o : GroundPlane.cpp
	g++ $(CCFLAGS) $(INCDIR) -c GroundPlane.cpp
Image.o : Image.cpp
	g++ $(CCFLAGS) $(INCDIR) -c Image.cpp
Material.o : Material.cpp
	g++ $(CCFLAGS) $(INCDIR) -c Material.cpp
Colour.o : Colour.cpp
	g++ $(CCFLAGS) $(INCDIR) -c Colour.cpp
Noise.o : Noise.cpp
	g++ $(CCFLAGS) $(INCDIR) -c Noise.cpp
perlin.o : perlin.c
	g++ $(CCFLAGS) $(INCDIR) -c perlin.c
Lattice.o : Lattice.cpp
	g++ $(CCFLAGS) $(INCDIR) -c Lattice.cpp
LatticePoint.o : LatticePoint.cpp
	g++ $(CCFLAGS) $(INCDIR) -c LatticePoint.cpp
LatticeBin.o : LatticeBin.cpp
	g++ $(CCFLAGS) $(INCDIR) -c LatticeBin.cpp
ImageGroundPlane.o : ImageGroundPlane.cpp
	g++ $(CCFLAGS) $(INCDIR) -c ImageGroundPlane.cpp

ObjGroundPlane.o : ObjGroundPlane.cpp
	g++ $(CCFLAGS) $(INCDIR) -c ObjGroundPlane.cpp
#Previewer.o : Previewer.cpp 
#	g++ $(CCFLAGS) $(INCDIR) -c Previewer.cpp

Brain.o  : Brain.cpp 
	g++ $(CCFLAGS) $(INCDIR) -c Brain.cpp
OpCode.o  : OpCode.cpp 
	g++ $(CCFLAGS) $(INCDIR) -c OpCode.cpp

VarObj.o : VarObj.cpp
	g++ $(CCFLAGS) $(INCDIR) -c VarObj.cpp
VarList.o : VarList.cpp
	g++ $(CCFLAGS) $(INCDIR) -c VarList.cpp
fuzzy.o : fuzzy.cpp
	g++ $(CCFLAGS) $(INCDIR) -c fuzzy.cpp
AgentRender.o : AgentRender.cpp
	g++ $(CCFLAGS) $(INCDIR) -c AgentRender.cpp
OpCodeList.o	: OpCodeList.cpp
	g++ $(CCFLAGS) $(INCDIR) -c OpCodeList.cpp		

BrainUtils.o	: BrainUtils.cpp
	g++ $(CCFLAGS) $(INCDIR) -c BrainUtils.cpp
CallList.o : CallList.cpp
	g++ $(CCFLAGS) $(INCDIR)   -c CallList.cpp		


Layout : $(LAYOUT)
	g++  $(CCFLAGS) $(LAYOUT) $(LIBS) $(INCDIR) $(XLIBS) $(MATHS) -o Layout

LayoutMain.o : LayoutMain.cpp
	g++ $(CCFLAGS) $(INCDIR) -c LayoutMain.cpp
LayoutCallback.o : LayoutCallback.cpp
	g++ $(CCFLAGS) $(INCDIR) -c LayoutCallback.cpp
BrainComp : $(COMPOBJ)
	g++ -o BrainComp  $(CCFLAGS) $(COMPOBJ) $(LIBS) $(INCDIR) $(XLIBS) $(MATHS) $(GRAPHICSLIB)
CompMain.o	: CompMain.cpp
	g++ $(CCFLAGS) $(INCDIR) -c CompMain.cpp
CompFunctions.o	: CompFunctions.cpp
	g++ $(CCFLAGS) $(INCDIR) -c CompFunctions.cpp
ParseOpCode.o	: ParseOpCode.cpp
	g++ $(CCFLAGS) $(INCDIR) -c ParseOpCode.cpp
ParseVariables.o	: ParseVariables.cpp
	g++ $(CCFLAGS) $(INCDIR) -c ParseVariables.cpp
OpCodeFunctions.o	: OpCodeFunctions.cpp
	g++ $(CCFLAGS) $(INCDIR) -c OpCodeFunctions.cpp
DebugOpcode.o : DebugOpcode.cpp
	g++ $(CCFLAGS) $(INCDIR)   -c DebugOpcode.cpp


clean :
	rm -f *.o; rm -f *~; rm -f core; rm -f ../lib/*.a

