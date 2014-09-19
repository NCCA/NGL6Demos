# as I want to support 4.8 and 5 this will set a flag for some of the mac stuff
# mainly in the types.h file for the setMacVisual which is native in Qt5
isEqual(QT_MAJOR_VERSION, 5) {
        cache()
        DEFINES +=QT5BUILD
}


QT -= core
QT -=opengl
QT -=xml
TARGET=SDLJoyPad
CONFIG-=app_bundle
DESTDIR=./
OBJECTS_DIR=obj
SOURCES=src/main.cpp \
        src/Game.cpp \
        src/Rocket.cpp \
        src/StarMap.cpp

HEADERS+=include/Game.h \
         include/Rocket.h \
         include/XBoxController.h \
         include/StarMap.h
OTHER_FILES+=shaders/TextureVertex.glsl \
             shaders/TextureFragment.glsl

# add the ngl lib
LIBS +=  -L/$(HOME)/NGL/lib -l NGL
# this is where to look for includes
INCLUDEPATH += $$(HOME)/NGL/include/
INCLUDEPATH +=include
INCLUDEPATH +=/usr/local/include


# use this to suppress some warning from boost
QMAKE_CXXFLAGS_WARN_ON +=  "-Wno-unused-parameter"
QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
macx:INCLUDEPATH+=/usr/local/boost/
linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native

QMAKE_CXXFLAGS+=$$system(sdl2-config  --cflags)
message(output from sdl2-config --cflags added to CXXFLAGS= $$QMAKE_CXXFLAGS)

LIBS+=$$system(sdl2-config  --libs)
message(output from sdl2-config --libs added to LIB=$$LIBS)



LIBS += -L/usr/local/lib
macx:LIBS+= -framework OpenGL
macx:DEFINES+=DARWIN
# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-g++* {
		DEFINES += LINUX
		LIBS+= -lGLEW
}
linux-clang* {
		DEFINES += LINUX
		LIBS+= -lGLEW
}

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


