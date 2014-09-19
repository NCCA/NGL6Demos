#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "OpenGLWindow.h"
class QMdiArea;
/// @file MainWindow.h
/// @brief This is the MainWindow Class which is generated by the Ui file, if we wish to add anything to the main
/// Ui we add it here
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/10/10
/// Revision History :
/// Initial Version 10/10/10 (Binary day ;-0 )
/// @class MainWindow
/// @brief the main re-sizable window which contains a GLWindow widget used to hold our
/// basic gl applications
///// pre declare some classes we need

class MainWindow : public QMainWindow
{
Q_OBJECT
protected :
    /// @brief override the keyPressEvent inherited from QObject so we can handle key presses.
    /// @param [in] _event the event to process
    void keyPressEvent( QKeyEvent *_event );
/// @brief override the resizeEvent inherited from QObject so we can handle key presses.
/// @param [in] _event the event to process

    void resizeEvent ( QResizeEvent * _event );
public:
    /// @brief constructor
    /// @param _parent the parent window the for this window
    MainWindow(QWidget *_parent = 0);
    /// @brief  dtor free up the GLWindow and all resources
    ~MainWindow();
    /// \brief here we add our GLWindow extending the base functionality of our class
private slots :

private:
    QMdiArea *m_mdiArea;



};

#endif // MAINWINDOW_H
