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

MultisyncPage::MultisyncPage(MainWindow *parent) : AbstractSyncPage(parent)
{
	setupUi(this);
	
	stop_sync_btn->setVisible(false);
	
	setAdvancedGB();
}

void MultisyncPage::setAdvancedGB()
{
    QVBoxLayout * column1_layout  = new QVBoxLayout (advanced);
    QVBoxLayout * column2_layout  = new QVBoxLayout (advanced);
    
	sync_hidden = new QCheckBox(advanced);
    sync_hidden->setChecked(true);
    sync_hidden->setStatusTip(tr("Synchronise hidden files and folders"));
    sync_hidden->setText(tr("Synchronise hidden files and folders"));
	column1_layout->addWidget(sync_hidden);
	
    propagate_deletions = new QCheckBox (advanced);
    propagate_deletions->setChecked(false);
    propagate_deletions->setStatusTip(tr("Propagate deletions"));
    propagate_deletions->setText(tr("Propagate deletions"));
    connect(propagate_deletions, SIGNAL(toggled(bool)), this, SLOT(propagatedStateChanged(bool)));
    connect(propagate_deletions, SIGNAL(clicked(bool)), this, SLOT(propagatedClicked(bool)));
	column2_layout->addWidget(propagate_deletions);
    
	sync_nosubdirs = new QCheckBox(advanced);
    sync_nosubdirs->setChecked(false);
    sync_nosubdirs->setStatusTip(tr("Do not synchronise subdirectories"));
    sync_nosubdirs->setText(tr("Do not synchronise subdirectories"));
	column1_layout->addWidget(sync_nosubdirs);
	
    QHBoxLayout * bl_layout = new QHBoxLayout (advanced);
    ignore_blacklist = new QCheckBox(advanced);
    ignore_blacklist->setChecked(false);
    ignore_blacklist->setStatusTip(tr("Ignore blacklist"));
    ignore_blacklist->setText(tr("Ignore blacklist"));
    bl_layout->addWidget(ignore_blacklist);
    bl_layout->addItem(new QSpacerItem(10, 5, QSizePolicy::Fixed, QSizePolicy::Fixed));
    
    edit_blacklist = new QToolButton(advanced);
    edit_blacklist->setText(tr("Edit blacklist"));
    edit_blacklist->setStatusTip(tr("Edit blacklist for this tab"));
    connect(edit_blacklist, SIGNAL(released()), this, SLOT(editBlacklist()));
    bl_layout->addWidget(edit_blacklist);
	column2_layout->addLayout(bl_layout);
	
	/*backup_folders = new QCheckBox (tr("Do not backup updated files"), advanced);
	backup_folders->setStatusTip(tr("Do not backup updated files"));
	connect(backup_folders, SIGNAL(toggled(bool)), this, SLOT(backupFoldersStateChanged(bool)));
	column1_layout->addWidget(backup_folders);
	
    update_only = new QCheckBox (tr("Update existing files only"), advanced);
	update_only->setStatusTip(tr("Update existing files only"));
	connect(update_only, SIGNAL(toggled(bool)), this, SLOT(updateOnlyStateChanged(bool)));
	column2_layout->addWidget(update_only);*/
    
    files_blacklist = mp_parent->files_blacklist;
    folders_blacklist = mp_parent->folders_blacklist;
    exts_blacklist = mp_parent->exts_blacklist;
    
    symlinks = new QCheckBox;
#ifdef Q_WS_WIN
    symlinks->setChecked(false);
#else
	symlinks->setParent(advanced);
	symlinks->setChecked(false);
	symlinks->setStatusTip(tr("Follow symbolic links"));
    symlinks->setText(tr("Follow symbolic links"));
    column1_layout->addWidget(symlinks);
    QLabel * blank_label1 = new QLabel("", advanced);
    column2_layout->addWidget(blank_label1);
#endif
    
    QLabel * folder1_label = new QLabel (advanced);
    folder1_label->setText(tr("<b>Sources:</b>"));
    column1_layout->addWidget(folder1_label);
    QLabel * blank_label2 = new QLabel("", advanced);
    column2_layout->addWidget(blank_label2);
    
	backup_folder_1 = new QCheckBox (tr("Do not backup updated files"), advanced);
	backup_folder_1->setStatusTip(tr("Do not backup updated files from sources"));
	column1_layout->addWidget(backup_folder_1);
	
    update_only_1 = new QCheckBox (tr("Update existing files only"), advanced);
	update_only_1->setStatusTip(tr("Update existing files in sources only"));
	column2_layout->addWidget(update_only_1);
    QLabel * blank_label3 = new QLabel("", advanced);
    column2_layout->addWidget(blank_label3);
	
	move = new QCheckBox (tr("Move contents to destination, leaving sources empty"), advanced);
	move->setStatusTip(tr("Move contents to destination, leaving sources empty"));
	connect(move, SIGNAL(toggled(bool)), this, SLOT(moveStateChanged(bool)));
	column1_layout->addWidget(move);
    QLabel * blank_label4 = new QLabel("", advanced);
    column2_layout->addWidget(blank_label4);
	
	QLabel * folder2_label = new QLabel (advanced);
    folder2_label->setText(tr("<b>Destination:</b>"));
    column1_layout->addWidget(folder2_label);
    QLabel * blank_label5 = new QLabel("", advanced);
    column2_layout->addWidget(blank_label5);
    
	backup_folder_2 = new QCheckBox (tr("Do not backup updated files"), advanced);
	backup_folder_2->setStatusTip(tr("Do not backup updated files from destination"));
	column1_layout->addWidget(backup_folder_2);
	
    update_only_2 = new QCheckBox (tr("Update existing files only"), advanced);
	update_only_2->setStatusTip(tr("Update existing files in destination only"));
	column2_layout->addWidget(update_only_2);
	
	clone_folder1 = new QCheckBox (tr("Clone sources"), advanced);
	clone_folder1->setStatusTip(tr("Clone sources"));
	connect(clone_folder1, SIGNAL(toggled(bool)), this, SLOT(cloneStateChanged(bool)));
	column1_layout->addWidget(clone_folder1);
	QLabel * blank_label6 = new QLabel("", advanced);
    column2_layout->addWidget(blank_label6);
    
    /*advanced->gridLayout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 1);
	filters = new QGroupBox(tr("Filters"), advanced);
	filters->setCheckable(true);
	filters->setChecked(false);
	filters->setMaximumWidth(150);
	QVBoxLayout * vlayout_filters = new QVBoxLayout(filters);
#ifndef Q_WS_MAC
	vlayout_filters->setContentsMargins(9, 6, 9, 9);
#endif
	lw_filters = new QListWidget(filters);
	QListWidgetItem * item;
    for (int f = 0; f < mp_parent->filter_list->count(); ++f) {
		item = new QListWidgetItem (mp_parent->filter_list->item(f)->text());
		item->setCheckState(Qt::Unchecked);
		lw_filters->addItem(item);
	}
	vlayout_filters->addWidget(lw_filters);*/
	
	advanced->addLayout(column1_layout, 0, 1);
	advanced->addLayout(column2_layout, 0, 2);
	advanced->addItem(new QSpacerItem(10, 5, QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 3);
	//advanced->addWidget(page->filters, 0, 4);
	//mainglayout->addWidget(advanced, 7, 0);
}

MultisyncPage * MainWindow::addMultiTab()
{
	MultisyncPage * multi_page = new MultisyncPage(this);
    multi_page->blacklist = new QWidget ();
    multi_page->tab_stw->addWidget(multi_page->blacklist);
    multi_page->setBlacklistWidget();
    
	QString title;
    int n = 1; gen_title:
    title = tr("Multisync #%1").arg(n); bool ok = true;
    for (int i = 0; i < multi_tabWidget->count(); ++i) {
        if (multi_tabWidget->tabText(i) == title) { ok = false; }
    }
    if (!ok) { n++; goto gen_title; }
    multi_tabWidget->addTab(multi_page, QIcon(QString::fromUtf8(":/new/prefix1/images/multisync.png")), title);
    
	multi_page->tw_multi->setHorizontalHeaderLabels(QStringList() << tr("Source") << tr("Destination"));
    multi_page->tw_multi->verticalHeader()->hide();
    multi_page->tw_multi->setShowGrid(false);
    multi_page->tw_multi->setStatusTip(tr("List of synchronised files and folders"));
    multi_page->tw_multi->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	
	connect(multi_page->add_multi, SIGNAL(released()), this, SLOT(addMultisync()));
	connect(multi_page->remove_multi, SIGNAL(released()), this, SLOT(removeMultisync()));
	connect(multi_page->browse_multi, SIGNAL(released()), this, SLOT(browseMultiDestination()));
	connect(multi_page->sync_multi, SIGNAL(released()), multi_page, SLOT(sync()));
	connect(multi_page->stop_sync_btn, SIGNAL(released()), multi_page, SLOT(stopSync()));
	connect(multi_page->search_multi, SIGNAL(textEdited(const QString)), this, SLOT(searchTw(QString)));
	connect(multi_page->save_multi, SIGNAL(released()), multi_page, SLOT(saveMultisync()));
	connect(multi_page->saveas_multi, SIGNAL(released()), multi_page, SLOT(saveAsMultisync()));
	connect(multi_page->load_multi, SIGNAL(released()), multi_page, SLOT(loadMultisync()));
	connect(multi_page->destination_multi, SIGNAL(editingFinished()), multi_page, SLOT(destinationTextChanged()));
	connect(multi_page->tab_name, SIGNAL(editingFinished()), multi_page, SLOT(multitabNameChanged()));
	
	multi_page->tab_name->setText(multi_tabWidget->tabText(multi_tabWidget->indexOf(multi_page)));
	
	multi_tabWidget->setCurrentIndex(multi_tabWidget->indexOf(multi_page));
    
	return multi_page;
}

void MainWindow::addMultisync()
{
	QListWidgetItem * item = new QListWidgetItem();
	QString path = QFileDialog::getExistingDirectory(this,
                "Choose a directory",
                QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty()) return;
	if (path.startsWith(QDir::homePath(), Qt::CaseInsensitive)) {
		path.replace(QDir::homePath(), "HOMEPATH");
	} else if (path.startsWith(QDir::rootPath(), Qt::CaseInsensitive)) {
		path.replace(QDir::rootPath(), "ROOTPATH/");
	}
	path.replace("\\", "/");
	item->setText(path);
	item->setCheckState(Qt::Checked);
	((MultisyncPage *)multi_tabWidget->currentWidget())->list_multi->addItem(item);
}

void MainWindow::removeMultisync()
{
	if (((MultisyncPage *)multi_tabWidget->currentWidget())->list_multi->currentItem()==0) { QMessageBox::warning(this, tr("Synkron"), tr("No source selected.")); return; }
	QMessageBox msgBox; msgBox.setText(tr("Are you sure you want to remove the selected source from the list?"));
	msgBox.setWindowTitle(QString("Synkron")); msgBox.setIcon(QMessageBox::Question);
 	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
 	switch (msgBox.exec()) {
 	case QMessageBox::Yes:
		delete ((MultisyncPage *)multi_tabWidget->currentWidget())->list_multi->currentItem();
   		break;
 	case QMessageBox::No:
		break;
	default:
   		break;
 	}
}

void MainWindow::browseMultiDestination()
{
	((MultisyncPage *)multi_tabWidget->currentWidget())->destination_multi->setText(QFileDialog::getExistingDirectory(
                this,
                "Choose a directory",
                QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}

int MultisyncPage::sync()
{
	if (syncing) return 0;
	if (destination_multi->text()=="") { addTableItem(tr("%1    Synchronisation failed: Choose a destination first").arg(QTime().currentTime().toString("hh:mm:ss")), "", "", QBrush(Qt::red), QBrush(Qt::white)); return 0; }
	bool found = false;
	for (int n = 0; n < list_multi->count(); ++n) {
		if (list_multi->item(n)->checkState()==Qt::Checked) {
			found = true; break;
		}
	}
	if (!found) { QMessageBox::information(mp_parent, tr("Synkron"), tr("No sources selected.")); return 0; }
	setMultisyncEnabled(false); QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	syncing = true;
    extensions.clear();
	/*if (filters->isChecked()) {
		for (int f = 0; f < lw_filters->count(); ++f) {
			if (lw_filters->item(f)->checkState()==Qt::Checked) {
				for (int l = 0; l < mp_parent->filter_list->count(); ++l) {
					if (mp_parent->filter_list->item(l)->text() == lw_filters->item(f)->text()) {
						for (int e = 0; e < ((Filter *)mp_parent->filter_list->item(l))->extensions.count(); ++e) {
							extensions << ((Filter *)mp_parent->filter_list->item(l))->extensions.at(e);
						} break;
					}
				}
			}
		}
	}*/
	dir_filters = QDir::NoDotAndDotDot | QDir::Files;
    if (sync_hidden->isChecked()) { dir_filters |= QDir::Hidden; }
    if (!sync_nosubdirs->isChecked()) { dir_filters |= QDir::AllDirs; }
    
	QStringList pathlist; QString path;
	QDir destination; QDir syncfolder; int all_synced_files = 0;
	for (int i = 0; i < list_multi->count(); ++i) {
		if (list_multi->item(i)->checkState()==Qt::Unchecked) continue;
		destination.setPath(destination_multi->text());
		if (!destination.exists()) {
            if (!QDir().mkpath(destination.path())) {
                addTableItem(tr("%1	Synchronisation failed: Failed to create directory %2").arg(QTime().currentTime().toString("hh:mm:ss")).arg(destination.path()), "", "", QBrush(Qt::red), QBrush(Qt::white));
                setMultisyncEnabled(true); QApplication::restoreOverrideCursor(); return 0;
            } else {
                addTableItem(tr("%1	Directory %2 created").arg(QTime().currentTime().toString("hh:mm:ss")).arg(destination.path()), "", "", QBrush(Qt::darkBlue), QBrush(Qt::white));
            }
        }
		synced_files = 0;
		pathlist = list_multi->item(i)->text().split("/");
		QString s = pathlist.at(0);
		pathlist[0] = s.remove(":");
		for (int v = 0; v < pathlist.count(); ++v) {
			if (!destination.cd(pathlist.at(v))) {
				if (!destination.mkdir(pathlist.at(v))) {
				    addTableItem(tr("%1	Synchronisation failed: Error creating directory in %2").arg(QTime().currentTime().toString("hh:mm:ss").arg(destination.path())), "", "", QBrush(Qt::red), QBrush(Qt::white));
					setMultisyncEnabled(true); QApplication::restoreOverrideCursor(); return 0;
				}
				destination.cd(pathlist.at(v));
			}
		}
		path = list_multi->item(i)->text();
        if (path.startsWith("HOMEPATH", Qt::CaseSensitive)) {
			path.replace("HOMEPATH", QDir::homePath());
		} else if (path.startsWith("ROOTPATH", Qt::CaseSensitive)) {
			path.replace("ROOTPATH", QDir::rootPath());
		}
		syncfolder.setPath(path);
		if (!syncfolder.exists()) {
            if (!QDir().mkpath(syncfolder.path())) {
                addTableItem(tr("%1	Synchronisation failed: Failed to create directory %2").arg(QTime().currentTime().toString("hh:mm:ss")).arg(syncfolder.path()), "", "", QBrush(Qt::red), QBrush(Qt::white));
                continue;
            } else {
                addTableItem(tr("%1	Directory %2 created").arg(QTime().currentTime().toString("hh:mm:ss")).arg(syncfolder.path()), "", "", QBrush(Qt::darkBlue), QBrush(Qt::white));
            }
        }
    	sync_folder_1 = syncfolder.path();
    	sync_folder_2 = destination.path();
    	if (propagate_deletions->isChecked()) {
            folder_prop_list_map.clear();
            QString sync_folder;
            for (int i = 0; i < 2; ++i) {
                if (i == 0) sync_folder = syncfolder.absolutePath();
                else sync_folder = destination.absolutePath();
                QStringList prop_files_list;
                QFile file(QString("%1/%2").arg(sync_folder).arg(".synkron.syncdb"));
                if (!file.exists()) continue;
                if (!file.open(QFile::ReadOnly | QFile::Text)) {
		          //QMessageBox::critical(this, tr("Save database"), tr("Cannot write file %1:\n%2.").arg(db_file_name).arg(file.errorString()));
		          continue;
                }
                QTextStream in(&file);
                in.setCodec("UTF-8");
                while (!in.atEnd()) { prop_files_list << in.readLine(); }
                folder_prop_list_map.insert(sync_folder, prop_files_list);
            }
        }
    	update_time = (QDateTime::currentDateTime()).toString("yyyy.MM.dd-hh.mm.ss");
		if (move->isChecked()) moveContents(syncfolder, destination);
		else subSync(syncfolder, destination, false);
		countExtsBl();
		if (propagate_deletions->isChecked()) {
            saveFolderDatabase(syncfolder.absolutePath());
            saveFolderDatabase(destination.absolutePath());
        }
        all_synced_files += synced_files;
		addTableItem(tr("%1	%2: %3 file(s) %4").arg(QTime().currentTime().toString("hh:mm:ss")).arg(list_multi->item(i)->text()).arg(synced_files).arg(move->isChecked() ? tr("moved") : tr("synchronised")), "", "", QBrush(Qt::green));
	}
	mp_parent->saveSettings();
	syncing = false;
    setMultisyncEnabled(true); QApplication::restoreOverrideCursor();
	if (!mp_parent->syncingAll) {
	    mp_parent->showTrayMessage(tr("Synchronisation complete"), tr("%1 file(s) %2").arg(all_synced_files).arg(move->isChecked() ? tr("moved") : tr("synchronised")));
	}
	return all_synced_files;
}

void MultisyncPage::saveMultisync()
{
	if (slist_path == "") {
		saveAsMultisync(); return;
	}
	saveAsMultisync(slist_path);
}

void MultisyncPage::saveAsMultisync()
{
	QString file_name = QFileDialog::getSaveFileName(this,
                tr("Synkron - Save Multisync"),
                QString("%1/multisync.slist").arg(destination_multi->text()),
                tr("Synkron Multisyncs (*.slist)"));
    saveAsMultisync(file_name);
}

void MultisyncPage::saveAsMultisync(QString file_name)
{
	if (file_name.isEmpty()) { return; }
	QDomDocument domdoc("SynkronMultisync");
    QDomElement el_sync = domdoc.createElement("sync");
    domdoc.appendChild(el_sync);
	QStringList source_list; QString checkstates;
	for (int i = 0; i < list_multi->count(); ++i) {
		source_list << list_multi->item(i)->text();
		checkstates.append(list_multi->item(i)->checkState() == Qt::Checked ? "+" : "-");
	}
    QString dest = destination_multi->text();
    QDomElement el_sources = domdoc.createElement("sources");
    el_sources.setAttribute("paths", source_list.join("|"));
    el_sources.setAttribute("checkstates", checkstates);
    el_sync.appendChild(el_sources);
    QDomElement el_destination = domdoc.createElement("destination");
    el_destination.setAttribute("data", dest);
    el_sync.appendChild(el_destination);
    QDomElement el_name = domdoc.createElement("name");
    el_name.setAttribute("data", tab_name->text());
    el_sync.appendChild(el_name);
    QFile file(file_name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::critical(this, tr("Synkron"), tr("Cannot write file %1:\n%2.").arg(file_name).arg(file.errorString()));
		return;
	}
	QTextStream out(&file);
	out << domdoc.toString();
	slist_path = file_name;
}

void MultisyncPage::loadMultisync()
{
	QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                QDir::homePath(),
                                                tr("Synkron Multisyncs (*.slist)"));
    loadMultisync(file_name);
}

void MultisyncPage::loadMultisync(QString file_name)
{
    if (file_name.isEmpty()) { return; }
	QFile file(file_name);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::critical(this, tr("Synkron"), tr("Cannot read file %1:\n%2.").arg(file_name).arg(file.errorString()));
		return;
	}
	QTextStream in(&file);
    QDomDocument domdoc;
    domdoc.setContent(in.readAll());
    QDomElement el_sync = domdoc.firstChildElement("sync");
    QDomElement el_sources = el_sync.firstChildElement("sources");
    QStringList source_list = el_sources.attribute("paths").split('|');
    list_multi->clear();
    QListWidgetItem * item;
	for (int i = 0; i < source_list.count(); ++i) {
		if (source_list.at(i)=="") continue;
		item = new QListWidgetItem (source_list.at(i));
		item->setCheckState(el_sources.attribute("checkstates").at(i) == '+' ? Qt::Checked : Qt::Unchecked);
		list_multi->addItem(item);
	}
	QDomElement el_destination = el_sync.firstChildElement("destination");
	if (el_destination.attribute("data")!="") {
		destination_multi->setText(el_destination.attribute("data"));
	}
	QDomElement el_name = el_sync.firstChildElement("name");
	if (el_name.attribute("data")!="") {
		tab_name->setText(el_name.attribute("data"));
		multitabNameChanged();
	}
	slist_path = file_name;
}

void MultisyncPage::setMultisyncEnabled(bool enable)
{
	list_multi->setEnabled(enable);
	add_multi->setEnabled(enable);
	remove_multi->setEnabled(enable);
	destination_multi->setEnabled(enable);
	browse_multi->setEnabled(enable);
	load_multi->setEnabled(enable);
	saveas_multi->setEnabled(enable);
	save_multi->setEnabled(enable);
	search_multi->setEnabled(enable);
	advanced->setEnabled(enable);
	syncing = !enable;
	stop_sync_btn->setVisible(!enable);
    sync_multi->setVisible(enable);
    qApp->processEvents();
}

void MultisyncPage::multitabNameChanged()
{
	QMapIterator<QTableWidgetItem*, SyncSchedule*>  i(mp_parent->item_sched_map);
	while (i.hasNext()) {
		i.next();
		for (int n = 0; n < i.value()->sched_multitab_list.count(); ++n) {
			if (mp_parent->multi_tabWidget->tabText(mp_parent->multi_tabWidget->indexOf(this))==i.value()->sched_multitab_list.at(n)) {
				i.value()->sched_multitab_list[n] = tab_name->text();
			}
		}
	}
    mp_parent->multi_tabWidget->setTabText(mp_parent->multi_tabWidget->indexOf(this), tab_name->text());
}

void MultisyncPage::moveChecked(bool checked)
{
	if (checked) {
		sync_multi->setText(tr("Move contents"));
		sync_multi->setStatusTip(tr("Move contents to destination, leaving sources empty"));
	} else {
		sync_multi->setText(tr("Multisync"));
		sync_multi->setStatusTip(tr("Start multisync"));
	}
}

void MultisyncPage::cloneChecked(bool checked)
{
	if (checked) {
		sync_multi->setText(tr("Clone sources"));
		sync_multi->setStatusTip(tr("Clone sources"));
	} else {
		sync_multi->setText(tr("Multisync"));
		sync_multi->setStatusTip(tr("Start multisync"));
	}
}

void MultisyncPage::showThisPage()
{
    mp_parent->multi_tabWidget->setCurrentWidget(this);
    mp_parent->actionMultisync->trigger();
}

void MultisyncPage::deleteAllFolderDatabases()
{
    for (int i = 0; i < list_multi->count(); ++i) {
		if (list_multi->item(i)->checkState()==Qt::Unchecked) continue;
		QString path = list_multi->item(i)->text();
        if (path.startsWith("HOMEPATH", Qt::CaseSensitive)) {
			path.replace("HOMEPATH", QDir::homePath());
		} else if (path.startsWith("ROOTPATH", Qt::CaseSensitive)) {
			path.replace("ROOTPATH", QDir::rootPath());
		}
		deleteFolderDatabase(path);
    }
    deleteFolderDatabase(destination_multi->text());
}

void MultisyncPage::saveAllFolderDatabases()
{
    for (int i = 0; i < list_multi->count(); ++i) {
		if (list_multi->item(i)->checkState()==Qt::Unchecked) continue;
		QString path = list_multi->item(i)->text();
        if (path.startsWith("HOMEPATH", Qt::CaseSensitive)) {
			path.replace("HOMEPATH", QDir::homePath());
		} else if (path.startsWith("ROOTPATH", Qt::CaseSensitive)) {
			path.replace("ROOTPATH", QDir::rootPath());
		}
		saveFolderDatabase(path);
    }
    saveFolderDatabase(destination_multi->text());
}
