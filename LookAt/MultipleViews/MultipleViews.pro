TARGET=MultipleViews
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
SOURCES+= src/main.cpp \
					src/OpenGLWindow.cpp \
		src/NGLScene.cpp

HEADERS+= include/OpenGLWindow.h \
		include/NGLScene.h
INCLUDEPATH +=./include

DESTDIR=./
OTHER_FILES+= shaders/PhongFragment.glsl \
							shaders/PhongVertex.glsl
CONFIG += console
CONFIG -= app_bundle
# use this to suppress some warning from boost
QMAKE_CXXFLAGS_WARN_ON += "-Wno-unused-parameter"

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
macx:INCLUDEPATH+=/usr/local/include/
linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native
# define the _DEBUG flag for the graphics lib
DEFINES +=NGL_DEBUG

LIBS += -L/usr/local/lib
# add the ngl lib
LIBS +=  -L/$(HOME)/NGL/lib -l NGL

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
				DEFINES+=USING_GLEW
				INCLUDEPATH+=-I c:/boost_1_44_0
				INCLUDEPATH+=-I c:/boost

				INCLUDEPATH+= -I C:/NGL/Support/glew
				LIBS+= -L C:/NGL/lib
				LIBS+= -lmingw32
				DEFINES += WIN32
				DEFINES += USING_GLEW
				DEFINES +=GLEW_STATIC
				DEFINES+=_WIN32
				SOURCES+=C:/NGL/Support/glew/glew.c
				INCLUDEPATH+=C:/NGL/Support/glew/
}


