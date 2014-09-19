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

void NGLScene::createBodies()
{
	/// this data has been exported from the maya script in the main project dir
	Body bodies[]=
	{
		{ 0,"pCube1",0.0f,-5.75734941405f,40.0f,0.622222219046f,0.0f,0.2f,0.0f,0.0f,b2_staticBody },
		{ 0,"pCube2",-15.1802719259f,-0.774503669689f,5.0f,1.0f,0.0f,0.2f,0.0f,0.0f,b2_staticBody },
		{ 0,"pCube3",13.3989134856f,-0.619602935751f,5.0f,1.0f,0.0f,0.2f,0.0f,0.0f,b2_staticBody },
		{ 0,"pCube4",-0.619602935751f,-0.464702201813f,5.0f,1.0f,0.0f,0.2f,0.0f,0.0f,b2_kinematicBody },
		{ 0,"pCube5",0.0f,3.4852665136f,5.0f,1.0f,0.0f,0.2f,0.0f,0.0f,b2_staticBody },
		{ 0,"pCube6",7.12543376114f,6.19602935751f,5.0f,1.0f,0.0f,0.2f,0.0f,0.0f,b2_staticBody },
		{ 0,"pCube7",-8.82934183445f,8.2097388987f,5.0f,1.0f,0.0f,0.2f,0.0f,0.0f,b2_staticBody },
		{ 0,"pCube8",-19.6723932101f,17.2714318341f,1.0f,1.0f,0.0f,0.2f,0.0f,0.0f,b2_dynamicBody },
		{ 0,"pCube9",-11.2303032105f,17.1939814671f,1.0f,1.0f,0.0f,0.2f,0.0f,0.0f,b2_dynamicBody },
		{ 0,"pCube10",1.31665623847f,17.7361340359f,1.0f,1.0f,0.0f,0.2f,0.0f,0.0f,b2_dynamicBody },
		{ 0,"pCube11",13.4763638526f,9.83619660505f,5.0f,1.0f,45.0f,0.2f,0.0f,0.0f,b2_staticBody },
		{ 0,"pCube12",7.82248706386f,-2.78821321088f,5.0f,1.0f,45.0f,0.2f,0.0f,0.0f,b2_staticBody },
	};
	const static int numBodies=12;
	for(int i=0; i<numBodies; ++i)
	{
		b2BodyDef bodyDef;
		bodyDef.position.Set(bodies[i].tx, bodies[i].ty);
		bodyDef.type=bodies[i].type;
		bodyDef.angle=ngl::radians(bodies[i].rotation);
		b2Body* body = m_world->CreateBody(&bodyDef);


    b2PolygonShape box;
    // half width for box def
    box.SetAsBox(bodies[i].width/2.0,bodies[i].height/2.0);
    body->CreateFixture(&box, 0.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = bodies[i].density;
    fixtureDef.friction = bodies[i].friction;
    // make it bouncy
    fixtureDef.restitution=bodies[i].restitution;
    bodies[i].body=body;
    m_bsodies.push_back(bodies[i]);
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
  m_projection=ngl::ortho(-30,30,-10,20, 0.1f, 10.0f);
  ngl::Vec3 from(0,0,1);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  m_view=ngl::lookAt(from,to,up);
  // box2d physic setup first gravity down in the y
  b2Vec2 gravity(0.0f, -20.0f);
  m_world= new b2World(gravity);

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
  createBodies();


  startTimer(20);


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

  // static bodies
  for(unsigned int i=0; i<m_bsodies.size(); ++i)
  {
    m_transform.reset();
    {
      b2Vec2 position = m_bsodies[i].body->GetPosition();

      shader->setShaderParam4f("Colour",0.0f,0.0f,1.0f,1.0f);
      m_transform.setScale(m_bsodies[i].width,m_bsodies[i].height,0.1);
      m_transform.setPosition(position.x,position.y,0);
      m_transform.setRotation(0,0,ngl::degrees(m_bsodies[i].body->GetAngle()));

      loadMatricesToShader();
      prim->draw("cube");
    }
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
  if((pos.x<=-29 || pos.x>29))
  {
    dir=-dir;
    m_body->SetLinearVelocity(dir);
  }
  // fallen off so reset
  if(pos.y<=-15)
  {
    m_body->SetTransform(b2Vec2(0.0f,0.0f),0.0f);
    m_body->SetLinearVelocity(b2Vec2(0.0f,0.0f));

  }

  renderLater();
}


