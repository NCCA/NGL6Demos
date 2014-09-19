#include "SelectObject.h"
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/NGLStream.h>
// set the initial colour to 0
unsigned char SelectObject::s_gColourID[3] = {0, 0, 0};

SelectObject::SelectObject(ngl::Vec3 pos)
{

  // set the initial values
  m_pos=pos;
  m_active=false;
  // generate an automatic colour id value for selection
  // grab the current active colour id values
  m_colourID[0] = s_gColourID[0];
  m_colourID[1] = s_gColourID[1];
  m_colourID[2] = s_gColourID[2];
  // now we need to increment the values for the next one
  //1 add to the red chan value
  ++s_gColourID[0];
  // if we are at full value
  if(s_gColourID[0] == 255)
  {
    // set red channel to 0
    s_gColourID[0] = 0;
    // inc blue channel
    ++s_gColourID[1];
  // if full go to next
    if(s_gColourID[1] == 255)
    {
      s_gColourID[1] = 0;
      ++s_gColourID[2];
    }
  }
}

bool SelectObject::checkSelectionColour(const unsigned char col[3])
{
  // see if the colour passed in is the same as our objects one
  if(m_colourID[0] == col[0] && m_colourID[1] == col[1] && m_colourID[2] == col[2])
  {
   m_active^=true;
   return true;
  }
}


void SelectObject::loadMatricesToShader( ngl::Transformation &_tx,const ngl::Mat4 &_globalTx,const std::string &_name,ngl::Camera *_cam)
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[_name]->use();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=_tx.getMatrix()*_globalTx;
  MV=_tx.getMatrix()*_globalTx*_cam->getViewMatrix() ;
  MVP=MV*_cam->getProjectionMatrix() ;
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MVP",MVP);
  //std::cout<<"Shaded MVP \n"<<MVP<<"\n";
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
}

void SelectObject::loadMatricesToColourShader(
                                              ngl::Transformation &_tx,
                                              const ngl::Mat4 &_globalTx,
                                              const std::string &_name,
                                              ngl::Camera *_cam
                                              )
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[_name]->use();
  ngl::Mat4 MV;
  ngl::Mat4 MVP;

  MV=_tx.getMatrix()*_globalTx*_cam->getViewMatrix() ;
  MVP=MV*_cam->getProjectionMatrix();
  shader->setShaderParamFromMat4("MVP",MVP);
  //std::cout<<"Colour MVP \n"<<MVP<<"\n";

}



void SelectObject::draw(bool _selection,const std::string &_shaderName,const ngl::Mat4 &_globalTx,ngl::Camera *_cam)
{
  // grab the VBO instance
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[_shaderName]->use();

  ngl::Transformation t;
  t.setPosition(m_pos);
  if(_selection)
  {
    shader->setShaderParam4f("Colour",m_colourID[0]/255.0f, m_colourID[1]/255.0f,m_colourID[2]/255.0f,1);

    loadMatricesToColourShader(t,_globalTx,_shaderName,_cam);
  }
  else
  {
    loadMatricesToShader(t,_globalTx,_shaderName,_cam);
  }
  if(m_active == true)
  {
    // draw a teapot
    prim->draw("teapot");
  }
  else
  {
    // draw a cube
    prim->draw("cube");
  }
}
