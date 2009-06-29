/********************************************************************************
** Form generated from reading ui file 'multisync_page.ui'
**
** Created: Wed Jun 24 04:08:11 2009
**      by: Qt User Interface Compiler version 4.5.1
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
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QStackedWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "extendedlineedit.h"
#include "extendedtreewidget.h"

QT_BEGIN_NAMESPACE

class Ui_MultisyncForm
{
public:
    QGridLayout *gridLayout;
    QStackedWidget *tab_stw;
    QWidget *page;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout1;
    QSpacerItem *spacerItem;
    QSpacerItem *spacerItem1;
    QLineEdit *tab_name;
    QSplitter *splitter;
    QWidget *widget;
    QVBoxLayout *verticalLayout_6;
    QGridLayout *gridLayout_2;
    QLabel *label_21;
    QLabel *label_12;
    QListWidget *list_multi;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QPushButton *add_multi;
    QPushButton *remove_multi;
    QPushButton *vars_multi;
    QSpacerItem *verticalSpacer;
    QLabel *label_22;
    QLabel *label_13;
    QLineEdit *destination_multi;
    QPushButton *browse_multi;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_4;
    ExtendedLineEdit *search_multi;
    QStackedWidget *logs_stw;
    QWidget *page_2;
    QVBoxLayout *verticalLayout;
    QTableWidget *tw_multi;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_2;
    ExtendedTreeWidget *analyse_tree;
    QGridLayout *gridLayout2;
    QSpacerItem *spacerItem2;
    QPushButton *sync_multi;
    QPushButton *stop_sync_btn;
    QPushButton *go_to_analyse;
    QPushButton *show_advanced;
    QVBoxLayout *advanced_layout;

    void setupUi(QWidget *MultisyncForm)
    {
        if (MultisyncForm->objectName().isEmpty())
            MultisyncForm->setObjectName(QString::fromUtf8("MultisyncForm"));
        MultisyncForm->resize(662, 483);
        gridLayout = new QGridLayout(MultisyncForm);
        gridLayout->setMargin(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tab_stw = new QStackedWidget(MultisyncForm);
        tab_stw->setObjectName(QString::fromUtf8("tab_stw"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout_5 = new QVBoxLayout(page);
        verticalLayout_5->setSpacing(5);
        verticalLayout_5->setMargin(4);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        gridLayout1 = new QGridLayout();
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout1->addItem(spacerItem, 0, 2, 1, 1);

        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout1->addItem(spacerItem1, 0, 0, 1, 1);

        tab_name = new QLineEdit(page);
        tab_name->setObjectName(QString::fromUtf8("tab_name"));

        gridLayout1->addWidget(tab_name, 0, 1, 1, 1);


        verticalLayout_5->addLayout(gridLayout1);

        splitter = new QSplitter(page);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        widget = new QWidget(splitter);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMaximumSize(QSize(16777215, 250));
        verticalLayout_6 = new QVBoxLayout(widget);
        verticalLayout_6->setMargin(0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(5);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_21 = new QLabel(widget);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/sources.png")));
        label_21->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_2->addWidget(label_21, 0, 0, 1, 1);

        label_12 = new QLabel(widget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_2->addWidget(label_12, 0, 1, 1, 1);

        list_multi = new QListWidget(widget);
        list_multi->setObjectName(QString::fromUtf8("list_multi"));

        gridLayout_2->addWidget(list_multi, 0, 2, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        add_multi = new QPushButton(widget);
        add_multi->setObjectName(QString::fromUtf8("add_multi"));
        QIcon icon;
        icon.addPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/add.png")), QIcon::Normal, QIcon::Off);
        add_multi->setIcon(icon);

        verticalLayout_3->addWidget(add_multi);

        remove_multi = new QPushButton(widget);
        remove_multi->setObjectName(QString::fromUtf8("remove_multi"));
        QIcon icon1;
        icon1.addPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/remove.png")), QIcon::Normal, QIcon::Off);
        remove_multi->setIcon(icon1);

        verticalLayout_3->addWidget(remove_multi);

        vars_multi = new QPushButton(widget);
        vars_multi->setObjectName(QString::fromUtf8("vars_multi"));

        verticalLayout_3->addWidget(vars_multi);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_3);


        gridLayout_2->addLayout(horizontalLayout, 0, 3, 1, 1);

        label_22 = new QLabel(widget);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/Synkron16.png")));

        gridLayout_2->addWidget(label_22, 1, 0, 1, 1);

        label_13 = new QLabel(widget);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_2->addWidget(label_13, 1, 1, 1, 1);

        destination_multi = new QLineEdit(widget);
        destination_multi->setObjectName(QString::fromUtf8("destination_multi"));

        gridLayout_2->addWidget(destination_multi, 1, 2, 1, 1);

        browse_multi = new QPushButton(widget);
        browse_multi->setObjectName(QString::fromUtf8("browse_multi"));
        QIcon icon2;
        icon2.addPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/folder_16.png")), QIcon::Normal, QIcon::Off);
        browse_multi->setIcon(icon2);

        gridLayout_2->addWidget(browse_multi, 1, 3, 1, 1);


        verticalLayout_6->addLayout(gridLayout_2);

        splitter->addWidget(widget);
        verticalLayoutWidget = new QWidget(splitter);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayout_4 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_4->setSpacing(5);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        search_multi = new ExtendedLineEdit(verticalLayoutWidget);
        search_multi->setObjectName(QString::fromUtf8("search_multi"));

        verticalLayout_4->addWidget(search_multi);

        logs_stw = new QStackedWidget(verticalLayoutWidget);
        logs_stw->setObjectName(QString::fromUtf8("logs_stw"));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        verticalLayout = new QVBoxLayout(page_2);
        verticalLayout->setMargin(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tw_multi = new QTableWidget(page_2);
        if (tw_multi->columnCount() < 2)
            tw_multi->setColumnCount(2);
        tw_multi->setObjectName(QString::fromUtf8("tw_multi"));
        tw_multi->setColumnCount(2);

        verticalLayout->addWidget(tw_multi);

        logs_stw->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        verticalLayout_2 = new QVBoxLayout(page_3);
        verticalLayout_2->setMargin(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        analyse_tree = new ExtendedTreeWidget(page_3);
        analyse_tree->setObjectName(QString::fromUtf8("analyse_tree"));

        verticalLayout_2->addWidget(analyse_tree);

        logs_stw->addWidget(page_3);

        verticalLayout_4->addWidget(logs_stw);

        splitter->addWidget(verticalLayoutWidget);
        logs_stw->raise();
        search_multi->raise();

        verticalLayout_5->addWidget(splitter);

        gridLayout2 = new QGridLayout();
        gridLayout2->setSpacing(5);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout2->addItem(spacerItem2, 0, 1, 1, 1);

        sync_multi = new QPushButton(page);
        sync_multi->setObjectName(QString::fromUtf8("sync_multi"));
        QIcon icon3;
        icon3.addPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/multisync.png")), QIcon::Normal, QIcon::Off);
        sync_multi->setIcon(icon3);

        gridLayout2->addWidget(sync_multi, 0, 3, 1, 1);

        stop_sync_btn = new QPushButton(page);
        stop_sync_btn->setObjectName(QString::fromUtf8("stop_sync_btn"));
        QIcon icon4;
        icon4.addPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/stop_sync22.png")), QIcon::Normal, QIcon::Off);
        stop_sync_btn->setIcon(icon4);

        gridLayout2->addWidget(stop_sync_btn, 0, 4, 1, 1);

        go_to_analyse = new QPushButton(page);
        go_to_analyse->setObjectName(QString::fromUtf8("go_to_analyse"));
        QIcon icon5;
        icon5.addPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/analyse_16.png")), QIcon::Normal, QIcon::Off);
        go_to_analyse->setIcon(icon5);

        gridLayout2->addWidget(go_to_analyse, 0, 2, 1, 1);

        show_advanced = new QPushButton(page);
        show_advanced->setObjectName(QString::fromUtf8("show_advanced"));
        QIcon icon6;
        icon6.addPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/configure16.png")), QIcon::Normal, QIcon::Off);
        show_advanced->setIcon(icon6);

        gridLayout2->addWidget(show_advanced, 0, 0, 1, 1);


        verticalLayout_5->addLayout(gridLayout2);

        advanced_layout = new QVBoxLayout();
        advanced_layout->setObjectName(QString::fromUtf8("advanced_layout"));

        verticalLayout_5->addLayout(advanced_layout);

        tab_stw->addWidget(page);

        gridLayout->addWidget(tab_stw, 0, 0, 1, 1);


        retranslateUi(MultisyncForm);

        logs_stw->setCurrentIndex(0);


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
#ifndef QT_NO_STATUSTIP
        vars_multi->setStatusTip(QApplication::translate("MultisyncForm", "Modify source path variables", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        vars_multi->setText(QApplication::translate("MultisyncForm", "Variables", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("MultisyncForm", "<b>Destination:</b>", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        destination_multi->setStatusTip(QApplication::translate("MultisyncForm", "Set a destination", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        browse_multi->setStatusTip(QApplication::translate("MultisyncForm", "Browse for destination", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        browse_multi->setText(QApplication::translate("MultisyncForm", "Browse", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        search_multi->setStatusTip(QApplication::translate("MultisyncForm", "Search sync log", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        search_multi->setText(QApplication::translate("MultisyncForm", "Search", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        tw_multi->setStatusTip(QApplication::translate("MultisyncForm", "Multisync log", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        QTreeWidgetItem *___qtreewidgetitem = analyse_tree->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("MultisyncForm", "1", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        sync_multi->setStatusTip(QApplication::translate("MultisyncForm", "Start multisync", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        sync_multi->setText(QApplication::translate("MultisyncForm", "Multisync", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        stop_sync_btn->setStatusTip(QApplication::translate("MultisyncForm", "Stop multisync", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        stop_sync_btn->setText(QApplication::translate("MultisyncForm", "Stop multisync", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        go_to_analyse->setStatusTip(QApplication::translate("MultisyncForm", "Analyse", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        go_to_analyse->setText(QApplication::translate("MultisyncForm", "Analyse", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        show_advanced->setStatusTip(QApplication::translate("MultisyncForm", "Show advanced options of configuration", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        show_advanced->setText(QApplication::translate("MultisyncForm", "Advanced", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MultisyncForm);
    } // retranslateUi

};

namespace Ui {
    class MultisyncForm: public Ui_MultisyncForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MULTISYNC_PAGE_H
