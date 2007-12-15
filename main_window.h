#include "ui_main_window.h"
#include "ui_multisync_page.h"
#include "ui_about.h"
#include "mtfile.h"
#include "mtadvancedgroupbox.h"

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
	
	void subSync(QDir&, QDir&, bool);
	void moveContents(QDir&, QDir&);
	void addTableItem(QString, QString = "", QString = "", QBrush = Qt::white, QBrush = Qt::black);
	void unknownError(QString, QString, QString, QString, QString = "");
	
	QSet<QString> extensions;
	bool syncing;
	
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
    QListWidget * lw_filters;
    MainWindow * mp_parent;
    int synced_files;
    
public slots:
    virtual int sync() = 0;
    void stopSync() { syncing = false; };
};

class SyncPage : public AbstractSyncPage
{
    Q_OBJECT
    
signals:
    void sigsync(QWidget *);
    
public slots:
    void syncPage();
    void moveChecked(int state);
    void folder1TextChanged() { QDir dir(sync_folder_1->text()); sync_folder_1->setText(dir.path()); }
    void folder2TextChanged() { QDir dir(sync_folder_2->text()); sync_folder_2->setText(dir.path()); }
    int sync();
    
public:
    SyncPage(MainWindow *parent = 0) : AbstractSyncPage(parent) {};
    
	QTableWidget * tableWidget() { return tw; }
	bool followSymlinks() { return symlinks->isChecked(); }
	QString syncFolder1Text() { return sync_folder_1->text(); }
	QString syncFolder2Text() { return sync_folder_2->text(); }
	void setFoldersEnabled(bool);
	void setSyncingOn(bool);
	void setPeriodicalEnabled(bool);
	
    QWidget * tab;
    QLabel * icon_label;
    QLabel * min_text;
    QLabel * sync_text;
    QLineEdit * tab_name;
    QLineEdit * sync_folder_1;
    QLineEdit * sync_folder_2;
    ExtendedLineEdit * log_search;
    QTableWidget * tw;
    QPushButton * browse_1;
    QPushButton * browse_2;
    QPushButton * sync_btn;
    QPushButton * stop_sync_btn;
    QPushButton * back;
    QPushButton * resync;
    QPushButton * periodical_start;
    QPushButton * periodical_stop;
    MTAdvancedGroupBox * advanced;
    QCheckBox * periodical_sync;
    QSpinBox * sync_interval;
    QTimer * sync_timer;
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
	void setMultisyncEnabled(bool);

public slots:
	void setAdvancedGB();
	void multitabNameChanged();
	void showAdvancedGroupBox(bool show) { advanced->setChecked(show); }
	void saveMultisync();
    void saveAsMultisync();
    void saveAsMultisync(QString file_name);
    void loadMultisync();
    void loadMultisync(QString);
    void moveChecked(int state);
    void destinationTextChanged() { QDir dir(destination_multi->text()); destination_multi->setText(dir.path()); }
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
	QStringList sched_time_list;
	QStringList sched_checked_time_list;
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

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow();
    
    QStringList synchronised;
    QStringList files_blacklist;
    QStringList folders_blacklist;
    bool syncingAll;
	bool runHidden() { return run_hidden; }
	bool showTrayMessage(QString, QString);
        
public slots:
    void saveSettings();

private slots:
	
// Synchronisation
	void sync() { sync(tabWidget->currentWidget()); }
    void sync(QWidget *);
    void syncAll();
    void tabNameChanged();
    SyncPage * addSyncTab();
    void startPeriodical(QWidget* syncTab = 0);
    void stopPeriodical();
    void showAdvancedGroupBox(bool show, SyncPage * page) { page->advanced->setChecked(show); }
	
// Restore
    void toRestorePage();
    void restoreItemChanged(QListWidgetItem *, QListWidgetItem *);
    void restoreFiles();
    void setCleanGB();
    void cleanTemporary();
    void selTmpAll();
    
// Blacklist
	void addToBlackList(int);
    void addFileToBlacklist();
    void removeFileFromBlacklist();
    void addFolderToBlacklist();
    void removeFolderFromBlacklist();
    //void delTmpSel();
    
// Multisync
	MultisyncPage * addMultiTab();
	void addMultisync();
    void removeMultisync();
    void browseMultiDestination();
    
// Scheduler
    void addSchedule();
    QTableWidgetItem * addSchedule(QStringList, QStringList, QStringList);
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
    
// Filters
    void addFilter();
    void addFilter(QString, QStringList);
    void removeFilter();
    void addFilterExtension();
    void removeFilterExtension();
    void filterChanged();
    void setFiltersEnabled(bool);
    
// Other
    void changeLanguage(); void langChanged();
    void browse(QAbstractButton *);
    void checkForUpdates(); void httpRequestFinished(bool);
    void about();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void trayIconVisible(bool);
    void minimizeTrayIcon() { trayIconVisible(false); }
    void maximizeTrayIcon() { trayIconVisible(true); }
    void switchView(QAction*);
    void addTab();
    void closeTab();
    bool closeDialogue();
    void closeApp() { no_closedialogue = true; this->close(); }
    void saveSyncLog();
    void setRunHidden(bool b) { run_hidden = b; }

private:
    float f_ver;
	QString ver;
    bool run_hidden;
    bool sched_removed;
    bool no_closedialogue;
    QHttp * http; QBuffer * http_buffer;
    
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
    QSystemTrayIcon *trayIcon;
#ifdef Q_WS_MAC
	QAction * actionBrushedMetalStyle;
#endif

	friend class SyncSchedule;
	friend class SyncPage;
	friend class MultisyncPage;
};

class About : public QDialog, private Ui::About
{
    Q_OBJECT
    
public:
    About(QString, QString, QString);
};
