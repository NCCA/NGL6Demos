/****************************************************************************
basic OpenGL demo modified from http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
****************************************************************************/
#include <QApplication>
#include <iostream>
#include "MainWindow.h"



int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  MainWindow window;
  // set the window size
  window.resize(1024, 720);
  // and finally show
  window.show();

  return app.exec();
}



