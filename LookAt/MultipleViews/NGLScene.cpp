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


//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the offset for full window mode mouse data
//----------------------------------------------------------------------------------------------------------------------
const static int FULLOFFSET=4;

NGLScene::NGLScene(QWindow *_parent) : OpenGLWindow(_parent)
{
  for(int i=0; i<5; ++i)
  {
    m_panelMouseInfo[i].m_spinXFace=0;
    m_panelMouseInfo[i].m_spinYFace=0;
    m_panelMouseInfo[i].m_rotate=false;
  }

  // now we need to set the scales for the ortho windos
  m_panelMouseInfo[FRONT].m_modelPos.set(0,0,1);
  m_panelMouseInfo[SIDE].m_modelPos.set(0,0,1);
  m_panelMouseInfo[TOP].m_modelPos.set(0,0,1);


  // mouse rotation values set to 0
  m_activeWindow=ALL;
  m_mouseX=0;
  m_mouseY=0;
  setTitle("Multiple Views");
  m_width=this->size().width();
  m_height=this->size().height();

}


NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  Init->NGLQuit();
}

void NGLScene::resizeEvent(QResizeEvent *_event )
{
  m_width=this->size().width();
  m_height=this->size().height();
  if(isExposed())
  {
    renderLater();
  }
}


void NGLScene::initialize()
{
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglDiffuseShader"]->use();

  shader->setShaderParam4f("Colour",1,1,1,1);
  shader->setShaderParam3f("lightPos",1,1,1);
  shader->setShaderParam4f("lightDiffuse",1,1,1,1);
  // get the VBO instance and draw the built in teapot
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createLineGrid("grid",40,40,40);
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());


}

void NGLScene::frameActive()
{
  int win = (int)getActiveQuadrant();
  if(m_activeWindow != ALL)
  {
    win= FULLOFFSET;
  }
  m_panelMouseInfo[win].m_modelPos.set(0,0,1);
}

void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_transformStack.getCurrentTransform().getMatrix()*m_mouseGlobalTX;
  MV=  M*m_view;
  MVP= M*m_view*m_projection;
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
 }


void NGLScene::top(MODE _m)
{
  m_mouseGlobalTX.identity();
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglDiffuseShader"]->use();

  // Rotation based on the mouse position for our global transform
  int win=FULLOFFSET;
  if(_m == PANEL)
  {
    win=TOP;
  }


  // get the VBO instance and draw the built in teapot
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

  // first draw a top  persp // front //side
  ngl::Vec3 from(0,2,0);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,0,-1);
  m_transformStack.pushTransform();
  {
    /// a top view (left upper)
    m_view=ngl::lookAt(from,to,up);
    m_projection=ngl::ortho(-1,1,-1,1, 0.1f, 100.0f);
    // x,y w/h
    if(_m==PANEL)
    {
      glViewport (0,m_height/2, m_width/2, m_height/2);
    }
    else
    {
      glViewport(0,0,m_width,m_height);
    }
      // draw
    // in this case set to an identity
    ngl::Vec3 p=m_panelMouseInfo[win].m_modelPos;
    m_transformStack.setPosition(p.m_x,0,-p.m_y);
    m_transformStack.setScale(p.m_z,p.m_z,p.m_z);

    loadMatricesToShader();
    prim->draw("troll");
    m_transformStack.addPosition(0,-1,0);
    loadMatricesToShader();
    prim->draw("grid");
  }
  m_transformStack.popTransform();

}

void NGLScene::side(MODE _m)
{
  m_mouseGlobalTX.identity();
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglDiffuseShader"]->use();

  int win=FULLOFFSET;
  if(_m == PANEL)
  {
    win=SIDE;
  }

   // get the VBO instance and draw the built in teapot
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

  // first draw a top  persp // front //side
  ngl::Vec3 from(0,2,0);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,0,-1);
  /// a side view (bottom right)
  m_transformStack.pushTransform();
  {
    from.set(2,0,0);
    up.set(0,1,0);
    m_view=ngl::lookAt(from,to,up);
    m_projection=ngl::ortho(-1,1,-1,1, 0.1f, 100.0f);
    // x,y w/h
    if(_m==PANEL)
    {
      glViewport (m_width/2, 0, m_width/2, m_height/2);
    }
    else
    {
      glViewport(0,0,m_width,m_height);
    }

    // draw
    ngl::Vec3 p=m_panelMouseInfo[win].m_modelPos;

    m_transformStack.setPosition(0,p.m_y,-p.m_x);
    m_transformStack.setScale(p.m_z,p.m_z,p.m_z);


    loadMatricesToShader();
    prim->draw("troll");
    m_transformStack.setRotation(90,90,0);
    m_transformStack.addPosition(0,0,2);
    loadMatricesToShader();
    prim->draw("grid");
  }
  m_transformStack.popTransform();



}
void NGLScene::persp(MODE _m)
{
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglDiffuseShader"]->use();

  // Rotation based on the mouse position for our global transform
  // 4 is the panel full screen mode
  int win=FULLOFFSET;
  if(_m == PANEL)
  {
    win=PERSP;
  }
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_panelMouseInfo[win].m_spinXFace);
  rotY.rotateY(m_panelMouseInfo[win].m_spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotY*rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_panelMouseInfo[win].m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_panelMouseInfo[win].m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_panelMouseInfo[win].m_modelPos.m_z;
  // set this in the TX stack

   // get the VBO instance and draw the built in teapot
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

  // first draw a top  persp // front //side
  ngl::Vec3 from(0,2,0);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,0,-1);
  m_transformStack.pushTransform();
  {
    /// a perspective view (right upper)
    //first set the global mouse rotation for this one
    from.set(0,1,1);
    up.set(0,1,0);
    m_view=ngl::lookAt(from,to,up);
    m_projection=ngl::perspective(45,float(m_width/m_height),0.01,100);
    // x,y w/h
    if(_m==PANEL)
    {
      glViewport (m_width/2, m_height/2, m_width/2, m_height/2);
    }
    else
    {
      glViewport(0,0,m_width,m_height);
    }

    // draw
    loadMatricesToShader();
    prim->draw("troll");
    m_transformStack.setPosition(0,-0.55,0);
    loadMatricesToShader();
    prim->draw("grid");
  }
  m_transformStack.popTransform();

}
void NGLScene::front(MODE _m)
{
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglDiffuseShader"]->use();
  m_mouseGlobalTX.identity();
  int win=FULLOFFSET;
  if(_m == PANEL)
  {
    win=FRONT;
  }

   // get the VBO instance and draw the built in teapot
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

  // first draw a top  persp // front //side
  ngl::Vec3 from(0,2,0);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,0,-1);
  /// a front view (bottom left)
  m_transformStack.pushTransform();
  {
    from.set(0,0,2);
    up.set(0,1,0);
    m_view=ngl::lookAt(from,to,up);
    m_projection=ngl::ortho(-1,1,-1,1, 0.01f, 200.0f);
    // x,y w/h
    if(_m==PANEL)
    {
      glViewport (0,0, m_width/2, m_height/2);
    }
    else
    {
      glViewport(0,0,m_width,m_height);
    }
    // draw
    // in this case set to an identity
    ngl::Vec3 p=m_panelMouseInfo[win].m_modelPos;
    m_transformStack.setPosition(p.m_x,p.m_y,0);
    m_transformStack.setScale(p.m_z,p.m_z,p.m_z);
    loadMatricesToShader();
    prim->draw("troll");
    m_transformStack.setRotation(90,0,0);
    m_transformStack.addPosition(0,0,-1);
    loadMatricesToShader();
    prim->draw("grid");
  }
  m_transformStack.popTransform();

}


NGLScene::WINDOW NGLScene::getActiveQuadrant() const
{
  // find where the  mouse is and set quadrant
  if( (m_mouseX <m_width/2) && (m_mouseY<m_height/2) )
  {
    return TOP;
  }
  else if( (m_mouseX >=m_width/2) && (m_mouseY<m_height/2) )
  {
    return PERSP;
  }
  else if( (m_mouseX <=m_width/2) && (m_mouseY>m_height/2) )
  {
    return FRONT;
  }
  else if( (m_mouseX >=m_width/2) && (m_mouseY>m_height/2) )
  {
    return SIDE;
  }
  else // should never get here but stops warning
  {
    return ALL;
  }
}


void NGLScene::toggleWindow()
{
  if(m_activeWindow ==ALL)
  {
    m_activeWindow=getActiveQuadrant();
    //store mouse info as we have gone fullscreen
    m_panelMouseInfo[FULLOFFSET].m_modelPos = m_panelMouseInfo[m_activeWindow].m_modelPos;
    m_panelMouseInfo[FULLOFFSET].m_origX = m_panelMouseInfo[m_activeWindow].m_origX;
    m_panelMouseInfo[FULLOFFSET].m_origY = m_panelMouseInfo[m_activeWindow].m_origY;

    m_panelMouseInfo[FULLOFFSET].m_spinXFace = m_panelMouseInfo[m_activeWindow].m_spinXFace;
    m_panelMouseInfo[FULLOFFSET].m_spinYFace = m_panelMouseInfo[m_activeWindow].m_spinYFace;
    m_panelMouseInfo[FULLOFFSET].m_origXPos = m_panelMouseInfo[m_activeWindow].m_origXPos;
    m_panelMouseInfo[FULLOFFSET].m_origYPos = m_panelMouseInfo[m_activeWindow].m_origYPos;
    m_panelMouseInfo[FULLOFFSET].m_rotate = m_panelMouseInfo[m_activeWindow].m_rotate;
    m_panelMouseInfo[FULLOFFSET].m_translate = m_panelMouseInfo[m_activeWindow].m_translate;
  }
  else
  {
    // store info as we are minimised
    m_panelMouseInfo[m_activeWindow].m_modelPos = m_panelMouseInfo[FULLOFFSET].m_modelPos;
    m_panelMouseInfo[m_activeWindow].m_origX = m_panelMouseInfo[FULLOFFSET].m_origX;
    m_panelMouseInfo[m_activeWindow].m_origY = m_panelMouseInfo[FULLOFFSET].m_origY;

    m_panelMouseInfo[m_activeWindow].m_spinXFace = m_panelMouseInfo[FULLOFFSET].m_spinXFace;
    m_panelMouseInfo[m_activeWindow].m_spinYFace = m_panelMouseInfo[FULLOFFSET].m_spinYFace;
    m_panelMouseInfo[m_activeWindow].m_origXPos = m_panelMouseInfo[FULLOFFSET].m_origXPos;
    m_panelMouseInfo[m_activeWindow].m_origYPos = m_panelMouseInfo[FULLOFFSET].m_origYPos;
    m_panelMouseInfo[m_activeWindow].m_rotate = m_panelMouseInfo[FULLOFFSET].m_rotate;
    m_panelMouseInfo[m_activeWindow].m_translate = m_panelMouseInfo[FULLOFFSET].m_translate;
    m_activeWindow=ALL;
  }

}


void NGLScene::render()
{
  // clear the screen and depth buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   switch(m_activeWindow)
   {
     case ALL :
     {
       front(PANEL);
       side(PANEL);
       top(PANEL);
       persp(PANEL);
       break;
     }
     case FRONT : {front(FULLSCREEN); break; }
     case SIDE : {side(FULLSCREEN); break; }
     case TOP : {top(FULLSCREEN); break; }
     case PERSP : {persp(FULLSCREEN); break; }

   }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{
  m_mouseX=_event->x();
  m_mouseY=_event->y();
  int win;
  if(m_activeWindow ==ALL)
    win=(int)getActiveQuadrant();
  else
    win=FULLOFFSET;
  // note the method buttons() is the button state when event was called
  // this is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
  if(m_panelMouseInfo[win].m_rotate && _event->buttons() == Qt::LeftButton)
  {
    int diffx=_event->x()-m_panelMouseInfo[win].m_origX;
    int diffy=_event->y()-m_panelMouseInfo[win].m_origY;
    m_panelMouseInfo[win].m_spinXFace += (float) 0.5f * diffy;
    m_panelMouseInfo[win].m_spinYFace += (float) 0.5f * diffx;
    m_panelMouseInfo[win].m_origX = _event->x();
    m_panelMouseInfo[win].m_origY = _event->y();
    renderLater();

	}
	// right mouse translate code
	else if(m_panelMouseInfo[win].m_translate && _event->buttons() == Qt::RightButton)
	{
		int diffX = (int)(_event->x() - m_panelMouseInfo[win].m_origXPos);
		int diffY = (int)(_event->y() - m_panelMouseInfo[win].m_origYPos);
		m_panelMouseInfo[win].m_origXPos=_event->x();
		m_panelMouseInfo[win].m_origYPos=_event->y();
		m_panelMouseInfo[win].m_modelPos.m_x += INCREMENT * diffX;
		m_panelMouseInfo[win].m_modelPos.m_y -= INCREMENT * diffY;
		renderLater();

	}
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent ( QMouseEvent * _event)
{
	int win;
	if(m_activeWindow ==ALL)
		win=(int)getActiveQuadrant();
	else
		win=FULLOFFSET;

  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if(_event->button() == Qt::LeftButton)
  {
    m_panelMouseInfo[win].m_origX = _event->x();
    m_panelMouseInfo[win].m_origY = _event->y();
    m_panelMouseInfo[win].m_rotate =true;
  }
  // right mouse translate mode
  else if(_event->button() == Qt::RightButton)
  {
    m_panelMouseInfo[win].m_origXPos = _event->x();
    m_panelMouseInfo[win].m_origYPos = _event->y();
    m_panelMouseInfo[win].m_translate=true;
  }

}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent ( QMouseEvent * _event )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  int win;
  if(m_activeWindow ==ALL)
    win=(int)getActiveQuadrant();
  else
    win=FULLOFFSET;
  if (_event->button() == Qt::LeftButton)
  {
    m_panelMouseInfo[win].m_rotate=false;
  }
  // right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_panelMouseInfo[win].m_translate=false;
  }
 }

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent(QWheelEvent *_event)
{

	int win;
	if(m_activeWindow ==ALL)
		win=(int)getActiveQuadrant();
	else
		win=FULLOFFSET;
	// check the diff of the wheel position (0 means no change)
	if(_event->delta() > 0)
	{
		m_panelMouseInfo[win].m_modelPos.m_z+=ZOOM;
	}
	else if(_event->delta() <0 )
	{
		m_panelMouseInfo[win].m_modelPos.m_z-=ZOOM;
	}
	renderLater();
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the NGLScene
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  // turn on wirframe rendering
  case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
  // turn off wire frame
  case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
  // show full screen
  case Qt::Key_F : frameActive(); break;
    // show windowed
    case Qt::Key_Space : toggleWindow(); break;
  default : break;
  }
  // finally update the NGLScene and re-draw
  //if (isExposed())
    renderLater();
}
