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

TARGET=OpenCVKinect

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
src/ScreenQuad.cpp

INCLUDEPATH+=./include \
/usr/local/include/libfreenect \
/usr/local/include

HEADERS += include/MainWindow.h \
include/QKinect.h \
include/Context.h \
include/OpenGLWindow.h \
include/VideoWindow.h \
include/ScreenQuad.h

OTHER_FILES+= shaders/ColourTextureVert.glsl \
shaders/ColourTextureFrag.glsl
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
LIBS +=  -L//usr/local/lib -lopencv_core \
-lopencv_imgproc -lopencv_calib3d -lopencv_video -lopencv_features2d \
-lopencv_ml -lopencv_highgui -lopencv_objdetect -lopencv_contrib -lopencv_legacy \
-lopencv_nonfree

# define the _DEBUG flag for the graphics lib
DEFINES +=NGL_DEBUG

LIBS += -L/usr/local/lib
# add the ngl lib
LIBS +=  -L/$(HOME)/NGL/lib -l NGL
LIBS += -L/usr/local/lib/ -lfreenect -lfreenect_sync
# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-clang* {
		DEFINES += LINUX
		LIBS+= -lGLEW
                DEFINES+=GL42
}
linux-g++* {
		DEFINES += LINUX
                DEFINES+=GL42
                LIBS+= -lGLEW
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

