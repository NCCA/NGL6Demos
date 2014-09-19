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
#include "GlutCallback.h"
#include "Util.h"
#include <stdio.h>  // for printf
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include "BBox.h"
#include "Environment.h"
#include "CurveFollow.h"
#include "EnvObj.h"
#include "ParamFile.h"
#include <tiffio.h>     /* Sam Leffler's libtiff library. */

using namespace std;
using namespace GraphicsLib;

// Variables to store the main and sub window id's
int Win;
int win2;
int win3;
int win4;
// width and height of the window
int WIDTH = 720; //1024;
int HEIGHT = 576; //800;
// The Environment used by all the simulations
Environment *Env;
// The world bounding box used by all the simulations
BBox *bb;

char s[30]; 
// default update rate for simulation
int updateRate=1;
bool Fullscreen = false;
// flags used for various parts of the simulations
// use a random seed set in param file
bool RandomSeed=false;
// path follow set by pressing the 'c' key
bool PathFollow=false;
// draw env set by the 'P' key
bool DrawEnvToggle=false;
// cdir is the current direction the centroid is following
bool cdir=true;
// flag to indicate if the simulation is to be paused set by the 'p' key
bool Pause=false;
// flag to indicate if flocking is on or off set by the ' ' space key
bool FlockMode=false;
// flag to indicate if the menu is to be drawn set by the 'h' key
bool HideMenu=true;
// flag to indcate if the mouse rotate set by the F1 key
bool MouseRotate=true;
// write to tiff file
bool WriteTiff = false;
char TiffName[200];
// Camera control flags
extern bool CameraSet;
extern bool CamFollowCentroid;
extern bool DrawEnvDetails;
//bool CamFollowRover=false;
extern bool CamFollowCentroidIndex;
// the Camera offset from the centroid set in the script file and defaults to 10.0
extern GLfloat cEoffx,cEoffy,cEoffz;
// the values to spin the main window by in x y and z
extern int spinxface  ;
extern int spinyface  ;
extern int spinzface  ;
int FPSframe=0,FPStime,FPStimebase=0;
GLfloat fps;
// defualt camera positions
Point3 EYE(80.0f,70.0f,120.0f);
Point3 LOOK(0.0f,0.0f,0.0f);
Vector UP(0.0f,1.0f,0.0f); //Y == UP
// The current simulation time
GLfloat TIME=0.0f;
// the difference in time in seconds
GLfloat dtSec;
// how much we incriment the follow path each step
GLfloat PathStep=0.01f;
// The current time
time_t Time;
unsigned long int NFrames=0;
double AveFrame=0.0;
bool DrawLattice=false;
int ClipW=720; int ClipH=576;

/*! witetiff write a tiff file in the directory ../Frames
\param char *filename the name of the file to write
\param char *description the text to write in the tiff
\param int x the x position to start the screen grab from
\param int y the y position to start the screen gram from
\param int width the width of the image to write
\param int height the height of the image to write
\int compression (not LZW doesn't wor) use COMPRESSION_NONE
*/

int writetiff(char *filename, char *description,
  int x, int y, int width, int height, int compression)
{
   TIFF *file;
   GLubyte *image, *p;
   int i;
 // open the file (uses libtiff)
   file = TIFFOpen(filename, "w");
   if (file == NULL) {
     return 1;
   }
   image = (GLubyte *) malloc(width * height * sizeof(GLubyte) * 3);

  /* OpenGL's default 4 byte pack alignment would leave extra bytes at the
     end of each image row so that each full row contained a number of bytes
     divisible by 4.  Ie, an RGB row with 3 pixels and 8-bit componets would
     be laid out like "RGBRGBRGBxxx" where the last three "xxx" bytes exist
     just to pad the row out to 12 bytes (12 is divisible by 4). To make sure
     the rows are packed as tight as possible (no row padding), set the pack
     alignment to 1. */
   glPixelStorei(GL_PACK_ALIGNMENT, 1);
//set  the tiff feilds (from the nate robin demo)
  glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
  TIFFSetField(file, TIFFTAG_IMAGEWIDTH, (uint32) width);
  TIFFSetField(file, TIFFTAG_IMAGELENGTH, (uint32) height);
  TIFFSetField(file, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(file, TIFFTAG_COMPRESSION, compression);
  TIFFSetField(file, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField(file, TIFFTAG_SAMPLESPERPIXEL, 3);
  TIFFSetField(file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(file, TIFFTAG_ROWSPERSTRIP, 1);
  TIFFSetField(file, TIFFTAG_IMAGEDESCRIPTION, description);
  p = image;
  for (i = height - 1; i >= 0; i--) {
    if (TIFFWriteScanline(file, p, i, 0) < 0) {
      free(image);
      TIFFClose(file);
      return 1;
    }
    p += width * sizeof(GLubyte) * 3;
  }
  TIFFClose(file);
  return 0;
}




/*! function to call the redisplays for all the subwindows and the main window
sets the active window to the main window (Win) at the end */
void RedisplayAll(void)
{
if(Fullscreen == false)
{
glutSetWindow(win2);
glutPostRedisplay();
glutSetWindow(win3);
glutPostRedisplay();
glutSetWindow(win4);
glutPostRedisplay();
glutSetWindow(Win);
}

glutPostRedisplay();
}


/*! function to write a string to the screen
\param void *font the font to render with (see glut.h for more info)
\param const char *string the string to render */
void RenderString(void* font, const char* string)
{
	char* p = (char*) string;
	while (*p != '\0') glutBitmapCharacter(font, *p++);
}

/*! this function calculates the current time on the centroid follow path */

void UpdatePath(void)
{
//if we are following a path
if(PathFollow==true)
    {
    // if were going forward incriment
    if(cdir==true)
    {
    TIME +=PathStep;
    // until we get to 1.0 then we reverse
    if(TIME > 1.0f)
        {cdir=false;}
    }
	else
    {
    // else were going backwards so do the reverse
    TIME -=PathStep;
    if(TIME < 0.0f)
        {cdir=true;}
    }
	// now we update the curve path time for each of the emitters
    for(unsigned long int i=0; i<Env->nEmitters; i++)
   		Env->SetCurvePathTime(i,TIME);
	}

}

/*! draw a simple menu on the screen this is a bit buggy as the camera isn't taken into
account properly
\todo sort out menu so it works from any camera angle
*/
void Menu(void)
{

//GLfloat x=-8.0f;
//GLfloat y=-12.0f;
//GLfloat z=0.0f;
char text[150];


glPushMatrix();
glTranslatef(0,0,0);
   //move to the correct position and render the strings
glRasterPos2f(-20.0f,26.0f);
  // sprintf(text,"Simulation Time: %04ds Flock Mode %s Path Follow %s FPS %s",(int)dtSec,FlockMode ?  "On" : "Off",
   //			 PathFollow ? "On" : "Off",s);

   // just write the FPS for now the whole menu need re-sorting
   sprintf(text,"FPS %s",s);
   RenderString(GLUT_BITMAP_9_BY_15,text);
   
   /*
   glColor3f(1.0f,1.0f,0.0f);
   glRasterPos3f(x,y,z);
   RenderString(GLUT_BITMAP_9_BY_15,"[Space] : Toggle Flock On/Off");
   glRasterPos2f(x,y-2.0f);
   RenderString(GLUT_BITMAP_9_BY_15,"c : Toggle follow curve");
   glRasterPos2f(x,y-4.0f);
   RenderString(GLUT_BITMAP_9_BY_15,"p : pause Simulation");
   glRasterPos2f(x,y-6.0f);
   RenderString(GLUT_BITMAP_9_BY_15,"0 : dump to output file");
   glRasterPos2f(x,y-8.0f);
   RenderString(GLUT_BITMAP_9_BY_15,"d : debug Agents");
   glRasterPos2f(x,y-10.0f);
   RenderString(GLUT_BITMAP_9_BY_15,"h : Hide Menu");
   glRasterPos2f(x,y-12.0f);
   RenderString(GLUT_BITMAP_9_BY_15,"Mouse L/R button rotates View");
*/
glPopMatrix();


}

/*! This is the main window display routine */
void display()
{
	// clear the current buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // display the menu

    // update the centroid path
    UpdatePath();
	// if we are following the centroid with the camera re-set the camera to the
    // current centroid position
    if(CamFollowCentroid==true)
		{
        Point3 cEye=Env->Emitters[CamFollowCentroidIndex].GetCentroid();
		Point3 cLook(cEye.x-cEoffx,cEye.y-cEoffy,cEye.z-cEoffz);
        SetCamera(cLook,cEye,UP,640,480,45,64/48,0.1f,450.0f);
        }
// 	else if(CamFollowRover==true)
// 		{
//         Point3 cEye=Env->rover->pos;
// 		Point3 cLook(cEye.x+5.0,cEye.y+1.0,cEye.z);
//         SetCamera(cLook,cEye,UP,640,480,45,64/48,0.1f,450.0f);
//         }

    //presereve the current transformation stack
	glPushMatrix();
        glPushMatrix();
        //rotate the global scene based on the mouse rotations
     		if(HideMenu ==false)
    	        {
				glColor3f(1.0f,1.0f,0.0f);
				Menu();
				}
			// do the mouse rotations
			if(MouseRotate)
                 {
                 glRotated	( (GLdouble) spinxface, 1.0, 0.0, 0.0 ) ;
				 glRotated	( (GLdouble) spinyface, 0.0, 1.0, 0.0 ) ;
				 glRotated	( (GLdouble) spinzface, 0.0, 0.0, 1.0 ) ;
                 }
				

            //draw the axis and the Agents
            if(DrawEnvDetails == true)
            	DrawAxisSimple(50.0f);
			// draw the bin lattice
			if(DrawLattice == true)
				Env->lat->Draw();
            Env->DoUpdates(0);
        glPopMatrix();
    glPopMatrix();

    // render
    glutSwapBuffers();
}


/*! Display2 draws the orthographic top projection of the agents */
void Display2()
{
//clear screen
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// setup for ortho projection
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
double winHt = 60.0; // half-height of the window
glOrtho(-winHt, winHt, -winHt, winHt, 0.001f, 10000.0f);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(0,100,0, 0, 0.0,0, 0.0,0.0,-1.0);
// draw the agents ect.
DrawAxisSimple(50.0);
if(DrawLattice == true)
		Env->lat->Draw();
Env->DoUpdates(1);
glutSwapBuffers();
}
/*! Display3 draws the orthographic side projection of the agents looking down X*/
void Display3()
{
//setup the projection
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
double winHt = 60.0; // half-height of the window
glOrtho(-winHt, winHt, -winHt, winHt, 0.001, 10000.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(100,0,0, 0, 0.0,0, 0.0,1.0,0.0);
//draw simulation
DrawAxisSimple(50.0);
if(DrawLattice == true)
	Env->lat->Draw();
Env->DoUpdates(1);
glutSwapBuffers();
}
/*! Display4 draws the orthographic side projection of the agents looking down Z*/
void Display4()
{
//setup the projection
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
double winHt = 60.0; // half-height of the window
glOrtho(-winHt, winHt, -winHt, winHt, 0.001, 10000.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(0,0,100, 0, 0.0,0, 0.0,-1.0,0.0);
//draw the agents
glRotatef(180,0,0,1);
DrawAxisSimple(50.0);
if(DrawLattice == true)
		Env->lat->Draw();
Env->DoUpdates(1);
glutSwapBuffers();
}
/*! InitGL set's up the opengl drawing state */
void InitGL()
{
glClearColor(0.3f, 0.3f, 0.3f, 1.0f);			   // Grey Background
// disable alpha blending as we won't be using it
glDisable(GL_BLEND);
// if we havn't set a camera in the file set a default one
if(CameraSet == false)
	{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(WIDTH*64/48.0,HEIGHT*64/48.0, -WIDTH,HEIGHT, 0.1, 450.0);
	SetCamera(EYE,LOOK,UP,WIDTH,HEIGHT,45,64/48,0.1,450.0);
	}
// render things smoothly
glShadeModel(GL_FLAT);
// enable depth testing for drawing
glEnable(GL_DEPTH_TEST);
//glDisable(GL_DEPTH_TEST);
GLfloat Colour[]={1.0,1.0,1.0,1.0};
glLightModelfv(GL_LIGHT_MODEL_AMBIENT,Colour);
glEnable(GL_LIGHT0);
glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}
/*! the idle function called every time glut is not busy */
void idle(void)
{
// if were not paused update the emitters
if(Pause == false)
{
for(int i=0; i<updateRate; i++)
	Env->UpdateEmitters();
}
// now redisplay everything
RedisplayAll();
// calculate the FPS stuff
// update the difference in time
time_t now=time(NULL);
dtSec=difftime(now,Time);
	
FPSframe++;
NFrames++;
	FPStime=glutGet(GLUT_ELAPSED_TIME);
	if (FPStime - FPStimebase > 1000) {
		sprintf(s,"%4.2f",
		FPSframe*1000.0/(FPStime-FPStimebase));
		FPStimebase = FPStime;
		FPSframe = 0;
		AveFrame+=FPSframe*1000.0/(FPStime-FPStimebase);
		//NFrames++;
	}
	// if were writing out TIFF files generate the filename
	if(WriteTiff==true)
		{
		char t[300];
		sprintf(t,"../Frames/%s.%03ld.tif",TiffName,NFrames);
		writetiff(t, "Flock Render", 0, 0, ClipW,ClipH,COMPRESSION_NONE); //PACKBITS);
		}
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
    strcpy(txt,"Flock : ParamFile =");
    // create the name for the tiff file output
	strcat(txt,argv[1]);
	strcpy(TiffName,argv[1]);
	for(unsigned int i=0; i<strlen(TiffName); i++)
		{
		if(TiffName[i]=='.')
			{TiffName[i]='_'; break;}
			
		}
    strcat(txt,"   www.dec.bournemouth.ac.uk/staff/jmacey/flock.html");
    strcat(txt," Press h for Menu ");
    
    // check arguments for the differnt mode
	if( argc >=3)
    	{
        if(argv[2][0] == 'f' || argv[2][0] == 'c')
    		{
        	Fullscreen =true;
        	//glutGameModeString("800x600:32@60");
	    	char res[100];
			if(argv[2][0]=='c')
				{
				sprintf(res,"%dx%d:32@60",ClipW,ClipH);
				Win=glutCreateWindow(txt);
				}
			else
			{
			//printf(res,"%dx%d:32@60",WIDTH,HEIGHT);
			glutGameModeString("1024x800:32@60");
			glutEnterGameMode();
			}
	
  			}
     	}
	
    else
    	{
    	// create the main window
    	Win=glutCreateWindow(txt);
		Fullscreen = false;
    	}
    if( argc >=4)
    	{
        if(argv[3][0] == 'p')
    		{
        	Pause = true;
  			}
     	}

	//set the time to the current time
    Time=time(NULL);
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
    ReadParamFile(argv[1]);//,Env,bb);
	Env->OpenOutFile();
    Env->AddAgentsToWorldList();
    if(RandomSeed == true)
    	srand(time(NULL));
    else
    	srand(2); // use the same seed

	if(Fullscreen == false)
    {
	// setup the subwindows
    win2=glutCreateSubWindow(Win,0,0,240,220);
    glutSetWindow(win2);
    glutDisplayFunc(Display2);
    InitGL();
	glutKeyboardFunc(KeyPressed);
    glutSpecialFunc(SpecialKeyPressed);

	win3=glutCreateSubWindow(Win,0,220,240,220);
    glutSetWindow(win3);
    glutDisplayFunc(Display3);
    InitGL();
	glutKeyboardFunc(KeyPressed);
    glutSpecialFunc(SpecialKeyPressed);

	win4=glutCreateSubWindow(Win,0,440,240,220);
	glutSetWindow(win4);
	glutDisplayFunc(Display4);
	InitGL();
	glutKeyboardFunc(KeyPressed);
    glutSpecialFunc(SpecialKeyPressed);
    }
	//write details to the output file
    //FILE *f=Env->getOutFile();
    //fprintf(f,"FLOCKING SYSTEM OUTPUT FILE\n");
	//fprintf(f,"FRAME_COUNT 500\n");
	//Env->OutputSpeciesToFile();
	// start the simulation

    glutMainLoop();
	//this never gets called due to glutmainloop not exiting properly
    return 1;
}


//end of file

void ReLoadScript(void)
{
	char buff[100];
 //   delete Env;
    cout <<"Enter New File Name "<<endl;
    cin >> buff;
    ReadParamFile(buff);//,Env,bb);
	Env->OpenOutFile();
    Env->AddAgentsToWorldList();
// 	FILE *f=Env->getOutFile();
//     fprintf(f,"FLOCKING SYSTEM OUTPUT FILE\n");
// 	fprintf(f,"FRAME_COUNT 500\n");
// 	Env->OutputSpeciesToFile();
	
}



