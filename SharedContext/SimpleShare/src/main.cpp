/****************************************************************************
basic OpenGL demo modified from http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
****************************************************************************/
#include <QtGui/QGuiApplication>
#include <iostream>
#include "NGLScene.h"
#include "Context.h"
#include <QObject>
#include <cstdlib>

void cleanUp()
{
  Context::close();
}

int main(int argc, char **argv)
{
  QGuiApplication app(argc, argv);
  // create an OpenGL format specifier
  QSurfaceFormat format;
  // set the number of samples for multisampling
  // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
  format.setSamples(4);
  // with luck we have the latest GL version so set to this
  format.setMajorVersion(4);
  format.setMinorVersion(1);
  // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
  format.setProfile(QSurfaceFormat::CoreProfile);
  // now set the depth buffer to 24 bits
  format.setDepthBufferSize(24);
  Context::setFormat(format);
  std::vector <NGLScene *> windows;
  for(int i=0; i<7; ++i)
  {
    windows.push_back(new NGLScene(i));
    windows[i]->resize(200,200);
    windows[i]->setPosition(i*200,200);
    windows[i]->show();
  }


  std::atexit(cleanUp);
  return app.exec();
}



