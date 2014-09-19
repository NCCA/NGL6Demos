/*
  Copyright (C) 2011 Jon Macey

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------

#include "Text.h"
#include <iostream>
#include <ngl/ShaderLib.h>
#include "SDL.h"
#include "SDL_ttf.h"


//---------------------------------------------------------------------------
/// @brief code taken from here http://jeffreystedfast.blogspot.com/2008/06/calculating-nearest-power-of-2.html
/// @param _num the number we wish to get the nearest power from
// OpenGL needs textures to be in powers of two, this function will get the
// nearest power of two to the current value passed in
//---------------------------------------------------------------------------
unsigned int nearestPowerOfTwo ( unsigned int _num )
{
    unsigned int j, k;
    (j = _num & 0xFFFF0000) || (j = _num);
    (k = j & 0xFF00FF00) || (k = j);
    (j = k & 0xF0F0F0F0) || (j = k);
    (k = j & 0xCCCCCCCC) || (k = j);
    (j = k & 0xAAAAAAAA) || (j = k);
    return j << 1;
}
// end citation

//---------------------------------------------------------------------------
Text::Text( const std::string &_f, int _size)
{
	TTF_Init();
	TTF_Font *font = TTF_OpenFont(_f.c_str(), _size );
	SDL_Color color = { 0, 0, 0,0 };
	if(font ==0 )
	{
		std::cerr<<"Error loading font "<<TTF_GetError()<<"\n";
		exit(EXIT_FAILURE);
	}
  // so first we grab the font metric of the font being used
  // this allows us to get the height which should be the same for all
  // fonts of the same class as this is the total glyph height
  float fontHeight=TTF_FontHeight(font);

  std::cerr<<"Font height is "<<fontHeight<<"\n";

  // loop for all basic keyboard chars we will use space to ~
  // should ngl::Really change this to unicode at some stage
  const static char startChar=' ';
  const static char endChar='~';
  // Most OpenGL cards need textures to be in powers of 2 (128x512 1024X1024 etc etc) so
  // to be safe we will conform to this and calculate the nearest power of 2 for the glyph height
  // we will do the same for each width of the font below
  int heightPow2=nearestPowerOfTwo(fontHeight);

  // we are now going to create a texture / billboard for each font
  // they will be the same height but will possibly have different widths
  // as some of the fonts will be the same width, to save VAO space we will only create
  // a vao if we don't have one of the set width. To do this we use the has below
  std::map <int,ngl::VertexArrayObject *> widthVAO;

  for(char c=startChar; c<=endChar; ++c)
  {
    FontChar fc;
    // get the width of the font and calculate the ^2 size
   // TTF_GlyphMetrics(TTF_Font *font, Uint16 ch, int *minx, int *maxx, int *miny, int *maxy, int *advance)


    int width;
    int height;
    char cc[2];
    sprintf(cc,"%c",c);
    // need a null terminated string
    cc[1]='\0';
    TTF_SizeText(font,cc,&width,&height);
    int widthPow2=nearestPowerOfTwo(width);
	
    // now we set the texture co-ords for our quad it is a simple
    // triangle billboard with tex-cords as shown
    //  s0/t0  ---- s1,t0
    //         |\ |
    //         | \|
    //  s0,t1  ---- s1,t1
    // each quad will have the same s0 and the range s0-s1 == 0.0 -> 1.0
    ngl::Real s0=0.0;
    // we now need to scale the tex cord to it ranges from 0-1 based on the coverage
    // of the glyph and not the power of 2 texture size. This will ensure that kerns
    // / ligatures match
    ngl::Real s1=width*1.0/width; // use this on older machines widthPow2;
    // t0 will always be the same
    ngl::Real t0=0.0;
    // this will scale the height so we only get coverage of the glyph as above
    ngl::Real t1=height*1.0/height; // use this on older gpus heightPow2;
    // we need to store the font width for later drawing
    fc.width=width;
   
    SDL_Surface* msg = TTF_RenderText_Blended( font, cc, color );
    // this is needed on some older machines
    //SDL_Surface *powerOfTwo=SDL_CreateRGBSurface(0,widthPow2,heightPow2,32,0,0,0,0);
    //SDL_BlitSurface(msg,NULL,powerOfTwo,NULL);
    // now we create the OpenGL texture ID and bind to make it active
    glGenTextures(1, &fc.textureID);
    glBindTexture(GL_TEXTURE_2D, fc.textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, msg->w, msg->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, msg->pixels );
    //SDL_FreeSurface(powerOfTwo);
    SDL_FreeSurface(msg);

    // the image in in RGBA format and unsigned byte load it ready for later
 //   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthTexture, heightTexture,0, GL_RGBA, GL_UNSIGNED_BYTE, data);
 //   delete [] data;
    // see if we have a Billboard of this width already
    if (!widthVAO.count(width))
    {
        // this structure is used by the VAO to store the data to be uploaded
        // for drawing the quad
        struct textVertData
        {
        ngl::Real x;
        ngl::Real y;
        ngl::Real u;
        ngl::Real v;
        };
        // we are creating a billboard with two triangles so we only need the
        // 6 verts, (could use index and save some space but shouldn't be too much of an
        // issue
        textVertData d[6];
        // load values for triangle 1
        d[0].x=0;
        d[0].y=0;
        d[0].u=s0;
        d[0].v=t0;

        d[1].x=fc.width;
        d[1].y=0;
        d[1].u=s1;
        d[1].v=t0;

        d[2].x=0;
        d[2].y=fontHeight;
        d[2].u=s0;
        d[2].v=t1;
        // load values for triangle two
        d[3].x=0;
        d[3].y=0+fontHeight;
        d[3].u=s0;
        d[3].v=t1;


        d[4].x=fc.width;
        d[4].y=0;
        d[4].u=s1;
        d[4].v=t0;


        d[5].x=fc.width;
        d[5].y=fontHeight;
        d[5].u=s1;
        d[5].v=t1;


        // now we create a VAO to store the data
        ngl::VertexArrayObject *vao=ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
        // bind it so we can set values
        vao->bind();
        // set the vertex data (2 for x,y 2 for u,v)
        vao->setData(6*sizeof(textVertData),d[0].x);
        // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)
        vao->setVertexAttributePointer(0,2,GL_FLOAT,sizeof(textVertData),0);
        // We can now create another set of data (which will be added to the VAO)
        // in this case the UV co-ords
        // now we set this as the 2nd attribute pointer (1) to match inUV in the shader
        vao->setVertexAttributePointer(1,2,GL_FLOAT,sizeof(textVertData),2);
        // say how many indecis to be rendered
        vao->setNumIndices(6);

        // now unbind
        vao->unbind();
        // store the vao pointer for later use in the draw method
        fc.vao=vao;
        widthVAO[width]=vao;
    }
    else
    {
      fc.vao=widthVAO[width];
    }
    // finally add the element to the map, this must be the last
    // thing we do
    m_characters[c]=fc;
  }
  std::cout<<"created "<<widthVAO.size()<<" unique billboards\n";
  // set a default colour (black) incase user forgets
  this->setColour(0,0,0);
  setTransform(1.0,1.0);
  
}


//---------------------------------------------------------------------------
Text::~Text()
{
  // our dtor should clear out the textures and remove the VAO's
 /* foreach( FontChar m, m_characters)
  {
    glDeleteTextures(1,&m.textureID);
    m.vao->removeVOA();
  }
*/
}




//---------------------------------------------------------------------------
void Text::renderText( float _x, float _y,  const std::string &text ) const
{
  // make sure we are in texture unit 0 as this is what the
  // shader expects
  glActiveTexture(GL_TEXTURE0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // use the built in text rendering shader
  (*shader)["nglTextShader"]->use();
  // the y pos will always be the same so set it once for each
  // string we are rendering
  shader->setRegisteredUniform1f("ypos",_y);
  // now enable blending and disable depth sorting so the font renders
  // correctly
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // now loop for each of the char and draw our billboard
  unsigned int textLength=text.length();

  for (unsigned int i = 0; i < textLength; ++i)
  {
    // set the shader x position this will change each time
    // we render a glyph by the width of the char
    shader->setRegisteredUniform1f("xpos",_x);
    // so find the FontChar data for our current char
//    FontChar f = m_characters[text[i].toAscii()];
//    FontChar f = m_characters[text[i]];
  std::map <char,FontChar>::const_iterator currentchar=m_characters.find(text[i]);
  // make sure we have a valid shader
  if(currentchar!=m_characters.end())
  {
	FontChar f=currentchar->second;
    // bind the pre-generated texture
    glBindTexture(GL_TEXTURE_2D, f.textureID);
    // bind the vao
    f.vao->bind();
    // draw 
    f.vao->draw();
    // now unbind the vao
    f.vao->unbind();
    // finally move to the next glyph x position by incrementing
    // by the width of the char just drawn
    _x+=f.width;

  }	
  }
  // finally disable the blend and re-enable depth sort
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

}

//---------------------------------------------------------------------------
void Text::setScreenSize(int _w, int _h )
{

  float scaleX=2.0/_w;
  float scaleY=-2.0/_h;
  // in shader we do the following code to transform from
  // x,y to NDC
  // gl_Position=vec4( ((xpos+inVert.x)*scaleX)-1,((ypos+inVert.y)*scaleY)+1.0,0.0,1.0); "
  // so all we need to do is calculate the scale above and pass to shader every time the
  // screen dimensions change
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglTextShader"]->use();
  std::cout<<"scaleX "<<scaleX <<" "<<scaleY<<"\n";
  shader->setRegisteredUniform1f("scaleX",scaleX);
  shader->setRegisteredUniform1f("scaleY",scaleY);
}

//---------------------------------------------------------------------------
// our text shader uses the alpha of the texture to modulate visibility
// when we render the text we use this colour passed to the shader
// it is default to black but this will change it
// the shader uses the following code
// vec4 text=texture(tex,vertUV.st);
// fragColour.rgb=textColour.rgb;
// fragColour.a=text.a;

void Text::setColour(const ngl::Colour &_c )
{
  // get shader instance
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // make current shader active
  (*shader)["nglTextShader"]->use();
  // set the values
  shader->setRegisteredUniform3f("textColour",_c.r(),_c.g(),_c.b());
}


//---------------------------------------------------------------------------
void Text::setColour(ngl::Real _r,  ngl::Real _g, ngl::Real _b)
{

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglTextShader"]->use();

  shader->setRegisteredUniform3f("textColour",_r,_g,_b);
}

void Text::setTransform(float _x, float _y)
{

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglTextShader"]->use();

  shader->setRegisteredUniform2f("transform",_x,_y);
}

//---------------------------------------------------------------------------

