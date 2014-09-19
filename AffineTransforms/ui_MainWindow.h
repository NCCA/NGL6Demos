/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *m_centralwidget;
    QGridLayout *s_mainGridLayout;
    QGroupBox *s_matrixGB;
    QGridLayout *gridLayout;
    QDoubleSpinBox *m_m30;
    QDoubleSpinBox *m_m00;
    QDoubleSpinBox *m_m10;
    QDoubleSpinBox *m_m20;
    QDoubleSpinBox *m_m03;
    QDoubleSpinBox *m_m02;
    QDoubleSpinBox *m_m01;
    QDoubleSpinBox *m_m11;
    QDoubleSpinBox *m_m21;
    QDoubleSpinBox *m_m31;
    QDoubleSpinBox *m_m12;
    QDoubleSpinBox *m_m13;
    QDoubleSpinBox *m_m22;
    QDoubleSpinBox *m_m23;
    QDoubleSpinBox *m_m32;
    QDoubleSpinBox *m_m33;
    QWidget *widget;
    QFormLayout *formLayout_2;
    QFormLayout *s_FormLayoutMatrixOrder;
    QComboBox *m_matrixOrder;
    QLabel *s_matrixOrderLabel;
    QTabWidget *s_rotateTabWidget;
    QWidget *s_rotateTabWidgetPage1;
    QGridLayout *gridLayout_2;
    QDoubleSpinBox *m_rx;
    QDoubleSpinBox *m_ry;
    QDoubleSpinBox *m_rz;
    QLabel *s_rotZLabel;
    QLabel *s_rotXLabel;
    QLabel *s_rotYLabel;
    QWidget *tab;
    QGridLayout *gridLayout_5;
    QDoubleSpinBox *m_eulerAngle;
    QDoubleSpinBox *m_eulerXAxis;
    QDoubleSpinBox *m_eulerYAxis;
    QDoubleSpinBox *m_eulerZAxis;
    QLabel *s_eulerAxisLabel;
    QLabel *s_eulerAngleLabel;
    QGroupBox *s_translateGroupBox;
    QGridLayout *gridLayout_3;
    QDoubleSpinBox *m_tx;
    QDoubleSpinBox *m_ty;
    QDoubleSpinBox *m_tz;
    QLabel *s_txLabel;
    QLabel *s_tyLabel;
    QLabel *s_tzLabel;
    QFrame *frame_2;
    QGroupBox *s_scaleGroupBox;
    QGridLayout *gridLayout_4;
    QDoubleSpinBox *m_sx;
    QDoubleSpinBox *m_sy;
    QDoubleSpinBox *m_sz;
    QLabel *s_txLabel_2;
    QLabel *s_tyLabel_2;
    QLabel *s_tzLabel_2;
    QComboBox *m_vboSelection;
    QPushButton *m_reset;
    QCheckBox *m_wireframe;
    QCheckBox *m_normals;
    QSlider *m_normalSize;
    QFrame *frame;
    QPushButton *m_colour;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1321, 798);
        MainWindow->setBaseSize(QSize(0, 0));
        m_centralwidget = new QWidget(MainWindow);
        m_centralwidget->setObjectName(QStringLiteral("m_centralwidget"));
        s_mainGridLayout = new QGridLayout(m_centralwidget);
        s_mainGridLayout->setObjectName(QStringLiteral("s_mainGridLayout"));
        s_matrixGB = new QGroupBox(m_centralwidget);
        s_matrixGB->setObjectName(QStringLiteral("s_matrixGB"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(s_matrixGB->sizePolicy().hasHeightForWidth());
        s_matrixGB->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(s_matrixGB);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        m_m30 = new QDoubleSpinBox(s_matrixGB);
        m_m30->setObjectName(QStringLiteral("m_m30"));
        m_m30->setDecimals(3);
        m_m30->setMinimum(-20);
        m_m30->setMaximum(20);
        m_m30->setSingleStep(0.01);
        m_m30->setValue(0);

        gridLayout->addWidget(m_m30, 3, 3, 1, 1);

        m_m00 = new QDoubleSpinBox(s_matrixGB);
        m_m00->setObjectName(QStringLiteral("m_m00"));
        m_m00->setDecimals(3);
        m_m00->setMinimum(-20);
        m_m00->setMaximum(20);
        m_m00->setSingleStep(0.01);
        m_m00->setValue(1);

        gridLayout->addWidget(m_m00, 0, 3, 1, 1);

        m_m10 = new QDoubleSpinBox(s_matrixGB);
        m_m10->setObjectName(QStringLiteral("m_m10"));
        m_m10->setDecimals(3);
        m_m10->setMinimum(-20);
        m_m10->setMaximum(20);
        m_m10->setSingleStep(0.01);
        m_m10->setValue(0);

        gridLayout->addWidget(m_m10, 1, 3, 1, 1);

        m_m20 = new QDoubleSpinBox(s_matrixGB);
        m_m20->setObjectName(QStringLiteral("m_m20"));
        m_m20->setDecimals(3);
        m_m20->setMinimum(-20);
        m_m20->setMaximum(20);
        m_m20->setSingleStep(0.01);
        m_m20->setValue(0);

        gridLayout->addWidget(m_m20, 2, 3, 1, 1);

        m_m03 = new QDoubleSpinBox(s_matrixGB);
        m_m03->setObjectName(QStringLiteral("m_m03"));
        m_m03->setDecimals(3);
        m_m03->setMinimum(-20);
        m_m03->setMaximum(20);
        m_m03->setSingleStep(0.01);
        m_m03->setValue(0);

        gridLayout->addWidget(m_m03, 0, 7, 1, 1);

        m_m02 = new QDoubleSpinBox(s_matrixGB);
        m_m02->setObjectName(QStringLiteral("m_m02"));
        m_m02->setDecimals(3);
        m_m02->setMinimum(-20);
        m_m02->setMaximum(20);
        m_m02->setSingleStep(0.01);
        m_m02->setValue(0);

        gridLayout->addWidget(m_m02, 0, 6, 1, 1);

        m_m01 = new QDoubleSpinBox(s_matrixGB);
        m_m01->setObjectName(QStringLiteral("m_m01"));
        m_m01->setDecimals(3);
        m_m01->setMinimum(-20);
        m_m01->setMaximum(20);
        m_m01->setSingleStep(0.01);
        m_m01->setValue(0);

        gridLayout->addWidget(m_m01, 0, 4, 1, 1);

        m_m11 = new QDoubleSpinBox(s_matrixGB);
        m_m11->setObjectName(QStringLiteral("m_m11"));
        m_m11->setDecimals(3);
        m_m11->setMinimum(-20);
        m_m11->setMaximum(20);
        m_m11->setSingleStep(0.01);
        m_m11->setValue(1);

        gridLayout->addWidget(m_m11, 1, 4, 1, 1);

        m_m21 = new QDoubleSpinBox(s_matrixGB);
        m_m21->setObjectName(QStringLiteral("m_m21"));
        m_m21->setDecimals(3);
        m_m21->setMinimum(-20);
        m_m21->setMaximum(20);
        m_m21->setSingleStep(0.01);
        m_m21->setValue(0);

        gridLayout->addWidget(m_m21, 2, 4, 1, 1);

        m_m31 = new QDoubleSpinBox(s_matrixGB);
        m_m31->setObjectName(QStringLiteral("m_m31"));
        m_m31->setDecimals(3);
        m_m31->setMinimum(-20);
        m_m31->setMaximum(20);
        m_m31->setSingleStep(0.01);
        m_m31->setValue(0);

        gridLayout->addWidget(m_m31, 3, 4, 1, 1);

        m_m12 = new QDoubleSpinBox(s_matrixGB);
        m_m12->setObjectName(QStringLiteral("m_m12"));
        m_m12->setDecimals(3);
        m_m12->setMinimum(-20);
        m_m12->setMaximum(20);
        m_m12->setSingleStep(0.01);
        m_m12->setValue(0);

        gridLayout->addWidget(m_m12, 1, 6, 1, 1);

        m_m13 = new QDoubleSpinBox(s_matrixGB);
        m_m13->setObjectName(QStringLiteral("m_m13"));
        m_m13->setDecimals(3);
        m_m13->setMinimum(-20);
        m_m13->setMaximum(20);
        m_m13->setSingleStep(0.01);
        m_m13->setValue(0);

        gridLayout->addWidget(m_m13, 1, 7, 1, 1);

        m_m22 = new QDoubleSpinBox(s_matrixGB);
        m_m22->setObjectName(QStringLiteral("m_m22"));
        m_m22->setDecimals(3);
        m_m22->setMinimum(-20);
        m_m22->setMaximum(20);
        m_m22->setSingleStep(0.01);
        m_m22->setValue(1);

        gridLayout->addWidget(m_m22, 2, 6, 1, 1);

        m_m23 = new QDoubleSpinBox(s_matrixGB);
        m_m23->setObjectName(QStringLiteral("m_m23"));
        m_m23->setDecimals(3);
        m_m23->setMinimum(-20);
        m_m23->setMaximum(20);
        m_m23->setSingleStep(0.01);
        m_m23->setValue(0);

        gridLayout->addWidget(m_m23, 2, 7, 1, 1);

        m_m32 = new QDoubleSpinBox(s_matrixGB);
        m_m32->setObjectName(QStringLiteral("m_m32"));
        m_m32->setDecimals(3);
        m_m32->setMinimum(-20);
        m_m32->setMaximum(20);
        m_m32->setSingleStep(0.01);
        m_m32->setValue(0);

        gridLayout->addWidget(m_m32, 3, 6, 1, 1);

        m_m33 = new QDoubleSpinBox(s_matrixGB);
        m_m33->setObjectName(QStringLiteral("m_m33"));
        m_m33->setDecimals(3);
        m_m33->setMinimum(-20);
        m_m33->setMaximum(20);
        m_m33->setSingleStep(0.01);
        m_m33->setValue(1);

        gridLayout->addWidget(m_m33, 3, 7, 1, 1);


        s_mainGridLayout->addWidget(s_matrixGB, 0, 6, 1, 2);

        widget = new QWidget(m_centralwidget);
        widget->setObjectName(QStringLiteral("widget"));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        formLayout_2 = new QFormLayout(widget);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        s_FormLayoutMatrixOrder = new QFormLayout();
        s_FormLayoutMatrixOrder->setObjectName(QStringLiteral("s_FormLayoutMatrixOrder"));
        m_matrixOrder = new QComboBox(widget);
        m_matrixOrder->setObjectName(QStringLiteral("m_matrixOrder"));

        s_FormLayoutMatrixOrder->setWidget(0, QFormLayout::FieldRole, m_matrixOrder);

        s_matrixOrderLabel = new QLabel(widget);
        s_matrixOrderLabel->setObjectName(QStringLiteral("s_matrixOrderLabel"));

        s_FormLayoutMatrixOrder->setWidget(0, QFormLayout::LabelRole, s_matrixOrderLabel);


        formLayout_2->setLayout(0, QFormLayout::LabelRole, s_FormLayoutMatrixOrder);


        s_mainGridLayout->addWidget(widget, 1, 6, 1, 2);

        s_rotateTabWidget = new QTabWidget(m_centralwidget);
        s_rotateTabWidget->setObjectName(QStringLiteral("s_rotateTabWidget"));
        sizePolicy.setHeightForWidth(s_rotateTabWidget->sizePolicy().hasHeightForWidth());
        s_rotateTabWidget->setSizePolicy(sizePolicy);
        s_rotateTabWidgetPage1 = new QWidget();
        s_rotateTabWidgetPage1->setObjectName(QStringLiteral("s_rotateTabWidgetPage1"));
        gridLayout_2 = new QGridLayout(s_rotateTabWidgetPage1);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        m_rx = new QDoubleSpinBox(s_rotateTabWidgetPage1);
        m_rx->setObjectName(QStringLiteral("m_rx"));
        m_rx->setMinimum(-180);
        m_rx->setMaximum(180);

        gridLayout_2->addWidget(m_rx, 1, 0, 1, 1);

        m_ry = new QDoubleSpinBox(s_rotateTabWidgetPage1);
        m_ry->setObjectName(QStringLiteral("m_ry"));
        m_ry->setMinimum(-180);
        m_ry->setMaximum(180);

        gridLayout_2->addWidget(m_ry, 1, 1, 1, 1);

        m_rz = new QDoubleSpinBox(s_rotateTabWidgetPage1);
        m_rz->setObjectName(QStringLiteral("m_rz"));
        m_rz->setMinimum(-180);
        m_rz->setMaximum(180);

        gridLayout_2->addWidget(m_rz, 1, 2, 1, 1);

        s_rotZLabel = new QLabel(s_rotateTabWidgetPage1);
        s_rotZLabel->setObjectName(QStringLiteral("s_rotZLabel"));
        s_rotZLabel->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(s_rotZLabel, 0, 2, 1, 1);

        s_rotXLabel = new QLabel(s_rotateTabWidgetPage1);
        s_rotXLabel->setObjectName(QStringLiteral("s_rotXLabel"));
        s_rotXLabel->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(s_rotXLabel, 0, 0, 1, 1);

        s_rotYLabel = new QLabel(s_rotateTabWidgetPage1);
        s_rotYLabel->setObjectName(QStringLiteral("s_rotYLabel"));
        s_rotYLabel->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(s_rotYLabel, 0, 1, 1, 1);

        s_rotateTabWidget->addTab(s_rotateTabWidgetPage1, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_5 = new QGridLayout(tab);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        m_eulerAngle = new QDoubleSpinBox(tab);
        m_eulerAngle->setObjectName(QStringLiteral("m_eulerAngle"));
        m_eulerAngle->setMaximum(360);

        gridLayout_5->addWidget(m_eulerAngle, 1, 0, 1, 1);

        m_eulerXAxis = new QDoubleSpinBox(tab);
        m_eulerXAxis->setObjectName(QStringLiteral("m_eulerXAxis"));
        m_eulerXAxis->setMinimum(-1);
        m_eulerXAxis->setMaximum(1);
        m_eulerXAxis->setSingleStep(0.01);
        m_eulerXAxis->setValue(1);

        gridLayout_5->addWidget(m_eulerXAxis, 1, 1, 1, 1);

        m_eulerYAxis = new QDoubleSpinBox(tab);
        m_eulerYAxis->setObjectName(QStringLiteral("m_eulerYAxis"));
        m_eulerYAxis->setMinimum(-1);
        m_eulerYAxis->setMaximum(1);
        m_eulerYAxis->setSingleStep(0.01);

        gridLayout_5->addWidget(m_eulerYAxis, 1, 2, 1, 1);

        m_eulerZAxis = new QDoubleSpinBox(tab);
        m_eulerZAxis->setObjectName(QStringLiteral("m_eulerZAxis"));
        m_eulerZAxis->setMinimum(-1);
        m_eulerZAxis->setMaximum(1);
        m_eulerZAxis->setSingleStep(0.01);

        gridLayout_5->addWidget(m_eulerZAxis, 1, 3, 1, 1);

        s_eulerAxisLabel = new QLabel(tab);
        s_eulerAxisLabel->setObjectName(QStringLiteral("s_eulerAxisLabel"));

        gridLayout_5->addWidget(s_eulerAxisLabel, 0, 1, 1, 2);

        s_eulerAngleLabel = new QLabel(tab);
        s_eulerAngleLabel->setObjectName(QStringLiteral("s_eulerAngleLabel"));

        gridLayout_5->addWidget(s_eulerAngleLabel, 0, 0, 1, 1);

        s_rotateTabWidget->addTab(tab, QString());

        s_mainGridLayout->addWidget(s_rotateTabWidget, 2, 6, 1, 2);

        s_translateGroupBox = new QGroupBox(m_centralwidget);
        s_translateGroupBox->setObjectName(QStringLiteral("s_translateGroupBox"));
        sizePolicy.setHeightForWidth(s_translateGroupBox->sizePolicy().hasHeightForWidth());
        s_translateGroupBox->setSizePolicy(sizePolicy);
        gridLayout_3 = new QGridLayout(s_translateGroupBox);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        m_tx = new QDoubleSpinBox(s_translateGroupBox);
        m_tx->setObjectName(QStringLiteral("m_tx"));
        m_tx->setMinimum(-20);
        m_tx->setMaximum(20);
        m_tx->setSingleStep(0.1);

        gridLayout_3->addWidget(m_tx, 1, 0, 1, 1);

        m_ty = new QDoubleSpinBox(s_translateGroupBox);
        m_ty->setObjectName(QStringLiteral("m_ty"));
        m_ty->setMinimum(-20);
        m_ty->setMaximum(20);
        m_ty->setSingleStep(0.1);

        gridLayout_3->addWidget(m_ty, 1, 1, 1, 1);

        m_tz = new QDoubleSpinBox(s_translateGroupBox);
        m_tz->setObjectName(QStringLiteral("m_tz"));
        m_tz->setMinimum(-20);
        m_tz->setMaximum(20);
        m_tz->setSingleStep(0.1);

        gridLayout_3->addWidget(m_tz, 1, 2, 1, 1);

        s_txLabel = new QLabel(s_translateGroupBox);
        s_txLabel->setObjectName(QStringLiteral("s_txLabel"));
        s_txLabel->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(s_txLabel, 0, 0, 1, 1);

        s_tyLabel = new QLabel(s_translateGroupBox);
        s_tyLabel->setObjectName(QStringLiteral("s_tyLabel"));
        s_tyLabel->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(s_tyLabel, 0, 1, 1, 1);

        s_tzLabel = new QLabel(s_translateGroupBox);
        s_tzLabel->setObjectName(QStringLiteral("s_tzLabel"));
        s_tzLabel->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(s_tzLabel, 0, 2, 1, 1);


        s_mainGridLayout->addWidget(s_translateGroupBox, 3, 6, 1, 2);

        frame_2 = new QFrame(m_centralwidget);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);

        s_mainGridLayout->addWidget(frame_2, 3, 8, 1, 1);

        s_scaleGroupBox = new QGroupBox(m_centralwidget);
        s_scaleGroupBox->setObjectName(QStringLiteral("s_scaleGroupBox"));
        sizePolicy.setHeightForWidth(s_scaleGroupBox->sizePolicy().hasHeightForWidth());
        s_scaleGroupBox->setSizePolicy(sizePolicy);
        gridLayout_4 = new QGridLayout(s_scaleGroupBox);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        m_sx = new QDoubleSpinBox(s_scaleGroupBox);
        m_sx->setObjectName(QStringLiteral("m_sx"));
        m_sx->setMinimum(-20);
        m_sx->setMaximum(20);
        m_sx->setSingleStep(0.1);
        m_sx->setValue(1);

        gridLayout_4->addWidget(m_sx, 1, 0, 1, 1);

        m_sy = new QDoubleSpinBox(s_scaleGroupBox);
        m_sy->setObjectName(QStringLiteral("m_sy"));
        m_sy->setMinimum(-20);
        m_sy->setMaximum(20);
        m_sy->setSingleStep(0.1);
        m_sy->setValue(1);

        gridLayout_4->addWidget(m_sy, 1, 1, 1, 1);

        m_sz = new QDoubleSpinBox(s_scaleGroupBox);
        m_sz->setObjectName(QStringLiteral("m_sz"));
        m_sz->setMinimum(-20);
        m_sz->setMaximum(20);
        m_sz->setSingleStep(0.1);
        m_sz->setValue(1);

        gridLayout_4->addWidget(m_sz, 1, 2, 1, 1);

        s_txLabel_2 = new QLabel(s_scaleGroupBox);
        s_txLabel_2->setObjectName(QStringLiteral("s_txLabel_2"));
        s_txLabel_2->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(s_txLabel_2, 0, 0, 1, 1);

        s_tyLabel_2 = new QLabel(s_scaleGroupBox);
        s_tyLabel_2->setObjectName(QStringLiteral("s_tyLabel_2"));
        s_tyLabel_2->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(s_tyLabel_2, 0, 1, 1, 1);

        s_tzLabel_2 = new QLabel(s_scaleGroupBox);
        s_tzLabel_2->setObjectName(QStringLiteral("s_tzLabel_2"));
        s_tzLabel_2->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(s_tzLabel_2, 0, 2, 1, 1);


        s_mainGridLayout->addWidget(s_scaleGroupBox, 4, 6, 1, 1);

        m_vboSelection = new QComboBox(m_centralwidget);
        m_vboSelection->setObjectName(QStringLiteral("m_vboSelection"));

        s_mainGridLayout->addWidget(m_vboSelection, 7, 0, 1, 1);

        m_reset = new QPushButton(m_centralwidget);
        m_reset->setObjectName(QStringLiteral("m_reset"));

        s_mainGridLayout->addWidget(m_reset, 7, 1, 1, 1);

        m_wireframe = new QCheckBox(m_centralwidget);
        m_wireframe->setObjectName(QStringLiteral("m_wireframe"));

        s_mainGridLayout->addWidget(m_wireframe, 7, 2, 1, 1);

        m_normals = new QCheckBox(m_centralwidget);
        m_normals->setObjectName(QStringLiteral("m_normals"));

        s_mainGridLayout->addWidget(m_normals, 7, 3, 1, 1);

        m_normalSize = new QSlider(m_centralwidget);
        m_normalSize->setObjectName(QStringLiteral("m_normalSize"));
        m_normalSize->setLayoutDirection(Qt::RightToLeft);
        m_normalSize->setMinimum(1);
        m_normalSize->setMaximum(20);
        m_normalSize->setPageStep(1);
        m_normalSize->setValue(6);
        m_normalSize->setSliderPosition(6);
        m_normalSize->setOrientation(Qt::Horizontal);
        m_normalSize->setInvertedAppearance(true);
        m_normalSize->setTickPosition(QSlider::TicksBothSides);

        s_mainGridLayout->addWidget(m_normalSize, 7, 4, 1, 1);

        frame = new QFrame(m_centralwidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        m_colour = new QPushButton(frame);
        m_colour->setObjectName(QStringLiteral("m_colour"));
        m_colour->setGeometry(QRect(291, 0, 85, 32));

        s_mainGridLayout->addWidget(frame, 7, 6, 1, 2);

        MainWindow->setCentralWidget(m_centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1321, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);
        QWidget::setTabOrder(m_vboSelection, m_reset);
        QWidget::setTabOrder(m_reset, m_wireframe);
        QWidget::setTabOrder(m_wireframe, m_normals);
        QWidget::setTabOrder(m_normals, m_m11);
        QWidget::setTabOrder(m_m11, m_m12);
        QWidget::setTabOrder(m_m12, m_m13);
        QWidget::setTabOrder(m_m13, m_m20);
        QWidget::setTabOrder(m_m20, m_m21);
        QWidget::setTabOrder(m_m21, m_m22);
        QWidget::setTabOrder(m_m22, m_m23);
        QWidget::setTabOrder(m_m23, m_m30);
        QWidget::setTabOrder(m_m30, m_m31);
        QWidget::setTabOrder(m_m31, m_m32);
        QWidget::setTabOrder(m_m32, m_m33);
        QWidget::setTabOrder(m_m33, m_m10);
        QWidget::setTabOrder(m_m10, m_m03);
        QWidget::setTabOrder(m_m03, m_m02);
        QWidget::setTabOrder(m_m02, m_m01);
        QWidget::setTabOrder(m_m01, m_m00);

        retranslateUi(MainWindow);

        m_matrixOrder->setCurrentIndex(1);
        m_vboSelection->setCurrentIndex(6);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Affine Transform Demo using ngl::Matrix", 0));
        s_matrixGB->setTitle(QApplication::translate("MainWindow", "Transform Matrix", 0));
        m_matrixOrder->clear();
        m_matrixOrder->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Rotate Translate Scale", 0)
         << QApplication::translate("MainWindow", "Translate Rotate Scale", 0)
         << QApplication::translate("MainWindow", "Gimbal Lock", 0)
         << QApplication::translate("MainWindow", "Euler Translate Scale", 0)
         << QApplication::translate("MainWindow", "Translate Euler Scale", 0)
        );
        s_matrixOrderLabel->setText(QApplication::translate("MainWindow", "Matrix Order", 0));
        s_rotZLabel->setText(QApplication::translate("MainWindow", "z", 0));
        s_rotXLabel->setText(QApplication::translate("MainWindow", "x", 0));
        s_rotYLabel->setText(QApplication::translate("MainWindow", "y", 0));
        s_rotateTabWidget->setTabText(s_rotateTabWidget->indexOf(s_rotateTabWidgetPage1), QApplication::translate("MainWindow", "Rotation", 0));
        s_eulerAxisLabel->setText(QApplication::translate("MainWindow", "Axis", 0));
        s_eulerAngleLabel->setText(QApplication::translate("MainWindow", "Angle", 0));
        s_rotateTabWidget->setTabText(s_rotateTabWidget->indexOf(tab), QApplication::translate("MainWindow", "Euler", 0));
        s_translateGroupBox->setTitle(QApplication::translate("MainWindow", "Translate", 0));
        s_txLabel->setText(QApplication::translate("MainWindow", "x", 0));
        s_tyLabel->setText(QApplication::translate("MainWindow", "y", 0));
        s_tzLabel->setText(QApplication::translate("MainWindow", "z", 0));
        s_scaleGroupBox->setTitle(QApplication::translate("MainWindow", "Scale", 0));
        s_txLabel_2->setText(QApplication::translate("MainWindow", "x", 0));
        s_tyLabel_2->setText(QApplication::translate("MainWindow", "y", 0));
        s_tzLabel_2->setText(QApplication::translate("MainWindow", "z", 0));
        m_vboSelection->clear();
        m_vboSelection->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "sphere", 0)
         << QApplication::translate("MainWindow", "cylinder", 0)
         << QApplication::translate("MainWindow", "cone", 0)
         << QApplication::translate("MainWindow", "disk", 0)
         << QApplication::translate("MainWindow", "plane", 0)
         << QApplication::translate("MainWindow", "torus", 0)
         << QApplication::translate("MainWindow", "teapot", 0)
         << QApplication::translate("MainWindow", "octahedron", 0)
         << QApplication::translate("MainWindow", "dodecahedron", 0)
         << QApplication::translate("MainWindow", "icosahedron", 0)
         << QApplication::translate("MainWindow", "tetrahedron", 0)
         << QApplication::translate("MainWindow", "football", 0)
         << QApplication::translate("MainWindow", "cube", 0)
         << QApplication::translate("MainWindow", "troll", 0)
         << QApplication::translate("MainWindow", "buddah", 0)
         << QApplication::translate("MainWindow", "dragon", 0)
         << QApplication::translate("MainWindow", "bunny", 0)
        );
        m_reset->setText(QApplication::translate("MainWindow", "reset", 0));
        m_wireframe->setText(QApplication::translate("MainWindow", "wireframe", 0));
        m_normals->setText(QApplication::translate("MainWindow", "normals   size", 0));
        m_colour->setText(QApplication::translate("MainWindow", "Colour", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
