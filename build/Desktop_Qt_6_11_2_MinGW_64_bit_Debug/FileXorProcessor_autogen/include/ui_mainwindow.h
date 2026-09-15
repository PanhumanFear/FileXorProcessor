/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox;
    QListWidget *fileListWidget;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLineEdit *fileMaskEdit;
    QLabel *label;
    QWidget *layoutWidget1;
    QFormLayout *formLayout;
    QCheckBox *deleteSourceCheckBox;
    QCheckBox *overwriteCheckBox;
    QCheckBox *timerCheckBox;
    QWidget *layoutWidget2;
    QFormLayout *formLayout_4;
    QLabel *label_4;
    QLineEdit *xorKeyEdit;
    QWidget *layoutWidget3;
    QFormLayout *formLayout_5;
    QLabel *label_5;
    QSpinBox *intervalSpinBox;
    QWidget *layoutWidget4;
    QFormLayout *formLayout_6;
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *resumeButton;
    QPushButton *stopButton;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QPushButton *browseInputButton;
    QLineEdit *inputPathEdit;
    QWidget *widget1;
    QGridLayout *gridLayout_3;
    QLabel *label_3;
    QPushButton *browseOutputButton;
    QLineEdit *outputPathEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(0, 0, 782, 539));
        fileListWidget = new QListWidget(groupBox);
        fileListWidget->setObjectName("fileListWidget");
        fileListWidget->setGeometry(QRect(30, 320, 311, 181));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(30, 270, 135, 46));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        fileMaskEdit = new QLineEdit(layoutWidget);
        fileMaskEdit->setObjectName("fileMaskEdit");

        gridLayout->addWidget(fileMaskEdit, 2, 0, 1, 1);

        label = new QLabel(layoutWidget);
        label->setObjectName("label");

        gridLayout->addWidget(label, 1, 0, 1, 1);

        layoutWidget1 = new QWidget(groupBox);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(400, 20, 131, 74));
        formLayout = new QFormLayout(layoutWidget1);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        deleteSourceCheckBox = new QCheckBox(layoutWidget1);
        deleteSourceCheckBox->setObjectName("deleteSourceCheckBox");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, deleteSourceCheckBox);

        overwriteCheckBox = new QCheckBox(layoutWidget1);
        overwriteCheckBox->setObjectName("overwriteCheckBox");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, overwriteCheckBox);

        timerCheckBox = new QCheckBox(layoutWidget1);
        timerCheckBox->setObjectName("timerCheckBox");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, timerCheckBox);

        layoutWidget2 = new QWidget(groupBox);
        layoutWidget2->setObjectName("layoutWidget2");
        layoutWidget2->setGeometry(QRect(20, 120, 141, 46));
        formLayout_4 = new QFormLayout(layoutWidget2);
        formLayout_4->setObjectName("formLayout_4");
        formLayout_4->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(layoutWidget2);
        label_4->setObjectName("label_4");

        formLayout_4->setWidget(0, QFormLayout::ItemRole::LabelRole, label_4);

        xorKeyEdit = new QLineEdit(layoutWidget2);
        xorKeyEdit->setObjectName("xorKeyEdit");

        formLayout_4->setWidget(1, QFormLayout::ItemRole::LabelRole, xorKeyEdit);

        layoutWidget3 = new QWidget(groupBox);
        layoutWidget3->setObjectName("layoutWidget3");
        layoutWidget3->setGeometry(QRect(20, 180, 90, 46));
        formLayout_5 = new QFormLayout(layoutWidget3);
        formLayout_5->setObjectName("formLayout_5");
        formLayout_5->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(layoutWidget3);
        label_5->setObjectName("label_5");

        formLayout_5->setWidget(0, QFormLayout::ItemRole::LabelRole, label_5);

        intervalSpinBox = new QSpinBox(layoutWidget3);
        intervalSpinBox->setObjectName("intervalSpinBox");
        intervalSpinBox->setMaximum(3600000);
        intervalSpinBox->setValue(5000);

        formLayout_5->setWidget(1, QFormLayout::ItemRole::LabelRole, intervalSpinBox);

        layoutWidget4 = new QWidget(groupBox);
        layoutWidget4->setObjectName("layoutWidget4");
        layoutWidget4->setGeometry(QRect(330, 150, 381, 131));
        formLayout_6 = new QFormLayout(layoutWidget4);
        formLayout_6->setObjectName("formLayout_6");
        formLayout_6->setContentsMargins(0, 0, 0, 0);
        startButton = new QPushButton(layoutWidget4);
        startButton->setObjectName("startButton");

        formLayout_6->setWidget(0, QFormLayout::ItemRole::SpanningRole, startButton);

        pauseButton = new QPushButton(layoutWidget4);
        pauseButton->setObjectName("pauseButton");

        formLayout_6->setWidget(1, QFormLayout::ItemRole::SpanningRole, pauseButton);

        resumeButton = new QPushButton(layoutWidget4);
        resumeButton->setObjectName("resumeButton");

        formLayout_6->setWidget(2, QFormLayout::ItemRole::SpanningRole, resumeButton);

        stopButton = new QPushButton(layoutWidget4);
        stopButton->setObjectName("stopButton");

        formLayout_6->setWidget(3, QFormLayout::ItemRole::SpanningRole, stopButton);

        widget = new QWidget(groupBox);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(21, 23, 135, 76));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        browseInputButton = new QPushButton(widget);
        browseInputButton->setObjectName("browseInputButton");

        gridLayout_2->addWidget(browseInputButton, 1, 0, 1, 1);

        inputPathEdit = new QLineEdit(widget);
        inputPathEdit->setObjectName("inputPathEdit");

        gridLayout_2->addWidget(inputPathEdit, 2, 0, 1, 1);

        widget1 = new QWidget(groupBox);
        widget1->setObjectName("widget1");
        widget1->setGeometry(QRect(210, 25, 135, 71));
        gridLayout_3 = new QGridLayout(widget1);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(widget1);
        label_3->setObjectName("label_3");

        gridLayout_3->addWidget(label_3, 0, 0, 1, 1);

        browseOutputButton = new QPushButton(widget1);
        browseOutputButton->setObjectName("browseOutputButton");

        gridLayout_3->addWidget(browseOutputButton, 1, 0, 1, 1);

        outputPathEdit = new QLineEdit(widget1);
        outputPathEdit->setObjectName("outputPathEdit");

        gridLayout_3->addWidget(outputPathEdit, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
        fileMaskEdit->setText(QCoreApplication::translate("MainWindow", ".txt, testFile.bin", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\234\320\260\321\201\320\272\320\260 \321\204\320\260\320\271\320\273\320\276\320\262:", nullptr));
        deleteSourceCheckBox->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\321\217\321\202\321\214 \320\270\321\201\321\205\320\276\320\264\320\275\321\213\320\265", nullptr));
        overwriteCheckBox->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\200\320\265\320\267\320\260\320\277\320\270\321\201\321\213\320\262\320\260\321\202\321\214", nullptr));
        timerCheckBox->setText(QCoreApplication::translate("MainWindow", "\320\222\320\272\320\273\321\216\321\207\320\270\321\202\321\214 \321\202\320\260\320\271\320\274\320\265\321\200", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "XOR \320\272\320\273\321\216\321\207 (hex):", nullptr));
        xorKeyEdit->setText(QCoreApplication::translate("MainWindow", "123456789ABCDEF", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\320\230\320\275\321\202\320\265\321\200\320\262\320\260\320\273 (\320\274\321\201):", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\320\260\321\200\321\202", nullptr));
        pauseButton->setText(QCoreApplication::translate("MainWindow", "\320\237\320\260\321\203\320\267\320\260", nullptr));
        resumeButton->setText(QCoreApplication::translate("MainWindow", "\320\222\320\276\320\267\320\276\320\261\320\275\320\276\320\262\320\270\321\202\321\214", nullptr));
        stopButton->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\320\276\320\277", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\237\321\203\321\202\321\214 \320\277\320\276\320\270\321\201\320\272\320\260:", nullptr));
        browseInputButton->setText(QCoreApplication::translate("MainWindow", "\320\236\320\261\320\267\320\276\321\200...", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\320\237\321\203\321\202\321\214 \321\201\320\276\321\205\321\200\320\260\320\275\320\265\320\275\320\270\321\217", nullptr));
        browseOutputButton->setText(QCoreApplication::translate("MainWindow", "\320\236\320\261\320\267\320\276\321\200...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
