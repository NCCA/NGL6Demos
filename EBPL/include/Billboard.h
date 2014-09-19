#ifndef __BILLBOARD_H_
#define __BILLBOARD_H_

#ifdef __USINGWINDOWS_
	#include <GL/gl.h>
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
#include "Vector.h"
#include "Texture.h"
#include "Camera.h"
class Billboard
{

public :
	Vector ul;
	Vector ur;
    Vector dl;
    Vector dr;
	Billboard(GraphicsLib::Camera Cam,bool FullSize);
	Billboard(GraphicsLib::Camera Cam, bool FullSize,char *TexName);
	void CalcBB(GraphicsLib::Camera Cam, bool FullSize);
	void Draw(void);
	void LoadTexture(char *TexName);
	void DrawTextured(void);
    void Reset(Camera cam);
private :
	GLuint TextureID;
	GraphicsLib::Texture Tex1;
	bool Rotate;
};



#endif 
