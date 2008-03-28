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

#include "syncfolders.h"

//SyncFolders

SyncFolders::SyncFolders(QWidget * parent):
    QWidget(parent)
{
    folders_list.clear();
    
    connect(this, SIGNAL(sigfolderschanged()), this, SLOT(foldersChanged()));
    
    QVBoxLayout * main_vlayout = new QVBoxLayout(this);
    main_vlayout->setMargin(0); main_vlayout->setSpacing(6);
    /*QLabel * label1 = new QLabel(this);
    label1->setText(tr("<b>Sync folders:</b>"));
    main_vlayout->addWidget(label1);*/
    
    folders_vlayout = new QVBoxLayout(this);
    folders_vlayout->setMargin(0); folders_vlayout->setSpacing(6);
    addFolder();
    addFolder();
    main_vlayout->addLayout(folders_vlayout);
    
    add_folder_btn = new QToolButton (this);
    add_folder_btn->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/add.png")));
    connect(add_folder_btn, SIGNAL(released()), this, SLOT(addFolder()));
    main_vlayout->addWidget(add_folder_btn);
}

SyncFolder * SyncFolders::addFolder()
{
    SyncFolder * folder = new SyncFolder(this);
    connect(folder, SIGNAL(sigremove(SyncFolder *)), this, SLOT(removeFolder(SyncFolder *)));
    folders_vlayout->addWidget(folder);
    folders_list << folder;
    emit sigfolderschanged();
    return folder;
}

QStringList SyncFolders::pathsList()
{
    QStringList paths;
    for (int i = 0; i < folders_list.count(); ++i) {
        paths << folders_list.at(i)->path();
    }
    return paths;
}

void SyncFolders::removeAllFolders()
{
    for (int i = 0; i < folders_list.count();) {
        delete folders_list.takeAt(i);
    }
    folders_list.clear();
}

void SyncFolders::removeFolder(int i)
{
    delete folders_list.takeAt(i);
    emit sigfolderschanged();
}

void SyncFolders::removeFolder(SyncFolder * folder)
{
    int i = folders_list.indexOf(folder);
    if (i >= 0)
        delete folders_list.takeAt(i);
    else
        delete folder;
    emit sigfolderschanged();
}

void SyncFolders::foldersChanged()
{
    bool enable = true;
    if (folders_list.count() == 2) enable = false;
    for (int i = 0; i < folders_list.count(); ++i) {
        folders_list.at(i)->remove_folder_btn->setEnabled(enable);
    }
}

void SyncFolders::addToFolders(int n)
{
    for (int i = folders_list.count(); i < n; ++i) {
        addFolder();
    }
}

//SyncFolder

SyncFolder::SyncFolder(QWidget * parent):
    QWidget(parent)
{
    QHBoxLayout * hlayout = new QHBoxLayout(this);
    hlayout->setMargin(0); hlayout->setSpacing(6);
    
    remove_folder_btn = new QToolButton (this);
    remove_folder_btn->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/remove.png")));
    connect(remove_folder_btn, SIGNAL(released()), this, SLOT(removeFolder()));
    hlayout->addWidget(remove_folder_btn);
    
    folder_name_lne = new QLineEdit (this);
    connect(folder_name_lne, SIGNAL(editingFinished()), this, SLOT(lneEdited()));
    hlayout->addWidget(folder_name_lne);
    
    browse_btn = new QPushButton (this);
    browse_btn->setText(tr("Browse"));
    browse_btn->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/folder_16.png")));
    connect(browse_btn, SIGNAL(released()), this, SLOT(browse()));
    hlayout->addWidget(browse_btn);
}

void SyncFolder::browse()
{
    QString text = QFileDialog::getExistingDirectory(
                this,
                "Choose a directory",
                QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (text != "") folder_name_lne->setText(text);
}

void SyncFolder::lneEdited()
{
    if (folder_name_lne->text()=="") return;
    QDir dir;
    dir.setPath(folder_name_lne->text());
    folder_name_lne->setText(dir.path());
}
