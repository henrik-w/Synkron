#include "ui_main_window.h"
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

class MainWindow;

class SyncPage : public QObject
{
    Q_OBJECT
    
signals:
    void sigsync(QWidget *);
    
public slots:
    void syncPage();
    
public:
    QWidget * tab;
    QLabel * icon_label;
    QLabel * min_text;
    QLabel * sync_text;
    QLineEdit * tab_name;
    QLineEdit * sync_folder_1;
    QLineEdit * sync_folder_2;
    QLineEdit * log_search;
    QPushButton * browse_1;
    QPushButton * browse_2;
    QPushButton * sync;
    QTableWidget * tw;
    QPushButton * back;
    QPushButton * resync;
    QPushButton * periodical_start;
    QPushButton * periodical_stop;
    QCheckBox * periodical_sync;
    QCheckBox * sync_hidden;
    MTAdvancedGroupBox * advanced;
    QCheckBox * backup_folder_1;
    QCheckBox * backup_folder_2;
    QCheckBox * update_only_1;
    QCheckBox * update_only_2;
    QSpinBox * sync_interval;
    QTimer * sync_timer;
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

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow();
    bool runHidden() { return run_hidden; }
        
private slots:
    void browse(QAbstractButton *);
    void sync() { sync(tabWidget->currentWidget()); }
    void sync(QWidget *);
    void syncAll();
    void subSync(QDir&, QDir&, SyncPage *, bool);
    void updateGeometry();
    void checkForUpdates(); void httpRequestFinished(bool);
    void about();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void trayIconVisible(bool);
    void minimizeTrayIcon() { trayIconVisible(false); }
    void maximizeTrayIcon() { trayIconVisible(true); }
    void toRestorePage();
    void restoreItemChanged(QListWidgetItem *, QListWidgetItem *);
    void restoreFiles();
    void addToBlackList(int);
    void switchView(QAction*);
    void addFileToBlacklist();
    void removeFileFromBlacklist();
    void addFolderToBlacklist();
    void removeFolderFromBlacklist();
    void tabNameChanged(QString);
    SyncPage * addTab();
    void closeTab() { tabWidget->removeTab(tabWidget->currentIndex()); }
    void delTmpAll();
    void delTmpSel();
    void startPeriodical(QWidget* syncTab = 0);
    void stopPeriodical();
    void setPeriodicalEnabled(SyncPage *, bool);
    void setRunHidden(bool b) { run_hidden = b; }
    void setFoldersEnabled(SyncPage *, bool);
    void showAdvancedGroupBox(bool show, SyncPage * page) { page->advanced->setChecked(show); }
    void folder1TextChanged() { SyncPage * page = tabs.value(tabWidget->currentWidget()); page->backup_folder_1->setStatusTip(tr("Do not backup synchronised files from folder 1 - %1").arg(page->sync_folder_1->text().split("/").last())); }
    void folder2TextChanged() { SyncPage * page = tabs.value(tabWidget->currentWidget()); page->backup_folder_2->setStatusTip(tr("Do not backup synchronised files from folder 2 - %1").arg(page->sync_folder_2->text().split("/").last())); }
    void addTableItem(SyncPage *, QString, QString = "", QString = "", QBrush = Qt::white, QBrush = Qt::black);
    void toMultisync();
    void addMultisync();
    void removeMultisync();
    void browseMultiDestination();
    void multisync();
    void toScheduler();
    void addSchedule() { addSchedule(QStringList(), QStringList(), QStringList()); }
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
    void saveSettings();
    void startSchedule() { startSchedule(tw_schedules->item(tw_schedules->currentRow(), 0)); }
    void startSchedule(QTableWidgetItem *);
    void stopSchedule() { stopSchedule(tw_schedules->item(tw_schedules->currentRow(), 0)); }
    void stopSchedule(QTableWidgetItem *);
    void logSearch(const QString);
    void restoreSearch(const QString);
    void startAllSchedules();
    void stopAllSchedules();
    void enableSchedule(int);
    void changeLanguage(); void langChanged();
    void saveMultisync();
    void saveAsMultisync();
    void saveAsMultisync(QString file_name);
    void loadMultisync();
    void loadMultisync(QString);
    void setMultisyncEnabled(bool);

private:
    float f_ver;
	QString ver;
    int syncFiles;
    bool syncingAll;
    bool run_hidden;
    bool sched_removed;
    QStringList synchronised;
    QStringList files_blacklist;
    QStringList folders_blacklist;
    QHttp * http; QBuffer * http_buffer;
    
    void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent*);
    void readSettings();
    void createTrayIcon();
    void createActions();
    void unknownError(SyncPage *, QString, QString, QString, QString, QString = "");
    void toBlacklist();

    QButtonGroup * btngrpBrowse;
    QActionGroup * actgrpView;
    QComboBox * langComboBox;
    QMap<QWidget *, SyncPage *> tabs;
    QMap<QTableWidgetItem*, SyncSchedule*> item_sched_map;
    QMap<QString, QString> synkron_i18n;
    
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
};

class About : public QDialog, private Ui::About
{
    Q_OBJECT
    
public:
    About(QString ver) { setupUi(this); ver_label->setText(tr("version <b>%1</b>").arg(ver)); };
};
