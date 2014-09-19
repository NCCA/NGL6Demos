#include "MainWindow.h"
#include "GLWindow.h"
#include "ui_MainWindow.h"
#include <QApplication>

//----------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *_parent ):  QMainWindow(_parent),m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);
  QGLFormat format;
  format.setVersion(4,1);
  format.setProfile( QGLFormat::CoreProfile);
  m_gl=new  GLWindow(format,this);
  m_ui->s_mainWindowGridLayout->addWidget(m_gl,0,0,3,1);
  connect(m_ui->m_showDepthMap ,SIGNAL(toggled(bool)),m_gl,SLOT(setDebugQuad(bool)));
  connect(m_ui->m_animate ,SIGNAL(toggled(bool)),m_gl,SLOT(setAnimation(bool)));
  connect(m_ui->m_textureSize ,SIGNAL( currentIndexChanged(int)) ,m_gl,SLOT(changeTextureSize(int)));
  connect(m_ui->m_texMinFilter ,SIGNAL( currentIndexChanged(int)) ,m_gl,SLOT(changeTextureMinFilter(int)));
  connect(m_ui->m_texMagFilter ,SIGNAL( currentIndexChanged(int)) ,m_gl,SLOT(changeTextureMagFilter(int)));


  connect(m_ui->m_zNear, SIGNAL( valueChanged(double)), m_gl, SLOT(setZNear(double)));
  connect(m_ui->m_zfar, SIGNAL( valueChanged(double)), m_gl, SLOT(setZFar(double)));
  connect(m_ui->m_fov, SIGNAL( valueChanged(int)), m_gl, SLOT(setFOV(int)));

  connect(m_ui->m_lightYPosition, SIGNAL( valueChanged(double)), m_gl, SLOT(changeLightYPos(double)));
  connect(m_ui->m_lightZOffset, SIGNAL( valueChanged(double)), m_gl, SLOT(changeLightZOffset(double)));
  connect(m_ui->m_lightXOffset, SIGNAL( valueChanged(double)), m_gl, SLOT(changeLightXOffset(double)));


  connect(m_ui->m_polyOffsetFactor, SIGNAL( valueChanged(double)), m_gl, SLOT(changePolygonOffsetFactor(double)));
  connect(m_ui->m_polygonOffsetUnits, SIGNAL( valueChanged(double)), m_gl, SLOT(changePolygonOffsetScale(double)));

  connect(m_ui->m_colour, SIGNAL( clicked()), m_gl, SLOT(setColour()));

  this->setWindowTitle(QString("FBO Based Shadows"));
}

//----------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
  delete m_ui;
}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::keyPressEvent(
                               QKeyEvent *_event
                              )
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  case Qt::Key_Escape : QApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
  case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
  case Qt::Key_F : this->showFullScreen(); break;
  case Qt::Key_G : this->showNormal(); break;


  default : break;
  }
  // finally update the GLWindow and re-draw
  m_gl->updateGL();
}


