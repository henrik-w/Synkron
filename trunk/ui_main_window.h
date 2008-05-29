/********************************************************************************
** Form generated from reading ui file 'main_window.ui'
**
** Created: Thu 29. May 14:30:42 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QStackedWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTimeEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "extendedlineedit.h"
#include "extendedlistwidget.h"
#include "mtadvancedgroupbox.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionAbout;
    QAction *actionFolder_1;
    QAction *actionFolder_2;
    QAction *actionNew_sync;
    QAction *actionClose_sync;
    QAction *actionSynchronise;
    QAction *actionRestore;
    QAction *actionBlacklist;
    QAction *actionRun_hidden;
    QAction *actionSync_all;
    QAction *actionCheck_for_updates;
    QAction *actionMultisync;
    QAction *actionScheduler;
    QAction *actionChange_language;
    QAction *actionFilters;
    QAction *actionDisable_tray_messages;
    QAction *actionSave_log;
    QAction *actionSyncView;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QStackedWidget *mainStackedWidget;
    QWidget *page;
    QGridLayout *gridLayout1;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *page_2;
    QGridLayout *gridLayout2;
    QGridLayout *gridLayout3;
    QLabel *label;
    QLabel *label_5;
    QSpacerItem *spacerItem;
    QGridLayout *gridLayout4;
    ExtendedListWidget *restore_list;
    QGridLayout *gridLayout5;
    QPushButton *selTmpAllBtn;
    QSpacerItem *spacerItem1;
    ExtendedLineEdit *restore_search;
    QGridLayout *gridLayout6;
    QLineEdit *path_of_syncfile;
    QCheckBox *to_black_list;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_2;
    QDateTimeEdit *date_of_sync;
    QSpacerItem *spacerItem2;
    QGridLayout *gridLayout7;
    QPushButton *restore_files;
    QSpacerItem *spacerItem3;
    MTAdvancedGroupBox *restore_clean_gb;
    MTAdvancedGroupBox *restore_select_gb;
    QWidget *page_3;
    QGridLayout *gridLayout8;
    QGridLayout *gridLayout9;
    QLabel *label_7;
    QSpacerItem *spacerItem4;
    QLabel *label_8;
    QListWidget *blacklist_fileslist;
    QGridLayout *gridLayout10;
    QSpacerItem *spacerItem5;
    QPushButton *blacklist_removefile;
    QPushButton *blacklist_addfile;
    QFrame *line;
    QGridLayout *gridLayout11;
    QSpacerItem *spacerItem6;
    QLabel *label_6;
    QLabel *label_9;
    QListWidget *blacklist_folderslist;
    QGridLayout *gridLayout12;
    QSpacerItem *spacerItem7;
    QPushButton *blacklist_removefolder;
    QPushButton *blacklist_addfolder;
    QListWidget *blacklist_extslist;
    QFrame *line_2;
    QLabel *label_20;
    QHBoxLayout *hboxLayout;
    QPushButton *blacklist_addext;
    QPushButton *blacklist_removeext;
    QSpacerItem *spacerItem8;
    QWidget *page_4;
    QVBoxLayout *vboxLayout;
    QTabWidget *multi_tabWidget;
    QWidget *tab_2;
    QWidget *page_5;
    QGridLayout *gridLayout13;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *vboxLayout1;
    QGridLayout *gridLayout14;
    QLabel *label_13;
    QLabel *label_12;
    QSpacerItem *spacerItem9;
    QTableWidget *tw_schedules;
    QGridLayout *gridLayout15;
    QPushButton *add_schedule;
    QPushButton *remove_schedule;
    QPushButton *startall_schedules;
    QSpacerItem *spacerItem10;
    QPushButton *stopall_schedules;
    QGroupBox *schedGroupBox;
    QGridLayout *gridLayout16;
    QGridLayout *gridLayout17;
    QLineEdit *sched_name;
    QSpacerItem *spacerItem11;
    QSpacerItem *spacerItem12;
    QHBoxLayout *hboxLayout1;
    QVBoxLayout *vboxLayout2;
    QGridLayout *gridLayout18;
    QListWidget *sched_multitab_lw;
    QListWidget *sched_tab_lw;
    QLabel *label_10;
    QLabel *label_11;
    QGridLayout *gridLayout19;
    QPushButton *sched_stop;
    QPushButton *sched_start;
    QSpacerItem *spacerItem13;
    QTabWidget *timing_tabWidget;
    QWidget *sync_times;
    QVBoxLayout *vboxLayout3;
    QLabel *label_15;
    QListWidget *sched_time_lw;
    QGridLayout *gridLayout20;
    QSpacerItem *spacerItem14;
    QPushButton *sched_add_time;
    QTimeEdit *sched_time_edit;
    QPushButton *sched_remove_time;
    MTAdvancedGroupBox *sched_dates_agb;
    QWidget *periodically;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacerItem15;
    QLabel *label_19;
    QSpinBox *sched_interval_spin;
    QLabel *label_14;
    QSpacerItem *spacerItem16;
    QWidget *page_6;
    QGridLayout *gridLayout21;
    QHBoxLayout *hboxLayout3;
    QLabel *label_18;
    QLabel *label_16;
    QSpacerItem *spacerItem17;
    QListWidget *filter_list;
    QLabel *label_17;
    QListWidget *filter_extensions_list;
    QSpacerItem *spacerItem18;
    QPushButton *filter_add_extension;
    QPushButton *filter_remove_extension;
    QPushButton *filter_add;
    QPushButton *filter_remove;
    QSpacerItem *spacerItem19;
    QWidget *page_7;
    QVBoxLayout *vboxLayout4;
    QVBoxLayout *vboxLayout5;
    QGridLayout *gridLayout22;
    QLabel *label_21;
    QSpacerItem *spacerItem20;
    QLabel *label_24;
    QTableWidget *syncs_syncview;
    QVBoxLayout *vboxLayout6;
    QHBoxLayout *hboxLayout4;
    QLabel *label_23;
    QLabel *label_22;
    QSpacerItem *spacerItem21;
    QTableWidget *multisyncs_syncview;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuView;
    QMenu *menuOptions;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(663, 580);
    MainWindow->setMinimumSize(QSize(0, 580));
    const QIcon icon = QIcon(QString::fromUtf8(":/new/prefix1/images/Synkron128.png"));
    MainWindow->setWindowIcon(icon);
    actionQuit = new QAction(MainWindow);
    actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
    actionAbout = new QAction(MainWindow);
    actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
    actionAbout->setIcon(icon);
    actionFolder_1 = new QAction(MainWindow);
    actionFolder_1->setObjectName(QString::fromUtf8("actionFolder_1"));
    const QIcon icon1 = QIcon(QString::fromUtf8(":/new/prefix1/images/folder.png"));
    actionFolder_1->setIcon(icon1);
    actionFolder_2 = new QAction(MainWindow);
    actionFolder_2->setObjectName(QString::fromUtf8("actionFolder_2"));
    actionFolder_2->setIcon(icon1);
    actionNew_sync = new QAction(MainWindow);
    actionNew_sync->setObjectName(QString::fromUtf8("actionNew_sync"));
    const QIcon icon2 = QIcon(QString::fromUtf8(":/new/prefix1/images/tab-new_32.png"));
    actionNew_sync->setIcon(icon2);
    actionClose_sync = new QAction(MainWindow);
    actionClose_sync->setObjectName(QString::fromUtf8("actionClose_sync"));
    const QIcon icon3 = QIcon(QString::fromUtf8(":/new/prefix1/images/tab-remove_32.png"));
    actionClose_sync->setIcon(icon3);
    actionSynchronise = new QAction(MainWindow);
    actionSynchronise->setObjectName(QString::fromUtf8("actionSynchronise"));
    actionSynchronise->setCheckable(true);
    actionSynchronise->setChecked(true);
    const QIcon icon4 = QIcon(QString::fromUtf8(":/new/prefix1/images/Synchronise.png"));
    actionSynchronise->setIcon(icon4);
    actionRestore = new QAction(MainWindow);
    actionRestore->setObjectName(QString::fromUtf8("actionRestore"));
    actionRestore->setCheckable(true);
    const QIcon icon5 = QIcon(QString::fromUtf8(":/new/prefix1/images/Restore.png"));
    actionRestore->setIcon(icon5);
    actionBlacklist = new QAction(MainWindow);
    actionBlacklist->setObjectName(QString::fromUtf8("actionBlacklist"));
    actionBlacklist->setCheckable(true);
    const QIcon icon6 = QIcon(QString::fromUtf8(":/new/prefix1/images/blacklist.png"));
    actionBlacklist->setIcon(icon6);
    actionRun_hidden = new QAction(MainWindow);
    actionRun_hidden->setObjectName(QString::fromUtf8("actionRun_hidden"));
    actionRun_hidden->setCheckable(true);
    actionSync_all = new QAction(MainWindow);
    actionSync_all->setObjectName(QString::fromUtf8("actionSync_all"));
    const QIcon icon7 = QIcon(QString::fromUtf8(":/new/prefix1/images/sync_all.png"));
    actionSync_all->setIcon(icon7);
    actionCheck_for_updates = new QAction(MainWindow);
    actionCheck_for_updates->setObjectName(QString::fromUtf8("actionCheck_for_updates"));
    actionMultisync = new QAction(MainWindow);
    actionMultisync->setObjectName(QString::fromUtf8("actionMultisync"));
    actionMultisync->setCheckable(true);
    const QIcon icon8 = QIcon(QString::fromUtf8(":/new/prefix1/images/multisync.png"));
    actionMultisync->setIcon(icon8);
    actionScheduler = new QAction(MainWindow);
    actionScheduler->setObjectName(QString::fromUtf8("actionScheduler"));
    actionScheduler->setCheckable(true);
    const QIcon icon9 = QIcon(QString::fromUtf8(":/new/prefix1/images/scheduler64.png"));
    actionScheduler->setIcon(icon9);
    actionChange_language = new QAction(MainWindow);
    actionChange_language->setObjectName(QString::fromUtf8("actionChange_language"));
    actionFilters = new QAction(MainWindow);
    actionFilters->setObjectName(QString::fromUtf8("actionFilters"));
    actionFilters->setCheckable(true);
    const QIcon icon10 = QIcon(QString::fromUtf8(":/new/prefix1/images/filter32.png"));
    actionFilters->setIcon(icon10);
    actionDisable_tray_messages = new QAction(MainWindow);
    actionDisable_tray_messages->setObjectName(QString::fromUtf8("actionDisable_tray_messages"));
    actionDisable_tray_messages->setCheckable(true);
    actionSave_log = new QAction(MainWindow);
    actionSave_log->setObjectName(QString::fromUtf8("actionSave_log"));
    const QIcon icon11 = QIcon(QString::fromUtf8(":/new/prefix1/images/save.png"));
    actionSave_log->setIcon(icon11);
    actionSyncView = new QAction(MainWindow);
    actionSyncView->setObjectName(QString::fromUtf8("actionSyncView"));
    actionSyncView->setCheckable(true);
    const QIcon icon12 = QIcon(QString::fromUtf8(":/new/prefix1/images/syncview_48.png"));
    actionSyncView->setIcon(icon12);
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(2, 2, 2, 2);
    mainStackedWidget = new QStackedWidget(centralwidget);
    mainStackedWidget->setObjectName(QString::fromUtf8("mainStackedWidget"));
    page = new QWidget();
    page->setObjectName(QString::fromUtf8("page"));
    gridLayout1 = new QGridLayout(page);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout1->setContentsMargins(0, 0, 0, 0);
    tabWidget = new QTabWidget(page);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    tabWidget->addTab(tab, icon, QString());

    gridLayout1->addWidget(tabWidget, 0, 0, 1, 1);

    mainStackedWidget->addWidget(page);
    page_2 = new QWidget();
    page_2->setObjectName(QString::fromUtf8("page_2"));
    gridLayout2 = new QGridLayout(page_2);
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    gridLayout2->setContentsMargins(0, 0, 0, 0);
    gridLayout3 = new QGridLayout();
    gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
    gridLayout3->setHorizontalSpacing(6);
    gridLayout3->setVerticalSpacing(6);
    gridLayout3->setContentsMargins(0, 0, 0, 0);
    label = new QLabel(page_2);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout3->addWidget(label, 0, 1, 1, 1);

    label_5 = new QLabel(page_2);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/Restore16.png")));

    gridLayout3->addWidget(label_5, 0, 0, 1, 1);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout3->addItem(spacerItem, 0, 2, 1, 1);


    gridLayout2->addLayout(gridLayout3, 0, 0, 1, 2);

    gridLayout4 = new QGridLayout();
    gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
    gridLayout4->setHorizontalSpacing(6);
    gridLayout4->setVerticalSpacing(6);
    gridLayout4->setContentsMargins(0, 0, 0, 0);
    restore_list = new ExtendedListWidget(page_2);
    restore_list->setObjectName(QString::fromUtf8("restore_list"));

    gridLayout4->addWidget(restore_list, 1, 0, 1, 1);

    gridLayout5 = new QGridLayout();
    gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
    gridLayout5->setHorizontalSpacing(6);
    gridLayout5->setVerticalSpacing(6);
    gridLayout5->setContentsMargins(0, 0, 0, 0);
    selTmpAllBtn = new QPushButton(page_2);
    selTmpAllBtn->setObjectName(QString::fromUtf8("selTmpAllBtn"));

    gridLayout5->addWidget(selTmpAllBtn, 0, 0, 1, 1);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout5->addItem(spacerItem1, 0, 1, 1, 1);


    gridLayout4->addLayout(gridLayout5, 2, 0, 1, 1);

    restore_search = new ExtendedLineEdit(page_2);
    restore_search->setObjectName(QString::fromUtf8("restore_search"));

    gridLayout4->addWidget(restore_search, 0, 0, 1, 1);


    gridLayout2->addLayout(gridLayout4, 1, 0, 1, 1);

    gridLayout6 = new QGridLayout();
    gridLayout6->setObjectName(QString::fromUtf8("gridLayout6"));
    gridLayout6->setHorizontalSpacing(6);
    gridLayout6->setVerticalSpacing(6);
    gridLayout6->setContentsMargins(0, 0, 0, 0);
    path_of_syncfile = new QLineEdit(page_2);
    path_of_syncfile->setObjectName(QString::fromUtf8("path_of_syncfile"));
    path_of_syncfile->setAcceptDrops(false);
    path_of_syncfile->setReadOnly(true);

    gridLayout6->addWidget(path_of_syncfile, 5, 0, 1, 1);

    to_black_list = new QCheckBox(page_2);
    to_black_list->setObjectName(QString::fromUtf8("to_black_list"));

    gridLayout6->addWidget(to_black_list, 6, 0, 1, 1);

    label_3 = new QLabel(page_2);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout6->addWidget(label_3, 2, 0, 1, 1);

    label_4 = new QLabel(page_2);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout6->addWidget(label_4, 4, 0, 1, 1);

    label_2 = new QLabel(page_2);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout6->addWidget(label_2, 1, 0, 1, 1);

    date_of_sync = new QDateTimeEdit(page_2);
    date_of_sync->setObjectName(QString::fromUtf8("date_of_sync"));
    date_of_sync->setReadOnly(true);

    gridLayout6->addWidget(date_of_sync, 3, 0, 1, 1);

    spacerItem2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout6->addItem(spacerItem2, 7, 0, 1, 1);

    gridLayout7 = new QGridLayout();
    gridLayout7->setObjectName(QString::fromUtf8("gridLayout7"));
    gridLayout7->setHorizontalSpacing(6);
    gridLayout7->setVerticalSpacing(6);
    gridLayout7->setContentsMargins(0, 0, 0, 0);
    restore_files = new QPushButton(page_2);
    restore_files->setObjectName(QString::fromUtf8("restore_files"));
    restore_files->setIcon(icon5);

    gridLayout7->addWidget(restore_files, 0, 1, 1, 1);

    spacerItem3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout7->addItem(spacerItem3, 0, 0, 1, 1);


    gridLayout6->addLayout(gridLayout7, 10, 0, 1, 1);

    restore_clean_gb = new MTAdvancedGroupBox(page_2);
    restore_clean_gb->setObjectName(QString::fromUtf8("restore_clean_gb"));

    gridLayout6->addWidget(restore_clean_gb, 9, 0, 1, 1);

    restore_select_gb = new MTAdvancedGroupBox(page_2);
    restore_select_gb->setObjectName(QString::fromUtf8("restore_select_gb"));

    gridLayout6->addWidget(restore_select_gb, 8, 0, 1, 1);


    gridLayout2->addLayout(gridLayout6, 1, 1, 1, 1);

    mainStackedWidget->addWidget(page_2);
    page_3 = new QWidget();
    page_3->setObjectName(QString::fromUtf8("page_3"));
    gridLayout8 = new QGridLayout(page_3);
    gridLayout8->setObjectName(QString::fromUtf8("gridLayout8"));
    gridLayout8->setContentsMargins(0, 0, 0, 0);
    gridLayout9 = new QGridLayout();
    gridLayout9->setObjectName(QString::fromUtf8("gridLayout9"));
    gridLayout9->setHorizontalSpacing(6);
    gridLayout9->setVerticalSpacing(6);
    gridLayout9->setContentsMargins(0, 0, 0, 0);
    label_7 = new QLabel(page_3);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout9->addWidget(label_7, 0, 1, 1, 1);

    spacerItem4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout9->addItem(spacerItem4, 0, 2, 1, 1);

    label_8 = new QLabel(page_3);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    label_8->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/file.png")));

    gridLayout9->addWidget(label_8, 0, 0, 1, 1);


    gridLayout8->addLayout(gridLayout9, 0, 0, 1, 1);

    blacklist_fileslist = new QListWidget(page_3);
    blacklist_fileslist->setObjectName(QString::fromUtf8("blacklist_fileslist"));

    gridLayout8->addWidget(blacklist_fileslist, 1, 0, 1, 1);

    gridLayout10 = new QGridLayout();
    gridLayout10->setObjectName(QString::fromUtf8("gridLayout10"));
    gridLayout10->setHorizontalSpacing(6);
    gridLayout10->setVerticalSpacing(6);
    gridLayout10->setContentsMargins(0, 0, 0, 0);
    spacerItem5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout10->addItem(spacerItem5, 0, 2, 1, 1);

    blacklist_removefile = new QPushButton(page_3);
    blacklist_removefile->setObjectName(QString::fromUtf8("blacklist_removefile"));
    const QIcon icon13 = QIcon(QString::fromUtf8(":/new/prefix1/images/remove.png"));
    blacklist_removefile->setIcon(icon13);

    gridLayout10->addWidget(blacklist_removefile, 0, 1, 1, 1);

    blacklist_addfile = new QPushButton(page_3);
    blacklist_addfile->setObjectName(QString::fromUtf8("blacklist_addfile"));
    const QIcon icon14 = QIcon(QString::fromUtf8(":/new/prefix1/images/add.png"));
    blacklist_addfile->setIcon(icon14);

    gridLayout10->addWidget(blacklist_addfile, 0, 0, 1, 1);


    gridLayout8->addLayout(gridLayout10, 2, 0, 1, 1);

    line = new QFrame(page_3);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    gridLayout8->addWidget(line, 3, 0, 1, 1);

    gridLayout11 = new QGridLayout();
    gridLayout11->setObjectName(QString::fromUtf8("gridLayout11"));
    gridLayout11->setHorizontalSpacing(6);
    gridLayout11->setVerticalSpacing(6);
    gridLayout11->setContentsMargins(0, 0, 0, 0);
    spacerItem6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout11->addItem(spacerItem6, 0, 2, 1, 1);

    label_6 = new QLabel(page_3);
    label_6->setObjectName(QString::fromUtf8("label_6"));

    gridLayout11->addWidget(label_6, 0, 1, 1, 1);

    label_9 = new QLabel(page_3);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/folder_16.png")));

    gridLayout11->addWidget(label_9, 0, 0, 1, 1);


    gridLayout8->addLayout(gridLayout11, 4, 0, 1, 1);

    blacklist_folderslist = new QListWidget(page_3);
    blacklist_folderslist->setObjectName(QString::fromUtf8("blacklist_folderslist"));

    gridLayout8->addWidget(blacklist_folderslist, 5, 0, 1, 1);

    gridLayout12 = new QGridLayout();
    gridLayout12->setObjectName(QString::fromUtf8("gridLayout12"));
    gridLayout12->setHorizontalSpacing(6);
    gridLayout12->setVerticalSpacing(6);
    gridLayout12->setContentsMargins(0, 0, 0, 0);
    spacerItem7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout12->addItem(spacerItem7, 0, 2, 1, 1);

    blacklist_removefolder = new QPushButton(page_3);
    blacklist_removefolder->setObjectName(QString::fromUtf8("blacklist_removefolder"));
    blacklist_removefolder->setIcon(icon13);

    gridLayout12->addWidget(blacklist_removefolder, 0, 1, 1, 1);

    blacklist_addfolder = new QPushButton(page_3);
    blacklist_addfolder->setObjectName(QString::fromUtf8("blacklist_addfolder"));
    blacklist_addfolder->setIcon(icon14);

    gridLayout12->addWidget(blacklist_addfolder, 0, 0, 1, 1);


    gridLayout8->addLayout(gridLayout12, 6, 0, 1, 1);

    blacklist_extslist = new QListWidget(page_3);
    blacklist_extslist->setObjectName(QString::fromUtf8("blacklist_extslist"));

    gridLayout8->addWidget(blacklist_extslist, 9, 0, 1, 1);

    line_2 = new QFrame(page_3);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);

    gridLayout8->addWidget(line_2, 7, 0, 1, 1);

    label_20 = new QLabel(page_3);
    label_20->setObjectName(QString::fromUtf8("label_20"));

    gridLayout8->addWidget(label_20, 8, 0, 1, 1);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    blacklist_addext = new QPushButton(page_3);
    blacklist_addext->setObjectName(QString::fromUtf8("blacklist_addext"));
    blacklist_addext->setIcon(icon14);

    hboxLayout->addWidget(blacklist_addext);

    blacklist_removeext = new QPushButton(page_3);
    blacklist_removeext->setObjectName(QString::fromUtf8("blacklist_removeext"));
    blacklist_removeext->setIcon(icon13);

    hboxLayout->addWidget(blacklist_removeext);

    spacerItem8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem8);


    gridLayout8->addLayout(hboxLayout, 10, 0, 1, 1);

    mainStackedWidget->addWidget(page_3);
    page_4 = new QWidget();
    page_4->setObjectName(QString::fromUtf8("page_4"));
    vboxLayout = new QVBoxLayout(page_4);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    multi_tabWidget = new QTabWidget(page_4);
    multi_tabWidget->setObjectName(QString::fromUtf8("multi_tabWidget"));
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    multi_tabWidget->addTab(tab_2, icon8, QString());

    vboxLayout->addWidget(multi_tabWidget);

    mainStackedWidget->addWidget(page_4);
    page_5 = new QWidget();
    page_5->setObjectName(QString::fromUtf8("page_5"));
    gridLayout13 = new QGridLayout(page_5);
    gridLayout13->setObjectName(QString::fromUtf8("gridLayout13"));
    gridLayout13->setContentsMargins(0, 0, 0, 0);
    splitter = new QSplitter(page_5);
    splitter->setObjectName(QString::fromUtf8("splitter"));
    splitter->setOrientation(Qt::Vertical);
    layoutWidget = new QWidget(splitter);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    vboxLayout1 = new QVBoxLayout(layoutWidget);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    vboxLayout1->setContentsMargins(0, 0, 0, 0);
    gridLayout14 = new QGridLayout();
    gridLayout14->setObjectName(QString::fromUtf8("gridLayout14"));
    gridLayout14->setHorizontalSpacing(6);
    gridLayout14->setVerticalSpacing(6);
    gridLayout14->setContentsMargins(0, 0, 0, 0);
    label_13 = new QLabel(layoutWidget);
    label_13->setObjectName(QString::fromUtf8("label_13"));

    gridLayout14->addWidget(label_13, 0, 1, 1, 1);

    label_12 = new QLabel(layoutWidget);
    label_12->setObjectName(QString::fromUtf8("label_12"));
    label_12->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/scheduler16.png")));

    gridLayout14->addWidget(label_12, 0, 0, 1, 1);

    spacerItem9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout14->addItem(spacerItem9, 0, 2, 1, 1);


    vboxLayout1->addLayout(gridLayout14);

    tw_schedules = new QTableWidget(layoutWidget);
    if (tw_schedules->columnCount() < 2)
        tw_schedules->setColumnCount(2);
    tw_schedules->setObjectName(QString::fromUtf8("tw_schedules"));
    tw_schedules->setEditTriggers(QAbstractItemView::EditKeyPressed);
    tw_schedules->setShowGrid(false);
    tw_schedules->setColumnCount(2);

    vboxLayout1->addWidget(tw_schedules);

    gridLayout15 = new QGridLayout();
    gridLayout15->setObjectName(QString::fromUtf8("gridLayout15"));
    gridLayout15->setHorizontalSpacing(6);
    gridLayout15->setVerticalSpacing(6);
    gridLayout15->setContentsMargins(0, 0, 0, 0);
    add_schedule = new QPushButton(layoutWidget);
    add_schedule->setObjectName(QString::fromUtf8("add_schedule"));
    add_schedule->setIcon(icon14);

    gridLayout15->addWidget(add_schedule, 0, 3, 1, 1);

    remove_schedule = new QPushButton(layoutWidget);
    remove_schedule->setObjectName(QString::fromUtf8("remove_schedule"));
    remove_schedule->setIcon(icon13);

    gridLayout15->addWidget(remove_schedule, 0, 4, 1, 1);

    startall_schedules = new QPushButton(layoutWidget);
    startall_schedules->setObjectName(QString::fromUtf8("startall_schedules"));

    gridLayout15->addWidget(startall_schedules, 0, 0, 1, 1);

    spacerItem10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout15->addItem(spacerItem10, 0, 2, 1, 1);

    stopall_schedules = new QPushButton(layoutWidget);
    stopall_schedules->setObjectName(QString::fromUtf8("stopall_schedules"));

    gridLayout15->addWidget(stopall_schedules, 0, 1, 1, 1);


    vboxLayout1->addLayout(gridLayout15);

    splitter->addWidget(layoutWidget);
    schedGroupBox = new QGroupBox(splitter);
    schedGroupBox->setObjectName(QString::fromUtf8("schedGroupBox"));
    schedGroupBox->setEnabled(false);
    gridLayout16 = new QGridLayout(schedGroupBox);
    gridLayout16->setObjectName(QString::fromUtf8("gridLayout16"));
    gridLayout16->setContentsMargins(-1, 6, -1, -1);
    gridLayout17 = new QGridLayout();
    gridLayout17->setObjectName(QString::fromUtf8("gridLayout17"));
    gridLayout17->setHorizontalSpacing(6);
    gridLayout17->setVerticalSpacing(6);
    gridLayout17->setContentsMargins(0, 0, 0, 0);
    sched_name = new QLineEdit(schedGroupBox);
    sched_name->setObjectName(QString::fromUtf8("sched_name"));

    gridLayout17->addWidget(sched_name, 0, 1, 1, 1);

    spacerItem11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout17->addItem(spacerItem11, 0, 2, 1, 1);

    spacerItem12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout17->addItem(spacerItem12, 0, 0, 1, 1);


    gridLayout16->addLayout(gridLayout17, 0, 0, 1, 1);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    hboxLayout1->setContentsMargins(0, 0, 0, 0);
    vboxLayout2 = new QVBoxLayout();
    vboxLayout2->setSpacing(6);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    vboxLayout2->setContentsMargins(0, 0, 0, 0);
    gridLayout18 = new QGridLayout();
    gridLayout18->setObjectName(QString::fromUtf8("gridLayout18"));
    sched_multitab_lw = new QListWidget(schedGroupBox);
    sched_multitab_lw->setObjectName(QString::fromUtf8("sched_multitab_lw"));

    gridLayout18->addWidget(sched_multitab_lw, 1, 1, 1, 1);

    sched_tab_lw = new QListWidget(schedGroupBox);
    sched_tab_lw->setObjectName(QString::fromUtf8("sched_tab_lw"));

    gridLayout18->addWidget(sched_tab_lw, 1, 0, 1, 1);

    label_10 = new QLabel(schedGroupBox);
    label_10->setObjectName(QString::fromUtf8("label_10"));

    gridLayout18->addWidget(label_10, 0, 0, 1, 1);

    label_11 = new QLabel(schedGroupBox);
    label_11->setObjectName(QString::fromUtf8("label_11"));

    gridLayout18->addWidget(label_11, 0, 1, 1, 1);


    vboxLayout2->addLayout(gridLayout18);

    gridLayout19 = new QGridLayout();
    gridLayout19->setObjectName(QString::fromUtf8("gridLayout19"));
    gridLayout19->setHorizontalSpacing(6);
    gridLayout19->setVerticalSpacing(6);
    gridLayout19->setContentsMargins(0, 0, 0, 0);
    sched_stop = new QPushButton(schedGroupBox);
    sched_stop->setObjectName(QString::fromUtf8("sched_stop"));

    gridLayout19->addWidget(sched_stop, 0, 2, 1, 1);

    sched_start = new QPushButton(schedGroupBox);
    sched_start->setObjectName(QString::fromUtf8("sched_start"));

    gridLayout19->addWidget(sched_start, 0, 1, 1, 1);

    spacerItem13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout19->addItem(spacerItem13, 0, 3, 1, 1);


    vboxLayout2->addLayout(gridLayout19);


    hboxLayout1->addLayout(vboxLayout2);

    timing_tabWidget = new QTabWidget(schedGroupBox);
    timing_tabWidget->setObjectName(QString::fromUtf8("timing_tabWidget"));
    sync_times = new QWidget();
    sync_times->setObjectName(QString::fromUtf8("sync_times"));
    vboxLayout3 = new QVBoxLayout(sync_times);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    label_15 = new QLabel(sync_times);
    label_15->setObjectName(QString::fromUtf8("label_15"));

    vboxLayout3->addWidget(label_15);

    sched_time_lw = new QListWidget(sync_times);
    sched_time_lw->setObjectName(QString::fromUtf8("sched_time_lw"));

    vboxLayout3->addWidget(sched_time_lw);

    gridLayout20 = new QGridLayout();
    gridLayout20->setObjectName(QString::fromUtf8("gridLayout20"));
    gridLayout20->setHorizontalSpacing(6);
    gridLayout20->setVerticalSpacing(6);
    gridLayout20->setContentsMargins(0, 0, 0, 0);
    spacerItem14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout20->addItem(spacerItem14, 0, 2, 1, 1);

    sched_add_time = new QPushButton(sync_times);
    sched_add_time->setObjectName(QString::fromUtf8("sched_add_time"));
    sched_add_time->setIcon(icon14);

    gridLayout20->addWidget(sched_add_time, 0, 1, 1, 1);

    sched_time_edit = new QTimeEdit(sync_times);
    sched_time_edit->setObjectName(QString::fromUtf8("sched_time_edit"));

    gridLayout20->addWidget(sched_time_edit, 0, 0, 1, 1);

    sched_remove_time = new QPushButton(sync_times);
    sched_remove_time->setObjectName(QString::fromUtf8("sched_remove_time"));
    sched_remove_time->setIcon(icon13);

    gridLayout20->addWidget(sched_remove_time, 0, 3, 1, 1);


    vboxLayout3->addLayout(gridLayout20);

    sched_dates_agb = new MTAdvancedGroupBox(sync_times);
    sched_dates_agb->setObjectName(QString::fromUtf8("sched_dates_agb"));

    vboxLayout3->addWidget(sched_dates_agb);

    timing_tabWidget->addTab(sync_times, QString());
    periodically = new QWidget();
    periodically->setObjectName(QString::fromUtf8("periodically"));
    hboxLayout2 = new QHBoxLayout(periodically);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    spacerItem15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem15);

    label_19 = new QLabel(periodically);
    label_19->setObjectName(QString::fromUtf8("label_19"));

    hboxLayout2->addWidget(label_19);

    sched_interval_spin = new QSpinBox(periodically);
    sched_interval_spin->setObjectName(QString::fromUtf8("sched_interval_spin"));
    sched_interval_spin->setMinimum(1);
    sched_interval_spin->setMaximum(9999999);
    sched_interval_spin->setValue(5);

    hboxLayout2->addWidget(sched_interval_spin);

    label_14 = new QLabel(periodically);
    label_14->setObjectName(QString::fromUtf8("label_14"));

    hboxLayout2->addWidget(label_14);

    spacerItem16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem16);

    timing_tabWidget->addTab(periodically, QString());

    hboxLayout1->addWidget(timing_tabWidget);


    gridLayout16->addLayout(hboxLayout1, 1, 0, 1, 1);

    splitter->addWidget(schedGroupBox);

    gridLayout13->addWidget(splitter, 0, 0, 1, 1);

    mainStackedWidget->addWidget(page_5);
    page_6 = new QWidget();
    page_6->setObjectName(QString::fromUtf8("page_6"));
    gridLayout21 = new QGridLayout(page_6);
    gridLayout21->setObjectName(QString::fromUtf8("gridLayout21"));
    gridLayout21->setContentsMargins(2, 2, 2, 2);
    hboxLayout3 = new QHBoxLayout();
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    label_18 = new QLabel(page_6);
    label_18->setObjectName(QString::fromUtf8("label_18"));
    label_18->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/filter16.png")));

    hboxLayout3->addWidget(label_18);

    label_16 = new QLabel(page_6);
    label_16->setObjectName(QString::fromUtf8("label_16"));

    hboxLayout3->addWidget(label_16);

    spacerItem17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout3->addItem(spacerItem17);


    gridLayout21->addLayout(hboxLayout3, 0, 0, 1, 3);

    filter_list = new QListWidget(page_6);
    filter_list->setObjectName(QString::fromUtf8("filter_list"));

    gridLayout21->addWidget(filter_list, 1, 0, 3, 3);

    label_17 = new QLabel(page_6);
    label_17->setObjectName(QString::fromUtf8("label_17"));

    gridLayout21->addWidget(label_17, 1, 3, 1, 3);

    filter_extensions_list = new QListWidget(page_6);
    filter_extensions_list->setObjectName(QString::fromUtf8("filter_extensions_list"));

    gridLayout21->addWidget(filter_extensions_list, 2, 3, 1, 3);

    spacerItem18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout21->addItem(spacerItem18, 3, 3, 1, 1);

    filter_add_extension = new QPushButton(page_6);
    filter_add_extension->setObjectName(QString::fromUtf8("filter_add_extension"));
    filter_add_extension->setEnabled(false);
    filter_add_extension->setIcon(icon14);

    gridLayout21->addWidget(filter_add_extension, 3, 4, 1, 1);

    filter_remove_extension = new QPushButton(page_6);
    filter_remove_extension->setObjectName(QString::fromUtf8("filter_remove_extension"));
    filter_remove_extension->setEnabled(false);
    filter_remove_extension->setIcon(icon13);

    gridLayout21->addWidget(filter_remove_extension, 3, 5, 1, 1);

    filter_add = new QPushButton(page_6);
    filter_add->setObjectName(QString::fromUtf8("filter_add"));
    filter_add->setEnabled(true);
    filter_add->setIcon(icon14);

    gridLayout21->addWidget(filter_add, 4, 0, 1, 1);

    filter_remove = new QPushButton(page_6);
    filter_remove->setObjectName(QString::fromUtf8("filter_remove"));
    filter_remove->setEnabled(false);
    filter_remove->setIcon(icon13);

    gridLayout21->addWidget(filter_remove, 4, 1, 1, 1);

    spacerItem19 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout21->addItem(spacerItem19, 4, 2, 1, 1);

    mainStackedWidget->addWidget(page_6);
    page_7 = new QWidget();
    page_7->setObjectName(QString::fromUtf8("page_7"));
    vboxLayout4 = new QVBoxLayout(page_7);
    vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
    vboxLayout4->setContentsMargins(0, 0, 0, 0);
    vboxLayout5 = new QVBoxLayout();
    vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
    gridLayout22 = new QGridLayout();
    gridLayout22->setObjectName(QString::fromUtf8("gridLayout22"));
    label_21 = new QLabel(page_7);
    label_21->setObjectName(QString::fromUtf8("label_21"));

    gridLayout22->addWidget(label_21, 0, 1, 1, 1);

    spacerItem20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout22->addItem(spacerItem20, 0, 2, 1, 1);

    label_24 = new QLabel(page_7);
    label_24->setObjectName(QString::fromUtf8("label_24"));
    label_24->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/Synkron16.png")));

    gridLayout22->addWidget(label_24, 0, 0, 1, 1);


    vboxLayout5->addLayout(gridLayout22);

    syncs_syncview = new QTableWidget(page_7);
    if (syncs_syncview->columnCount() < 1)
        syncs_syncview->setColumnCount(1);
    syncs_syncview->setObjectName(QString::fromUtf8("syncs_syncview"));
    syncs_syncview->setColumnCount(1);

    vboxLayout5->addWidget(syncs_syncview);


    vboxLayout4->addLayout(vboxLayout5);

    vboxLayout6 = new QVBoxLayout();
    vboxLayout6->setObjectName(QString::fromUtf8("vboxLayout6"));
    hboxLayout4 = new QHBoxLayout();
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    label_23 = new QLabel(page_7);
    label_23->setObjectName(QString::fromUtf8("label_23"));
    label_23->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/multisync_16.png")));

    hboxLayout4->addWidget(label_23);

    label_22 = new QLabel(page_7);
    label_22->setObjectName(QString::fromUtf8("label_22"));

    hboxLayout4->addWidget(label_22);

    spacerItem21 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout4->addItem(spacerItem21);


    vboxLayout6->addLayout(hboxLayout4);

    multisyncs_syncview = new QTableWidget(page_7);
    if (multisyncs_syncview->columnCount() < 1)
        multisyncs_syncview->setColumnCount(1);
    multisyncs_syncview->setObjectName(QString::fromUtf8("multisyncs_syncview"));
    multisyncs_syncview->setColumnCount(1);

    vboxLayout6->addWidget(multisyncs_syncview);


    vboxLayout4->addLayout(vboxLayout6);

    mainStackedWidget->addWidget(page_7);

    gridLayout->addWidget(mainStackedWidget, 0, 0, 1, 1);

    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 663, 21));
    menuFile = new QMenu(menubar);
    menuFile->setObjectName(QString::fromUtf8("menuFile"));
    menuHelp = new QMenu(menubar);
    menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
    menuView = new QMenu(menubar);
    menuView->setObjectName(QString::fromUtf8("menuView"));
    menuOptions = new QMenu(menubar);
    menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindow->setStatusBar(statusbar);
    toolBar = new QToolBar(MainWindow);
    toolBar->setObjectName(QString::fromUtf8("toolBar"));
    toolBar->setMovable(false);
    toolBar->setOrientation(Qt::Horizontal);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

    menubar->addAction(menuFile->menuAction());
    menubar->addAction(menuView->menuAction());
    menubar->addAction(menuOptions->menuAction());
    menubar->addAction(menuHelp->menuAction());
    menuFile->addAction(actionNew_sync);
    menuFile->addAction(actionClose_sync);
    menuFile->addAction(actionSave_log);
    menuFile->addSeparator();
    menuFile->addAction(actionQuit);
    menuHelp->addAction(actionCheck_for_updates);
    menuHelp->addAction(actionAbout);
    menuView->addAction(actionSynchronise);
    menuView->addAction(actionMultisync);
    menuView->addAction(actionScheduler);
    menuView->addAction(actionRestore);
    menuView->addAction(actionBlacklist);
    menuView->addAction(actionFilters);
    menuOptions->addAction(actionRun_hidden);
    menuOptions->addAction(actionDisable_tray_messages);
    menuOptions->addAction(actionChange_language);
    toolBar->addAction(actionNew_sync);
    toolBar->addAction(actionClose_sync);
    toolBar->addSeparator();
    toolBar->addAction(actionSynchronise);
    toolBar->addAction(actionMultisync);
    toolBar->addAction(actionSyncView);
    toolBar->addAction(actionScheduler);
    toolBar->addAction(actionRestore);
    toolBar->addAction(actionBlacklist);
    toolBar->addAction(actionFilters);
    toolBar->addSeparator();
    toolBar->addAction(actionSync_all);

    retranslateUi(MainWindow);

    mainStackedWidget->setCurrentIndex(0);
    tabWidget->setCurrentIndex(0);
    multi_tabWidget->setCurrentIndex(0);
    timing_tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Synkron", 0, QApplication::UnicodeUTF8));
    actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionQuit->setStatusTip(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    actionAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionAbout->setStatusTip(QApplication::translate("MainWindow", "View About", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    actionFolder_1->setText(QApplication::translate("MainWindow", "Set synchronization folder 1", 0, QApplication::UnicodeUTF8));
    actionFolder_2->setText(QApplication::translate("MainWindow", "Set synchronization folder 2", 0, QApplication::UnicodeUTF8));
    actionNew_sync->setText(QApplication::translate("MainWindow", "New tab", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionNew_sync->setStatusTip(QApplication::translate("MainWindow", "Open new sync tab", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    actionClose_sync->setText(QApplication::translate("MainWindow", "Close tab", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionClose_sync->setStatusTip(QApplication::translate("MainWindow", "Close current sync tab", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    actionSynchronise->setText(QApplication::translate("MainWindow", "Synchronise", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionSynchronise->setStatusTip(QApplication::translate("MainWindow", "Go to Synchronise", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    actionRestore->setText(QApplication::translate("MainWindow", "Restore", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionRestore->setStatusTip(QApplication::translate("MainWindow", "Go to Restore", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    actionBlacklist->setText(QApplication::translate("MainWindow", "Blacklist", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionBlacklist->setStatusTip(QApplication::translate("MainWindow", "Go to Blacklist", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    actionRun_hidden->setText(QApplication::translate("MainWindow", "Run hidden", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionRun_hidden->setStatusTip(QApplication::translate("MainWindow", "Start Synkron hidden", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    actionSync_all->setText(QApplication::translate("MainWindow", "Sync all", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionSync_all->setStatusTip(QApplication::translate("MainWindow", "Sync all", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    actionCheck_for_updates->setText(QApplication::translate("MainWindow", "Check for updates", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionCheck_for_updates->setStatusTip(QApplication::translate("MainWindow", "Check for updates", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    actionMultisync->setText(QApplication::translate("MainWindow", "Multisync", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionMultisync->setStatusTip(QApplication::translate("MainWindow", "Go to Multisync", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    actionScheduler->setText(QApplication::translate("MainWindow", "Scheduler", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionScheduler->setStatusTip(QApplication::translate("MainWindow", "Go to Scheduler", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    actionChange_language->setText(QApplication::translate("MainWindow", "Change language", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionChange_language->setStatusTip(QApplication::translate("MainWindow", "Change language", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    actionFilters->setText(QApplication::translate("MainWindow", "Filters", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionFilters->setStatusTip(QApplication::translate("MainWindow", "Go to Filters", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    actionDisable_tray_messages->setText(QApplication::translate("MainWindow", "Disable tray icon messages", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionDisable_tray_messages->setStatusTip(QApplication::translate("MainWindow", "Disable tray icon messages", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    actionSave_log->setText(QApplication::translate("MainWindow", "Save log", 0, QApplication::UnicodeUTF8));
    actionSyncView->setText(QApplication::translate("MainWindow", "SyncView", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    actionSyncView->setStatusTip(QApplication::translate("MainWindow", "Go to SyncView", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Sync #1", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabToolTip(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Sync #1", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("MainWindow", "<b>Files available for restoring:</b>", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    restore_list->setStatusTip(QApplication::translate("MainWindow", "List of files available for restoring", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP


#ifndef QT_NO_STATUSTIP
    selTmpAllBtn->setStatusTip(QApplication::translate("MainWindow", "Select all temporary files", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    selTmpAllBtn->setText(QApplication::translate("MainWindow", "Select all", 0, QApplication::UnicodeUTF8));
    restore_search->setText(QApplication::translate("MainWindow", "Search", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    path_of_syncfile->setStatusTip(QApplication::translate("MainWindow", "Original file path", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP


#ifndef QT_NO_STATUSTIP
    to_black_list->setStatusTip(QApplication::translate("MainWindow", "Add file to blacklist", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    to_black_list->setText(QApplication::translate("MainWindow", "Do not synchronise this file again (add to blacklist)", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("MainWindow", "Date and time of synchronisation:", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("MainWindow", "Original file path:", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("MainWindow", "<b>About the selected item:</b>", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    date_of_sync->setStatusTip(QApplication::translate("MainWindow", "Date and time of synchronisation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    date_of_sync->setDisplayFormat(QApplication::translate("MainWindow", "yyyy.MM.dd-hh:mm:ss", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    restore_files->setStatusTip(QApplication::translate("MainWindow", "Restore selected files", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    restore_files->setText(QApplication::translate("MainWindow", "Restore", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("MainWindow", "<b>Files in the blacklist:</b>", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    blacklist_fileslist->setStatusTip(QApplication::translate("MainWindow", "List of files in the blacklist", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP


#ifndef QT_NO_STATUSTIP
    blacklist_removefile->setStatusTip(QApplication::translate("MainWindow", "Remove file from blacklist", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    blacklist_removefile->setText(QApplication::translate("MainWindow", "Remove", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    blacklist_addfile->setStatusTip(QApplication::translate("MainWindow", "Add file to black list", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    blacklist_addfile->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("MainWindow", "<b>Folders in the blacklist:</b>", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    blacklist_folderslist->setStatusTip(QApplication::translate("MainWindow", "List of folders in the blacklist", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP


#ifndef QT_NO_STATUSTIP
    blacklist_removefolder->setStatusTip(QApplication::translate("MainWindow", "Remove folder from blacklist", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    blacklist_removefolder->setText(QApplication::translate("MainWindow", "Remove", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    blacklist_addfolder->setStatusTip(QApplication::translate("MainWindow", "Add folder to blacklist", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    blacklist_addfolder->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    blacklist_extslist->setStatusTip(QApplication::translate("MainWindow", "List of extensions in the blacklist", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    label_20->setText(QApplication::translate("MainWindow", "<b>Extensions in the blacklist:</b>", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    blacklist_addext->setStatusTip(QApplication::translate("MainWindow", "Add extension to blacklist", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    blacklist_addext->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    blacklist_removeext->setStatusTip(QApplication::translate("MainWindow", "Remove extension from blacklist", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    blacklist_removeext->setText(QApplication::translate("MainWindow", "Remove", 0, QApplication::UnicodeUTF8));
    multi_tabWidget->setTabText(multi_tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Sync #1", 0, QApplication::UnicodeUTF8));
    multi_tabWidget->setTabToolTip(multi_tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Sync #1", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("MainWindow", "<b>Schedules:</b>", 0, QApplication::UnicodeUTF8));
    label_12->setText(QString());

#ifndef QT_NO_STATUSTIP
    tw_schedules->setStatusTip(QApplication::translate("MainWindow", "List of schedules", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP


#ifndef QT_NO_STATUSTIP
    add_schedule->setStatusTip(QApplication::translate("MainWindow", "Add a schedule", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    add_schedule->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    remove_schedule->setStatusTip(QApplication::translate("MainWindow", "Remove the current schedule", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    remove_schedule->setText(QApplication::translate("MainWindow", "Remove", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    startall_schedules->setStatusTip(QApplication::translate("MainWindow", "Start all schedules", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    startall_schedules->setText(QApplication::translate("MainWindow", "Start all", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    stopall_schedules->setStatusTip(QApplication::translate("MainWindow", "Stop all schedules", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    stopall_schedules->setText(QApplication::translate("MainWindow", "Stop all", 0, QApplication::UnicodeUTF8));
    schedGroupBox->setTitle(QApplication::translate("MainWindow", "Schedule options", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    sched_name->setStatusTip(QApplication::translate("MainWindow", "Set schedule name", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP


#ifndef QT_NO_STATUSTIP
    sched_multitab_lw->setStatusTip(QApplication::translate("MainWindow", "Select multisyncs to use", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP


#ifndef QT_NO_STATUSTIP
    sched_tab_lw->setStatusTip(QApplication::translate("MainWindow", "Select syncs to use", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    label_10->setText(QApplication::translate("MainWindow", "<b>Syncs to use:</b>", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("MainWindow", "<b>Multisyncs to use:</b>", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    sched_stop->setStatusTip(QApplication::translate("MainWindow", "Stop schedule", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    sched_stop->setText(QApplication::translate("MainWindow", "Stop", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    sched_start->setStatusTip(QApplication::translate("MainWindow", "Start schedule", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    sched_start->setText(QApplication::translate("MainWindow", "Start", 0, QApplication::UnicodeUTF8));
    label_15->setText(QApplication::translate("MainWindow", "<b>Times:</b>", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    sched_time_lw->setStatusTip(QApplication::translate("MainWindow", "List of sync times", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP


#ifndef QT_NO_STATUSTIP
    sched_add_time->setStatusTip(QApplication::translate("MainWindow", "Add a sync time", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    sched_add_time->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    sched_time_edit->setStatusTip(QApplication::translate("MainWindow", "Set sync time", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    sched_time_edit->setDisplayFormat(QApplication::translate("MainWindow", "H:mm", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    sched_remove_time->setStatusTip(QApplication::translate("MainWindow", "Remove the selected sync times", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    sched_remove_time->setText(QApplication::translate("MainWindow", "Remove", 0, QApplication::UnicodeUTF8));
    timing_tabWidget->setTabText(timing_tabWidget->indexOf(sync_times), QApplication::translate("MainWindow", "Sync times", 0, QApplication::UnicodeUTF8));
    label_19->setText(QApplication::translate("MainWindow", "Synchronise every", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("MainWindow", "minute(s)", 0, QApplication::UnicodeUTF8));
    timing_tabWidget->setTabText(timing_tabWidget->indexOf(periodically), QApplication::translate("MainWindow", "Synchronise periodically", 0, QApplication::UnicodeUTF8));
    label_16->setText(QApplication::translate("MainWindow", "<b>Filters:</b>", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    filter_list->setStatusTip(QApplication::translate("MainWindow", "List of filters", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    label_17->setText(QApplication::translate("MainWindow", "<b>Extensions in the selected filter:</b>", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    filter_extensions_list->setStatusTip(QApplication::translate("MainWindow", "List of extensions in the selected filter", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP


#ifndef QT_NO_STATUSTIP
    filter_add_extension->setStatusTip(QApplication::translate("MainWindow", "Add an extension", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    filter_add_extension->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    filter_remove_extension->setStatusTip(QApplication::translate("MainWindow", "Remove the current extension", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    filter_remove_extension->setText(QApplication::translate("MainWindow", "Remove", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    filter_add->setStatusTip(QApplication::translate("MainWindow", "Add a filter", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    filter_add->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    filter_remove->setStatusTip(QApplication::translate("MainWindow", "Remove the current filter", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    filter_remove->setText(QApplication::translate("MainWindow", "Remove", 0, QApplication::UnicodeUTF8));
    label_21->setText(QApplication::translate("MainWindow", "<b>Syncs:</b>", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    syncs_syncview->setStatusTip(QApplication::translate("MainWindow", "List of syncs", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    label_22->setText(QApplication::translate("MainWindow", "<b>Multisyncs:</b>", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_STATUSTIP
    multisyncs_syncview->setStatusTip(QApplication::translate("MainWindow", "List of multisyncs", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

    menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
    menuOptions->setTitle(QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
