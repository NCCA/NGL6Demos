#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GLWindow.h"
#include "Axis.h"

/// @namespace Ui our Ui namespace created from the MainWindow class
namespace Ui {
    class MainWindow;
}
//----------------------------------------------------------------------------------------------------------------------
/// @file MainWindow.h
/// @brief The main class for our UI window
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/10/10
/// Revision History :
/// Initial Version 10/10/10 (Binary day ;-0 )
/// @class MainWindow
/// @brief our main UI window, created by MainWindow.ui and extended here
//----------------------------------------------------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
  //----------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------
    explicit MainWindow(QWidget *parent = 0);
  //----------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------
    ~MainWindow();

private:
  //----------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------
    Ui::MainWindow *m_ui;
    //----------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------
    GLWindow *m_gl;
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief override the keyPressEvent inherited from QObject so we can handle key presses.
    /// @param [in] _event the event to process
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent( QKeyEvent *_event );

private slots :
    void setScale();
    void setTranslate();
    void setRotate();
    void reset();
    void updateMatrix(ngl::Mat4 _m );
    void setMatrix();
    void changeColour();
    void setEuler();
    void setTab(int _value);

};

#endif // MAINWINDOW_H
