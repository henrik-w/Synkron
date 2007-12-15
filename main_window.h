#include "ui_main_window.h"
#include "ui_about.h"

#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include <QStackedWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QCheckBox>
#include <QSpinBox>
#include <QDateTimeEdit>

class SyncPage : public QObject
{
    Q_OBJECT
    
signals:
    void sigsync(QWidget *);
    
public slots:
    void syncPage();
    
public:
    QWidget * tab;
    QStackedWidget * stacked_widget;
    QWidget * stacked_page_1;
    QWidget * stacked_page_2;
    //QWidget * stacked_page_3;
    QLabel * icon_label;
    QLineEdit * tab_name;
    QLineEdit * sync_folder_1;
    QLineEdit * sync_folder_2;
    //QLineEdit * path_of_syncfile;
    QPushButton * browse_1;
    QPushButton * browse_2;
    QPushButton * sync;
    QListWidget * lw;
    //QListWidget * restore_list;
    QPushButton * back;
    QPushButton * resync;
    //QPushButton * restore_files;
    //QPushButton * restore_cancel;
    //QPushButton * restore;
    QCheckBox * periodical_sync;
    ///QCheckBox * to_black_list;
    QSpinBox * sync_interval;
    //QDateTimeEdit * date_of_sync;
    QTimer * sync_timer;
};

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow();
        
private slots:
    void browse(QAbstractButton *);
    void sync() { sync(tabWidget->currentWidget()); }
    void sync(QWidget *);
    void syncAll();
    void subSync(QDir&, QDir&, SyncPage *, bool, bool, bool);
    void updateGeometry();
    void about();
    void goBack() { SyncPage * page = tabs.value(tabWidget->currentWidget()); if (page->stacked_widget->currentIndex()==1) { page->sync_timer->stop(); } page->stacked_widget->setCurrentIndex((page->stacked_widget->currentIndex()-1)); };
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
    void tabNameEdited();
    SyncPage * addTab();
    SyncPage * addTab(QString, QString, QString);
    void closeTab() { tabWidget->removeTab(tabWidget->currentIndex()); }

private:
    //QString directory1;
    //QString directory2;
    int syncFiles;
    bool syncingAll;
    QStringList synchronised;
    QStringList files_blacklist;
    QStringList folders_blacklist;
    //QTimer sync_timer;
    
    void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent*);
    void readSettings();
    void createTrayIcon();
    void createActions();
    void unknownError(SyncPage *, QString, QString, QString, QString = "");
    void toBlacklist();

    QButtonGroup * btngrpBrowse;
    QActionGroup * actgrpView;
    QMap<QWidget *, SyncPage *> tabs;
    
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *syncAction;
    QAction *quitAction;
    QAction *syncAllAction;

    QSystemTrayIcon *trayIcon;
};

class About : public QDialog, private Ui::About
{
    Q_OBJECT
    
public:
    About() {setupUi(this);};
};
