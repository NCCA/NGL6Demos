/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Wed Jan 16 11:48:34 2013
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
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *s_mainWindowGridLayout;
    QGroupBox *s_transformGB;
    QGridLayout *gridLayout;
    QLabel *label;
    QDoubleSpinBox *m_rotationX;
    QDoubleSpinBox *m_rotationY;
    QDoubleSpinBox *m_rotationZ;
    QLabel *label_2;
    QDoubleSpinBox *m_scaleX;
    QDoubleSpinBox *m_scaleY;
    QDoubleSpinBox *m_scaleZ;
    QLabel *label_3;
    QDoubleSpinBox *m_positionX;
    QDoubleSpinBox *m_positionY;
    QDoubleSpinBox *m_positionZ;
    QGroupBox *s_drawGB;
    QGridLayout *gridLayout_2;
    QComboBox *m_objectSelection;
    QCheckBox *m_wireframe;
    QPushButton *m_colour;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(972, 656);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        s_mainWindowGridLayout = new QGridLayout(centralwidget);
        s_mainWindowGridLayout->setObjectName(QString::fromUtf8("s_mainWindowGridLayout"));
        s_transformGB = new QGroupBox(centralwidget);
        s_transformGB->setObjectName(QString::fromUtf8("s_transformGB"));
        gridLayout = new QGridLayout(s_transformGB);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(s_transformGB);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        m_rotationX = new QDoubleSpinBox(s_transformGB);
        m_rotationX->setObjectName(QString::fromUtf8("m_rotationX"));
        m_rotationX->setMinimum(-360);
        m_rotationX->setMaximum(360);

        gridLayout->addWidget(m_rotationX, 1, 0, 1, 1);

        m_rotationY = new QDoubleSpinBox(s_transformGB);
        m_rotationY->setObjectName(QString::fromUtf8("m_rotationY"));
        m_rotationY->setMinimum(-360);
        m_rotationY->setMaximum(360);

        gridLayout->addWidget(m_rotationY, 1, 1, 1, 1);

        m_rotationZ = new QDoubleSpinBox(s_transformGB);
        m_rotationZ->setObjectName(QString::fromUtf8("m_rotationZ"));
        m_rotationZ->setMinimum(-360);
        m_rotationZ->setMaximum(360);

        gridLayout->addWidget(m_rotationZ, 1, 2, 1, 1);

        label_2 = new QLabel(s_transformGB);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        m_scaleX = new QDoubleSpinBox(s_transformGB);
        m_scaleX->setObjectName(QString::fromUtf8("m_scaleX"));
        m_scaleX->setMinimum(-4);
        m_scaleX->setMaximum(4);
        m_scaleX->setSingleStep(0.01);
        m_scaleX->setValue(1);

        gridLayout->addWidget(m_scaleX, 3, 0, 1, 1);

        m_scaleY = new QDoubleSpinBox(s_transformGB);
        m_scaleY->setObjectName(QString::fromUtf8("m_scaleY"));
        m_scaleY->setMinimum(-4);
        m_scaleY->setMaximum(4);
        m_scaleY->setSingleStep(0.01);
        m_scaleY->setValue(1);

        gridLayout->addWidget(m_scaleY, 3, 1, 1, 1);

        m_scaleZ = new QDoubleSpinBox(s_transformGB);
        m_scaleZ->setObjectName(QString::fromUtf8("m_scaleZ"));
        m_scaleZ->setMinimum(-4);
        m_scaleZ->setMaximum(4);
        m_scaleZ->setSingleStep(0.01);
        m_scaleZ->setValue(1);

        gridLayout->addWidget(m_scaleZ, 3, 2, 1, 1);

        label_3 = new QLabel(s_transformGB);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        m_positionX = new QDoubleSpinBox(s_transformGB);
        m_positionX->setObjectName(QString::fromUtf8("m_positionX"));
        m_positionX->setMinimum(-20);
        m_positionX->setMaximum(20);
        m_positionX->setSingleStep(0.01);

        gridLayout->addWidget(m_positionX, 5, 0, 1, 1);

        m_positionY = new QDoubleSpinBox(s_transformGB);
        m_positionY->setObjectName(QString::fromUtf8("m_positionY"));
        m_positionY->setMinimum(-20);
        m_positionY->setMaximum(20);
        m_positionY->setSingleStep(0.01);

        gridLayout->addWidget(m_positionY, 5, 1, 1, 1);

        m_positionZ = new QDoubleSpinBox(s_transformGB);
        m_positionZ->setObjectName(QString::fromUtf8("m_positionZ"));
        m_positionZ->setMinimum(-20);
        m_positionZ->setMaximum(20);
        m_positionZ->setSingleStep(0.01);

        gridLayout->addWidget(m_positionZ, 5, 2, 1, 1);


        s_mainWindowGridLayout->addWidget(s_transformGB, 0, 1, 1, 1);

        s_drawGB = new QGroupBox(centralwidget);
        s_drawGB->setObjectName(QString::fromUtf8("s_drawGB"));
        gridLayout_2 = new QGridLayout(s_drawGB);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        m_objectSelection = new QComboBox(s_drawGB);
        m_objectSelection->setObjectName(QString::fromUtf8("m_objectSelection"));

        gridLayout_2->addWidget(m_objectSelection, 0, 0, 1, 1);

        m_wireframe = new QCheckBox(s_drawGB);
        m_wireframe->setObjectName(QString::fromUtf8("m_wireframe"));

        gridLayout_2->addWidget(m_wireframe, 1, 0, 1, 1);

        m_colour = new QPushButton(s_drawGB);
        m_colour->setObjectName(QString::fromUtf8("m_colour"));

        gridLayout_2->addWidget(m_colour, 3, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 4, 0, 1, 1);


        s_mainWindowGridLayout->addWidget(s_drawGB, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        s_mainWindowGridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 972, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "ngl Qt Demo", 0, QApplication::UnicodeUTF8));
        s_transformGB->setTitle(QApplication::translate("MainWindow", "Transform", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Rotation", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Scale", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Position", 0, QApplication::UnicodeUTF8));
        s_drawGB->setTitle(QApplication::translate("MainWindow", "Draw", 0, QApplication::UnicodeUTF8));
        m_objectSelection->clear();
        m_objectSelection->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Teapot", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Sphere", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Cube", 0, QApplication::UnicodeUTF8)
        );
        m_wireframe->setText(QApplication::translate("MainWindow", "WireFrame", 0, QApplication::UnicodeUTF8));
        m_colour->setText(QApplication::translate("MainWindow", "Choose Colour", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
