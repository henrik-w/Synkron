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

#include "ui_main_window.h"
#include "ui_multisync_page.h"
#include "ui_about.h"
#include "ui_sync_view_item.h"
#include "mtfile.h"
#include "mtadvancedgroupbox.h"
#include "mtstringset.h"
#include "syncfolders.h"

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

#ifdef Q_WS_MAC
#include <Carbon/Carbon.h>
#endif

inline void release(QFileInfo *& f) { if (f) { delete f; f = NULL; } }
inline void release(QFile *& f) { if (f) { delete f; f = NULL; } }
inline void release(QObject *& o) { if (o) { delete o; o = NULL; } }

class MainWindow;

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
    void removeFileFromBlacklist();
    void addFolderToBlacklist();
    void removeFolderFromBlacklist();
    void addExtToBlacklist();
    void removeExtFromBlacklist();
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
    int sync();
    void subGroupSync(MTStringSet);
    void syncFoldersChanged();
    void showThisPage();
    void updateOnlyStateChanged(bool);
    void updateOnlyOneFolderStateChanged(bool);
    void backupFoldersStateChanged(bool);
    void backupOneFolderStateChanged(bool);
    
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
	
    QWidget * tab;
    QLabel * icon_label;
    QLineEdit * tab_name;
    ExtendedLineEdit * log_search;
    QTableWidget * tw;
    QPushButton * sync_btn;
    QPushButton * stop_sync_btn;
    QPushButton * back;
    QPushButton * resync;
    QCheckBox * show_sync_folders;
    QCheckBox * backup_folders;
    QCheckBox * update_only;
    MTAdvancedGroupBox * advanced;
    QStackedWidget * tab_stw;
    SyncFolders * sync_folders;
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

class SyncSchedule : public QObject
{
    Q_OBJECT
    
public:
    SyncSchedule(MainWindow *);
    QList<QTimer*> timers;
    
	QStringList sched_tab_list;
	QStringList sched_multitab_list;
	QStringList sched_time_list;
	QStringList sched_checked_time_list;
	
	int periodical_interval;
	int timing_tab_index;
	bool scheduling;
    
signals:
    void sigsched(QWidget *);
    
public slots:
	void syncSchedule();
	void checkSchedStatus();
	
private:
	MainWindow * sched_parent;
};

class Filter : public QListWidgetItem
{
public:
	Filter():
	QListWidgetItem(0, QListWidgetItem::UserType)
	{};
	Filter(QString text):
	QListWidgetItem(0, QListWidgetItem::UserType)
	{ setText(text); };
    
	QStringList extensions;
};

class ClientConnection : public QObject
{
    Q_OBJECT

public:
    ClientConnection(MainWindow *, QTcpSocket *);

public slots:
    void read();

private:
    quint64 c_blocksize;
    QTcpSocket * c_socket;
    MainWindow * c_parent;
};

class SyncViewItem : public QWidget, private Ui::SyncViewItem
{
	Q_OBJECT

public:
	SyncViewItem(AbstractSyncPage *);
	
	void setName(QString name) { sync_name_lbl->setText(name); };
	
public slots:
    void startSync();
	
private:
    AbstractSyncPage * parent_page;
	
};

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QSettings *);
    
    QStringList synchronised;
    QStringList files_blacklist;
    QStringList folders_blacklist;
    QStringList exts_blacklist;
    bool syncingAll;
    bool skip_close_event;
	bool runHidden() { return run_hidden; }
    bool shownManually() { return shown_manually; }
    //void setShownManually(bool sm) { shown_manually = sm; }
	bool showTrayMessage(QString, QString);
	QSettings * sync_settings;
        
public slots:
    void saveSettings();
    bool removeDir(QString);
    bool removeFile(QString);
    void setShownManually() { shown_manually = true; }
    bool restoreItem(QListWidgetItem*);
    bool restoreFile(QString, QString);

private slots:
	
// Synchronisation
	void sync() { sync(tabWidget->currentWidget()); }
    void sync(QWidget *);
    void syncAll();
    void tabNameChanged();
    SyncPage * addSyncTab();
    void showAdvancedGroupBox(bool show, SyncPage * page) { page->advanced->setChecked(show); }
	
// Restore
    void toRestorePage();
    void restoreItemChanged(QListWidgetItem *, QListWidgetItem *);
    void restoreFiles();
    void setCleanGB();
    void cleanTemporary();
    void selTmpAll();
    void deleteTempFile(QString);
    void restoreListConMenu(QPoint);
    void restoreCurrentItem();
    void deleteRestoreItem();
    void checkRestoreItem();
    void blacklistRestoreItem();
    
// Blacklist
	void addToBlackList(int);
    void addFileToBlacklist();
    void removeFileFromBlacklist();
    void addFolderToBlacklist();
    void removeFolderFromBlacklist();
    void addExtToBlacklist();
    void removeExtFromBlacklist();
    //void delTmpSel();
    
// Multisync
	MultisyncPage * addMultiTab();
	void addMultisync();
    void removeMultisync();
    void browseMultiDestination();
    
// Scheduler
    void addSchedule();
    SyncSchedule * addSchedule(QString/*QStringList, QStringList, QStringList, QStringList, int*/);
    void removeSchedule();
    void reloadSchedule() { scheduleActivated(tw_schedules->currentRow(), tw_schedules->currentColumn(), tw_schedules->currentRow(), tw_schedules->currentColumn()); }
    void reloadSchedStatus();
	void scheduleActivated(int, int, int, int);
    void setSchedName(const QString text) { tw_schedules->item(tw_schedules->currentRow(), 0)->setText(text); }
    void addSchedTime();
    void removeSchedTime();
    void saveSchedSettings(int);
    void setScheduleStatusOn(bool, QTableWidgetItem*);
    void schedTabClicked(QListWidgetItem*);
    void schedMultitabClicked(QListWidgetItem*);
    void schedTimeClicked(QListWidgetItem*);
    void startSchedule() { startSchedule(tw_schedules->item(tw_schedules->currentRow(), 0)); }
    void startSchedule(QTableWidgetItem *);
    void stopSchedule() { stopSchedule(tw_schedules->item(tw_schedules->currentRow(), 0)); }
    void stopSchedule(QTableWidgetItem *);
    void searchTw(const QString);
    void searchLw(const QString);
    void startAllSchedules();
    void stopAllSchedules();
    void enableSchedule(int);
    void activateSchedule();
    void schedIntervalChanged(int);
    void timngTabIndexChanged(int);
    
// Filters
    void addFilter();
    void addFilter(QString, QStringList);
    void removeFilter();
    void addFilterExtension();
    void removeFilterExtension();
    void filterChanged();
    void setFiltersEnabled(bool);
    
//SyncView
    void refreshSyncs();
    void refreshMultisyncs();
    void toSyncView();
    
// Other
    void changeLanguage(); void langChanged();
    //void browse(QAbstractButton *);
    void checkForUpdates(); void httpRequestFinished(bool);
    void about();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void trayIconVisible(bool);
    //void minimizeTrayIcon() { trayIconVisible(false); }
    //void maximizeTrayIcon() { trayIconVisible(true); }
    void switchView(QAction*);
    void addTab();
    void closeTab();
    bool closeDialogue();
    void closeApp() { no_closedialogue = true; this->close(); }
    void saveSyncLog();
    void setRunHidden(bool b) { run_hidden = b; }
    void globalDelete(QString);
    void globalRename(QString, QString);
    bool renameFile(QString, QString);
    void sendMessageAndClose();
    void addConnection();
    void initServer(QAbstractSocket::SocketError);

private:
    float f_ver;
	QString ver;
    bool run_hidden;
    bool sched_removed;
    bool no_closedialogue;
    QHttp * http; QBuffer * http_buffer;
    QTcpServer * tcp_server; QTcpSocket * tcp_socket;
    
    void closeEvent(QCloseEvent*);
    void readSettings();
    void createTrayIcon();
    void createActions();
    void toBlacklist();
    
    QButtonGroup * btngrpBrowse;
    QActionGroup * actgrpView;
    QComboBox * langComboBox;
    QMap<QWidget *, SyncPage *> tabs;
    QMap<QTableWidgetItem*, SyncSchedule*> item_sched_map;
    QMap<QString, QString> synkron_i18n;
    
    QCheckBox * restore_clean_selected;
    QCheckBox * restore_clean_by_date;
    QCheckBox * restore_clean_repeated;
    QSpinBox * restore_clean_date;
    QSpinBox * restore_clean_files;
    QPushButton * restore_clean;
    
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *syncAction;
    QAction *quitAction;
    QAction *syncAllAction;
    QAction *restoreAction;
    QAction *deleteRestoreItemAction;
    QAction *checkRestoreItemAction;
    QAction *blacklistRestoreItemAction;
    QSystemTrayIcon *trayIcon;
#ifdef Q_WS_MAC
	QAction * actionBrushedMetalStyle;
#endif
    bool shown_manually;

	friend class SyncSchedule;
	friend class SyncPage;
	friend class MultisyncPage;
	friend class SyncViewItem;
	friend class ClientConnection;
protected:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
};

class About : public QDialog, private Ui::About
{
    Q_OBJECT
    
public:
    About(QString, QString, QString);
};

class MTApplication : public QApplication
{
    Q_OBJECT
protected:
    void init() { app_main_window = NULL; };
public:
    MTApplication(int & argc, char ** argv):
    QApplication(argc, argv) { init(); };
    MTApplication(int & argc, char ** argv, bool GUIenabled):
    QApplication(argc, argv, GUIenabled) { init(); };
    MTApplication(int & argc, char ** argv, Type type):
    QApplication(argc, argv, type) { init(); };
#ifdef Q_WS_X11
    MTApplication(Display * display, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0):
    QApplication(display, visual, colormap) { init(); };
    MTApplication(Display * display, int & argc, char ** argv, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0):
    QApplication(display, argc, argv, visual, colormap) { init(); };
#endif
#ifdef Q_WS_MAC
    bool macEventFilter(EventHandlerCallRef, EventRef event) {
        switch (GetEventKind(event)) {
            case kEventAppActivated: if (app_main_window != NULL) {
                if (!app_main_window->runHidden() || app_main_window->shownManually())
                    { app_main_window->show(); }
            } break;
        }
        return false;
    };
#endif
    void setAppMainWindow(MainWindow * w) { app_main_window = w; };
private:
    MainWindow * app_main_window;
};
