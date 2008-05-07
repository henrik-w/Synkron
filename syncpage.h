/*******************************************************************
 This file is part of Synkron
 Copyright (C) 2005-2008 Matus Tomlein (matus.tomlein@gmail.com)

 Synkron is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public Licence
 as published by the Free Software Foundation; either version 2
 of the Licence, or (at your option) any later version.

 Synkron is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public Licence for more details.

 You should have received a copy of the GNU General Public Licence
 along with Synkron; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************/

#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include <QStackedWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QCheckBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QDateTimeEdit>
#include <QHttp>
#include <QBuffer>
#include <QTextStream>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLocale>
#include <QTranslator>
#include <QDomDocument>
#include <QDomElement>
#include <QInputDialog>
#include <QSet>
#include <QTcpServer>
#include <QTcpSocket>
#include <QProgressDialog>
#include <QPalette>
#include <QTreeWidget>
#include <QDesktopServices>

#ifdef Q_WS_MAC
#include <Carbon/Carbon.h>
#endif

#include "ui_main_window.h"
#include "ui_multisync_page.h"
#include "mtfile.h"
#include "mtadvancedgroupbox.h"
#include "mtstringset.h"
#include "syncfolders.h"
#include "extendedtreewidget.h"

class MainWindow;
class SyncViewItem;

class AbstractSyncPage : public QWidget
{
	Q_OBJECT
	
public:
	AbstractSyncPage() {};
	AbstractSyncPage(MainWindow * parent) { mp_parent = parent; syncing = false; };
	
	virtual QTableWidget * tableWidget() = 0;
	virtual bool followSymlinks() = 0;
	virtual QString syncFolder1Text() = 0;
	virtual QString syncFolder2Text() = 0;
	virtual QString tabNameText() = 0;
	virtual void deleteAllFolderDatabases() = 0;
	virtual void saveAllFolderDatabases() = 0;
	virtual int countSyncFolders() = 0;
	
	void subSync(QDir&, QDir&, bool);
	void moveContents(QDir&, QDir&);
	void addTableItem(QString, QString = "", QString = "", QBrush = Qt::white, QBrush = Qt::black);
	void unknownError(QString, QString, QString, QString, QString = "");
	void countExtsBl();
	void saveFolderDatabase(QString);
	void deleteFolderDatabase(QString);
	QStringList getEntryList(QString, QString);
	void backupAndRemoveDir(QString, bool = true, bool = true);
	void backupAndRemoveFile(QFileInfo, bool = true, bool = true);
	bool isInDatabase(QString);
	
	QSet<QString> extensions;
	bool syncing;
	QMap<QString, int> exts_bl_map;
	QMap<QString, QStringList> folder_prop_list_map;
    QDir::Filters dir_filters;
	
    QWidget * blacklist;
    QCheckBox * sync_hidden;
    QGroupBox * filters;
    QCheckBox * backup_folder_1;
    QCheckBox * backup_folder_2;
    QCheckBox * update_only_1;
    QCheckBox * update_only_2;
    QCheckBox * sync_nosubdirs;
    QCheckBox * ignore_blacklist;
    QCheckBox * move;
    QCheckBox * symlinks;
    QCheckBox * clone_folder1;
    QCheckBox * propagate_deletions;
    QListWidget * lw_filters;
    QToolButton * edit_blacklist;
    QListWidget * blacklist_fileslist;
    QListWidget * blacklist_folderslist;
    QListWidget * blacklist_extslist;
    QPushButton * blacklist_addfile;
    QPushButton * blacklist_addfolder;
    QPushButton * blacklist_addext;
    QPushButton * blacklist_removefile;
    QPushButton * blacklist_removefolder;
    QPushButton * blacklist_removeext;
    QPushButton * blacklist_back;
    
    MainWindow * mp_parent;
    int synced_files;
    QString update_time;
    QStringList files_blacklist;
    QStringList folders_blacklist;
    QStringList exts_blacklist;
    
public slots:
    virtual int sync() = 0;
    virtual void moveChecked(bool) = 0;
    virtual void cloneChecked(bool) = 0;
    virtual void blacklistStwChangeIndex(int) = 0;
    virtual void showThisPage() = 0;
    void stopSync() { syncing = false; };
    void cloneStateChanged(bool);
    void moveStateChanged(bool);
    void propagatedStateChanged(bool);
    void propagatedClicked(bool);
    void editBlacklist();
    void backFromBlacklist() { blacklistStwChangeIndex(0); };
    void addFileToBlacklist();
    void addFileToBlacklist(QString);
    void removeFileFromBlacklist();
    void removeFileFromBlacklist(QString);
    void addFolderToBlacklist();
    void addFolderToBlacklist(QString);
    void removeFolderFromBlacklist();
    void removeFolderFromBlacklist(QString);
    void addExtToBlacklist();
    void addExtToBlacklist(QString);
    void removeExtFromBlacklist();
    void removeExtFromBlacklist(QString);
    void setBlacklistWidget();
};

class SyncPage : public AbstractSyncPage
{
    Q_OBJECT
    
signals:
    void sigsync(QWidget *);
    
public slots:
    void syncPage();
    void moveChecked(bool);
    void cloneChecked(bool);
    //void folder1TextChanged() { QDir dir(sync_folder_1->text()); sync_folder_1->setText(dir.path()); }
    //void folder2TextChanged() { QDir dir(sync_folder_2->text()); sync_folder_2->setText(dir.path()); }
    void blacklistStwChangeIndex(int i) { tab_stw->setCurrentIndex(i); }
    int sync() { return sync(MTStringSet()); };
    int sync(MTStringSet);
    void subGroupSync(MTStringSet, MTStringSet = MTStringSet());
    void syncFoldersChanged();
    void showThisPage();
    void updateOnlyStateChanged(bool);
    void updateOnlyOneFolderStateChanged(bool);
    void backupFoldersStateChanged(bool);
    void backupOneFolderStateChanged(bool);
    void setSyncWidget();
    //void setAnalyseWidget();
    void goToAnalyse();
    void leaveAnalyse();
    void analyseFolders();
    bool subAnalyse(MTStringSet, QTreeWidgetItem *);
    void analyseTreeItemClicked(QTreeWidgetItem *, int);
    void analyseTreeItemDoubleClicked(QTreeWidgetItem *, int);
    void checkAnalyseTree();
    void subCheckAnalyseTree(QTreeWidgetItem *);
    void ignoreBlacklistClicked(bool) { if (logs_stw->currentIndex() == 1) checkAnalyseTree(); };
    void searchAnalyseTree();
    bool subSearchAnalyseTree(QString, QTreeWidgetItem *);
    void subCheckExpanded(QTreeWidgetItem *);
    void analyseTreeConMenu(QPoint);
    void openAnalyseTreeItem();
    void syncCurrentAnalyseItem();
    void deleteCurrentAnalyseItem();
    
public:
    SyncPage(MainWindow *parent = 0) : AbstractSyncPage(parent) {};
    
	QTableWidget * tableWidget() { return tw; }
	bool followSymlinks() { return symlinks->isChecked(); }
	QString syncFolder1Text() { return sync_folders->syncFolder(0)->path(); }
	QString syncFolder2Text() { return sync_folders->syncFolder(1)->path(); }
	QString tabNameText() { return tab_name->text(); }
	void setSyncEnabled(bool);
	void saveAllFolderDatabases();
	void deleteAllFolderDatabases();
	bool isInGroupDatabase(QString);
	int countSyncFolders() { return sync_folders->count(); };
	//void setAnalyseTreeItem(QString, QString);
	
    QWidget * tab;
    QWidget * analyse_widget;
    QLabel * icon_label;
    QLabel * sync_log_label;
    QLineEdit * tab_name;
    ExtendedLineEdit * log_search;
    QTableWidget * tw;
    QPushButton * sync_btn;
    QPushButton * stop_sync_btn;
    QPushButton * back;
    QPushButton * resync;
    QPushButton * go_to_analyse;
    QPushButton * analyse_back;
    QCheckBox * show_sync_folders;
    QCheckBox * backup_folders;
    QCheckBox * update_only;
    MTAdvancedGroupBox * advanced;
    QStackedWidget * tab_stw;
    QStackedWidget * logs_stw;
    SyncFolders * sync_folders;
    ExtendedTreeWidget * analyse_tree;
};

class MultisyncPage : public AbstractSyncPage, private Ui::MultisyncForm
{
	Q_OBJECT

public:
	MultisyncPage(MainWindow *parent = 0);
	
	QTableWidget * tableWidget() { return tw_multi; }
	bool followSymlinks() { return symlinks->isChecked(); }
	QString syncFolder1Text() { return sync_folder_1; }
	QString syncFolder2Text() { return sync_folder_2; }
	QString tabNameText() { return tab_name->text(); }
	void setMultisyncEnabled(bool);
	void deleteAllFolderDatabases();
	void saveAllFolderDatabases();
	int countSyncFolders() { return 2; };

public slots:
	void setAdvancedGB();
	void multitabNameChanged();
	void showAdvancedGroupBox(bool show) { advanced->setChecked(show); }
	void saveMultisync();
    void saveAsMultisync();
    void saveAsMultisync(QString file_name);
    void loadMultisync();
    void loadMultisync(QString);
    void moveChecked(bool);
    void cloneChecked(bool);
    void destinationTextChanged() { QDir dir(destination_multi->text()); destination_multi->setText(dir.path()); }
    void blacklistStwChangeIndex(int i) { tab_stw->setCurrentIndex(i); }
    void showThisPage();
    int sync();
	
private:
    QString sync_folder_1;
    QString sync_folder_2;
    QString slist_path;
	
	friend class MainWindow;
};
