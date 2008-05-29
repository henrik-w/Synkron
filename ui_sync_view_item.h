/********************************************************************************
** Form generated from reading ui file 'sync_view_item.ui'
**
** Created: Thu 29. May 14:30:42 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SYNC_VIEW_ITEM_H
#define UI_SYNC_VIEW_ITEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SyncViewItem
{
public:
    QHBoxLayout *hboxLayout;
    QLabel *sync_name_lbl;
    QLabel *sync_info_lbl;
    QSpacerItem *spacerItem;
    QToolButton *start_btn;
    QToolButton *stop_btn;
    QToolButton *edit_btn;

    void setupUi(QWidget *SyncViewItem)
    {
    if (SyncViewItem->objectName().isEmpty())
        SyncViewItem->setObjectName(QString::fromUtf8("SyncViewItem"));
    SyncViewItem->resize(622, 91);
    hboxLayout = new QHBoxLayout(SyncViewItem);
    hboxLayout->setSpacing(6);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(6, 6, 6, 6);
    sync_name_lbl = new QLabel(SyncViewItem);
    sync_name_lbl->setObjectName(QString::fromUtf8("sync_name_lbl"));

    hboxLayout->addWidget(sync_name_lbl);

    sync_info_lbl = new QLabel(SyncViewItem);
    sync_info_lbl->setObjectName(QString::fromUtf8("sync_info_lbl"));

    hboxLayout->addWidget(sync_info_lbl);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    start_btn = new QToolButton(SyncViewItem);
    start_btn->setObjectName(QString::fromUtf8("start_btn"));

    hboxLayout->addWidget(start_btn);

    stop_btn = new QToolButton(SyncViewItem);
    stop_btn->setObjectName(QString::fromUtf8("stop_btn"));

    hboxLayout->addWidget(stop_btn);

    edit_btn = new QToolButton(SyncViewItem);
    edit_btn->setObjectName(QString::fromUtf8("edit_btn"));

    hboxLayout->addWidget(edit_btn);


    retranslateUi(SyncViewItem);

    QMetaObject::connectSlotsByName(SyncViewItem);
    } // setupUi

    void retranslateUi(QWidget *SyncViewItem)
    {
    SyncViewItem->setWindowTitle(QApplication::translate("SyncViewItem", "SyncViewItem", 0, QApplication::UnicodeUTF8));
    sync_name_lbl->setText(QApplication::translate("SyncViewItem", "Sync name", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    start_btn->setStatusTip(QApplication::translate("SyncViewItem", "Start synchronisation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    start_btn->setText(QApplication::translate("SyncViewItem", "Start", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    stop_btn->setStatusTip(QApplication::translate("SyncViewItem", "Stop synchronisation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    stop_btn->setText(QApplication::translate("SyncViewItem", "Stop", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    edit_btn->setStatusTip(QApplication::translate("SyncViewItem", "Edit synchronisation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    edit_btn->setText(QApplication::translate("SyncViewItem", "Edit", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(SyncViewItem);
    } // retranslateUi

};

namespace Ui {
    class SyncViewItem: public Ui_SyncViewItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYNC_VIEW_ITEM_H
