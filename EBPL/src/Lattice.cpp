#include "Lattice.h"
#include "Material.h"


using namespace std;
using namespace GraphicsLib; 

Lattice::Lattice(int xin, int yin, int zin,BBox *bb,Environment *e,int BinSize)
{
// copy parameter
DoCollide=false;
ext = bb;
Env = e;
//Obj = obj;
Xdiv=xin;
Ydiv=yin;
Zdiv=zin;
Npoints=0;
// calculate the lengths of the object in x,y,z
GLfloat xlen,ylen,zlen;

xlen = bb->width;
ylen = bb->height;
zlen = bb->depth;
// calculate the initial step
GLfloat xstep=xlen/Xdiv;
GLfloat ystep=ylen/Ydiv;
GLfloat zstep=zlen/Zdiv;

//make sure the steps go from the extents of the bbox so we need to add a small ammount to the
//lattice propotional to -1 the number of lattice steps
xstep+=(xstep/(Xdiv-1));
ystep+=(ystep/(Ydiv-1));
zstep+=(zstep/(Zdiv-1));
Xstep=xstep; Ystep=ystep; Zstep=zstep;
#ifdef __DEBUG_
//cout <<"Lengths "<< xlen <<","<<ylen<<","<<zlen<<endl;
//cout <<"STEPS "<< Xstep<<" "<<Ystep<<" "<<Zstep<<endl;
#endif
// now to create the lattice points
  GLfloat z=bb->center.z-bb->depth/2;
  for(int zz=0; zz<Zdiv; zz++, z+=zstep )
    {
    GLfloat y=bb->center.y-bb->height/2;
		for(int yy=0; yy<Ydiv; yy++, y+=ystep)
        {
		GLfloat x=bb->center.x-bb->width/2;
         for(int xx=0; xx<Xdiv; xx++, x+=xstep)
            {
            // create a point
            Point3 p(x,y,z);
            LatticePoint lp(p,1.0);
			// add the lattice point to the list
            Points.push_back(lp);
			// incriment the point count
            Npoints++;
            }
   		}
     }
#ifdef __DEBUG_
	//cout <<"Npoints "<<Npoints<<endl;
#endif
// now we create the bins
bins = new LatticeBin[xin*yin*zin];
nBins = xin*yin*zin;
//for(unsigned long int i=0; i<nBins; i++)
  Point3 p[8]; 
  // now we have to set the size of the bins and there positions in space
  int i=0;
  for(int Z=0; Z<Zdiv; Z++)
      for(int Y=0; Y<Ydiv; Y++)
      	for(int X=0; X<Xdiv; X++)
        {
    	p[0].set(GetVertex(X,Y,Z));
    	p[1].set(GetVertex(X,Y,Z+1));
		p[2].set(GetVertex(X+1,Y,Z+1));
    	p[3].set(GetVertex(X+1,Y,Z));
	
		p[4].set(GetVertex(X,Y+1,Z));
    	p[5].set(GetVertex(X,Y+1,Z+1));
		p[6].set(GetVertex(X+1,Y+1,Z+1));
    	p[7].set(GetVertex(X+1,Y+1,Z));
		bins[i].SetExtents(p);
		bins[i].setWorldExtents(bb,e);
		bins[i].ResizeBin(BinSize);
		i++;
	
    	}
	//cout << "bins Set "<<i<<endl;
}


Lattice::~Lattice()
{
//remove the points list
Points.erase(Points.begin(),Points.end());
delete [] bins;
}

void Lattice::DrawVertex(int X, int Y, int Z)
{
// calculate the 3d position in a 1d array
int offset;
// first find the x,y,z offset
offset = (Xdiv*Ydiv)*Z;
// then add the x by y
offset +=(Xdiv * Y);
//finally add the x
offset += X;
// now draw a vertex at that point
Points[offset].Pos.Vertex();
}


Point3 Lattice::GetVertex(int X, int Y, int Z)
{
// calculate the 3d position in a 1d array
int offset;
// first find the x,y,z offset
offset = (Xdiv*Ydiv)*Z;
// then add the x by y
offset +=(Xdiv * Y);
//finally add the x
offset += X;
// and return the point at that position
return Points[offset].Pos;
}



void Lattice::Draw(void)
{
int x,y,z;
// for each of the points Draw the lattice points
glColor3f(1.0,1.0,1.0);
for(int i=0; i<Npoints; i++)
 	{
    // save the current tx matrix
    glPushMatrix();
        // load the name so we can select it in selection mode
        glLoadName(i);
        // save the name
        Points[i].Name=i;
    	// draw the point
        Points[i].Draw();
    // restore the tx matrix
    glPopMatrix();

    }
// set the material to BLACK
//Material m(BLACKPLASTIC);//
//m.Use();

// now draw the connected lattice points using the Vertex command
// first we draw the X lattice lines
// for each z div
for(z=0; z<Zdiv; z++)
{
// for each y div
for(y=0; y<Ydiv; y++)
	{
    // for each x div
    for(x=0; x<Xdiv-1; x++)
		{
        // draw as linies
         glBegin(GL_LINES);
        	// draw first vertex
            DrawVertex(x,y,z);
            // draw the next vertex in line
            DrawVertex(x+1,y,z);
        glEnd();
       	}


     }
 }
// Now the Y lattice lines
for(z=0; z<Zdiv; z++)
{
for(x=0; x<Xdiv; x++)

	{
	for(y=0; y<Ydiv-1; y++)
		{
        glBegin(GL_LINES);
        	DrawVertex(x,y,z);
            DrawVertex(x,y+1,z);
		glEnd();
        }
     }
   }
// and finally the Z lattice lines

for(y=0; y<Ydiv; y++)
{
for(x=0; x<Xdiv; x++)
	{
	for(z=0; z<Zdiv-1; z++)
		{
        glBegin(GL_LINES);
        	DrawVertex(x,y,z);
            DrawVertex(x,y,z+1);
		glEnd();
        }
     }
   }
// finally turn off the lighting
//glDisable(GL_LIGHTING);


}



void Lattice::FillBins(AgentList *a)
{
for(unsigned long int i=0; i<nBins; i++)
	{
 	AgentListItt begin = a->begin();
 	AgentListItt end = a->end();
 	bins[i].Reset();
     
 	while(begin != end)
 		{
 		bins[i].InBin((*begin));
        begin++;
 		}
	
    }
}

void Lattice::Update( void)
{
// check each of the bins and update
for(unsigned long int i=0; i<nBins; i++)
	{
    //cout <<"Doing Bin "<<i<<endl;
	if(bins[i].Empty()!=true)
		{
        //cout <<"Bin has Agents"<<endl;
        if(DoCollide==true)
        	bins[i].DoAgentCollideRoutines();
		
	}
}// end for

}

