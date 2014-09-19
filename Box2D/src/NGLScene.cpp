#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/TransformStack.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>




NGLScene::NGLScene(QWindow *_parent) : OpenGLWindow(_parent)
{
  setTitle("Box2D and NGL");

}


NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  Init->NGLQuit();
  delete m_world;
}

void NGLScene::resizeEvent(QResizeEvent *_event )
{
  if(isExposed())
  {
  int w=_event->size().width();
  int h=_event->size().height();
  // set the viewport for openGL
  glViewport(0,0,w,h);
  renderLater();
  }
}

void NGLScene::createStaticBodies()
{
  float x=-30.0f;
  float y=-20.0f;
  for(int i=0; i<8; ++i)
  {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    b2Body* body = m_world->CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(5.0f, 0.5f);
    body->CreateFixture(&box, 0.0f);
    staticBody b;
    b.dim.set(10.0f,1.0f);
    b.pos.set(x,y);
    m_staticBodies.push_back(b);
    x+=8.0;
    if(x>0.0)
      y-=3.8;
    else
      y+=3.8;

  }

  x=-30.0f;

  for(int i=0; i<8; ++i)
  {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    b2Body* body = m_world->CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(5.0f, 0.5f);
    body->CreateFixture(&box, 0.0f);
    staticBody b;
    b.dim.set(10.0f,1.0f);
    b.pos.set(x,y);
    m_staticBodies.push_back(b);
    x+=8.0;
    if(x<2.0)
      y+=3.8;
    else
      y+=3.8;

  }



}


void NGLScene::initialize()
{
  // we must call this first before any other GL commands to load and link the
  // gl commands from the lib, if this is not done program will crash
  ngl::NGLInit::instance();

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);

  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  (*shader)["nglDiffuseShader"]->use();
  shader->setShaderParam4f("Colour",1,1,0,1);
  shader->setShaderParam3f("lightPos",0.2,0.2,1);
  shader->setShaderParam4f("lightDiffuse",1,1,1,1);

  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());
  m_projection=ngl::ortho(-40,40,-20,20, 0.1f, 10.0f);
  ngl::Vec3 from(0,0,1);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  m_view=ngl::lookAt(from,to,up);
  // box2d physic setup first gravity down in the y
  b2Vec2 gravity(0.0f, -20.0f);
  m_world= new b2World(gravity);

  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set(0.0f, -20.0f);
  b2Body* groundBody = m_world->CreateBody(&groundBodyDef);
  b2PolygonShape groundBox;
  // the params are half widths so it should be 40 wide
  groundBox.SetAsBox(40.0f, 1.0f);
  groundBody->CreateFixture(&groundBox, 0.0f);

  // now for our actor
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(-0.1f, 0.0f);
  m_body = m_world->CreateBody(&bodyDef);
  m_body->SetAngularVelocity(0.0);
  m_body->SetAngularDamping(0.3f);
  m_body->SetLinearDamping(0.2f);
  m_body->SetFixedRotation(false);

  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(1.0f, 1.0f);
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.5f;
  fixtureDef.friction = 0.3f;
  // make it bouncy
  fixtureDef.restitution=.4f;
  m_body->CreateFixture(&fixtureDef);

  // finally some static bodies for a platform
  createStaticBodies();

  // create a moving platform
  bodyDef.type = b2_kinematicBody;
  bodyDef.position.Set(0.0,2.0);
  m_platform = m_world->CreateBody(&bodyDef);
  m_platform->SetFixedRotation(true);

  dynamicBox.SetAsBox(5.0f, 1.0f);
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.5f;
  fixtureDef.friction = 0.3f;
  // make it bouncy
  fixtureDef.restitution=.4f;
  m_platform->CreateFixture(&fixtureDef);
  m_platform->SetLinearVelocity(b2Vec2(-10,0));

  startTimer(0);


}


void NGLScene::loadMatricesToShader()
{

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  MVP= m_transform.getMatrix()*m_view*m_projection;
  normalMatrix=m_transform.getMatrix()*m_view;
  normalMatrix.inverse();
  shader->setRegisteredUniform("MVP",MVP);
  shader->setRegisteredUniform("normalMatrix",normalMatrix);

}

void NGLScene::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglDiffuseShader"]->use();


   // get the VBO instance and draw the built in teapot
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  // draw floor
  m_transform.reset();
  {
    shader->setShaderParam4f("Colour",0.0f,1.0f,0.0f,1.0f);
    m_transform.setScale(80.0,2.0,0.1);
    m_transform.setPosition(0,-20,0);
    loadMatricesToShader();
    prim->draw("cube");
  }
  // static bodies
  for(unsigned int i=0; i<m_staticBodies.size(); ++i)
  {
    ngl::Vec2 pos;
    ngl::Vec2 dim;
    pos=m_staticBodies[i].pos;
    dim=m_staticBodies[i].dim;

    m_transform.reset();
    {
      shader->setShaderParam4f("Colour",0.0f,0.0f,1.0f,1.0f);
      m_transform.setScale(dim.m_x,dim.m_y,0.1);
      m_transform.setPosition(pos.m_x,pos.m_y,0);
      loadMatricesToShader();
      prim->draw("cube");
    }
  }


  // draw our moving platform
  m_transform.reset();
  {
    b2Vec2 position = m_platform->GetPosition();
    shader->setShaderParam4f("Colour",0.0f,1.0f,0.0f,1.0f);
    m_transform.setScale(10.0f,2.0f,0.1f);
    m_transform.setPosition(position.x,position.y,0);
    loadMatricesToShader();
    prim->draw("cube");
  }
  // draw our main object
  m_transform.reset();
  {
    b2Vec2 position = m_body->GetPosition();
    float32 angle= ngl::degrees(m_body->GetAngle());

    shader->setShaderParam4f("Colour",1.0f,0.0f,0.0f,1.0f);
    m_transform.setScale(2.0f,2.0f,0.1f);
    m_transform.setPosition(position.x,position.y,0);
    m_transform.addRotation(0,0,angle);
    loadMatricesToShader();
    prim->draw("cube");
  }


}
//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{

}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent ( QMouseEvent * _event)
{


}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent ( QMouseEvent * _event )
{
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent(QWheelEvent *_event)
{


}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  m_keysPressed += (Qt::Key)_event->key();

  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  // turn on wirframe rendering
  case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
  // turn off wire frame
  case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
  // show full screen
  case Qt::Key_F : showFullScreen(); break;
  // show windowed
  case Qt::Key_N : showNormal(); break;

  case Qt::Key_R :
      m_body->SetTransform(b2Vec2(0.0f,0.0f),0.0f);
      m_body->SetLinearVelocity(b2Vec2(0.0f,0.0f));
  break;



  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())

    renderLater();
}

void NGLScene::keyReleaseEvent( QKeyEvent *_event	)
{
	// remove from our key set any keys that have been released
	m_keysPressed -= (Qt::Key)_event->key();
}


void NGLScene::timerEvent( QTimerEvent *_event)
{
  float32 timeStep = 1.0f / 60.0f;
  int32 velocityIterations = 6;
  int32 positionIterations = 2;
  m_world->Step(timeStep, velocityIterations, positionIterations);
  // process all the key presses
  b2Vec2 move(0.0f,0.0f);

  foreach(Qt::Key key, m_keysPressed)
  {
    switch (key)
    {
      case Qt::Key_Left :  { move.x=-100; break;}
      case Qt::Key_Right : { move.x=100; break;}
      case Qt::Key_Space :		 {  move.y=400.0; break;}
      default : break;
    }
  }
  m_body->ApplyForce(move,m_body->GetWorldCenter(),true);

  // constrain to window by bouncing!
  b2Vec2 pos=m_body->GetPosition();
  b2Vec2 dir=m_body->GetLinearVelocity();
  // reverse
  if((pos.x<=-39 || pos.x>39))
  {
    dir=-dir;
    m_body->SetLinearVelocity(dir);
  }
  // now change the moving platform
  pos=m_platform->GetPosition();
  dir=m_platform->GetLinearVelocity();
  if((pos.x<=-39 || pos.x>39))
  {
    dir=-dir;
    m_platform->SetLinearVelocity(dir);
  }

  renderLater();
}


