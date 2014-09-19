#ifndef _NOISE_H_
#define _NOISE_H_
#include <stdlib.h>
#include <ngl/Vec3.h>
#include <time.h>
#ifdef __USINGWINDOWS__
	#include <windows.h>
#endif



#ifdef LINUX
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif
#ifdef DARWIN
	#include <unistd.h>
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#endif

class Noise
{
private :
	float *noiseTable;
	unsigned char * index;
	GLfloat mySpline(GLfloat x);
	GLfloat latticeNoise(int i, int j, int k);


public :
	Noise();
	GLfloat marble(GLfloat A,GLfloat s,ngl::Vec3 p);
	GLfloat SQR(GLfloat x);
	GLfloat noise(GLfloat x,GLfloat y, GLfloat z);
	GLfloat noise(GLfloat scale, ngl::Vec3 p);
	GLfloat turbulance(GLfloat s, ngl::Vec3 p);
	GLfloat marble(GLfloat x, GLfloat y, GLfloat z);
	GLfloat marble(GLfloat strength, ngl::Vec3 p);
	GLfloat undulate(GLfloat x);
    void ResetTables(void);

	};



#endif
