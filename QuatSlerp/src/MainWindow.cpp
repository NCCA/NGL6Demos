#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QApplication>
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),  m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    // create an openGL format and pass to the new GLWidget
    QGLFormat format;
    format.setVersion(3,2);
    format.setProfile( QGLFormat::CoreProfile);

    m_gl=new  GLWindow(format,this);
    // row span col span
    m_ui->s_mainWindowGridLayout->addWidget(m_gl,0,0,12,1);
    // set the rotation signals
    connect(m_ui->m_srotationX,SIGNAL(valueChanged(double)),this,SLOT(startRotationChanged()));
    connect(m_ui->m_srotationY,SIGNAL(valueChanged(double)),this,SLOT(startRotationChanged()));
    connect(m_ui->m_srotationZ,SIGNAL(valueChanged(double)),this,SLOT(startRotationChanged()));
    connect(m_ui->m_erotationX,SIGNAL(valueChanged(double)),this,SLOT(endRotationChanged()));
    connect(m_ui->m_erotationY,SIGNAL(valueChanged(double)),this,SLOT(endRotationChanged()));
    connect(m_ui->m_erotationZ,SIGNAL(valueChanged(double)),this,SLOT(endRotationChanged()));
    connect(m_ui->m_interpolate,SIGNAL(valueChanged(int)),this,SLOT(interpolationChanged(int)));
    connect(m_gl,SIGNAL(changeStartQuat(QString)),m_ui->m_q1,SLOT(setText(QString)));
    connect(m_gl,SIGNAL(changeEndQuat(QString)),m_ui->m_q2,SLOT(setText(QString)));
    connect(m_gl,SIGNAL(changeInterpQuat(QString)),m_ui->m_q3,SLOT(setText(QString)));
    connect(m_gl,SIGNAL(clearMatrixText()),m_ui->m_matrix,SLOT(clear()));
    connect(m_gl,SIGNAL(appendMatrixText(QString)),m_ui->m_matrix,SLOT(append(QString)));
    connect(m_gl,SIGNAL(updateInterpText(QString)),m_ui->m_interpText,SLOT(setText(QString)));

}

MainWindow::~MainWindow()
{
    delete m_ui;
}



void MainWindow::startRotationChanged()
{
  m_gl->setStartRotation(ngl::Vec3(
                                    m_ui->m_srotationX->value(),
                                    m_ui->m_srotationY->value(),
                                    m_ui->m_srotationZ->value())
                         );
}

void MainWindow::endRotationChanged()
{
  m_gl->setEndRotation(ngl::Vec3(
                                  m_ui->m_erotationX->value(),
                                  m_ui->m_erotationY->value(),
                                  m_ui->m_erotationZ->value())
                                );
}

void MainWindow::interpolationChanged(int _v)
{
  m_gl->setInterpolation(float(_v/1000.0));
}
