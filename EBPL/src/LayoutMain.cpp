#ifdef __USINGWINDOWS__
	#include <windows.h>
#else
	#include <unistd.h>
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
#include "Util.h"
#include <stdio.h>  // for printf
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <vector>
#include "BBox.h"
#include "GlutCallback.h"
#include "Agent.h"
#include <string.h>
using namespace std;
char fname[200];
typedef std::vector<Agent>                    AList;
//AgentList itterator typeded
typedef std::vector<Agent>::iterator          AListItt;
AList Agents;
// width and height of the window
int WIDTH = 800;
int HEIGHT = 660;
// The world bounding box used by all the simulations
BBox *bb;
unsigned long int nAgents=0;
// default update rate for simulation
int updateRate=1;

// flag to indcate if the mouse rotate set by the F1 key
bool MouseRotate=true;
// Camera control flags
// the values to spin the main window by in x y and z
extern int spinxface  ;
extern int spinyface  ;
extern int spinzface  ;
extern GLfloat Xrot,Yrot,Zrot;
// defualt camera positions
/*
Point3 EYE(80.0f,70.0f,120.0f);
Point3 LOOK(0.0f,0.0f,0.0f);
Vector UP(0.0f,1.0f,0.0f); //Y == UP
*/
Point3 EYE(0.0f,80.0f,0.0f);
Point3 LOOK(0.0f,0.0f,0.0f);
Vector UP(0.0f,0.0f,1.0f); //Y == UP



extern GLfloat Xpos,Ypos,Zpos;

void DrawLayoutGrid(void)
{
glPushMatrix();
	glPushMatrix();
    	glTranslatef(Xpos,Ypos,Zpos);
    	glColor3f(1.0,1.0,1.0);
    	glutWireCube(1.0);
	glPopMatrix();
    glBegin(GL_LINES);
    	glColor3f(1.0,0.0,0.0);
        glVertex3f(bb->center.x-bb->width/2,Ypos,Zpos);
		glVertex3f(bb->center.x+bb->width/2,Ypos,Zpos);
    glEnd();
	glBegin(GL_LINES);
    	glColor3f(0.0,1.0,0.0);
        glVertex3f(Xpos,bb->center.y-bb->height/2,Zpos);
		glVertex3f(Xpos,bb->center.y+bb->height/2,Zpos);
    glEnd();
glBegin(GL_LINES);
    	glColor3f(0.0,0.0,1.0);
        glVertex3f(Xpos,Ypos,bb->center.z-bb->depth/2);
		glVertex3f(Xpos,Ypos,bb->center.z+bb->depth/2);
    glEnd();
glBegin(GL_LINES);
	glColor3f(1,1,0);
    glVertex3f(Xpos,Ypos,Zpos);
    glVertex3f(Xpos+Xrot,Ypos+Yrot,Zpos+Zrot);

glEnd();

glPopMatrix();

}

/*! function to write a string to the screen
\param void *font the font to render with (see glut.h for more info)
\param const char *string the string to render */
void RenderString(void* font, const char* string)
{
	char* p = (char*) string;
	while (*p != '\0') glutBitmapCharacter(font, *p++);
}

/*! This is the main window display routine */
void display()
{
    // clear the current buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //presereve the current transformation stack
	glPushMatrix();
        glPushMatrix();
        //rotate the global scene based on the mouse rotations
          	if(MouseRotate)
                 {
                 glRotated	( (GLdouble) spinxface, 1.0, 0.0, 0.0 ) ;
				 glRotated	( (GLdouble) spinyface, 0.0, 1.0, 0.0 ) ;
				 glRotated	( (GLdouble) spinzface, 0.0, 0.0, 1.0 ) ;
                 }
     		//draw the axis and the Agents
            //DrawAxisSimple(50.0f);
			bb->Draw();
			DrawLayoutGrid();
            for(unsigned long int i=0; i<nAgents; i++)
            	{
                //Agents[i].CalcAngle();
                Agents[i].Draw();

                }
       glPopMatrix();
    glPopMatrix();
// set the position offsets
glPushMatrix();
	char text[100];
   //move to the correct position and render the strings
   glColor3f(1.0,1.0,0.0);
   glRasterPos2f(-50.2f,55.0f);
   sprintf(text,"Number of Agents %ld Xpos %02.2f Ypos %02.2f Zpos %02.2f",nAgents,Xpos,Ypos,Zpos);
   RenderString(GLUT_BITMAP_9_BY_15,text);
   glRasterPos2f(-50.2f,50.0f);
   sprintf(text,"Xrot %02.2f Yrot %02.2f Zrot %02.2f",Xrot,Yrot,Zrot);
   RenderString(GLUT_BITMAP_9_BY_15,text);

glPopMatrix();

glutSwapBuffers();
}


/*! InitGL set's up the opengl drawing state */
void InitGL()
{
glClearColor(0.3f, 0.3f, 0.3f, 1.0f);			   // Grey Background
// disable alpha blending as we won't be using it
glDisable(GL_BLEND);
// if we havn't set a camera in the file set a default one
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(WIDTH*64/48.0,HEIGHT*64/48.0, -WIDTH,HEIGHT, 0.1, 450.0);
	SetCamera(EYE,LOOK,UP,640,480,45,64/48,0.1,450.0);
// render things smoothly
glShadeModel(GL_SMOOTH);
// enable depth testing for drawing
glEnable(GL_DEPTH_TEST);
}
/*! the idle function called every time glut is not busy */
void idle(void)
{
glutPostRedisplay();
}


// application main loop
int main(int argc, char **argv)
{
	// buffer to hold the txt for the main banner
    char txt[300];

    //initialise glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(10, 15);
    // create the banner text
    strcpy(txt,"FlockLayout : ParamFile =");
    strcat(txt,argv[1]);
    strcat(txt,"   www.dec.bournemouth.ac.uk/staff/jmacey/flock.html");
    strcat(txt," Press h for Menu ");
    // create the main window
    if(argc < 5)
    	{
        cout << "error arguments are Layout <filname> <BBox extents W,H,D>"<<endl;
		exit(0);
        }
    /* Open file and test for successful operation */
    strcpy(fname,argv[1]);

	GLfloat w,h,d;
    w=atof(argv[2]);
	h=atof(argv[3]);
	d=atof(argv[4]);
    Point3 p(0,0,0);
    bb = new BBox(p,w,h,d);
    glutCreateWindow(txt);
	//set the time to the current time
    //load the glut callbacks
    glutDisplayFunc(display);
    glutIdleFunc(idle);
	glutMouseFunc(MouseHandler);
    glutMotionFunc(MouseMoved);
    glutKeyboardFunc(KeyPressed);
    glutSpecialFunc(SpecialKeyPressed);
	//initialise opengl
    InitGL();
// read in the script file and setup the environment
	Xpos=0.0;
    Ypos=0.0;
    Zpos=0.0;
	// setup the subwindows
    glutMainLoop();
	//this never gets called due to glutmainloop not exiting properly
    return 1;
}


//end of file

