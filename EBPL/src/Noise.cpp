#include "Noise.h"
#include <cmath>
using namespace ngl;
GLfloat Noise::SQR(GLfloat in)
{
return in*in;
}


void Noise :: ResetTables(void)
{
int i;
srand(time(NULL));
for(i=0; i<256; i++)
	{
	int which=rand()%256;
	unsigned char tmp=index[which];
	index[which]=index[i];
	index[i]=tmp;
	}
for(i=0; i<256; i++) noiseTable[i]=rand()/32767.99;


}

Noise :: Noise()
{
int i;
index = new unsigned char[256];
for(i=0; i<256; i++)
	index[i]=i;

for(i=0; i<256; i++)
	{
	int which=rand()%256;
	unsigned char tmp=index[which];
	index[which]=index[i];
	index[i]=tmp;
	}

noiseTable = new float[256];
for(i=0; i<256; i++) noiseTable[i]=rand()/32767.99;

}

GLfloat Noise::latticeNoise(int i, int j, int k)
{
#define PERM(x) index[(x)&255]
#define INDEX(ix,iy,iz) PERM( (ix) + PERM((iy)+PERM(iz)))

return noiseTable[INDEX(i,j,k)];


}

GLfloat Noise::noise(GLfloat scale, ngl::Vec3 p)
{

#define Lerp(F, A,B) A + F * ( B - A )
GLfloat d[2][2][2];
Vec3 pp;
pp.m_x=p.m_x * scale + 10000;
pp.m_y=p.m_y * scale + 10000;
pp.m_z=p.m_z * scale + 10000;

long ix = (long) pp.m_x;
long iy = (long) pp.m_y;
long iz = (long) pp.m_z;
GLfloat tx,ty,tz,x0,x1,x2,x3,y0,y1;
tx=pp.m_x-ix; ty=pp.m_y-iy; tz=pp.m_z-iz;


for(int k=0; k<=1; k++)
for(int j=0; j<=1; j++)
for(int i=0; i<=1; i++)
	d[k][j][i]=latticeNoise(ix+i,iy+j,iz+k);

x0=Lerp(tx, d[0][0][0],d[0][0][1]);
x1=Lerp(tx, d[0][1][0],d[0][1][1]);
x2=Lerp(tx, d[1][0][0],d[1][0][1]);
x3=Lerp(tx, d[1][1][0],d[1][1][1]);
y0=Lerp(ty, x0,x1);
y1=Lerp(ty, x2,x3);
return Lerp(tz,y0,y1);

}

GLfloat Noise::undulate(GLfloat x)
{
if(x<-0.4) return 0.15 + 2.857 * SQR(x+0.75);
else if(x < 0.4)return 0.95 - 2.9125 * SQR(x);
else return 0.26 + 2.666 * SQR(x-0.7);



}

GLfloat Noise :: turbulance(GLfloat s, ngl::Vec3 p)
{
return 0.5*noise(s,p) + 0.25*noise(2*s,p)+0.125*noise(4*s,p);
}

GLfloat Noise::marble(GLfloat A, GLfloat s, ngl::Vec3 p)
{
return undulate(sin(2*M_PI*p.m_z+A*turbulance(s,p)));

}



