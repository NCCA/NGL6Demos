TARGET=BrainComp
OBJECTS_DIR=obj
# as I want to support 4.8 and 5 this will set a flag for some of the mac stuff
# mainly in the types.h file for the setMacVisual which is native in Qt5
isEqual(QT_MAJOR_VERSION, 5) {
	cache()
	DEFINES +=QT5BUILD
}



MOC_DIR=moc
CONFIG-=app_bundle
QT+=gui opengl core
SOURCES+=src/CompMain.cpp \
src/VarObj.cpp \
src/VarList.cpp \
src/OpCode.cpp \
src/OpCodeList.cpp \
src/Brain.cpp \
src/fuzzy.cpp \
src/CompFunctions.cpp \
src/ParseOpCode.cpp \
src/ParseVariables.cpp \
src/AgentRender.cpp  \
src/AnimatedObj.cpp \
src/Agent.cpp \
src/Noise.cpp \
src/BrainUtils.cpp \
src/OpCodeFunctions.cpp \
src/CallList.cpp \
src/DebugOpcode.cpp \
src/perlin.cpp

HEADERS+=include/CompMain.h \
include/VarObj.h \
include/VarList.h \
include/OpCode.h \
include/OpCodeList.h \
include/Brain.h \
include/fuzzy.h \
include/CompFunctions.h \
include/ParseOpCode.h \
include/ParseVariables.h \
include/AgentRender.h  \
include/AnimatedObj.h \
include/Agent.h \
include/Noise.h \
include/BrainUtils.h \
include/OpCodeFunctions.h \
include/CallList.h \
include/DebugOpcode.h \
include/perlin.h
INCLUDEPATH +=./include

DESTDIR=./
OTHER_FILES+=
CONFIG += console
CONFIG -= app_bundle
# note each command you add needs a ; as it will be run as a single line
# first check if we are shadow building or not easiest way is to check out against current
!equals(PWD, $${OUT_PWD}){
	copydata.commands = echo "creating destination dirs" ;
	# now make a dir
	copydata.commands += mkdir -p $$OUT_PWD/shaders ;
	copydata.commands += echo "copying files" ;
	# then copy the files
	copydata.commands += $(COPY_DIR) $$PWD/shaders/* $$OUT_PWD/shaders/ ;
	# now make sure the first target is built before copy
	first.depends = $(first) copydata
	export(first.depends)
	export(copydata.commands)
	# now add it as an extra target
	QMAKE_EXTRA_TARGETS += first copydata
}
# use this to suppress some warning from boost
QMAKE_CXXFLAGS_WARN_ON += "-Wno-unused-parameter"
QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
macx:INCLUDEPATH+=/usr/local/include/
linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native
# define the _DEBUG flag for the graphics lib
DEFINES +=NGL_DEBUG

unix:LIBS += -L/usr/local/lib
# add the ngl lib
unix:LIBS +=  -L/$(HOME)/NGL/lib -l NGL

# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-*{
		linux-*:QMAKE_CXXFLAGS +=  -march=native
		linux-*:DEFINES+=GL42
		DEFINES += LINUX
}
DEPENDPATH+=include
# if we are on a mac define DARWIN
macx:DEFINES += DARWIN
# this is where to look for includes
INCLUDEPATH += $$(HOME)/NGL/include/

win32: {
        PRE_TARGETDEPS+=C:/NGL/lib/NGL.lib
        INCLUDEPATH+=-I c:/boost
        DEFINES+=GL42
        DEFINES += WIN32
        DEFINES+=_WIN32
        DEFINES+=_USE_MATH_DEFINES
        LIBS += -LC:/NGL/lib/ -lNGL
        DEFINES+=NO_DLL
}

