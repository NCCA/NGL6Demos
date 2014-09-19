/// @file MainWindow.cpp
/// @brief basic implementation file for the MainWindow class
#include "MainWindow.h"
#include "ui_MainWindow.h"

//----------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow( QWidget *parent ) : QMainWindow(parent), m_ui(new Ui::MainWindow)
{
  // setup the user interface
  m_ui->setupUi(this);
  // now we create our glwindow class
  // create an openGL format and pass to the new GLWidget
  QGLFormat format;
  format.setVersion(3,2);
  format.setProfile( QGLFormat::CoreProfile);
  m_gl = new GLWindow(format,this);
  m_ui->s_mainGridLayout->addWidget(m_gl, 0, 0, 6, 6);

  // the following code connects the ui components to the GL class
  /// connect the vboSelection combo to the index value in the gl window class
  connect(m_ui->m_vboSelection,SIGNAL(currentIndexChanged(int)),m_gl,SLOT(vboChanged(int )));
  /// connect the normal and vertexx tick boxes
  connect(m_ui->m_normals,SIGNAL(toggled(bool)),m_gl,SLOT(toggleNormals(bool)));
  /// connect the wireframe tick box
  connect(m_ui->m_wireframe,SIGNAL(toggled(bool)),m_gl,SLOT(toggleWireframe(bool)));
  connect(m_ui->m_sx,SIGNAL(valueChanged(double)),this,SLOT(setScale()));
  connect(m_ui->m_sy,SIGNAL(valueChanged(double)),this,SLOT(setScale()));
  connect(m_ui->m_sz,SIGNAL(valueChanged(double)),this,SLOT(setScale()));
  connect(m_ui->m_tx,SIGNAL(valueChanged(double)),this,SLOT(setTranslate()));
  connect(m_ui->m_ty,SIGNAL(valueChanged(double)),this,SLOT(setTranslate()));
  connect(m_ui->m_tz,SIGNAL(valueChanged(double)),this,SLOT(setTranslate()));
  connect(m_ui->m_rx,SIGNAL(valueChanged(double)),this,SLOT(setRotate()));
  connect(m_ui->m_ry,SIGNAL(valueChanged(double)),this,SLOT(setRotate()));
  connect(m_ui->m_rz,SIGNAL(valueChanged(double)),this,SLOT(setRotate()));
  /// connect the reset button
  connect(m_ui->m_reset,SIGNAL(clicked()),this,SLOT(reset()));
  // connect the emit matrix dirty to update the matrix.
  connect(m_gl,SIGNAL(matrixDirty(ngl::Mat4)),this,SLOT(updateMatrix(ngl::Mat4)));
  // connect the slider to the normal drawing attrib size
  connect(m_ui->m_normalSize,SIGNAL(valueChanged(int)),m_gl,SLOT(setNormalSize(int)));
  /*
  // now connect the matrix spin wheels with the set matrix command
  connect(m_ui->m_m00,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  connect(m_ui->m_m01,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  connect(m_ui->m_m02,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  connect(m_ui->m_m03,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  connect(m_ui->m_m10,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  connect(m_ui->m_m11,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  connect(m_ui->m_m12,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  connect(m_ui->m_m13,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  connect(m_ui->m_m20,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  connect(m_ui->m_m21,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  connect(m_ui->m_m22,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  connect(m_ui->m_m23,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  connect(m_ui->m_m30,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  connect(m_ui->m_m31,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  connect(m_ui->m_m32,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  connect(m_ui->m_m33,SIGNAL(valueChanged(double)),this,SLOT(setMatrix()));
  */
  connect(m_ui->m_colour,SIGNAL(clicked()),this,SLOT(changeColour()));
  connect(m_ui->m_matrixOrder,SIGNAL(currentIndexChanged(int)),m_gl,SLOT(setMatrixOrder(int)));
  connect(m_ui->m_matrixOrder,SIGNAL(currentIndexChanged(int)),this,SLOT(setTab(int)));

  connect(m_ui->m_eulerAngle,SIGNAL(valueChanged(double)),this,SLOT(setEuler()));
  connect(m_ui->m_eulerXAxis,SIGNAL(valueChanged(double)),this,SLOT(setEuler()));
  connect(m_ui->m_eulerYAxis,SIGNAL(valueChanged(double)),this,SLOT(setEuler()));
  connect(m_ui->m_eulerZAxis,SIGNAL(valueChanged(double)),this,SLOT(setEuler()));
}

//----------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete m_ui;
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::setScale()
{
  m_gl->setScale(m_ui->m_sx->value(), m_ui->m_sy->value(),m_ui->m_sz->value());
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::setTranslate()
{
  m_gl->setTranslate(m_ui->m_tx->value(), m_ui->m_ty->value(),m_ui->m_tz->value());
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::setRotate()
{
  m_gl->setRotate(m_ui->m_rx->value(), m_ui->m_ry->value(),m_ui->m_rz->value());
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::reset()
{
  m_ui->m_rx->setValue(0.0);
  m_ui->m_ry->setValue(0.0);
  m_ui->m_rz->setValue(0.0);
  m_ui->m_sx->setValue(1.0);
  m_ui->m_sy->setValue(1.0);
  m_ui->m_sz->setValue(1.0);
  m_ui->m_tx->setValue(0.0);
  m_ui->m_ty->setValue(0.0);
  m_ui->m_tz->setValue(0.0);
  m_ui->m_normals->setChecked(false);
  m_ui->m_wireframe->setChecked(false);
  m_gl->resetMouse();
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::updateMatrix(ngl::Mat4 _m )
{
  ngl::Real *data=_m.openGL();
  m_ui->m_m00->setValue(data[0]);
  m_ui->m_m10->setValue(data[1]);
  m_ui->m_m20->setValue(data[2]);
  m_ui->m_m30->setValue(data[3]);

  m_ui->m_m01->setValue(data[4]);
  m_ui->m_m11->setValue(data[5]);
  m_ui->m_m21->setValue(data[6]);
  m_ui->m_m31->setValue(data[7]);

  m_ui->m_m02->setValue(data[8]);
  m_ui->m_m12->setValue(data[9]);
  m_ui->m_m22->setValue(data[10]);
  m_ui->m_m32->setValue(data[11]);

  m_ui->m_m03->setValue(data[12]);
  m_ui->m_m13->setValue(data[13]);
  m_ui->m_m23->setValue(data[14]);
  m_ui->m_m33->setValue(data[15]);

}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::setMatrix()
{

}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::keyPressEvent(QKeyEvent *_event  )
{
  this->setFocus(Qt::ActiveWindowFocusReason);
  float increment=0.1;
  if ( QApplication::keyboardModifiers ()  == Qt::ShiftModifier)
  {
    increment=-0.1;
  }

  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  case Qt::Key_Escape :{ QApplication::exit(EXIT_SUCCESS); break;}
  case Qt::Key_W : {m_ui->m_wireframe->toggle(); break;}
  case Qt::Key_N : {m_ui->m_normals->toggle(); break;}

  case Qt::Key_S :
  {
    m_ui->m_sx->setValue(m_ui->m_sx->value()+increment);
    m_ui->m_sy->setValue(m_ui->m_sy->value()+increment);
    m_ui->m_sz->setValue(m_ui->m_sz->value()+increment);

    break;

  }
  case Qt::Key_X : { m_ui->m_rx->setValue(m_ui->m_rx->value()+increment*10); break; }
  case Qt::Key_Y : { m_ui->m_ry->setValue(m_ui->m_ry->value()+increment*10); break; }
  case Qt::Key_Z : { m_ui->m_rz->setValue(m_ui->m_rz->value()+increment*10); break; }

  case Qt::Key_I : { m_ui->m_ty->setValue(m_ui->m_ty->value()+increment); break; }
  case Qt::Key_M : { m_ui->m_ty->setValue(m_ui->m_ty->value()-increment); break; }
  case Qt::Key_J : { m_ui->m_tx->setValue(m_ui->m_tx->value()-increment); break; }
  case Qt::Key_K : { m_ui->m_tx->setValue(m_ui->m_tx->value()+increment); break; }
  case Qt::Key_O : { m_ui->m_tz->setValue(m_ui->m_tz->value()-increment); break; }
  case Qt::Key_L : { m_ui->m_tz->setValue(m_ui->m_tz->value()+increment); break; }


  default : break;
  }
  // finally update the GLWindow and re-draw
  m_gl->updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::changeColour()
{
  // this slot is called when the colour button is pushed
  // we first use the colour dialog to get the colour

  QColor colour = QColorDialog::getColor();
  // we now check to see if a colour was set or if the cancel was pressed
  // using the isValid method
  if (colour.isValid())
  {
    // if it was we get the colour values using the ..F() function and call the
    // SetColour method in the GLWindow
    m_gl->setColour(colour.redF(),colour.greenF(),colour.blueF());
    // finally we update the GLWindow.
    m_gl->updateGL();
  }
}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::setEuler()
{
  m_gl->setEuler(m_ui->m_eulerAngle->value(),
                 m_ui->m_eulerXAxis->value(),
                 m_ui->m_eulerYAxis->value(),
                 m_ui->m_eulerZAxis->value());
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::setTab(int _value )
{
  if(_value >2)
  {
    m_ui->s_rotateTabWidget->setCurrentIndex(1);
  }
  else
  {
    m_ui->s_rotateTabWidget->setCurrentIndex(0);
  }
}
//----------------------------------------------------------------------------------------------------------------------
