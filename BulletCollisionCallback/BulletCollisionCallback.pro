TARGET=BulletCollisionCallback
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
					src/NGLScene.cpp \
					src/PhysicsWorld.cpp \
					src/CollisionShape.cpp \
					src/Vehicle.cpp

HEADERS+= include/OpenGLWindow.h \
					include/NGLScene.h \
					include/PhysicsWorld.h \
					include/CollisionShape.h \
					include/Vehicle.h

INCLUDEPATH +=./include

DESTDIR=./

CONFIG += console
CONFIG -= app_bundle
INCLUDEPATH+=/usr/local/include/bullet
INCLUDEPATH+=/usr/local/include
LIBS+= -L/usr/local/lib -lBulletDynamics  -lBulletCollision -lLinearMath
#LIBS+= -L/Users/jmacey/Downloads/bullet3/lib -lBulletCollision_xcode4_debug -lBulletDynamics_xcode4_debug -lLinearMath_xcode4_debug
# use this to suppress some warning from boost
QMAKE_CXXFLAGS_WARN_ON += "-Wno-unused-parameter"
#QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:INCLUDEPATH+=/usr/local/include/

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

