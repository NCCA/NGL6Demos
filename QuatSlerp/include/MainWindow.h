#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GLWindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *m_ui;
		/// @brief our openGL widget
    GLWindow *m_gl;

private slots :

    void startRotationChanged();
    void endRotationChanged();
    void interpolationChanged(int _v);
};

#endif // MAINWINDOW_H
