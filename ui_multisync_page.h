/********************************************************************************
** Form generated from reading ui file 'multisync_page.ui'
**
** Created: Thu 29. May 14:30:42 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MULTISYNC_PAGE_H
#define UI_MULTISYNC_PAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "extendedlineedit.h"
#include "mtadvancedgroupbox.h"

QT_BEGIN_NAMESPACE

class Ui_MultisyncForm
{
public:
    QGridLayout *gridLayout;
    QStackedWidget *tab_stw;
    QWidget *page;
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout1;
    QSpacerItem *spacerItem;
    QSpacerItem *spacerItem1;
    QLineEdit *tab_name;
    QHBoxLayout *hboxLayout;
    QLabel *label_21;
    QLabel *label_12;
    QSpacerItem *spacerItem2;
    QListWidget *list_multi;
    QGridLayout *gridLayout2;
    QPushButton *add_multi;
    QPushButton *remove_multi;
    QSpacerItem *spacerItem3;
    QHBoxLayout *hboxLayout1;
    QLabel *label_22;
    QLabel *label_13;
    QLineEdit *destination_multi;
    QPushButton *browse_multi;
    ExtendedLineEdit *search_multi;
    QTableWidget *tw_multi;
    QGridLayout *gridLayout3;
    QSpacerItem *spacerItem4;
    QPushButton *sync_multi;
    QPushButton *load_multi;
    QPushButton *save_multi;
    QPushButton *saveas_multi;
    QPushButton *stop_sync_btn;
    MTAdvancedGroupBox *advanced;

    void setupUi(QWidget *MultisyncForm)
    {
    if (MultisyncForm->objectName().isEmpty())
        MultisyncForm->setObjectName(QString::fromUtf8("MultisyncForm"));
    MultisyncForm->resize(492, 452);
    gridLayout = new QGridLayout(MultisyncForm);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    tab_stw = new QStackedWidget(MultisyncForm);
    tab_stw->setObjectName(QString::fromUtf8("tab_stw"));
    page = new QWidget();
    page->setObjectName(QString::fromUtf8("page"));
    vboxLayout = new QVBoxLayout(page);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(4, 4, 4, 4);
    gridLayout1 = new QGridLayout();
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem, 0, 2, 1, 1);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem1, 0, 0, 1, 1);

    tab_name = new QLineEdit(page);
    tab_name->setObjectName(QString::fromUtf8("tab_name"));

    gridLayout1->addWidget(tab_name, 0, 1, 1, 1);


    vboxLayout->addLayout(gridLayout1);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    label_21 = new QLabel(page);
    label_21->setObjectName(QString::fromUtf8("label_21"));
    label_21->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/sources.png")));

    hboxLayout->addWidget(label_21);

    label_12 = new QLabel(page);
    label_12->setObjectName(QString::fromUtf8("label_12"));

    hboxLayout->addWidget(label_12);

    spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem2);


    vboxLayout->addLayout(hboxLayout);

    list_multi = new QListWidget(page);
    list_multi->setObjectName(QString::fromUtf8("list_multi"));

    vboxLayout->addWidget(list_multi);

    gridLayout2 = new QGridLayout();
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    gridLayout2->setHorizontalSpacing(6);
    gridLayout2->setVerticalSpacing(6);
    gridLayout2->setContentsMargins(0, 0, 0, 0);
    add_multi = new QPushButton(page);
    add_multi->setObjectName(QString::fromUtf8("add_multi"));
    const QIcon icon = QIcon(QString::fromUtf8(":/new/prefix1/images/add.png"));
    add_multi->setIcon(icon);

    gridLayout2->addWidget(add_multi, 0, 2, 1, 1);

    remove_multi = new QPushButton(page);
    remove_multi->setObjectName(QString::fromUtf8("remove_multi"));
    const QIcon icon1 = QIcon(QString::fromUtf8(":/new/prefix1/images/remove.png"));
    remove_multi->setIcon(icon1);

    gridLayout2->addWidget(remove_multi, 0, 3, 1, 1);

    spacerItem3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout2->addItem(spacerItem3, 0, 1, 1, 1);


    vboxLayout->addLayout(gridLayout2);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    hboxLayout1->setContentsMargins(0, 0, 0, 0);
    label_22 = new QLabel(page);
    label_22->setObjectName(QString::fromUtf8("label_22"));
    label_22->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/Synkron16.png")));

    hboxLayout1->addWidget(label_22);

    label_13 = new QLabel(page);
    label_13->setObjectName(QString::fromUtf8("label_13"));

    hboxLayout1->addWidget(label_13);

    destination_multi = new QLineEdit(page);
    destination_multi->setObjectName(QString::fromUtf8("destination_multi"));

    hboxLayout1->addWidget(destination_multi);

    browse_multi = new QPushButton(page);
    browse_multi->setObjectName(QString::fromUtf8("browse_multi"));
    const QIcon icon2 = QIcon(QString::fromUtf8(":/new/prefix1/images/folder_16.png"));
    browse_multi->setIcon(icon2);

    hboxLayout1->addWidget(browse_multi);


    vboxLayout->addLayout(hboxLayout1);

    search_multi = new ExtendedLineEdit(page);
    search_multi->setObjectName(QString::fromUtf8("search_multi"));

    vboxLayout->addWidget(search_multi);

    tw_multi = new QTableWidget(page);
    if (tw_multi->columnCount() < 2)
        tw_multi->setColumnCount(2);
    tw_multi->setObjectName(QString::fromUtf8("tw_multi"));
    tw_multi->setColumnCount(2);

    vboxLayout->addWidget(tw_multi);

    gridLayout3 = new QGridLayout();
    gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
    gridLayout3->setHorizontalSpacing(6);
    gridLayout3->setVerticalSpacing(6);
    gridLayout3->setContentsMargins(0, 0, 0, 0);
    spacerItem4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout3->addItem(spacerItem4, 0, 3, 1, 1);

    sync_multi = new QPushButton(page);
    sync_multi->setObjectName(QString::fromUtf8("sync_multi"));
    const QIcon icon3 = QIcon(QString::fromUtf8(":/new/prefix1/images/multisync.png"));
    sync_multi->setIcon(icon3);

    gridLayout3->addWidget(sync_multi, 0, 4, 1, 1);

    load_multi = new QPushButton(page);
    load_multi->setObjectName(QString::fromUtf8("load_multi"));
    const QIcon icon4 = QIcon(QString::fromUtf8(":/new/prefix1/images/open.png"));
    load_multi->setIcon(icon4);

    gridLayout3->addWidget(load_multi, 0, 0, 1, 1);

    save_multi = new QPushButton(page);
    save_multi->setObjectName(QString::fromUtf8("save_multi"));
    const QIcon icon5 = QIcon(QString::fromUtf8(":/new/prefix1/images/save.png"));
    save_multi->setIcon(icon5);

    gridLayout3->addWidget(save_multi, 0, 1, 1, 1);

    saveas_multi = new QPushButton(page);
    saveas_multi->setObjectName(QString::fromUtf8("saveas_multi"));
    const QIcon icon6 = QIcon(QString::fromUtf8(":/new/prefix1/images/save-as.png"));
    saveas_multi->setIcon(icon6);

    gridLayout3->addWidget(saveas_multi, 0, 2, 1, 1);

    stop_sync_btn = new QPushButton(page);
    stop_sync_btn->setObjectName(QString::fromUtf8("stop_sync_btn"));

    gridLayout3->addWidget(stop_sync_btn, 0, 5, 1, 1);


    vboxLayout->addLayout(gridLayout3);

    advanced = new MTAdvancedGroupBox(page);
    advanced->setObjectName(QString::fromUtf8("advanced"));

    vboxLayout->addWidget(advanced);

    tab_stw->addWidget(page);

    gridLayout->addWidget(tab_stw, 0, 0, 1, 1);


    retranslateUi(MultisyncForm);

    QMetaObject::connectSlotsByName(MultisyncForm);
    } // setupUi

    void retranslateUi(QWidget *MultisyncForm)
    {
    MultisyncForm->setWindowTitle(QApplication::translate("MultisyncForm", "Form", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    tab_name->setStatusTip(QApplication::translate("MultisyncForm", "Set multisync name", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    label_12->setText(QApplication::translate("MultisyncForm", "<b>Sources:</b>", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    list_multi->setStatusTip(QApplication::translate("MultisyncForm", "List of multisync sources", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP


#ifndef QT_NO_STATUSTIP
    add_multi->setStatusTip(QApplication::translate("MultisyncForm", "Add source", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    add_multi->setText(QApplication::translate("MultisyncForm", "Add", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    remove_multi->setStatusTip(QApplication::translate("MultisyncForm", "Remove source", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    remove_multi->setText(QApplication::translate("MultisyncForm", "Remove", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("MultisyncForm", "<b>Destination:</b>", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    destination_multi->setStatusTip(QApplication::translate("MultisyncForm", "Set a destination", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP


#ifndef QT_NO_STATUSTIP
    browse_multi->setStatusTip(QApplication::translate("MultisyncForm", "Browse for destination", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    browse_multi->setText(QApplication::translate("MultisyncForm", "Browse", 0, QApplication::UnicodeUTF8));
    search_multi->setText(QApplication::translate("MultisyncForm", "Search", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    tw_multi->setStatusTip(QApplication::translate("MultisyncForm", "Multisync log", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP


#ifndef QT_NO_STATUSTIP
    sync_multi->setStatusTip(QApplication::translate("MultisyncForm", "Start multisync", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    sync_multi->setText(QApplication::translate("MultisyncForm", "Multisync", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    load_multi->setStatusTip(QApplication::translate("MultisyncForm", "Load a multisync", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    load_multi->setText(QApplication::translate("MultisyncForm", "Load", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    save_multi->setStatusTip(QApplication::translate("MultisyncForm", "Save the current multisync", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    save_multi->setText(QApplication::translate("MultisyncForm", "Save", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    saveas_multi->setStatusTip(QApplication::translate("MultisyncForm", "Save the current multisync", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    saveas_multi->setText(QApplication::translate("MultisyncForm", "Save as", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    stop_sync_btn->setStatusTip(QApplication::translate("MultisyncForm", "Stop multisync", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    stop_sync_btn->setText(QApplication::translate("MultisyncForm", "Stop multisync", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(MultisyncForm);
    } // retranslateUi

};

namespace Ui {
    class MultisyncForm: public Ui_MultisyncForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MULTISYNC_PAGE_H
