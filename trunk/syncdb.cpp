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

#include "main_window.h"

void AbstractSyncPage::saveFolderDatabase(QString dir_path)
{
    QDir dir (dir_path);
    QFile file(QString("%1/%2").arg(dir.absolutePath()).arg(".synkron.syncdb"));
    MTDictionary other_tabs = getFolderDatabaseOfOtherTabs(file);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        addTableItem(tr("Cannot write file %1: %2").arg(file.fileName()).arg(file.errorString()), "", ":/new/prefix1/images/file.png", QBrush(Qt::red), QBrush(Qt::white));
        return;
    }
    QTextStream write_sfile(&file);
    write_sfile.setCodec("UTF-8");
    for (int i = 0; i < other_tabs.count(); ++i) {
        if (other_tabs.key(i).startsWith("<:?:>")) {
            write_sfile << other_tabs.key(i) << endl;
        } else {
            write_sfile << other_tabs.key(i) << "<->" << other_tabs.value(i) << endl;
        }
    }
    MTDictionary entry_list = getEntryList(dir.absolutePath(), dir.absolutePath());
    write_sfile << "<:?:>" << tabNameText() << endl;
    for (int i = 0; i < entry_list.count(); ++i) {
        write_sfile << entry_list.key(i) << "<->" << entry_list.value(i) << endl;
    }
}

MTDictionary AbstractSyncPage::getFolderDatabase(QString dir_path)
{
    //QStringList prop_files_list;
    MTDictionary prop_files_list;
    QFile file(QString("%1/%2").arg(dir_path).arg(".synkron.syncdb"));
    if (!file.exists()) return prop_files_list;
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
		//QMessageBox::critical(this, tr("Save database"), tr("Cannot write file %1:\n%2.").arg(db_file_name).arg(file.errorString()));
		return prop_files_list;
    }
    QTextStream read_sfile(&file);
    read_sfile.setCodec("UTF-8");
    QString line; QString last_tab;
    while (!read_sfile.atEnd()) {
        line = read_sfile.readLine();
        if (line.startsWith("<:?:>")) {
            last_tab = line;
            last_tab.remove(0, 5);
            continue;
        }
        if (last_tab != tabNameText()) continue;
        //prop_files_list << line;
        if (line.split("<->").count() < 2) {
            prop_files_list.setValue(line.split("<->").first(), "");
        } else {
            prop_files_list.setValue(line.split("<->").first(), line.split("<->").last());
        }
    }
    if (prop_files_list.count() == 0) {
        if (!read_sfile.seek(0)) return prop_files_list;
        read_sfile.setCodec("UTF-8");
        while (!read_sfile.atEnd()) {
            line = read_sfile.readLine();
            if (line.startsWith("<:?:>")) break;
            //prop_files_list << line;
            if (line.split("<->").count() < 2) {
                prop_files_list.setValue(line.split("<->").first(), "");
            } else {
                prop_files_list.setValue(line.split("<->").first(), line.split("<->").last());
            }
        }
    }
    return prop_files_list;
}

void AbstractSyncPage::saveAllFolderDatabases()
{
    QStringList paths = syncFoldersList();
    for (int i = 0; i < paths.count(); ++i) {
        saveFolderDatabase(paths.at(i));
    }
}

MTDictionary AbstractSyncPage::getFolderDatabaseOfOtherTabs(QFile & file)
{
    MTDictionary other_tabs;
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream read_sfile(&file);
        read_sfile.setCodec("UTF-8");
        QString last_tab; QStringList used_tabs; QString line;
        while (!read_sfile.atEnd()) {
            line = read_sfile.readLine();
            if (line.startsWith("<:?:>")) {
                last_tab = line;
                last_tab.remove(0, 5);
                if (used_tabs.contains(last_tab)) {
                    last_tab = tabNameText();
                } else {
                    used_tabs << last_tab;
                }
            }
            if (last_tab == tabNameText()) continue;
            //other_tabs << line;
            if (line.split("<->").count() < 2) {
                other_tabs.setValue(line.split("<->").first(), "");
            } else {
                other_tabs.setValue(line.split("<->").first(), line.split("<->").last());
            }
        }
        file.close();
    }
    return other_tabs;
}

void AbstractSyncPage::deleteFolderDatabase(QString dir_path)
{
    QDir dir (dir_path);
    QFile file(QString("%1/%2").arg(dir.absolutePath()).arg(".synkron.syncdb"));
    MTDictionary other_tabs = getFolderDatabaseOfOtherTabs(file);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        addTableItem(tr("Cannot write file %1: %2").arg(file.fileName()).arg(file.errorString()), "", ":/new/prefix1/images/file.png", QBrush(Qt::red), QBrush(Qt::white));
        return;
    }
    QTextStream write_sfile(&file);
    write_sfile.setCodec("UTF-8");
    for (int i = 0; i < other_tabs.count(); ++i) {
        if (other_tabs.key(i).startsWith("<:?:>")) {
            write_sfile << other_tabs.key(i) << endl;
        } else {
            write_sfile << other_tabs.key(i) << "<->" << other_tabs.value(i) << endl;
        }
    }
}

void AbstractSyncPage::deleteAllFolderDatabases()
{
    QStringList paths = syncFoldersList();
    for (int i = 0; i < paths.count(); ++i) {
        deleteFolderDatabase(paths.at(i));
    }
}

MTDictionary AbstractSyncPage::getEntryList(QString dir_path, QString parent_dir)
{
    QFileInfoList entries = QDir(dir_path).entryInfoList(QDir::NoDotAndDotDot | QDir::Hidden | QDir::Files | QDir::AllDirs, QDir::Name | QDir::DirsFirst | QDir::IgnoreCase);
    MTDictionary entry_list;
    QDir dir (parent_dir);
    for (int i = 0; i < entries.count(); ++i) {
        if (entries.at(i).fileName() == ".synkron.syncdb") continue;
        if (!entries.at(i).isDir() || entries.at(i).isSymLink()) {
            entry_list.setValue(dir.relativeFilePath(entries.at(i).absoluteFilePath()), entries.at(i).lastModified().toString(Qt::ISODate));
        } else {
            entry_list.setValue(dir.relativeFilePath(entries.at(i).absoluteFilePath()), "");
            entry_list << getEntryList(entries.at(i).absoluteFilePath(), parent_dir);
        }
    }
    return entry_list;
}

bool AbstractSyncPage::isInGroupDatabase(QString file_name)
{
    QString sync_folder_path;
    QStringList s_folders_list = currentSyncFoldersList();
    for (int i = 0; i < s_folders_list.count(); ++i) {
        if (file_name.startsWith(s_folders_list.at(i))) {
            sync_folder_path = s_folders_list.at(i);
            break;
        }
    }
    if (sync_folder_path == "") return false;
    MTDictionary prop_files_list = folder_prop_list_map.value(sync_folder_path);
    if (prop_files_list.contains(QDir(sync_folder_path).relativeFilePath(file_name))) return true;
    return false;
}

bool AbstractSyncPage::isInDatabase(QString file_name)
{
    QDir dir;
    if (file_name.startsWith(syncFolder1Text())) {
        dir.setPath(syncFolder1Text());
    } else if (file_name.startsWith(syncFolder2Text())) {
        dir.setPath(syncFolder2Text());
    } else {
        return false;
    }
    MTDictionary prop_files_list = folder_prop_list_map.value(dir.path());
    if (prop_files_list.contains(dir.relativeFilePath(file_name))) return true;
    return false;
}

void AbstractSyncPage::changeTabNameInDatabase(QString new_name, QString old_name)
{
    QStringList paths = syncFoldersList();
    for (int i = 0; i < paths.count(); ++i) {
        QDir dir(paths.at(i));
        QFile file(QString("%1/%2").arg(dir.absolutePath()).arg(".synkron.syncdb"));
        if (!file.exists()) return;
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
		  //QMessageBox::critical(this, tr("Save database"), tr("Cannot write file %1:\n%2.").arg(db_file_name).arg(file.errorString()));
		  return;
        }
        QTextStream read_sfile(&file);
        read_sfile.setCodec("UTF-8");
        QStringList new_db;
        QString line; QString last_tab;
        while (!read_sfile.atEnd()) {
            line = read_sfile.readLine();
            if (line.startsWith("<:?:>")) {
                last_tab = line;
                last_tab.remove(0, 5);
                if (last_tab == old_name) {
                    line = QString("<:?:>%1").arg(new_name);
                }
            }
            new_db << line;
        }
        file.close();
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            //addTableItem(tr("Cannot write file %1: %2").arg(file.fileName()).arg(file.errorString()), "", ":/new/prefix1/images/file.png", QBrush(Qt::red), QBrush(Qt::white));
            return;
        }
        QTextStream write_sfile(&file);
	    write_sfile.setCodec("UTF-8");
	    for (int n = 0; n < new_db.count(); ++n) {
            write_sfile << new_db.at(n) << endl;
        }
    }
}

bool AbstractSyncPage::checkForCollision(QString path1, QString path2)
{
    /*QString folder1; QString folder2;
    QStringList s_folders_list = currentSyncFoldersList();
    for (int i = 0; i < s_folders_list.count(); ++i) {
        if (path1.startsWith(s_folders_list.at(i))) {
            folder1 = s_folders_list.at(i);
        }
        if (path2.startsWith(s_folders_list.at(i))) {
            folder2 = s_folders_list.at(i);
        }
    }
    if (folder1.isEmpty() || folder2.isEmpty()) return false;
    MTDictionary dict1 = folder_prop_list_map.value(folder1);
    MTDictionary dict2 = folder_prop_list_map.value(folder2);
    if (!dict1.contains(QDir(folder1).relativeFilePath(path1))) return false;
    if (!dict2.contains(QDir(folder2).relativeFilePath(path2))) return false;
    QString date_str1 = dict1.value(dict1.indexOfKey(QDir(folder1).relativeFilePath(path1)));
    QString date_str2 = dict2.value(dict2.indexOfKey(QDir(folder2).relativeFilePath(path2)));
    if (date_str1.isEmpty() || date_str2.isEmpty()) return false;
    MTFileInfo file_info1(path1);
    MTFileInfo file_info2(path2);
    if (file_info1.lastModified().toString(Qt::ISODate) != date_str1 && file_info2.lastModified().toString(Qt::ISODate) != date_str2) {
        return true;
    }
    return false;*/
    if (fileIsDifferentFromDB(path1) && fileIsDifferentFromDB(path2)) {
        addTableItem(tr("Collision found between files: %1 and %2").arg(path1).arg(path2), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkCyan), QBrush(Qt::white));
        return true;
    }
    return false;
}

bool AbstractSyncPage::fileIsDifferentFromDB(QString path)
{
    QString folder;
    QStringList s_folders_list = currentSyncFoldersList();
    for (int i = 0; i < s_folders_list.count(); ++i) {
        if (path.startsWith(s_folders_list.at(i))) {
            folder = s_folders_list.at(i);
        }
    }
    if (folder.isEmpty()) return false;
    MTDictionary dict = folder_prop_list_map.value(folder);
    if (!dict.contains(QDir(folder).relativeFilePath(path))) return false;
    QString date_str = dict.value(dict.indexOfKey(QDir(folder).relativeFilePath(path)));
    if (date_str.isEmpty()) return false;
    MTFileInfo file_info(path);
    if (file_info.lastModified().toString(Qt::ISODate) != date_str) {
        return true;
    }
    return false;
}

void AbstractSyncPage::displayCollisions()
{
    QApplication::restoreOverrideCursor();
    QDialog * dialogue = new QDialog (this, Qt::Dialog);
	dialogue->setWindowModality(Qt::WindowModal);
	dialogue->setWindowTitle(tr("Collisions found"));
	QGridLayout * col_glayout = new QGridLayout(dialogue);
	col_glayout->setMargin(4); col_glayout->setSpacing(10);
    QLabel * clabel = new QLabel(dialogue);
    clabel->setText(tr("Collided files:"));
    col_glayout->addWidget(clabel, 0, 0);
    QTableWidget * dial_tw = new QTableWidget(0, 2, dialogue);
    dial_tw->setHorizontalHeaderLabels(QStringList() << tr("Newer files") << tr("Older files"));
    dial_tw->verticalHeader()->hide();
    dial_tw->setShowGrid(false);
    dial_tw->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    dial_tw->setLayoutDirection(Qt::LeftToRight);
    QCheckBox * newer; QCheckBox * older;
    for (int i = 0; i < collided.count(); ++i) {
        newer = new QCheckBox;
        newer->setText(collided.key(i));
        newer->setToolTip(collided.key(i));
        older = new QCheckBox;
        older->setText(collided.value(i));
        older->setToolTip(collided.value(i));
        new MTCheckBoxGroup(newer, older);
        dial_tw->insertRow(dial_tw->rowCount());
        dial_tw->setCellWidget(dial_tw->rowCount()-1, 0, newer);
        dial_tw->setCellWidget(dial_tw->rowCount()-1, 1, older);
    }
    dial_tw->resizeRowsToContents();
    col_glayout->addWidget(dial_tw, 1, 0);
    QPushButton * ok_button = new QPushButton(dialogue);
    ok_button->setText(tr("OK"));
    connect(ok_button, SIGNAL(released()), dialogue, SLOT(accept()));
    QPushButton * cancel_button = new QPushButton(dialogue);
    cancel_button->setText(tr("Cancel"));
    connect(cancel_button, SIGNAL(released()), dialogue, SLOT(reject()));
    QHBoxLayout * hbuttons = new QHBoxLayout;
    hbuttons->addStretch();
    hbuttons->addWidget(ok_button);
    hbuttons->addWidget(cancel_button);
    col_glayout->addLayout(hbuttons, 2, 0);
    dialogue->resize(500, 250);
    dialogue->show();
    switch (dialogue->exec()) {
		case 0:
            break;
		case 1:
            for (int i = 0; i < dial_tw->rowCount(); ++i) {
                if (((QCheckBox *) dial_tw->cellWidget(i, 0))->isChecked()) {
                    copyFile(((QCheckBox *) dial_tw->cellWidget(i, 0))->text(), ((QCheckBox *) dial_tw->cellWidget(i, 1))->text(), true);
                } else if (((QCheckBox *) dial_tw->cellWidget(i, 1))->isChecked()) {
                    copyFile(((QCheckBox *) dial_tw->cellWidget(i, 1))->text(), ((QCheckBox *) dial_tw->cellWidget(i, 0))->text(), true);
                }
            }
        default:
            break;
	}
    QApplication::setOverrideCursor(Qt::WaitCursor);
}

void AbstractSyncPage::propagatedStateChanged(bool checked)
{
    if (checked) {
        if (move->isChecked()) move->click();
        if (clone_folder1->isChecked()) clone_folder1->click();
    }
    if (countSyncFolders() == 2) {
        if (!checked && (alert_collisions->isChecked() || propagate_deletions->isChecked())) return;
        move->setEnabled(!checked);
        clone_folder1->setEnabled(!checked);
    }
}

void AbstractSyncPage::propagatedClicked(bool checked)
{
    if (checked) {
        if (!alert_collisions->isChecked() || !propagate_deletions->isChecked()) {
            saveAllFolderDatabases();
        }
    } else {
        if (!alert_collisions->isChecked() && !propagate_deletions->isChecked()) {
            deleteAllFolderDatabases();
        }
    }
}
