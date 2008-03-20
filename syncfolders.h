#include <QList>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QFileDialog>
#include <QIcon>
#include <QPushButton>

#ifndef SYNCFOLDERS_H
#define SYNCFOLDERS_H

class SyncFolder;

class SyncFolders : public QWidget
{
    Q_OBJECT

public:
    SyncFolders(QWidget * parent = 0);
    
    QList <SyncFolder *> folders_list;
    //QStringList paths_list;
    QVBoxLayout * folders_vlayout;
    QToolButton * add_folder_btn;
    
    SyncFolder * syncFolder(int i) { return folders_list.at(i); };
    SyncFolder * at(int i) { return folders_list.at(i); };
    int count() { return folders_list.count(); };
    QStringList pathsList();
    
public slots:
    SyncFolder * addFolder();
    void addToFolders(int);
    void removeFolder(int);
    void removeFolder(SyncFolder *);
    void removeAllFolders();
    void foldersChanged();
    
signals:
    void sigfolderschanged();
};

class SyncFolder : public QWidget
{
    Q_OBJECT

public:
    SyncFolder(QWidget * parent = 0);
    
    QToolButton * remove_folder_btn;
    QLineEdit * folder_name_lne;
    QPushButton * browse_btn;
    
public slots:
    void browse();
    void setPath(QString path) { folder_name_lne->setText(path); };
    QString path() { return folder_name_lne->text(); };
    void removeFolder() { emit sigremove(this); };
    void lneEdited();
    
signals:
    void sigremove(SyncFolder *);
};

#endif
