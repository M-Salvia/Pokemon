/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *InfoLabel;
    QHBoxLayout *horizontalLayout;
    QPushButton *ClearBtn;
    QPushButton *LevelUpBtn;
    QPushButton *AttackBtn;
    QLabel *AInfoLabel;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(348, 407);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setLayoutDirection(Qt::RightToLeft);
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setBold(true);
        label->setFont(font);

        verticalLayout->addWidget(label);

        InfoLabel = new QLabel(centralWidget);
        InfoLabel->setObjectName(QString::fromUtf8("InfoLabel"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial Black"));
        font1.setPointSize(10);
        font1.setBold(false);
        InfoLabel->setFont(font1);
        InfoLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(InfoLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        ClearBtn = new QPushButton(centralWidget);
        ClearBtn->setObjectName(QString::fromUtf8("ClearBtn"));
        ClearBtn->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ClearBtn->sizePolicy().hasHeightForWidth());
        ClearBtn->setSizePolicy(sizePolicy1);
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial Black"));
        font2.setBold(false);
        ClearBtn->setFont(font2);

        horizontalLayout->addWidget(ClearBtn);

        LevelUpBtn = new QPushButton(centralWidget);
        LevelUpBtn->setObjectName(QString::fromUtf8("LevelUpBtn"));
        sizePolicy1.setHeightForWidth(LevelUpBtn->sizePolicy().hasHeightForWidth());
        LevelUpBtn->setSizePolicy(sizePolicy1);
        LevelUpBtn->setFont(font2);

        horizontalLayout->addWidget(LevelUpBtn);

        AttackBtn = new QPushButton(centralWidget);
        AttackBtn->setObjectName(QString::fromUtf8("AttackBtn"));
        sizePolicy1.setHeightForWidth(AttackBtn->sizePolicy().hasHeightForWidth());
        AttackBtn->setSizePolicy(sizePolicy1);
        AttackBtn->setFont(font2);

        horizontalLayout->addWidget(AttackBtn);


        verticalLayout->addLayout(horizontalLayout);

        AInfoLabel = new QLabel(centralWidget);
        AInfoLabel->setObjectName(QString::fromUtf8("AInfoLabel"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Arial Black"));
        font3.setPointSize(12);
        font3.setBold(false);
        AInfoLabel->setFont(font3);
        AInfoLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(AInfoLabel);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Easy Pokenmon designed by Majiashuai", nullptr));
        InfoLabel->setText(QCoreApplication::translate("MainWindow", "Information", nullptr));
        ClearBtn->setText(QCoreApplication::translate("MainWindow", "Change", nullptr));
        LevelUpBtn->setText(QCoreApplication::translate("MainWindow", "LevelUp", nullptr));
        AttackBtn->setText(QCoreApplication::translate("MainWindow", "Attack", nullptr));
        AInfoLabel->setText(QCoreApplication::translate("MainWindow", "AttackInfo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
