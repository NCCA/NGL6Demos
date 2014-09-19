#include "MainWindow.h"
#include "GLWindow.h"

MainWindow::MainWindow(
                       QWidget *_parent
                      ):
                        QMainWindow(_parent)
{
    this->resize(QSize(1024,720));
    m_gl = new GLWindow(this);
    this->setWindowTitle(QString("Particle Factory Demo"));
}

MainWindow::~MainWindow()
{
}


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
  case Qt::Key_1 : m_gl->addParticles(0); break;
  case Qt::Key_2 : m_gl->addParticles(1); break;
  case Qt::Key_Z : m_gl->updateEmitAngle(30); break;
  case Qt::Key_X : m_gl->updateEmitAngle(90); break;
  case Qt::Key_C : m_gl->updateEmitAngle(200); break;
  case Qt::Key_V : m_gl->updateEmitAngle(360); break;
  case Qt::Key_Space : m_gl->clearParticles(); break;
  case Qt::Key_R : m_gl->removeParticle(); break;
  case Qt::Key_F : showFullScreen(); break;
	case Qt::Key_N : showNormal(); break;
  case Qt::Key_Left : m_gl->updateEmitterPos(-1,0,0); break;
  case Qt::Key_Right : m_gl->updateEmitterPos(1,0,0); break;
  case Qt::Key_Up : m_gl->updateEmitterPos(0,1,0); break;
  case Qt::Key_Down : m_gl->updateEmitterPos(0,-1,0); break;
  case Qt::Key_I : m_gl->updateEmitterPos(0,0,-1); break;
  case Qt::Key_O : m_gl->updateEmitterPos(0,0,1); break;

  default : break;

  }

  // finally update the GLWindow and re-draw
  m_gl->updateGL();
}

void MainWindow::resizeEvent ( QResizeEvent * _event )
{
  m_gl->resize(_event->size());
}

