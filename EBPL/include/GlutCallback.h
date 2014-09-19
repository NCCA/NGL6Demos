#ifndef __GLUTCALLBACK_H__
#define __GLUTCALLBACK_H__

#include "Camera.h"
#include "Vector.h"
#include "Point3.h"

extern void MouseHandler(int button, int state, int x, int y);
extern void MouseMoved(int x, int y);
extern void KeyPressed(unsigned char ch, int x, int y);
extern void SpecialKeyPressed(int key, int x, int y);
extern GraphicsLib::Camera GetCamera(void);


void SetCamera(GraphicsLib::Point3 Eye, GraphicsLib::Point3 Look, GraphicsLib::Vector Up, int WIDTH, 
				int HEIGHT,GLfloat ViewAngle, GLfloat aspect,
				GLfloat near, GLfloat far);



#endif
