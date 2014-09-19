#ifndef MAINWINDOWUI_H
#define MAINWINDOWUI_H

#include <QMainWindow>

namespace Ui {
class MainWindowUI;
}

class MainWindowUI : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindowUI(QWidget *parent = 0);
  ~MainWindowUI();
  
private:
  Ui::MainWindowUI *ui;
};

#endif // MAINWINDOWUI_H
