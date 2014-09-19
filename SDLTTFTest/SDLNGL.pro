QT -= core
QT -=opengl
QT -=xml
# as I want to support 4.8 and 5 this will set a flag for some of the mac stuff
# mainly in the types.h file for the setMacVisual which is native in Qt5
isEqual(QT_MAJOR_VERSION, 5) {
		cache()
		DEFINES +=QT5BUILD
}
TARGET=SDLNGL
CONFIG-=app_bundle
DESTDIR=./
OBJECTS_DIR=obj
SOURCES=src/main.cpp \
src/NGLDraw.cpp \
src/Text.cpp

OTHER_FILES+=shaders/PhongVertex.glsl \
shaders/PhongFragment.glsl

HEADERS+=include/NGLDraw.h \
          include/Text.h
# add the ngl lib
LIBS +=  -L/$(HOME)/NGL/lib -l NGL
# this is where to look for includes
INCLUDEPATH += $$(HOME)/NGL/include/
INCLUDEPATH +=include

# use this to suppress some warning from boost
QMAKE_CXXFLAGS_WARN_ON += "-Wno-unused-parameter"
QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
macx:INCLUDEPATH+=/usr/local/boost/

QMAKE_CXXFLAGS+=$$system(sdl2-config  --cflags)
message(output from sdl2-config --cflags added to CXXFLAGS= $$QMAKE_CXXFLAGS)

LIBS+=$$system(sdl2-config  --libs)
message(output from sdl2-config --libs added to LIB=$$LIBS)
LIBS+=-L/usr/local/lib -lSDL2_ttf

macx:INCLUDEPATH+=/usr/local/include
LIBS += -L/usr/local/lib
macx:LIBS+= -framework OpenGL
macx:DEFINES+=DARWIN
# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-*{
		linux-*:QMAKE_CXXFLAGS +=  -march=native
		linux-*:DEFINES+=GL42
		DEFINES += LINUX
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


