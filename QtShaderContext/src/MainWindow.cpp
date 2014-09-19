#include "NGLScene.h"
#include "MainWindow.h"
#include <QMdiSubWindow>
#include <QToolBar>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QRadioButton>
#include <QKeyEvent>
#include <QMdiArea>
#include <QApplication>
//#include "RGBWindow.h"
//#include "DepthWindow.h"

MainWindow::MainWindow(QWidget *_parent ): QMainWindow(_parent)
{
	this->resize(QSize(1024,720));
	QToolBar *toolbar = new QToolBar(this);
	toolbar->setObjectName("KinectToolbar");
	QLabel *label= new QLabel(toolbar);
	label->setText("angle");
	toolbar->addWidget(label);
	QDoubleSpinBox *angle = new QDoubleSpinBox(this);
	angle->setMaximum(30.0);
	angle->setMinimum(-30.0);
	angle->setSingleStep(1.0);

	toolbar->addWidget(angle);
	QPushButton *reset = new QPushButton("reset");

	toolbar->addWidget(reset);
	QLabel *label2= new QLabel(toolbar);
	label2->setText("Camera Mode");
	toolbar->addWidget(label2);
	QComboBox *videoMode= new QComboBox(toolbar);

	videoMode->addItem("RGB");
	videoMode->addItem("Bayer");
	videoMode->addItem("IR 8");
	videoMode->addItem("IR 10");
	videoMode->addItem("IR 10 P");
	videoMode->addItem("YUV_RGB");
	videoMode->addItem("YUV_RAW");

	toolbar->addWidget(videoMode);

	QRadioButton *off = new QRadioButton("LED off",toolbar);
	off->setDown(true);
	toolbar->addWidget(off);
	QRadioButton *red = new QRadioButton("LED red",toolbar);

	toolbar->addWidget(red);
	QRadioButton *green = new QRadioButton("LED green",toolbar);

	toolbar->addWidget(green);
	QRadioButton *yellow = new QRadioButton("LED yellow",toolbar);

	toolbar->addWidget(yellow);

	QRadioButton *redF = new QRadioButton("flash red ",toolbar);

	toolbar->addWidget(redF);
	QRadioButton *greenF = new QRadioButton("flash green",toolbar);

	toolbar->addWidget(greenF);
	QRadioButton *yellowF = new QRadioButton("flash yellow",toolbar);

	toolbar->addWidget(yellowF);




/*
	QPushButton *startVideo= new QPushButton("video");
	startVideo->setCheckable("true");
	QObject::connect(startVideo,SIGNAL(clicked(bool)),m_kinect,SLOT(toggleVideo(bool)));
	toolbar->addWidget(startVideo);

	QPushButton *startDepth= new QPushButton("depth");
	startVideo->setCheckable("true");
	QObject::connect(startDepth,SIGNAL(clicked(bool)),m_kinect,SLOT(toggleDepth(bool)));
	toolbar->addWidget(startDepth);
*/
	this->addToolBar(toolbar);

	m_mdiArea = new QMdiArea;
	m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setCentralWidget(m_mdiArea);
	this->setWindowTitle(QString("Kinect Demo"));



//	OpenGLWindow *rgb = new OpenGLWindow(this);
//	QMdiSubWindow *subWindow1 = new QMdiSubWindow;
//	subWindow1->setWidget(rgb);
//	subWindow1->setAttribute(Qt::WA_DeleteOnClose);
//	subWindow1->setWindowTitle("RGB Output");
//	subWindow1->resize(500,350);
//	m_mdiArea->addSubWindow(subWindow1);
// subWindow1->show();

//	DepthWindow *depth = new DepthWindow(this);
	QMdiSubWindow *subWindow2 = new QMdiSubWindow;
	NGLScene *scene = new NGLScene(this);
	scene->resize(250,250);
	scene->show();
	subWindow2->setWidget(scene);

	subWindow2->setAttribute(Qt::WA_DeleteOnClose);
	subWindow2->setWindowTitle("Depth Output");
	subWindow2->resize(500,350);
	m_mdiArea->addSubWindow(subWindow2);

//	subWindow2->show();


	this->setWindowTitle(QString("kinect demo"));


}

MainWindow::~MainWindow()
{
}


void MainWindow::keyPressEvent( QKeyEvent *_event )
{
	// this method is called every time the main window recives a key event.
	// we then switch on the key value and set the camera in the GLWindow
	switch (_event->key())
	{
	case Qt::Key_Escape : QApplication::exit(EXIT_SUCCESS); break;
//	case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;//
//	case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
//	case::Qt::Key_Up :  m_gl->incrementAngle(); break;
//	case::Qt::Key_Down : m_gl->decrementAngle(); break;
//	case::Qt::Key_Space : m_gl->zeroAngle(); break;
  default : break;
  }
  // finally update the GLWindow and re-draw
 // m_gl->updateGL();
}

void MainWindow::resizeEvent ( QResizeEvent * _event )
{




}



