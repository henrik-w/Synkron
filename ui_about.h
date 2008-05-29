/********************************************************************************
** Form generated from reading ui file 'about.ui'
**
** Created: Thu 29. May 14:30:42 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *label_6;
    QTabWidget *tabWidget;
    QWidget *about_synkron;
    QVBoxLayout *vboxLayout1;
    QTextBrowser *about_synkron_tb;
    QWidget *about_qt;
    QVBoxLayout *vboxLayout2;
    QTextBrowser *about_qt_tb;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *closeButton;

    void setupUi(QDialog *About)
    {
    if (About->objectName().isEmpty())
        About->setObjectName(QString::fromUtf8("About"));
    About->resize(354, 439);
    QPalette palette;
    QBrush brush(QColor(255, 255, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Base, brush);
    QBrush brush1(QColor(177, 195, 217, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Window, brush1);
    palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
    About->setPalette(palette);
    const QIcon icon = QIcon(QString::fromUtf8(":/new/prefix1/images/Synkron128.png"));
    About->setWindowIcon(icon);
    vboxLayout = new QVBoxLayout(About);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(2, 2, 2, 2);
    label_6 = new QLabel(About);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/Synkron-about.png")));
    label_6->setAlignment(Qt::AlignCenter);

    vboxLayout->addWidget(label_6);

    tabWidget = new QTabWidget(About);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    about_synkron = new QWidget();
    about_synkron->setObjectName(QString::fromUtf8("about_synkron"));
    vboxLayout1 = new QVBoxLayout(about_synkron);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    vboxLayout1->setContentsMargins(2, 2, 2, 2);
    about_synkron_tb = new QTextBrowser(about_synkron);
    about_synkron_tb->setObjectName(QString::fromUtf8("about_synkron_tb"));

    vboxLayout1->addWidget(about_synkron_tb);

    tabWidget->addTab(about_synkron, QString());
    about_qt = new QWidget();
    about_qt->setObjectName(QString::fromUtf8("about_qt"));
    vboxLayout2 = new QVBoxLayout(about_qt);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    vboxLayout2->setContentsMargins(2, 2, 2, 2);
    about_qt_tb = new QTextBrowser(about_qt);
    about_qt_tb->setObjectName(QString::fromUtf8("about_qt_tb"));

    vboxLayout2->addWidget(about_qt_tb);

    tabWidget->addTab(about_qt, QString());

    vboxLayout->addWidget(tabWidget);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    closeButton = new QPushButton(About);
    closeButton->setObjectName(QString::fromUtf8("closeButton"));

    hboxLayout->addWidget(closeButton);


    vboxLayout->addLayout(hboxLayout);


    retranslateUi(About);
    QObject::connect(closeButton, SIGNAL(released()), About, SLOT(close()));

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QDialog *About)
    {
    About->setWindowTitle(QApplication::translate("About", "About Synkron", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(about_synkron), QApplication::translate("About", "About Synkron", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(about_qt), QApplication::translate("About", "About Qt", 0, QApplication::UnicodeUTF8));
    closeButton->setText(QApplication::translate("About", "Close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(About);
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
