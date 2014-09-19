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
#include "Environment.h"
#include "ParamFile.h"

using namespace std;
using namespace GraphicsLib;
// a camera
Camera Cam;
/* pointers to external variables used by this file */
extern bool WriteTiff;
// the global environment used by the flocking system
extern Environment *Env;
// flag to indicate if path follow is set
extern bool PathFollow;
// flag to indicate if drawEnv objects is set
extern bool DrawEnvToggle;
// flag to indicate if mouse rotation is turned on
extern bool MouseRotate;
// flag to indicate if the Agents are flocking or not
extern bool FlockMode;
// flag to indicate if simulation should be paused or not
extern bool Pause;
// flag to indicate if the menu should be drawn or not
extern bool HideMenu;
// flag to indicate if the camera should follow the centroid
extern bool CamFollowCentroid;
//extern bool CamFollowRover;
// flag to indicate how many scatter vectors have been set in the script file
extern int NumScatterVect;
bool DrawEnvDetails=false;
extern bool DrawLattice;
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
// Noise value to use
int NoiseIndex=0;
extern void ReLoadScript(void);
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
 //Cam.setDisplay(WIDTH, HEIGHT);
 Cam.set(Eye,Look,Up);
 Cam.setShape(ViewAngle,aspect, n,f,PERSPECTIVE);
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
  switch (ch)
  {
  // ESC key to exit program
 	case 27:             /* ESC */
	 	delete Env; 		exit(0);
	break;
    //write to file
    case '0' : Env->ToggleFileWrite(); break;
	// set the camera to follow the centroid
	case '1' : CamFollowCentroid^=true; break;
    //case '2' : CamFollowRover^=true; break;
    // turn flocking on and off
    case ' ' : Env->SetDoCollide(); FlockMode^=true;  break;
	// set the follow centroid flag for all the emitters
    case 'c' :
    for(unsigned long int i=0; i<Env->nEmitters; i++)
		    Env->SetFollowCurve(i);
    PathFollow ^=1;
    break;
    //dump agent debug info to the console
    
    // Draw the Environment objects
    case 'e' : DrawEnvToggle^=1;
    // follow goals if set
    case 'g' :
    for(unsigned long int i=0; i<Env->nEmitters; i++)
    	Env->Emitters[i].ToggleFollowGoals(); break;
    //toggle menu
    case 'h' : HideMenu^=true; break;
	// toggle draw extents
    case 'x' : Env->ToggleDrawExtents(); break;
    // pause simulation
    case 'p' : Pause ^=true; break;
    //display pred hit for agents
    case 'P' :
//     	for(unsigned long int i=0; i<Env->nEmitters; i++)
//         	{
//             for(unsigned long int x=0; x<Env->Emitters[i].nAgents; x++)
//             	Env->Emitters[i].Agents[x].DisplayPredHit^=true;
//             }
    break;
    //set the agents to scatter
    //case 's' : Env->Emitters[0].FlockScatter(NumScatterVect); break;
    case 'l' :
    for(unsigned long int i=0; i<Env->nEmitters; i++)
        	{
            Env->Emitters[i].ToggleLockedCentroid();
            }
    break;
    
    case '4' : DrawLattice^=true; break;
	case 'q' : DrawEnvDetails ^=true; Env->DrawEnvDetails^=true; break;
    case 't' : WriteTiff^=true; break;
    case 'L' : ReLoadScript(); break;
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



