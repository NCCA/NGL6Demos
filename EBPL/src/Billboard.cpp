#include "Billboard.h"
using namespace std;
Billboard::Billboard(Camera Cam, bool FullSize,char *TexName)
{
LoadTexture(TexName);
CalcBB(Cam,FullSize);
TextureID=Tex1.setTexture();
Rotate=FullSize;
}
Billboard::Billboard(Camera Cam, bool FullSize)
{
Rotate=FullSize;
CalcBB(Cam,FullSize);
}

void Billboard::Reset(Camera cam)
{
Rotate=true;
CalcBB(cam,false);

}
void Billboard::CalcBB(Camera Cam, bool FullSize)
{
// cam axis is assumed to be three 4tuples, ordered x, y, z so...
// strip out the camera axis and build vectors
Vector cam_up=Cam.v;
Vector cam_left=Cam.u;
// find diagonal vectors (4 points of poly)
ul=cam_up + cam_left;
ur=cam_up - cam_left;
ul.normalize();
ur.normalize();

if(FullSize==true)
  {

//	O=Tan(viewAngle/2)*(Far-near)
	float offset=tan(Cam.viewAngle/2)*(Cam.farDist-Cam.eye.z);
	offset/=4.0;
	ul.x*=offset;
	ul.z*=offset/Cam.aspect;
	ur.x*=offset;
	ur.z*=offset/Cam.aspect;

  }
dl=ur;    dl.Reverse();
dr=ul;    dr.Reverse();
}


void Billboard::Draw(void)
{
	glDisable(GL_LIGHTING);
	glBegin(GL_TRIANGLE_STRIP);

	glColor3f(1.0,0,0);
   glVertex3f(dl.x,dl.y,dl.z);  // bottom left
	glColor3f(0.0,1.0,0.0);
   glVertex3f(ul.x,ul.y,ul.z);   // top left
	glColor3f(0.0,0.0,1.0);
   glVertex3f(dr.x,dr.y,dr.z);   // bottom right
	glColor3f(1.0,1.0,1.0);
   glVertex3f(ur.x,ur.y,ur.z);    // top right
glEnd();
}

void Billboard::LoadTexture(char *TexName)
{
#ifdef _DEBUG
	cout <<"T= "<<TexName<<endl;
#endif
Tex1.loadImage(TexName);
//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
TextureID=Tex1.setTexture();
}

void Billboard::DrawTextured(void)
{
//glPushMatrix();
glEnable(GL_TEXTURE_2D);
glDisable(GL_LIGHTING);
/*
glTexImage2D(GL_TEXTURE_2D, 0, 3, Tex1.getX(), Tex1.getY(), 0,
			 GL_RGB, GL_UNSIGNED_BYTE, Tex1.GetImage());
glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);*/
glScalef(12,12,1);
glBindTexture(GL_TEXTURE_2D,TextureID);

glBegin(GL_TRIANGLE_STRIP);
   glTexCoord2f(0.0f, 0.0f);
   glVertex3f(dl.x,dl.y,dl.z);  // bottom left
   glTexCoord2f(0.0f, 1.0f);
   glVertex3f(ul.x,ul.y,ul.z);   // top left
   glTexCoord2f(1.0f, 0.0f);
   glVertex3f(dr.x,dr.y,dr.z);   // bottom right
   glTexCoord2f(1.0f, 1.0f);
   glVertex3f(ur.x,ur.y,ur.z);    // top right
glEnd();
glDisable(GL_TEXTURE_2D);
glEnable(GL_LIGHTING);
//glPopMatrix();
}
