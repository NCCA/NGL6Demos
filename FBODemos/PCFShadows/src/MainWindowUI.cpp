#include "include/MainWindowUI.h"
#include "ui_MainWindowUI.h"

MainWindowUI::MainWindowUI(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindowUI)
{
  ui->setupUi(this);
}

MainWindowUI::~MainWindowUI()
{
  delete ui;
}
