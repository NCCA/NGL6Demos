
#include "AnimatedObj.h"
#include "Util.h"
using namespace std;
using namespace ngl; 

void AnimatedObj::SetFrameNo(int fn)
{
// set the frame number
FrameNo=fn;
}
AnimatedObj::AnimatedObj(char *obj1, char *obj2, int nframes,AnimateModes amode,int Mat)
{

//// set default values
//AnimateMode=amode;
//nFrames=nframes;
//M = new Material(Mat);
//UseMaterial=true;
	
//int fo=0;
//// depending on the animate mode we need to set the start frame
//switch (AnimateMode)
//{
//case FORWARDANIM :FrameNo=0; break; //if we are going forward set start frame to 0

//case LOOPANIM :
//    // if we are loopin it is easier to create a frame sequence and incriment through it
//    // each time
//    // first we create an array to hold the frame sequence
//    lframes= new int[nframes*2];
//	// now we loop through forwards storing the frame offsets
//    for(int i=0; i<nframes; i++)
//    	{lframes[fo++]=i; }
//	// the backwards to complete the loop
//    for(int i=nframes; i>0; i--)
//    	{lframes[fo++]=i; }
//    // finally we double the number of frames as we are looping
//    nFrames=nframes*2;
//    FrameNo=0;
//break;
//case BACKWARDANIM : FrameNo=nFrames; break; //start at the last frame
//UseMaterial =false;
//}
//// now load both the obj files
//cout <<"Loading 1 "<<obj1<<endl;
//Obj StartObj(obj1,false);
//cout <<"Loading 2 "<<obj2<<endl;
//Obj EndObj(obj2,false);
///*StartObj.Scale(2,2,2);
//EndObj.Scale(2,2,2);*/
//// we are going to loop from t=0.0 to t=1.0 in steps of 1.0/nframes
//GLfloat t=0.0f;
//GLfloat tstep= 1.0f/(GLfloat)nFrames;
//// now we need to allocate a sequence of display lists
//GLuint range=glGenLists(nframes);
//// now for each of the frames we caluculate the vertices and normals by tweening
//// from obj1 -> obj2 at time t
//for (int f=0; f<=nFrames; f++)
//	{
//	// create new display list
//    glNewList(range+f,GL_COMPILE);
//	// add display list frame to the frame list
//    Frames.push_back(range+f);
//    //now for each face
//    ///cout <<"Num Faces "<<StartObj.nFaces<<" "<<EndObj.nFaces<<endl;
//    for(unsigned long int i=0; i<StartObj.nFaces; i++)
//	{
   
//    //determine if it is a triangle or quad
//	if(StartObj.Face[i].NumVerts == 2)
//		glBegin(GL_TRIANGLES);
//    else
//		glBegin(GL_QUADS);
//	for(unsigned long int x=0; x<=StartObj.Face[i].NumVerts; x++)
//		{
//        unsigned long int V=StartObj.Face[i].Vert[x];
//		unsigned long int N=StartObj.Face[i].Norm[x];
//    //	unsigned long int T=StartObj.Face[i].Tex[x];
		
//		//and use the lerp function to calculate the new verts and normals
        
///*         glNormal3f(
//         			lerp(StartObj.Norm[N].x,EndObj.Norm[N].x,t),
// 					lerp(StartObj.Norm[N].y,EndObj.Norm[N].y,t),
//                     lerp(StartObj.Norm[N].z,EndObj.Norm[N].z,t));*/
//		         glNormal3f(
//         			StartObj.Norm[N].x,
// 					StartObj.Norm[N].y,
//                     StartObj.Norm[N].z);

//        glVertex3f(
//        						Lerp(StartObj.Verts[V].x,EndObj.Verts[V].x,t),
//										Lerp(StartObj.Verts[V].y,EndObj.Verts[V].y,t),
//          					Lerp(StartObj.Verts[V].z,EndObj.Verts[V].z,t)
//          				);
//        }
//		glEnd();

//	}
//	// end the list
//    glEndList();
//	#ifdef __DEBUG_
////	cout << "Calculating Frame "<< f <<" tstep "<<t<<endl;
//    #endif
//    // change the time step
//    t+=tstep;
//    }//end frame loop

//if(AnimateMode == LOOPANIM)
//{
//for(int i=0; i<nframes; i++)
//	{
//    Frames.push_back(nFrames-i);
//    }
//}


}
void AnimatedObj::UpdateFrameNo(void)
{
// update the frame number of the current frame to be drawn
switch(AnimateMode)
{
case FORWARDANIM :
case LOOPANIM :
	FrameNo++;
break;
case BACKWARDANIM :
	FrameNo--;
break;
}

}
void AnimatedObj::Draw(void)
{
glPushMatrix();
// set the material to use
//M->Use(1.0);
//call the correct display list
if(AnimateMode ==FORWARDANIM )
	{
	if(FrameNo >= nFrames)
		FrameNo=0;
  	glCallList(Frames[FrameNo]);
    }
else if(AnimateMode ==BACKWARDANIM)
	{
	if(FrameNo <= nFrames)
		FrameNo=nFrames;
        glCallList(Frames[FrameNo]);
  	}
else
	{
    if(FrameNo >= nFrames)
		FrameNo=0;
    glCallList(Frames[lframes[FrameNo]]);
    }
glPopMatrix();
}


AnimatedObj::~AnimatedObj()
{
Frames.erase(Frames.begin(),Frames.end());
}


void AnimatedObj::SetMaterial(int mat)
{
//M->Change((STDMAT)mat);
}
