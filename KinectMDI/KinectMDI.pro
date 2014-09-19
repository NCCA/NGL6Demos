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
# use this to suppress some warning from boost
unix:QMAKE_CXXFLAGS_WARN_ON += "-Wno-unused-parameter"

unix:QMAKE_CXXFLAGS+= -msse -msse2 -msse3
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
unix:LIBS += -L/usr/local/lib/ -lfreenect -lfreenect_sync

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


