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

// QDesktopServices::openUrl(string);

void SyncPage::goToAnalyse()
{
    sync_log_label->setText(tr("<b>Analysis:</b>"));
    analyse_tree->clear();
    analyse_tree->setColumnCount(0);
    QStringList labels;
    labels << tr("File name");
    for (int i = 0; i < sync_folders->count(); ++i) {
        labels << tr("Folder %1").arg(i+1);
    }
    analyse_tree->setHeaderLabels(labels);
    analyse_tree->header()->setStretchLastSection(false);
    analyse_tree->header()->setResizeMode(0, QHeaderView::Stretch);
    for (int i = 1; i < labels.count(); ++i) {
        analyse_tree->header()->setResizeMode(i, QHeaderView::ResizeToContents);
    }
    
    logs_stw->setCurrentIndex(1);
    analyseFolders();
}

void MultisyncPage::aboutToShowAnalyseMenu()
{
    analyse_con_menu->clear();
    for (int i = 0; i < list_multi->count(); ++i) {
        analyse_con_menu->addAction(list_multi->item(i)->text());
    }
}

void MultisyncPage::goToAnalyse(QAction * action)
{
    analyse_tree->clear();
    analyse_tree->setColumnCount(0);
    QStringList labels;
    labels << tr("File name");
    labels << tr("Source");
    labels << tr("Destination");
    analyse_tree->setHeaderLabels(labels);
    analyse_tree->header()->setStretchLastSection(false);
    analyse_tree->header()->setResizeMode(0, QHeaderView::Stretch);
    for (int i = 1; i < labels.count(); ++i) {
        analyse_tree->header()->setResizeMode(i, QHeaderView::ResizeToContents);
    }
    
    logs_stw->setCurrentIndex(1);
    analyse(action);
}

void MultisyncPage::analyse(QAction * action)
{
    if (syncing) return;
    QTreeWidgetItem * parent_item = new QTreeWidgetItem (analyse_tree);
    parent_item->setText(0, tr("Root directory"));
    QStringList data0; data0 << ""; data0 << "checked";
    parent_item->setData(0, Qt::UserRole, QVariant(data0));
    parent_item->setIcon(0, QIcon(":/new/prefix1/images/folder_16.png"));
    parent_item->setCheckState(0, Qt::Checked);
    if (ignore_blacklist->isChecked()) parent_item->setDisabled(true);
    parent_item->setExpanded(true);

    bool do_return = false;
    MTStringSet folders_set;    
    QDir destination(destination_multi->text());
    if (!destination.exists()) {
        parent_item->setText(2, tr("NOT FOUND"));
        parent_item->setForeground(2, QBrush(Qt::red));
    }
    QStringList pathlist = action->text().split("/");
    QString s = pathlist.at(0);
    pathlist[0] = s.remove(":");
    for (int v = 0; v < pathlist.count(); ++v) {
        if (!destination.cd(pathlist.at(v))) {
            do_return = true;
        }
    }
    QString path = action->text();
    QMapIterator<QString, QString> iter(vars_map);
    while (iter.hasNext()) {
        iter.next();
        if (path.contains(iter.key())) {
            path.replace(iter.key(), iter.value());
        }
    }
    path.replace("//", "/");
    QDir syncfolder(path);
    if (!syncfolder.exists()) {
        parent_item->setText(1, tr("NOT FOUND"));
        parent_item->setForeground(1, QBrush(Qt::red));
        return;
    }
    if (do_return) return;
    sync_folder_1 = syncfolder.path();
    sync_folder_2 = destination.path();
    folders_set << sync_folder_1;
    folders_set << sync_folder_2;
    parent_item->setData(1, Qt::UserRole, QVariant(sync_folder_1));
    parent_item->setData(2, Qt::UserRole, QVariant(sync_folder_2));

    dir_filters = QDir::NoDotAndDotDot | QDir::Files;
    if (sync_hidden->isChecked()) { dir_filters |= QDir::Hidden; }
    if (!sync_nosubdirs->isChecked()) { dir_filters |= QDir::AllDirs; }

    if (propagate_deletions->isChecked()) {
        folder_prop_list_map.clear();
        QString sync_folder; //QStringList prop_files_list;
        for (int i = 0; i < 2; ++i) {
            if (i == 0) sync_folder = syncfolder.absolutePath();
            else sync_folder = destination.absolutePath();
            //prop_files_list = getFolderDatabase(sync_folder);
            folder_prop_list_map.insert(sync_folder, getFolderDatabase(sync_folder));
        }
    }
    update_time = (QDateTime::currentDateTime()).toString("yyyy.MM.dd-hh.mm.ss");
    syncing = true; synced_files = 0;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    subAnalyse(folders_set, parent_item);
    parent_item->setText(0, tr("Root directory") + " - " + tr("%1 files and folders need to be synchronised").arg(synced_files));
    QApplication::restoreOverrideCursor();
    syncing = false;
}

void SyncPage::leaveAnalyse()
{
    sync_log_label->setText(tr("<b>Sync log:</b>"));
    logs_stw->setCurrentIndex(0);
}

void SyncPage::analyseFolders()
{
    if (syncing) return;
    QTreeWidgetItem * parent_item = new QTreeWidgetItem (analyse_tree);
    parent_item->setText(0, tr("Root directory"));
    QStringList data0; data0 << ""; data0 << "checked";
    parent_item->setData(0, Qt::UserRole, QVariant(data0));
    MTStringSet folders_set; QDir dir;
    for (int i = 0; i < sync_folders->count(); ++i) {
        dir.setPath(sync_folders->at(i)->path());
        if (!dir.exists()) {
            parent_item->setText(i+1, tr("NOT FOUND"));
            parent_item->setForeground(i+1, QBrush(Qt::red));
        } else folders_set << dir.path();
        parent_item->setData(i+1, Qt::UserRole, QVariant(sync_folders->at(i)->path()));
    }
    parent_item->setIcon(0, QIcon(":/new/prefix1/images/folder_16.png"));
    parent_item->setCheckState(0, Qt::Checked);
    if (ignore_blacklist->isChecked()) parent_item->setDisabled(true);
    parent_item->setExpanded(true);

    extensions.clear();
    if (filters->isChecked()) {
        for (int f = 0; f < lw_filters->count(); ++f) {
            if (lw_filters->item(f)->checkState()==Qt::Checked) {
                for (int l = 0; l < mp_parent->filter_list->count(); ++l) {
                    if (mp_parent->filter_list->item(l)->text()==lw_filters->item(f)->text()) {
                        for (int e = 0; e < ((Filter *)mp_parent->filter_list->item(l))->extensions.count(); ++e) {
                            extensions << QString("*%1").arg(((Filter *)mp_parent->filter_list->item(l))->extensions.at(e));
                        } break;
                    }
                }
            }
        }
    }
    dir_filters = QDir::NoDotAndDotDot | QDir::Files;
    if (sync_hidden->isChecked()) { dir_filters |= QDir::Hidden; }
    if (!sync_nosubdirs->isChecked()) { dir_filters |= QDir::AllDirs; }
    if (propagate_deletions->isChecked() || alert_collisions->isChecked()) {
        folder_prop_list_map.clear(); //QStringList prop_files_list;
        for (int i = 0; i < sync_folders->count(); ++i) {
            //prop_files_list = getFolderDatabase(sync_folders->at(i)->path());
            folder_prop_list_map.insert(sync_folders->at(i)->path(), getFolderDatabase(sync_folders->at(i)->path()));
        }
    }
    syncing = true;
    synced_files = 0;

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    subAnalyse(folders_set, parent_item);
    //subCheckExpanded(parent_item);
    parent_item->setText(0, tr("Root directory") + " - " + tr("%1 files and folders need to be synchronised").arg(synced_files));
    QApplication::restoreOverrideCursor();

    synced_files = 0;
    syncing = false;
}

bool AbstractSyncPage::subAnalyse(const MTStringSet & folders_set, QTreeWidgetItem * parent_item/*, int num*/)
{
    MTStringSet file_names;
    QFileInfoList entries;
    QDir dir;
    for (int i = 0; i < folders_set.count(); ++i) {
        dir = folders_set.at(i);
        if (!dir.exists()) continue;
        if (extensions.count()==0) {
            entries = dir.entryInfoList(dir_filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
        } else {
            entries = dir.entryInfoList(extensions.toList(), dir_filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
        }
        for (int n = 0; n < entries.count(); ++n) {
            //if (!entries.at(n).absoluteFilePath().startsWith(sync_folders_set.at(i))) continue;
            file_names << entries.at(n).fileName();
        }
    }

    bool special = false;
    QTreeWidgetItem * child_item = 0;
    MTFileInfo * file_info = new MTFileInfo;
    QStringList s_folders_list = currentSyncFoldersList();
    for (int i = 0; i < file_names.count(); ++i) {
        if (file_names.at(i) == ".synkron.syncdb") continue;
        int sub_special = 0;
        QString rel_path = QString("%1/%2").arg(parent_item->data(0, Qt::UserRole).toStringList().at(0)).arg(file_names.at(i));
        if (rel_path.isEmpty()) continue;
        bool blacklisted = parent_item->checkState(0) == Qt::Unchecked;
        if (!ignore_blacklist->isChecked()) {
            for (int e = 0; e < exts_blacklist.count(); ++e) {
                if (rel_path.endsWith(exts_blacklist.at(e))) {
                    blacklisted = true;
                    break;
                }
            }
        }
        MTStringSet child_folders_set;
        child_item = new QTreeWidgetItem (parent_item);
        MTEvenDateTime newest_datetime;
        QList<int> newest_indices;
        QList<int> conflicted_indices;
        for (int i = 0; i < s_folders_list.count(); ++i) {
            file_info->setFile(QString("%1%2").arg(s_folders_list.at(i)).arg(rel_path));
            if (!file_info->exists()) {
                child_item->setData(i+1, Qt::UserRole, QVariant(file_info->absoluteFilePath()));
                sub_special++;
                if (propagate_deletions->isChecked()) {
                    if (isInGroupDatabase(file_info->absoluteFilePath())) {
                        child_item->setText(i+1, tr("DELETED"));
                        child_item->setForeground(i+1, QBrush(Qt::darkMagenta));
                        continue;
                    }
                }
                child_item->setText(i+1, tr("NOT FOUND"));
                child_item->setForeground(i+1, QBrush(Qt::red));
                continue;
            }
            if (file_info->isDir() && !file_info->isSymLink()) {
                if (!ignore_blacklist->isChecked()) {
                    /*if (folders_blacklist.contains(file_info->absoluteFilePath(), Qt::CaseInsensitive)) {
			    	    blacklisted = true;
                    }*/
                    for (int b = 0; b < folders_blacklist.count(); ++b) {
                        if (QRegExp(folders_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(file_info->absoluteFilePath())) {
                            blacklisted = true;
                        }
                    }
                }
                child_folders_set << file_info->absoluteFilePath();
            } else {
                if (!ignore_blacklist->isChecked()) {
                    /*if (files_blacklist.contains(file_info->absoluteFilePath(), Qt::CaseInsensitive)) {
                        blacklisted = true;
                    }*/
                    for (int b = 0; b < files_blacklist.count(); ++b) {
                        if (QRegExp(files_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(file_info->absoluteFilePath())) {
                            blacklisted = true;
                        }
                    }
                }
                child_item->setText(i+1, tr("OBSOLETE"));
                child_item->setForeground(i+1, QBrush(Qt::darkRed));
                int compared_dates = file_info->lastModified().compareWith(newest_datetime, allowed_difference);
                if (compared_dates == 0) { // has the same date as the newest_datetime
                    newest_indices << i+1;
                } else if (compared_dates > 0) { // is newer than the newest_datetime
                    newest_datetime = file_info->lastModified();
                    newest_indices.clear();
                    newest_indices << i+1;
                }
                if (compared_dates != 0 && alert_collisions->isChecked()) {
                    if (fileIsDifferentFromDB(file_info->absoluteFilePath())) {
                        conflicted_indices << i+1;
                    }
                }
                child_item->setToolTip(i+1, file_info->lastModified().toString(Qt::ISODate));
            }
            child_item->setData(i+1, Qt::UserRole, QVariant(file_info->absoluteFilePath()));
            
            if (child_item->text(0).isEmpty()) {
                if (file_info->isDir() && !file_info->isSymLink()) {
                    child_item->setIcon(0, QIcon(":/new/prefix1/images/folder_16.png"));
                } else {
                    child_item->setIcon(0, QIcon(":/new/prefix1/images/file.png"));
                }
                child_item->setText(0, file_info->fileName());
            }
        }
        if (newest_indices.count() != 0) { //If is not dir ---------------------
            if (!blacklisted && (newest_indices.count() != s_folders_list.count())) {
                sub_special++;
                child_item->setIcon(0, QIcon(":/new/prefix1/images/file_red.png"));
                synced_files += s_folders_list.count() - newest_indices.count();
            } else {
                if (analyse_special_only->isChecked()) {
                    delete child_item;
                    continue;
                }
            }
            for (int ind = 0; ind < newest_indices.count(); ++ind) {
                child_item->setForeground(newest_indices.at(ind), QBrush(Qt::darkGreen));
                child_item->setText(newest_indices.at(ind), tr("OK"));
            }
        } else if (sub_special && !blacklisted) {
            child_item->setIcon(0, QIcon(":/new/prefix1/images/folder_16_red.png"));
            synced_files += sub_special;
        }
        if (conflicted_indices.count() > 1) {
            for (int ind = 0; ind < conflicted_indices.count(); ++ind) {
                child_item->setText(conflicted_indices.at(ind), tr("COLLISION"));
                child_item->setForeground(conflicted_indices.at(ind), QBrush(Qt::black));
            }
        }
        child_item->setCheckState(0, blacklisted ? Qt::Unchecked : Qt::Checked);
        if (parent_item->checkState(0) == Qt::Unchecked || ignore_blacklist->isChecked()) child_item->setDisabled(true);
        QStringList data0;
        data0 << rel_path;
        if (child_item->checkState(0) == Qt::Checked) data0 << "checked";
        else data0 << "unchecked";
        child_item->setData(0, Qt::UserRole, QVariant(data0));
        if (blacklisted) sub_special = 0;
        //parent_item->addChild(child_item);
        if (child_folders_set.count() != 0) {
            if (fast_analyse->isChecked()) {
                QTreeWidgetItem * loading_item = new QTreeWidgetItem;
                loading_item->setText(0, tr("Loading..."));
                QStringList ld_data;
                ld_data << rel_path;
                ld_data << "unchecked";
                ld_data << "regular";
                loading_item->setData(0, Qt::UserRole, QVariant(ld_data));
                child_item->addChild(loading_item);
                sub_special = 0;
            } else {
                if (subAnalyse(child_folders_set, child_item)) sub_special = 1;
                else if (analyse_special_only->isChecked() && child_item->childCount() == 0 && !sub_special) {
                    delete child_item;
                    continue;
                }
            }
        }
        if (blacklisted) sub_special = 0;
        if (sub_special) {
            parent_item->setIcon(0, QIcon(":/new/prefix1/images/folder_16_red.png"));
            //parent_item->setExpanded(true);
            special = true;
        }
        //child_item->setData(0, Qt::UserRole, QVariant(data0));
    }
    delete file_info;
    return special;
}

/*void SyncPage::subCheckExpanded(QTreeWidgetItem * parent_item)
{
    for (int i = 0; i < parent_item->childCount(); ++i) {
        if (parent_item->child(i)->childCount() != 0) subCheckExpanded(parent_item->child(i));
        if (parent_item->child(i)->data(0, Qt::UserRole).toStringList().at(2) == "special") parent_item->setExpanded(true);
    }
}*/

void AbstractSyncPage::analyseTreeItemExpanded(QTreeWidgetItem * item)
{
    if (fast_analyse->isChecked() && item->isExpanded() && item->childCount() == 1) {
        if (item->child(0)->text(0) == tr("Loading...")) {
            MTStringSet folders_set;
            QString rel_path = item->data(0, Qt::UserRole).toStringList().at(0);
            MTFileInfo * file_info = new MTFileInfo;
            QStringList s_folders_list = currentSyncFoldersList();
            for (int i = 0; i < s_folders_list.count(); ++i) {
                file_info->setFile(QString("%1%2").arg(s_folders_list.at(i)).arg(rel_path));
                if (file_info->exists()) {
                    if (file_info->isDir() && !file_info->isSymLink()) {
                        folders_set << file_info->absoluteFilePath();
                    }
                }
            }
            release(file_info);
            if (folders_set.count() != 0) {
                QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
                subAnalyse(folders_set, item);
                QApplication::restoreOverrideCursor();
            }
            delete item->child(0);
        }
    }
}

void AbstractSyncPage::analyseTreeItemClicked(QTreeWidgetItem * item, int column)
{
    if (column != 0) return;
    if (ignore_blacklist->isChecked()) return;
    if (item->isDisabled()) return;
    bool was_checked = item->data(0, Qt::UserRole).toStringList().at(1) == "checked";
    bool is_checked = item->checkState(0) == Qt::Checked;
    if (was_checked == is_checked) return;
    QStringList data0;
    data0 << item->data(0, Qt::UserRole).toStringList().at(0);
    //data0 <<Â (is_checked ? "checked" : "unchecked");
    if (is_checked) data0 << "checked";
    else data0 << "unchecked";
    item->setData(0, Qt::UserRole, QVariant(data0));
    QString rel_path = data0.at(0);
    QFileInfo * file_info = 0;
    QStringList s_folders_list = currentSyncFoldersList();
    if (is_checked) {
        for (int e = 0; e < exts_blacklist.count(); ++e) {
            if (rel_path.endsWith(exts_blacklist.at(e))) {
                removeExtFromBlacklist(exts_blacklist.at(e));
            }
        }
        for (int i = 0; i < s_folders_list.count(); ++i) {
            release(file_info);
            file_info = new QFileInfo (item->data(i+1, Qt::UserRole).toString());
            if (!file_info->exists()) continue;
            if (file_info->isDir() && !file_info->isSymLink()) {
                /*if (folders_blacklist.contains(file_info->absoluteFilePath())) {
                    removeFolderFromBlacklist(file_info->absoluteFilePath());
                }*/
                for (int b = 0; b < folders_blacklist.count(); ++b) {
                    if (QRegExp(folders_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(file_info->absoluteFilePath())) {
                        removeFolderFromBlacklist(folders_blacklist.at(b));
                    }
                }
            } else {
                /*if (files_blacklist.contains(file_info->absoluteFilePath())) {
                    removeFileFromBlacklist(file_info->absoluteFilePath());
                }*/
                for (int b = 0; b < files_blacklist.count(); ++b) {
                    if (QRegExp(files_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(file_info->absoluteFilePath())) {
                        removeFileFromBlacklist(files_blacklist.at(b));
                    }
                }
            }
            release(file_info);
        }
    } else {
        for (int i = 0; i < s_folders_list.count(); ++i) {
            release(file_info);
            file_info = new QFileInfo (item->data(i+1, Qt::UserRole).toString());
            if (!file_info->exists()) continue;
            if (file_info->isDir() && !file_info->isSymLink()) {
                addFolderToBlacklist(file_info->absoluteFilePath());
            } else {
                addFileToBlacklist(file_info->absoluteFilePath());
            }
            break;
        }
        release(file_info);
    }
    checkAnalyseTree();
}

void AbstractSyncPage::checkAnalyseTree()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    subCheckAnalyseTree(analyseTree()->topLevelItem(0));
    QApplication::restoreOverrideCursor();
}

void AbstractSyncPage::subCheckAnalyseTree(QTreeWidgetItem * parent_item)
{
    QTreeWidgetItem * child_item;
    QFileInfo * file_info = 0;
    QStringList data0;
    QStringList s_folders_list = currentSyncFoldersList();
    for (int i = 0; i < parent_item->childCount(); ++i) {
        child_item = parent_item->child(i);
        bool blacklisted = false;
        data0 = child_item->data(0, Qt::UserRole).toStringList();
        QString rel_path = data0.at(0);
        for (int e = 0; e < exts_blacklist.count(); ++e) {
            if (rel_path.endsWith(exts_blacklist.at(e))) {
                blacklisted = true;
                break;
            }
        }
        if (!blacklisted) {
            for (int n = 0; n < s_folders_list.count(); ++n) {
                release(file_info);
                file_info = new QFileInfo (child_item->data(n+1, Qt::UserRole).toString());
                if (!file_info->exists()) continue;
                if (file_info->isDir() && !file_info->isSymLink()) {
                    /*if (folders_blacklist.contains(file_info->absoluteFilePath())) {
                        blacklisted = true;
                    }*/
                    for (int b = 0; b < folders_blacklist.count(); ++b) {
                        if (QRegExp(folders_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(file_info->absoluteFilePath())) {
                            blacklisted = true;
                        }
                    }
                } else {
                    /*if (files_blacklist.contains(file_info->absoluteFilePath())) {
                        blacklisted = true;
                    }*/
                    for (int b = 0; b < files_blacklist.count(); ++b) {
                        if (QRegExp(files_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(file_info->absoluteFilePath())) {
                            blacklisted = true;
                        }
                    }
                }
            }
            release(file_info);
        }
        if (parent_item->checkState(0) == Qt::Unchecked || ignore_blacklist->isChecked()) {
            blacklisted = true;
            child_item->setDisabled(true);
        } else {
            child_item->setDisabled(false);
        }
        if (ignore_blacklist->isChecked()) {
            blacklisted = false;
        }
        child_item->setCheckState(0, blacklisted ? Qt::Unchecked : Qt::Checked);
        data0.replace(1, blacklisted ? QString("unchecked") : QString("checked"));
        child_item->setData(0, Qt::UserRole, data0);
        if (child_item->childCount() != 0) subCheckAnalyseTree(child_item);
    }
}

void AbstractSyncPage::searchAnalyseTree()
{
    if (logsStw()->currentIndex() != 1) return;
    subSearchAnalyseTree(searchLeText(), analyseTree()->topLevelItem(0));
}

bool AbstractSyncPage::subSearchAnalyseTree(QString text, QTreeWidgetItem * parent_item)
{
    QTreeWidgetItem * child_item;
    bool parent_found = false;
    for (int i = 0; i < parent_item->childCount(); ++i) {
        child_item = parent_item->child(i);
        bool child_found = false;
        if (child_item->childCount() > 0) {
            child_found = subSearchAnalyseTree(text, child_item);
            if (child_found) parent_found = true;
            child_item->setExpanded(child_found);
        }
        if (!child_found && child_item->text(0).contains(text, Qt::CaseInsensitive)) {
            parent_found = true;
        }
    }
    parent_item->setExpanded(parent_found);
    return parent_found;
}

void AbstractSyncPage::analyseTreeItemDoubleClicked(QTreeWidgetItem * item, int column)
{
    if (column == 0) return;
    QDesktopServices::openUrl(QUrl::fromLocalFile(item->data(column, Qt::UserRole).toString()));
}

void AbstractSyncPage::analyseTreeConMenu(QPoint pos)
{
    QMenu * contextMenu = new QMenu(this);
    QAction * open_analyse_action;
    if (analyseTree()->currentColumn() != 0) {
        open_analyse_action = new QAction (tr("Open"), this);
        connect(open_analyse_action, SIGNAL(triggered()), this, SLOT(openAnalyseTreeItem()));
        contextMenu->addAction(open_analyse_action);
        contextMenu->addSeparator();
    }
    
    if (!is_multisync) {
        QAction * sync_analyse_action;
        if (analyseTree()->currentItem()->checkState(0) == Qt::Checked) {
            sync_analyse_action = new QAction (tr("Sync"), this);
            connect(sync_analyse_action, SIGNAL(triggered()), this, SLOT(syncCurrentAnalyseItem()));
            contextMenu->addAction(sync_analyse_action);
        }
    }
    
    QAction * del_analyse_action = new QAction (tr("Delete"), this);
    connect(del_analyse_action, SIGNAL(triggered()), this, SLOT(deleteCurrentAnalyseItem()));
    contextMenu->addAction(del_analyse_action);
    
    QAction * rename_analyse_action = new QAction (tr("Rename"), this);
    connect(rename_analyse_action, SIGNAL(triggered()), this, SLOT(renameCurrentAnalyseItem()));
    contextMenu->addAction(rename_analyse_action);
    
    QAction * bl_analyse_action = new QAction (this);
    if (analyseTree()->currentItem()->checkState(0) == Qt::Checked) {
        bl_analyse_action->setText(tr("Add to blacklist"));
    } else {
        bl_analyse_action->setText(tr("Remove from blacklist"));
    }
    connect(bl_analyse_action, SIGNAL(triggered()), this, SLOT(blacklistCurrentAnalyseItem()));
    contextMenu->addAction(bl_analyse_action);
    
    if (analyseTree()->currentItem()->checkState(0) == Qt::Checked) {
        QAction * bl_ext_analyse_action = new QAction (tr("Add extension to blacklist"), this);
        connect(bl_ext_analyse_action, SIGNAL(triggered()), this, SLOT(blExtCurrentAnalyseItem()));
        contextMenu->addAction(bl_ext_analyse_action);
    }
    
	contextMenu->move(pos);
	contextMenu->show();
}

void AbstractSyncPage::openAnalyseTreeItem()
{
    if (analyseTree()->currentColumn() == 0) return;
    QDesktopServices::openUrl(QUrl::fromLocalFile(analyseTree()->currentItem()
        ->data(analyseTree()->currentColumn(), Qt::UserRole).toString()));
}

void SyncPage::syncCurrentAnalyseItem()
{
    QTreeWidgetItem * item = analyse_tree->currentItem();
    MTStringSet sync_folders;
    MTStringSet rel_paths;
    MTFileInfo * file_info = 0;
    leaveAnalyse();
    bool is_dir = false;
    for (int i = 1; i < analyse_tree->columnCount(); ++i) {
        release(file_info);
        file_info = new MTFileInfo (item->data(i, Qt::UserRole).toString());
        if (file_info->exists()) {
            if (file_info->isDir() && !file_info->isSymLink()) {
                is_dir = true; break;
            }
        }
    }
    for (int i = 1; i < analyse_tree->columnCount(); ++i) {
        release(file_info);
        file_info = new MTFileInfo (item->data(i, Qt::UserRole).toString());
        /*QDir dir = file_info->absoluteDir();
        if (!file_info->isSymLink()){
            if (!dir.exists()) {
                if (!QDir().mkpath(dir.absolutePath())) {
                    addTableItem(tr("%1 Failed to create directory %2").arg(QTime().currentTime().toString("hh:mm:ss")).arg(dir.absolutePath()), "", "", QBrush(Qt::red), QBrush(Qt::white));
                    continue;
                } else {
                    addTableItem(tr("%1 Directory %2 created").arg(QTime().currentTime().toString("hh:mm:ss")).arg(dir.absolutePath()), "", "", QBrush(Qt::darkBlue), QBrush(Qt::white));
                }
            }
        }*/
        /*if (!file_info->exists() && file_info->isDir() && !file_info->isSymLink()) {
            if (!QDir().mkpath(file_info->absoluteFilePath())) {
                addTableItem(tr("%1	Failed to create directory %2").arg(QTime().currentTime().toString("hh:mm:ss")).arg(file_info->absoluteFilePath()), "", "", QBrush(Qt::red), QBrush(Qt::white));
                continue;
            } else {
                addTableItem(tr("%1	Directory %2 created").arg(QTime().currentTime().toString("hh:mm:ss")).arg(file_info->absoluteFilePath()), "", "", QBrush(Qt::darkBlue), QBrush(Qt::white));
            }
        }*/
        if (!file_info->exists()) {
            if (is_dir) {
                if (!QDir().mkpath(file_info->absoluteFilePath())) {
                    addTableItem(tr("%1 Failed to create directory %2").arg(QTime().currentTime().toString("hh:mm:ss")).arg(file_info->absoluteFilePath()), "", "", QBrush(Qt::red), QBrush(Qt::white));
                    continue;
                } else {
                    addTableItem(tr("%1 Directory %2 created").arg(QTime().currentTime().toString("hh:mm:ss")).arg(file_info->absoluteFilePath()), "", "", QBrush(Qt::darkBlue), QBrush(Qt::white));
                }
            } else {
                QDir dir = file_info->dir();
                if (!dir.exists()) {
                    if (!QDir().mkpath(dir.absolutePath())) {
                        addTableItem(tr("%1 Failed to create directory %2").arg(QTime().currentTime().toString("hh:mm:ss")).arg(dir.absolutePath()), "", "", QBrush(Qt::red), QBrush(Qt::white));
                        continue;
                    } else {
                        addTableItem(tr("%1 Directory %2 created").arg(QTime().currentTime().toString("hh:mm:ss")).arg(dir.absolutePath()), "", "", QBrush(Qt::darkBlue), QBrush(Qt::white));
                    }
                }
            }
        }
        if (is_dir) {
            sync_folders << file_info->absoluteFilePath();
        } else {
            rel_paths << file_info->fileName();
            sync_folders << file_info->dir().path();
        }
    }
    release(file_info);
    if (sync_folders.count() != 0) {
        if (rel_paths.count() == 0) {
            sync(sync_folders);
        } else {
            setSyncEnabled(false);
                subGroupSync(sync_folders, rel_paths);
            setSyncEnabled(true);
            if (propagate_deletions->isChecked()) saveAllFolderDatabases();
            addTableItem(tr("%1	Synchronisation complete: %2 file(s) %3").arg(QTime().currentTime().toString("hh:mm:ss")).arg(synced_files).arg(tr("synchronised")), "", "", QBrush(Qt::green));
		    mp_parent->showTrayMessage(tr("Synchronisation complete"), tr("%1 files %2").arg(synced_files).arg(tr("synchronised")));
            synced_files = 0;
        }
    }
}

void AbstractSyncPage::deleteCurrentAnalyseItem()
{
    QTreeWidgetItem * item = analyseTree()->currentItem();
    QMessageBox msgBox; msgBox.setText(tr("Are you sure you want to remove \"%1\" from every synced location?").arg(item->text(0)));
	msgBox.setWindowTitle(QString("Synkron")); msgBox.setIcon(QMessageBox::Question);
 	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    QFileInfo * file_info = 0;
    bool backup = true;
    bool sub_backup = true;
    QStringList s_folders_list = currentSyncFoldersList();
 	
 	switch (msgBox.exec()) {
 	case QMessageBox::Yes:
        leaveAnalyse();
        update_time = (QDateTime::currentDateTime()).toString("yyyy.MM.dd-hh.mm.ss");
        if (backupFolders()) backup = false;
        for (int i = 1; i < analyseTree()->columnCount(); ++i) {
            sub_backup = backup;
            release(file_info);
            file_info = new QFileInfo (item->data(i, Qt::UserRole).toString());
            if (sub_backup && s_folders_list.count() < 3) {
                if (file_info->absoluteFilePath().startsWith(s_folders_list.at(0)) && backup_folder_1->isChecked()) {
                    sub_backup = false;
                } else if (file_info->absoluteFilePath().startsWith(s_folders_list.at(1)) && backup_folder_2->isChecked()) {
                    sub_backup = false;
                }
            }
            if (!file_info->exists()) continue;
            if (file_info->isDir() && !file_info->isSymLink()) {
                syncing = true;
                backupAndRemoveDir(file_info->absoluteFilePath(), sub_backup);
                syncing = false;
            } else {
                backupAndRemoveFile(*file_info, sub_backup);
            }
        }
        release(file_info);
        break;
 	case QMessageBox::No:
     	break;
	default:
   		break;
 	}
}

void AbstractSyncPage::renameCurrentAnalyseItem()
{
    QTreeWidgetItem * item = analyseTree()->currentItem();
    bool ok = false;
    QStringList s_folders_list = currentSyncFoldersList();
    QFileInfo * file_info = 0;
    for (int i = 0; i < s_folders_list.count(); ++i) {
        file_info = new QFileInfo (item->data(i+1, Qt::UserRole).toString());
        if (file_info->exists()) break;
        else release(file_info);
    }
    if (!file_info->exists()) return;
    QString new_name = QInputDialog::getText(this, tr("Synkron - Rename file"), tr("Type a new name for \"%1\":").arg(file_info->fileName()), QLineEdit::Normal, file_info->fileName(), &ok);
    if (!ok) return;
    leaveAnalyse();
    for (int i = 1; i < analyseTree()->columnCount(); ++i) {
        release(file_info);
        file_info = new QFileInfo (item->data(i, Qt::UserRole).toString());
        if (!file_info->exists()) continue;
        if (file_info->isDir() && !file_info->isSymLink()) {
            QDir dir (file_info->absoluteFilePath());
            QString dirname = dir.dirName();
            dir.cdUp();
            if (!dir.rename(dirname, new_name)) {
                addTableItem(tr("Error renaming directory %1").arg(file_info->absoluteFilePath()), "", "", QBrush(Qt::red), QBrush(Qt::white));
            } else {
                addTableItem(tr("File %1 renamed").arg(file_info->absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
            }
        } else {
            QFile file (file_info->absoluteFilePath());
            if (!file.rename(QString("%1/%2").arg(file_info->dir().path()).arg(new_name))) {
                addTableItem(tr("Error renaming file %1").arg(file_info->absoluteFilePath()), "", "", QBrush(Qt::red), QBrush(Qt::white));
            } else {
                addTableItem(tr("File %1 renamed").arg(file_info->absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
            }
        }
	}
    release(file_info);
}

void AbstractSyncPage::blacklistCurrentAnalyseItem()
{
    analyseTree()->currentItem()->setCheckState(0, analyseTree()->currentItem()->checkState(0) == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    analyseTreeItemClicked(analyseTree()->currentItem(), 0);
}

void AbstractSyncPage::blExtCurrentAnalyseItem()
{
    addExtToBlacklist(analyseTree()->currentItem()->text(0).split(".").last().split(" ").last());
    checkAnalyseTree();
}
