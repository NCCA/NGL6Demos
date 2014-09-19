#include "Game.h"
#include <ngl/ShaderLib.h>
#include <ngl/NGLInit.h>
#include <ngl/Material.h>
#include <algorithm>

Game::Game()
{
  ngl::NGLInit::instance();
  m_maxRockets=200;
  m_activeRockets=0;
  // Now set the initial GLWindow attributes to default values
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);


  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Texture
  shader->createShaderProgram("Texture");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("TextureVertex",ngl::VERTEX);
  shader->attachShader("TextureFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("TextureVertex","shaders/TextureVertex.glsl");
  shader->loadShaderSource("TextureFragment","shaders/TextureFragment.glsl");
  // compile the shaders
  shader->compileShader("TextureVertex");
  shader->compileShader("TextureFragment");
  // add them to the program
  shader->attachShaderToProgram("Texture","TextureVertex");
  shader->attachShaderToProgram("Texture","TextureFragment");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Texture");
  // and make it active ready to load values
  (*shader)["Texture"]->use();
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,0,30);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  // now load to our new camera
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.05,350);
  m_shipMesh = new ngl::Obj("models/SpaceShip.obj","textures/spaceship.bmp");
  m_shipMesh->createVAO();

  m_rocket = new ngl::Obj("models/rocket.obj","textures/rockets.bmp");
  m_rocket->createVAO();
  m_starmap = new StarMap("textures/starmap.png","textures/PlanetMap.tif",m_cam);


}

Game::~Game()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_cam;
  delete m_shipMesh;
  Init->NGLQuit();
}

void Game::resize(int _w, int _h)
{
  glViewport(0,0,_w,_h);
  // now set the camera size values as the screen size has changed
  m_cam->setShape(45,(float)_w/_h,0.05,350);
}

void Game::draw()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Texture"]->use();

  // Rotation based on the mouse position for our global transform
  ngl::Transformation trans;
  ngl::Mat4 final;
  // add the translations
  final.m_m[3][0] = m_modelPos.m_x;
  final.m_m[3][1] = m_modelPos.m_y;
  final.m_m[3][2] = m_modelPos.m_z;
  // set this in the TX stack
  trans.setMatrix(final);
  m_transformStack.setGlobal(trans);
  m_transformStack.pushTransform();
    m_transformStack.setRotation(0,-90,0);
    loadMatricesToShader();
    m_shipMesh->draw();
  m_transformStack.popTransform();

  m_transformStack.pushTransform();
    m_transformStack.setPosition(-5,-0.6,2);
    m_transformStack.setRotation(0,-90,0);
    loadMatricesToShader();
    m_rocket->draw();
  m_transformStack.popTransform();

  m_transformStack.pushTransform();
    m_transformStack.setPosition(5,-0.6,2);
    m_transformStack.setRotation(0,-90,0);
    loadMatricesToShader();
    m_rocket->draw();
  m_transformStack.popTransform();


  // now draw the rockets
  std::list <Rocket *>::iterator start=m_rockets.begin();
  std::list <Rocket *>::iterator end=m_rockets.end();
  // we can use std::for_each and pass in a member function to draw
  std::for_each(start,end,std::mem_fun(&Rocket::draw));
  m_starmap->draw();

}


void Game::update()
{
  std::list <Rocket *>::iterator start=m_rockets.begin();
  std::list <Rocket *>::iterator end=m_rockets.end();
  // iterater an update the rockets
  std::for_each(start,end,std::mem_fun(&Rocket::update));
  // now check the list and call the isActive method and remove if false
  m_rockets.remove_if(std::not1(std::mem_fun(&Rocket::isActive)));
  // update the size of the active rockets
  m_activeRockets=m_rockets.size();
}

void Game::fireLeft(float _speed)
{
  if( m_activeRockets !=m_maxRockets )
  {
    ngl::Vec3 p=m_modelPos;
    p.m_x+=-5;
    p.m_y+=-0.6;
    p.m_z+=2;
    Rocket *r = new Rocket(p,_speed,m_rocket,m_cam);
    m_rockets.push_back(r);
    ++m_activeRockets;
  }
}

void Game::fireRight(float _speed)
{
  if( m_activeRockets !=m_maxRockets )
  {
    ngl::Vec3 p=m_modelPos;
    p.m_x+=5;
    p.m_y+=-0.6;
    p.m_z+=2;
    Rocket *r = new Rocket(p,_speed,m_rocket,m_cam);
    m_rockets.push_back(r);
    ++m_activeRockets;
  }

}

void Game::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat4 M;
  M=m_transformStack.getCurrAndGlobal().getMatrix();
  MV=  m_transformStack.getCurrAndGlobal().getMatrix()*m_cam->getViewMatrix();
  MVP= M*m_cam->getVPMatrix();
  shader->setShaderParamFromMat4("MVP",MVP);
}
