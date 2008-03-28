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
