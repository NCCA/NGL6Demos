#include "MainWindow.h"
#include <QToolBar>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QRadioButton>
#include <QApplication>
#include <QMdiSubWindow>
#include "Context.h"
#include "VideoWindow.h"
#include <QGLFormat>

MainWindow::MainWindow( QWidget *_parent ): QMainWindow(_parent)
{
	this->resize(QSize(1024,800));
	m_kinect=QKinect::instance();
	QToolBar *toolbar = new QToolBar(this);
	toolbar->setObjectName("KinectToolbar");
	QLabel *label= new QLabel(toolbar);
	label->setText("angle");
	toolbar->addWidget(label);
	QDoubleSpinBox *angle = new QDoubleSpinBox(this);
	angle->setMaximum(30.0);
	angle->setMinimum(-30.0);
	angle->setSingleStep(1.0);
	QObject::connect(angle,SIGNAL(valueChanged(double)),m_kinect,SLOT(setAngle(double)));

	toolbar->addWidget(angle);
	QPushButton *reset = new QPushButton("reset");
	QObject::connect(reset,SIGNAL(clicked()),m_kinect,SLOT(resetAngle()));

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
	QObject::connect(videoMode,SIGNAL(currentIndexChanged(int)),m_kinect,SLOT(setVideoMode(int)));

	toolbar->addWidget(videoMode);

	QRadioButton *off = new QRadioButton("LED off",toolbar);
	off->setDown(true);
	QObject::connect(off,SIGNAL(clicked()),m_kinect,SLOT(setLedOff()));
	toolbar->addWidget(off);
	QRadioButton *red = new QRadioButton("LED red",toolbar);
	QObject::connect(red,SIGNAL(clicked()),m_kinect,SLOT(setRedLed()));

	toolbar->addWidget(red);
	QRadioButton *green = new QRadioButton("LED green",toolbar);
	QObject::connect(green,SIGNAL(clicked()),m_kinect,SLOT(setGreenLed()));

	toolbar->addWidget(green);
	QRadioButton *yellow = new QRadioButton("LED yellow",toolbar);
	QObject::connect(yellow,SIGNAL(clicked()),m_kinect,SLOT(setYellowLed()));

	toolbar->addWidget(yellow);

  QRadioButton *redF = new QRadioButton("flash red ",toolbar);
  QObject::connect(redF,SIGNAL(clicked()),m_kinect,SLOT(setRedLedFlash()));

	toolbar->addWidget(redF);
	QRadioButton *greenF = new QRadioButton("flash green",toolbar);

	QObject::connect(greenF,SIGNAL(clicked()),m_kinect,SLOT(setGreenLedFlash()));
	toolbar->addWidget(greenF);
	QRadioButton *yellowF = new QRadioButton("flash yellow",toolbar);
	QObject::connect(yellowF,SIGNAL(clicked()),m_kinect,SLOT(setYellowLedFlash()));

	toolbar->addWidget(yellowF);

	this->addToolBar(toolbar);

	m_mdiArea = new QMdiArea;
	m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setCentralWidget(m_mdiArea);
	this->setWindowTitle(QString("Kinect Demo"));

	QGLFormat format;
	format.setVersion(4,1);
	format.setProfile( QGLFormat::CoreProfile);


	QMdiSubWindow *subWindow1 = new QMdiSubWindow;
	VideoWindow *rgb = new VideoWindow(VideoWindow::RGB);

	QWidget *container = QWidget::createWindowContainer(rgb);
	subWindow1->setWidget(container);
	subWindow1->setAttribute(Qt::WA_DeleteOnClose);
	subWindow1->setWindowTitle("RGB Output");
	subWindow1->resize(500,350);
	m_mdiArea->addSubWindow(subWindow1);
	subWindow1->show();

	VideoWindow *depth = new VideoWindow(VideoWindow::DEPTH);
	container = QWidget::createWindowContainer(depth);
	QMdiSubWindow *subWindow2 = new QMdiSubWindow;
	subWindow2->setWidget(container);
	subWindow2->setAttribute(Qt::WA_DeleteOnClose);
	subWindow2->setWindowTitle("Depth Output");
	subWindow2->resize(500,350);
	m_mdiArea->addSubWindow(subWindow2);

	subWindow2->show();


	this->setWindowTitle(QString("OpenCV and Kinect demo"));


}

MainWindow::~MainWindow()
{
}


void MainWindow::keyPressEvent(QKeyEvent *_event )
{
	// this method is called every time the main window recives a key event.
	// we then switch on the key value and set the camera in the GLWindow
	switch (_event->key())
	{
	case Qt::Key_Escape : m_kinect->shutDownKinect(); QApplication::exit(EXIT_SUCCESS); break;
  default : break;
  }
}

void MainWindow::resizeEvent ( QResizeEvent * _event )
{
 // m_gl->resize(_event->size());
}

