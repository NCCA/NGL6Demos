#ifdef LINUX
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif
#ifdef DARWIN
	#include <unistd.h>
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#endif
#include "Camera.h"

#include "Util.h"
#include "AgentRender.h"



using namespace std;

// a camera
Camera Cam;
Point3 EYE(1.0f,1.0f,2.0f);
Point3 LOOK(0.0f,0.0f,0.0f);
Vector UP(0.0f,1.0f,0.0f); //Y == UP

// variables used for mouse rotation of the scene
int spinxface = 0 ;
int spinyface = 0 ;
int spinzface = 0 ;
int  origx,origy,origz,RotateXY,RotateXZ=0;

int WIDTH = 720; //1024;
int HEIGHT = 576; //800;
AgentRender *agent;
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
}



void display()
{
	// clear the current buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // display the menu
	glPushMatrix();
    glColor3f(1.0,1.0,1.0);
                 glRotated	( (GLdouble) spinxface, 1.0, 0.0, 0.0 ) ;
				 glRotated	( (GLdouble) spinyface, 0.0, 1.0, 0.0 ) ;
				 glRotated	( (GLdouble) spinzface, 0.0, 0.0, 1.0 ) ;
    
    DrawAxis(2.0);
    agent->Draw();
    //glutWireSphere(1.0,12,12);
    glPopMatrix();
    glutSwapBuffers();
    glutPostRedisplay();
}

void idle(void)
{
    
	
    glutPostRedisplay();
}

void KeyPressed(unsigned char ch, int x, int y)
{
  switch (ch)
  {
  // ESC key to exit program
 	case 27:             /* ESC */
		exit(0);
	break;
    
    case 'w' : agent->ChangeCycle(0); break;
    case 'r' : agent->ChangeCycle(1); break;
    case 'n' : agent->ChangeCycle(2); break;
    case 'd' : agent->ChangeCycle(3); break;
    case 'p' : agent->ChangeCycle(4); break;
    case 's' : agent->ChangeCycle(5); break;
    //case ' ' : agent->UpdateFrame();
}
}

void InitGL()
{
glClearColor(0.0f, 0.0f, 0.0f, 1.0f);			   // Grey Background
// disable alpha blending as we won't be using it
glDisable(GL_BLEND);
// if we havn't set a camera in the file set a default one
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(WIDTH*64/48.0,HEIGHT*64/48.0, -WIDTH,HEIGHT, 0.1, 450.0);
SetCamera(EYE,LOOK,UP,WIDTH,HEIGHT,45,64/48,0.1,450.0);
// render things smoothly
glShadeModel(GL_SMOOTH);
// enable depth testing for drawing
glEnable(GL_DEPTH_TEST);
//glDisable(GL_DEPTH_TEST);
GLfloat Colour[]={1.0,1.0,1.0,1.0};
glLightModelfv(GL_LIGHT_MODEL_AMBIENT,Colour);
glEnable(GL_LIGHT0);
glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
}



int main(int argc, char **argv)
{
	// buffer to hold the txt for the main banner
    //initialise glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(10, 15);
    // create the banner text
    	// create the main window
	glutCreateWindow("Brain Test");
		
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(KeyPressed);
   	glutMouseFunc(MouseHandler);
    glutMotionFunc(MouseMoved);
	agent = new AgentRender();
	//initialise opengl
    InitGL();
// read in the script file and setup the environment
    // start the simulation
    srand(time(NULL));
	
    glutMainLoop();
	//this never gets called due to glutmainloop not exiting properly
    return 1;
}


//end of file




