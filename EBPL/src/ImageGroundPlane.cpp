#include "ImageGroundPlane.h"
#include "Util.h"
using namespace std;  
using namespace GraphicsLib; 

ImageGroundPlane::ImageGroundPlane(BBox *ext,GLfloat y,char *Fname,GLfloat YY)
: GroundPlane(ext,y,1.0,1.0,1.0,1.0)
{
image = new Texture(Fname);
// and calculate how many points we need to create the gp
Xsteps=image->Width;
Zsteps=image->Height;
gpPoints = new Point3[Xsteps*Zsteps];
nPoints = Xsteps*Zsteps;
int index=0;
Ydivisior=YY;
// calculate the lengths of the object in x,y,z
GLfloat xlen,zlen;

xlen=MaxX-MinX;
zlen=MaxZ-MinZ;
// calculate the initial step
GLfloat xstep=xlen/Xsteps;
GLfloat zstep=zlen/Zsteps;
divx=xlen/Xsteps;
divz=zlen/Zsteps;

GLfloat xpos;
GLfloat zpos=MinZ;
// now create the gp point lattice as a series of triangle strips
for(int z=0; z<Zsteps; z++)
	{
	xpos=MinX;
    for (int x=0; x<Xsteps; x++)
    	{
		gpPoints[index].set(xpos, y+ (image->GetFromXY(x,z)/Ydivisior),zpos);
		xpos+=xstep;
    	index++;
    	}
	zpos+=zstep;
	}
// finally create the display list
CreateDisplayList();

}


ImageGroundPlane::ImageGroundPlane(BBox *ext,GLfloat y,char *Fname, char * Fname2,GLfloat YY)
: GroundPlane(ext,y,1.0,1.0,1.0,1.0)
{
// calculate the extents of the gp
// load in the image
image = new Texture(Fname);
// load in the texture
Tex = new Texture(Fname2);
// calculate the number of points required
Xsteps=image->Width;
Zsteps=image->Height;
gpPoints = new Point3[Xsteps*Zsteps];
texPoints= new Point3[Xsteps*Zsteps];
nPoints = Xsteps*Zsteps;
int index=0;
Ydivisior=YY;
// calculate the lengths of the object in x,y,z
GLfloat xlen,zlen;

xlen=MaxX-MinX;
zlen=MaxZ-MinZ;
// calculate the initial step
GLfloat xstep=xlen/Xsteps;
GLfloat zstep=zlen/Zsteps;
divx=xlen/Xsteps;
divz=zlen/Zsteps;

Tdivx= 1.0/Xsteps;
Tdivz= 1.0/Zsteps;
// now create the actual display lattice triangle strips
GLfloat xpos;
GLfloat zpos=MinZ;
GLfloat xtex=0.0;
GLfloat ztex=0.0;
for(int z=0; z<Zsteps; z++)
	{
	xpos=MinX;
    xtex=0.0;
    for (int x=0; x<Xsteps; x++)
    	{
        gpPoints[index].set(xpos, y+ (image->GetFromXY(x,z)/Ydivisior),zpos);
		texPoints[index].set(xtex,ztex,0);
        xpos+=xstep;
        xtex+=Tdivx;

    	index++;
    	}
	zpos+=zstep;
    ztex+=Tdivz;
	}
// set the texture values
//UsingImage = true;
Tname=Tex->setTextureGL();
// crate the display lists
CreateDisplayList();


}

ImageGroundPlane::~ImageGroundPlane()
{
// remove the lists
	delete [] gpPoints;
}


 
GLfloat ImageGroundPlane::GetHeight(GLfloat x, GLfloat z)
{

// if were not using an image return the normal y value
 // else determine where in the terrain we are and get that value
 if( (x < MinX || x > MaxX) && (z < MinZ || z > MaxZ) )
		return MinY;
  else
     	{
		 GLfloat XX=rint((rint(x)-MinX)/divx);
		 GLfloat ZZ=rint((rint(z)-MinZ)/divz);
         int index=(int)XX + (int)(ZZ*Xsteps);
         if(index >(Xsteps*Zsteps))
         	return MinY;
         else
         return gpPoints[index].y;
        }

}
 
void ImageGroundPlane::CreateDisplayList(void)
{
    int index=0;
// create a new list in compile mode so we only create it and not draw it
glNewList(ListIndex,GL_COMPILE);

	glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glShadeModel(GL_SMOOTH);
glBindTexture(GL_TEXTURE_2D,Tname);

	// creat the display list
    for(int Y=0; Y<Zsteps-1; Y++)
	{
		glBegin(GL_TRIANGLE_STRIP);

		for(int X=0; X<Xsteps-1; X++)
		{

	
	    if(X <=Xsteps-2)
		{
		// calculate the normals for the 3 triangles
		Vector n1=
		CalcNormal (gpPoints[index],gpPoints[index+Xsteps],gpPoints[index+1]);
 		Vector n2=
		CalcNormal (gpPoints[index+1],gpPoints[index+Xsteps],gpPoints[index+Xsteps+1]);
		Vector n3=
		CalcNormal (gpPoints[index+Xsteps],gpPoints[index],gpPoints[index+Xsteps+1]);

		// normalise them and reverse n1 and n2 as they face the wrong way
        n1.normalize();
		n2.normalize();
		n3.normalize();
        n1.reverse();
        n2.reverse();
        
		// set the normal
        n1.Normal();
		if(Tex !=NULL)
    	   	glTexCoord2f(texPoints[index].x,texPoints[index].y);
		// draw the vertex
        gpPoints[index].Vertex();
		// and if using textures se the texture cords
		
		
		n2.Normal();
		if(Tex !=NULL)
        	glTexCoord2f(texPoints[index+Xsteps].x,texPoints[index+Xsteps].y);
        gpPoints[index+Xsteps].Vertex();
		
				
		n3.Normal();
		if(Tex !=NULL)
	      	glTexCoord2f(texPoints[index+1].x,texPoints[index+1].y);
        gpPoints[index+1].Vertex();

		//tri 2
		
		n2.Normal();		
		if(Tex !=NULL)
        	glTexCoord2f(texPoints[index+1].x,texPoints[index+1].y);
		gpPoints[index+1].Vertex();
		
		n3.Normal();
		if(Tex !=NULL)
        	glTexCoord2f(texPoints[index+Xsteps].x,texPoints[index+Xsteps].y);
        gpPoints[index+Xsteps].Vertex();

		n1.Normal();
		if(Tex !=NULL)
        	glTexCoord2f(texPoints[index+Xsteps+1].x,texPoints[index+Xsteps+1].y);
        gpPoints[index+Xsteps+1].Vertex();
   		

		}
		index++;
		}
		index+=1;
		glEnd();
        }

       glDisable(GL_LIGHTING);
       glDisable(GL_TEXTURE_2D);

glEndList();
}


void ImageGroundPlane::Draw(void)
{
glCallList(ListIndex);
}




