/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Mon Jan 14 08:25:16 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *s_mainWindowGridLayout;
    QGroupBox *s_transformGB;
    QGridLayout *gridLayout;
    QDoubleSpinBox *m_erotationX;
    QLabel *label;
    QDoubleSpinBox *m_srotationX;
    QDoubleSpinBox *m_srotationY;
    QDoubleSpinBox *m_srotationZ;
    QDoubleSpinBox *m_erotationZ;
    QDoubleSpinBox *m_erotationY;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QLineEdit *m_q2;
    QLabel *label_6;
    QLabel *label_5;
    QLabel *label_4;
    QSpacerItem *verticalSpacer_2;
    QSlider *m_interpolate;
    QTextEdit *m_matrix;
    QLineEdit *m_q1;
    QLabel *label_7;
    QLineEdit *m_q3;
    QLabel *m_interpText;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(985, 591);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        s_mainWindowGridLayout = new QGridLayout(centralwidget);
        s_mainWindowGridLayout->setObjectName(QString::fromUtf8("s_mainWindowGridLayout"));
        s_transformGB = new QGroupBox(centralwidget);
        s_transformGB->setObjectName(QString::fromUtf8("s_transformGB"));
        gridLayout = new QGridLayout(s_transformGB);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        m_erotationX = new QDoubleSpinBox(s_transformGB);
        m_erotationX->setObjectName(QString::fromUtf8("m_erotationX"));
        m_erotationX->setMinimum(-360);
        m_erotationX->setMaximum(360);
        m_erotationX->setValue(-300);

        gridLayout->addWidget(m_erotationX, 3, 1, 1, 1);

        label = new QLabel(s_transformGB);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        m_srotationX = new QDoubleSpinBox(s_transformGB);
        m_srotationX->setObjectName(QString::fromUtf8("m_srotationX"));
        m_srotationX->setMinimum(-360);
        m_srotationX->setMaximum(360);
        m_srotationX->setValue(45);

        gridLayout->addWidget(m_srotationX, 1, 1, 1, 1);

        m_srotationY = new QDoubleSpinBox(s_transformGB);
        m_srotationY->setObjectName(QString::fromUtf8("m_srotationY"));
        m_srotationY->setMinimum(-360);
        m_srotationY->setMaximum(360);
        m_srotationY->setValue(90);

        gridLayout->addWidget(m_srotationY, 1, 2, 1, 1);

        m_srotationZ = new QDoubleSpinBox(s_transformGB);
        m_srotationZ->setObjectName(QString::fromUtf8("m_srotationZ"));
        m_srotationZ->setMinimum(-360);
        m_srotationZ->setMaximum(360);
        m_srotationZ->setValue(80);

        gridLayout->addWidget(m_srotationZ, 1, 3, 1, 1);

        m_erotationZ = new QDoubleSpinBox(s_transformGB);
        m_erotationZ->setObjectName(QString::fromUtf8("m_erotationZ"));
        m_erotationZ->setMinimum(-360);
        m_erotationZ->setMaximum(360);
        m_erotationZ->setValue(360);

        gridLayout->addWidget(m_erotationZ, 3, 3, 1, 1);

        m_erotationY = new QDoubleSpinBox(s_transformGB);
        m_erotationY->setObjectName(QString::fromUtf8("m_erotationY"));
        m_erotationY->setMinimum(-360);
        m_erotationY->setMaximum(360);
        m_erotationY->setValue(270);

        gridLayout->addWidget(m_erotationY, 3, 2, 1, 1);

        label_2 = new QLabel(s_transformGB);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 1, 1, 1);


        s_mainWindowGridLayout->addWidget(s_transformGB, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        s_mainWindowGridLayout->addItem(horizontalSpacer, 0, 0, 16, 1);

        m_q2 = new QLineEdit(centralwidget);
        m_q2->setObjectName(QString::fromUtf8("m_q2"));
        m_q2->setFrame(false);
        m_q2->setReadOnly(true);

        s_mainWindowGridLayout->addWidget(m_q2, 4, 1, 1, 1);

        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        s_mainWindowGridLayout->addWidget(label_6, 5, 1, 1, 1);

        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        s_mainWindowGridLayout->addWidget(label_5, 3, 1, 1, 1);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        s_mainWindowGridLayout->addWidget(label_4, 1, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        s_mainWindowGridLayout->addItem(verticalSpacer_2, 9, 1, 1, 1);

        m_interpolate = new QSlider(centralwidget);
        m_interpolate->setObjectName(QString::fromUtf8("m_interpolate"));
        m_interpolate->setMinimum(0);
        m_interpolate->setMaximum(1000);
        m_interpolate->setSingleStep(1);
        m_interpolate->setPageStep(1);
        m_interpolate->setOrientation(Qt::Horizontal);
        m_interpolate->setTickPosition(QSlider::NoTicks);

        s_mainWindowGridLayout->addWidget(m_interpolate, 11, 1, 1, 1);

        m_matrix = new QTextEdit(centralwidget);
        m_matrix->setObjectName(QString::fromUtf8("m_matrix"));

        s_mainWindowGridLayout->addWidget(m_matrix, 8, 1, 1, 1);

        m_q1 = new QLineEdit(centralwidget);
        m_q1->setObjectName(QString::fromUtf8("m_q1"));
        m_q1->setFrame(false);
        m_q1->setReadOnly(true);

        s_mainWindowGridLayout->addWidget(m_q1, 2, 1, 1, 1);

        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        s_mainWindowGridLayout->addWidget(label_7, 7, 1, 1, 1);

        m_q3 = new QLineEdit(centralwidget);
        m_q3->setObjectName(QString::fromUtf8("m_q3"));
        m_q3->setFrame(false);
        m_q3->setReadOnly(true);

        s_mainWindowGridLayout->addWidget(m_q3, 6, 1, 1, 1);

        m_interpText = new QLabel(centralwidget);
        m_interpText->setObjectName(QString::fromUtf8("m_interpText"));

        s_mainWindowGridLayout->addWidget(m_interpText, 10, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 985, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Rotation using Quaternion SLERP", 0, QApplication::UnicodeUTF8));
        s_transformGB->setTitle(QApplication::translate("MainWindow", "Transform", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Start Rotation", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "End Rotation", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Interpolated Quat", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "End Quaternion", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Start Quaternion", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Rotation Matrix", 0, QApplication::UnicodeUTF8));
        m_interpText->setText(QApplication::translate("MainWindow", "Interpolate", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
