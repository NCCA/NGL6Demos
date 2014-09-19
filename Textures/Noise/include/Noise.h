// simple Perlin noise class cobbled together from Computer Graphics OpenGL by F,S Hill
// and Texturing and Modeling Ebert et-al
// also thanks to Ian Stephenson for help and debuggin tips
// more work needs to be done to add interpolated noise functions and improve the
// aliasing of the textures but it is ok for OpenGL use


#ifndef _NOISE_H_
#define _NOISE_H_
#include <stdlib.h>
#include <time.h>
#include <ngl/Types.h>
#include <ngl/Vec3.h>
#include <ngl/Random.h>
class Noise
{
public :
	Noise();
  GLfloat marble(GLfloat A,GLfloat s,ngl::Vec3 p);
	GLfloat sqr(GLfloat x);
	GLfloat noise(GLfloat x,GLfloat y, GLfloat z);
  GLfloat noise(GLfloat scale, ngl::Vec3 p);
  GLfloat turbulance(GLfloat s, ngl::Vec3 p);
	GLfloat marble(GLfloat x, GLfloat y, GLfloat z);
  GLfloat marble(GLfloat strength, ngl::Vec3 p);
	GLfloat undulate(GLfloat x);
  void resetTables();


private :

  float *noiseTable;
	unsigned char * index;
	GLfloat latticeNoise(int i, int j, int k);


};



#endif
