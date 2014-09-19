# -------------------------------------------------
# Project created by QtCreator 2009-11-05T22:11:46
# -------------------------------------------------
# as I want to support 4.8 and 5 this will set a flag for some of the mac stuff
# mainly in the types.h file for the setMacVisual which is native in Qt5
isEqual(QT_MAJOR_VERSION, 5) {
        cache()
        DEFINES +=QT5BUILD
}
QT += opengl
QT += core \
      gui

TARGET=Kinect
QMAKE_CXX=clang++ -fdiagnostics-fixit-info

DESTDIR=./
OBJECTS_DIR=obj
# this is where we want to put the intermediate build files ( .o)
OBJECTS_DIR=./obj/
MOC_DIR=./moc/
SOURCES += src/main.cpp \
src/MainWindow.cpp \
src/QKinect.cpp \
src/Context.cpp \
src/OpenGLWindow.cpp \
src/VideoWindow.cpp \
src/ScreenQuad.cpp \
src/PointCloud.cpp

INCLUDEPATH+=./include \
/usr/local/include/libfreenect \
/usr/local/include

HEADERS += include/MainWindow.h \
include/QKinect.h \
include/Context.h \
include/OpenGLWindow.h \
include/VideoWindow.h \
include/ScreenQuad.h \
include/PointCloud.h

OTHER_FILES+= shaders/ColourTextureVert.glsl \
shaders/ColourTextureFrag.glsl \
shaders/PointCloudVert.glsl
CONFIG += console
CONFIG -= app_bundle
DEFINES+=USING_QT_CREATOR
# use this to suppress some warning from boost
QMAKE_CXXFLAGS_WARN_ON += "-Wno-unused-parameter"

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
macx:INCLUDEPATH+=/usr/local/boost/
linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native

# define the _DEBUG flag for the graphics lib
DEFINES +=NGL_DEBUG

LIBS += -L/usr/local/lib
# add the ngl lib
LIBS +=  -L/$(HOME)/NGL/lib -l NGL
LIBS += -L/usr/local/lib/ -lfreenect -lfreenect_sync
# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-clang {
		DEFINES += LINUX
		LIBS+= -lGLEW
}
linux-g++-64 {
		DEFINES += LINUX
		LIBS+= -lGLEW
}
DEPENDPATH+=include
# if we are on a mac define DARWIN
macx:DEFINES += DARWIN

# this is where to look for includes
INCLUDEPATH += $$(HOME)/NGL/include/

win32: {
				DEFINES+=USING_GLEW

				INCLUDEPATH+=-I c:\boost_1_40_0
				INCLUDEPATH+=-I c:/SDL-1.2.14/include
				INCLUDEPATH+=-I c:/SDL-1.2.14/include/SDL
				INCLUDEPATH+=-I c:/SDL_ttf-2.0.9/include
				INCLUDEPATH+= -I C:/glew/include
				LIBS+= -L C:/GraphicsLib/lib
				LIBS+= -L C:/glew/bin
				LIBS+= -lmingw32
				LIBS+= -lglew32
}
