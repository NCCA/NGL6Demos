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

#include <stdlib.h>
#include "Camera.h"
#include "Point3.h"
#include "iostream"
#include "Agent.h"
#include "BBox.h"
#include <vector>
typedef std::vector<Agent>                    AList;
//AgentList itterator typeded
extern AList Agents;
extern char fname[200];
FILE *stream;

using namespace std;
extern BBox *bb;
// a camera
Camera Cam;
/* pointers to external variables used by this file */

// the global environment used by the flocking system

extern bool MouseRotate;
extern unsigned long int nAgents;
// The local eye look and up co-ordinates for the camera
static Point3 LocalEye;
static Point3 LocalLook;
static Vector LocalUp;
// View angle Aspect ratio near and far clip for the camers
static float VA;
static float AS;
static float ne,fa;

// variables used for mouse rotation of the scene
int spinxface = 0 ;
int spinyface = 0 ;
int spinzface = 0 ;
int  origx,origy,origz,RotateXY,RotateXZ=0;
GLfloat Xpos,Ypos,Zpos;
GLfloat Xrot=0.0;
GLfloat Yrot=0.0;
GLfloat Zrot=0.0;
Vector Dir(0,0,0);
/*! set the Camera values
\param Point3 Eye the eye position of the camera
\param Point3 Look where the camera is looking
\param Up the local up  vector
\param int WIDTH the width of the screen
\param int HEIGHT the height of the screen
\param GLfloat aspect the aspect ratio of the camera
\param GLfloat n the near plane
\param GLfloat f the far plane */
void SetCamera(Point3 Eye, Point3 Look, Vector Up, int WIDTH,
				int HEIGHT,GLfloat ViewAngle, GLfloat aspect,
				GLfloat n, GLfloat f)
{
 // set the camera properties
 Cam.setDisplay(WIDTH, HEIGHT);
 Cam.set(Eye,Look,Up);
 Cam.setShape(ViewAngle,aspect, n,f);
 // store the values in the local variables so we can re use them
 LocalEye=Eye;
 LocalLook=Look;
 LocalUp=Up;
 VA=ViewAngle;
 AS=aspect;
 ne=n;
 fa=f;
}

/*! glut key press callback function
\param unsigned char ch the key pressed
\param int x the x position of the mouse
\param int y the y position of the mouse*/

void KeyPressed(unsigned char ch, int x, int y)
{

  if(ch == ' ')
  {

    	Point3 p(Xpos,Ypos,Zpos);
    	Vector d(Xrot,Yrot,Zrot);
		Agent a(p,2.0,2.0,1.0,1.0,d,1,NULL);
        //a.xrot=Xrot; a.yrot=Yrot; a.zrot=Zrot;
		Agents.push_back(a);
		nAgents++;

	}

  switch (ch)
  {
  // ESC key to exit program
 	case 27:             /* ESC */
	 	//delete Env; //->CloseOutFile();
        exit(0);
	break;

	case 'y' :
    		Ypos +=0.5;
	        if(Ypos > bb->center.y+bb->height/2)
            	Ypos =bb->center.y+bb->height/2;

 	break;
	case 'b' :
    		Ypos -=0.5;
   	        if(Ypos < bb->center.y-bb->height/2)
            	Ypos =bb->center.y-bb->height/2;

    break;

	case 'h' :
    		Xpos +=0.5;
	        if(Xpos > bb->center.x+bb->width/2)
            	Xpos =bb->center.x+bb->width/2;

    break;
	case 'g' :
    		Xpos -=0.5;
    		if(Xpos < bb->center.x-bb->width/2)
            	Xpos =bb->center.x-bb->width/2;

    break;

    case 'j' :
    		Zpos +=0.5;
			if(Zpos > bb->center.z+bb->depth/2)
            	Zpos =bb->center.z+bb->depth/2;
    break;

    case 'u' : Zpos -=0.5;
   			if(Zpos < bb->center.z-bb->depth/2)
            	Zpos =bb->center.z-bb->depth/2;



    break;

    case 'z' : Zrot +=0.5; break;
	case 'Z' : Zrot -=0.5; break;
    case 'x' : Xrot +=0.5; break;
	case 'X' : Xrot -=0.5; break;
	case 'c' : Yrot +=0.5; break;
	case 'C' : Yrot -=0.5; break;
	case 's' :

    if ((stream = fopen(fname, "wt")) == NULL)
	{
        cout << "Failed to open file for writing : "<< fname<<endl;
        exit(0);
    }
	fprintf(stream,"%ld\n",nAgents);
    for(unsigned long int i=0; i<nAgents; i++)
    	{
        fprintf(stream,"%f %f %f %f %f %f\n",Agents[i].Pos.x,Agents[i].Pos.y,Agents[i].Pos.z,
        				Agents[i].Dir.x,Agents[i].Dir.y,Agents[i].Dir.z);


        }
	fclose(stream);

    default :
  break;
  }
  // tell the system we need to redisplay
 glutPostRedisplay();
}

/*! glut key press callback function
\param unsigned int key the key pressed
\param int x the x position of the mouse
\param int y the y position of the mouse*/

void SpecialKeyPressed(int key, int x, int y)
{

    switch (key)
    {
    // slide the camera left right up and down
    case GLUT_KEY_LEFT	:
		Cam.slide(0.5,0.0,0.0);
	break;
    case GLUT_KEY_UP	:
    	Cam.slide(0.0,0.5,0.0);
	break;
	case GLUT_KEY_RIGHT	:
		Cam.slide(-0.5,0.0,0.0);
    break;
	case GLUT_KEY_DOWN	:
    	Cam.slide(0.0,-0.5,0.0);
	break;
    //slide the camera along the z axis
	case GLUT_KEY_PAGE_UP	:
    	Cam.slide(0.0,0.0,0.5);
		break;
	case GLUT_KEY_PAGE_DOWN	:
    	Cam.slide(0.0,0.0,-0.5);
		break;
        //turn the mouse rotate on and off
	case GLUT_KEY_F1 :
    	MouseRotate^=1;
 	break;

    }
 //redisplay the scene
glutPostRedisplay();
}

//get the camera
Camera GetCamera(void)
{
return Cam;

}

/*! mouse movement glut callback
\param int x the x position of the mouse
\param int y the y position of the mouse */
void MouseMoved ( int x, int y )
{
 //were rotating in the XY plane calculate the new rotations
 if (RotateXY)
 {
	 // calculate how much we need to spin
     spinyface = ( spinyface + (x - origx) ) % 360 ;
	 spinxface = ( spinxface + (y - origy) ) % 360 ;
	 // save the values
     origx = x;
	 origy = y;
     //redraw
	 glutPostRedisplay();
  }
  //were rotating in the XZ plane
  else if (RotateXZ)
  {
	 //calculate how much to rotate
     spinyface = ( spinyface + (x - origx) ) % 360 ;
	 spinzface = ( spinzface + (y - origz)  )% 360 ;
	 //store the original values
     origx = x;
	 origz = y;
	 // redisplay
     glutPostRedisplay();
  }
}
/*! glut mouse handler
\param int button which mouse button is pressed
\param int down the state of the button
\param int x the x position of the mouse
\param int y the y position of the mouse */
void MouseHandler(int button, int down, int x, int y)
{
// reset the rotation axis flags
RotateXY = RotateXZ =0;
  switch(button)
  {
  case GLUT_LEFT_BUTTON:
	 //if left button pressed and down
     if (down == GLUT_DOWN)
        {
		//flag rotate in XY
        origx = x;
		origy = y;
		RotateXY = 1;
        }
     else RotateXY = 0;
   break;
   case GLUT_RIGHT_BUTTON:
	 if (down == GLUT_DOWN)
        {
		//flag rotate in XZ
        origx = x;
		origz = y;
		RotateXZ = 1;
        }
     else RotateXZ = 0;
   break;

   }
}



