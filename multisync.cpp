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
	is_multisync = true;

	stop_sync_btn->setVisible(false);

    analyse_con_menu = new QMenu;
    connect(analyse_con_menu, SIGNAL(triggered(QAction *)), this, SLOT(goToAnalyse(QAction *)));
    connect(analyse_con_menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowAnalyseMenu()));
    go_to_analyse->setMenu(analyse_con_menu);
    go_to_analyse->setMinimumWidth(go_to_analyse->sizeHint().width() + 10);
    connect(analyse_tree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(analyseTreeItemClicked(QTreeWidgetItem *, int)));
    connect(analyse_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(analyseTreeItemDoubleClicked(QTreeWidgetItem *, int)));
    connect(analyse_tree, SIGNAL(sigconmenu(QPoint)), this, SLOT(analyseTreeConMenu(QPoint)));
    connect(analyse_tree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(analyseTreeItemExpanded(QTreeWidgetItem *)));

    status_table_item = new QTableWidgetItem(tr("Press the \"Multisync\" button to start synchronisation"));
    tw_multi->insertRow(0);
    status_table_item->setBackground(Qt::blue);
    status_table_item->setForeground(Qt::white);
    tw_multi->setSpan(0, 0, 1, 2);
	tw_multi->setItem(0, 0, status_table_item);
	tw_multi->setRowHeight(0, 16);
	tw_multi->setTextElideMode(Qt::ElideMiddle);

    setAdvancedGB();

    vars_map.insert("HOMEPATH", QDir::homePath());
    vars_map.insert("ROOTPATH", QDir::rootPath());
}

void MultisyncPage::setAdvancedGB()
{
    advanced = new MTAdvancedGroupBox(chb_advanced, this);
    advanced_layout->addWidget(advanced);
    QGridLayout * main_layout = new QGridLayout;
    QSpacerItem * sitem = new QSpacerItem(1000, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    main_layout->addItem(sitem, 0, 1);
    main_layout->addItem(sitem, 0, 3);

	sync_hidden = new QCheckBox(advanced);
    sync_hidden->setChecked(true);
    sync_hidden->setStatusTip(tr("Synchronise hidden files and folders"));
    sync_hidden->setText(tr("Synchronise hidden files and folders"));
    main_layout->addWidget(sync_hidden, 0, 0);

    propagate_deletions = new QCheckBox (advanced);
    propagate_deletions->setChecked(false);
    propagate_deletions->setStatusTip(tr("Propagate deletions"));
    propagate_deletions->setText(tr("Propagate deletions"));
    connect(propagate_deletions, SIGNAL(toggled(bool)), this, SLOT(propagatedStateChanged(bool)));
    connect(propagate_deletions, SIGNAL(clicked(bool)), this, SLOT(propagatedClicked(bool)));
    main_layout->addWidget(propagate_deletions, 0, 2);

	sync_nosubdirs = new QCheckBox(advanced);
    sync_nosubdirs->setChecked(false);
    sync_nosubdirs->setStatusTip(tr("Do not synchronise subdirectories"));
    sync_nosubdirs->setText(tr("Do not synchronise subdirectories"));
    main_layout->addWidget(sync_nosubdirs, 1, 0);

    ignore_blacklist = new QCheckBox(advanced);
    ignore_blacklist->setChecked(false);
    ignore_blacklist->setStatusTip(tr("Ignore blacklist"));
    ignore_blacklist->setText(tr("Ignore blacklist"));
    main_layout->addWidget(ignore_blacklist, 1, 2);

    alert_collisions = new QCheckBox(advanced);
    alert_collisions->setText(tr("Detect collisions"));
    alert_collisions->setStatusTip(tr("Detect and alert collided files"));
    connect(alert_collisions, SIGNAL(clicked(bool)), this, SLOT(propagatedStateChanged(bool)));
    connect(alert_collisions, SIGNAL(clicked(bool)), this, SLOT(propagatedClicked(bool)));
    main_layout->addWidget(alert_collisions, 0, 4);

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
    main_layout->addWidget(symlinks, 1, 4);
#endif

    QLabel * folder1_label = new QLabel (advanced);
    folder1_label->setText(tr("<b>Sources:</b>"));
    main_layout->addWidget(folder1_label, 3, 0);

	backup_folder_1 = new QCheckBox (tr("Do not backup updated files"), advanced);
	backup_folder_1->setStatusTip(tr("Do not backup updated files from sources"));
    main_layout->addWidget(backup_folder_1, 4, 0);

    update_only_1 = new QCheckBox (tr("Update existing files only"), advanced);
	update_only_1->setStatusTip(tr("Update existing files in sources only"));
    main_layout->addWidget(update_only_1, 4, 2);

	move = new QCheckBox (tr("Move contents to destination, leaving sources empty"), advanced);
	move->setStatusTip(tr("Move contents to destination, leaving sources empty"));
	connect(move, SIGNAL(toggled(bool)), this, SLOT(moveStateChanged(bool)));
    main_layout->addWidget(move, 4, 4);

	QLabel * folder2_label = new QLabel (advanced);
    folder2_label->setText(tr("<b>Destination:</b>"));
    main_layout->addWidget(folder2_label, 5, 0);

	backup_folder_2 = new QCheckBox (tr("Do not backup updated files"), advanced);
	backup_folder_2->setStatusTip(tr("Do not backup updated files from destination"));
    main_layout->addWidget(backup_folder_2, 6, 0);

    update_only_2 = new QCheckBox (tr("Update existing files only"), advanced);
	update_only_2->setStatusTip(tr("Update existing files in destination only"));
    main_layout->addWidget(update_only_2, 6, 2);

	clone_folder1 = new QCheckBox (tr("Clone sources"), advanced);
	clone_folder1->setStatusTip(tr("Clone sources"));
	connect(clone_folder1, SIGNAL(toggled(bool)), this, SLOT(cloneStateChanged(bool)));
    main_layout->addWidget(clone_folder1, 6, 4);

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

    QLabel * analyse_label = new QLabel (advanced);
    analyse_label->setText(tr("<b>Analyse:</b>"));
    main_layout->addWidget(analyse_label, 7, 0);
	fast_analyse = new QCheckBox;
    fast_analyse->setText(tr("Fast analysis"));
    main_layout->addWidget(fast_analyse, 8, 0);
    analyse_special_only = new QCheckBox;
    analyse_special_only->setText(tr("List files which need to be synchronised only"));
    main_layout->addWidget(analyse_special_only, 8, 2);

    QLabel * other_label = new QLabel(advanced);
    other_label->setText(tr("<b>Other:</b>"));
    main_layout->addWidget(other_label, 7, 4);

    QHBoxLayout * buttons_hlayout = new QHBoxLayout;
    QLabel * edit_blacklist = new QLabel(advanced);
    edit_blacklist->setText(QString("<a href=\"edit\">%1</a>").arg(tr("Edit blacklist")));
    edit_blacklist->setStatusTip(tr("Edit blacklist for this tab"));
    connect(edit_blacklist, SIGNAL(linkActivated(QString)), this, SLOT(editBlacklist()));
    buttons_hlayout->addWidget(edit_blacklist);

    QLabel * change_ignorance = new QLabel(advanced);
    change_ignorance->setText(QString("<a href=\"edit\">%1</a>").arg(tr("Change allowed time difference")));
    change_ignorance->setStatusTip(tr("Change the allowed time difference between synchronised files"));
    connect(change_ignorance, SIGNAL(linkActivated(QString)), this, SLOT(changeAllowedDifference()));
    buttons_hlayout->addWidget(change_ignorance);
    buttons_hlayout->addStretch();
    main_layout->addLayout(buttons_hlayout, 8, 4);

    advanced->addLayout(main_layout, 0, 0);
    advanced->addItem(new QSpacerItem(10, 5, QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 1);
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
    multi_page->tw_multi->setLayoutDirection(Qt::LeftToRight);
	
	connect(multi_page->add_multi, SIGNAL(released()), this, SLOT(addSource()));
	connect(multi_page->remove_multi, SIGNAL(released()), this, SLOT(removeSource()));
	connect(multi_page->browse_multi, SIGNAL(released()), this, SLOT(browseMultiDestination()));
	connect(multi_page->sync_multi, SIGNAL(released()), multi_page, SLOT(sync()));
	connect(multi_page->stop_sync_btn, SIGNAL(released()), multi_page, SLOT(stopSync()));
	connect(multi_page->search_multi, SIGNAL(textEdited(const QString)), this, SLOT(searchTw(QString)));
	connect(multi_page->search_multi, SIGNAL(returnPressed()), multi_page, SLOT(searchAnalyseTree()));
	/*connect(multi_page->save_multi, SIGNAL(released()), multi_page, SLOT(save()));
	connect(multi_page->saveas_multi, SIGNAL(released()), multi_page, SLOT(saveAs()));
	connect(multi_page->load_multi, SIGNAL(released()), multi_page, SLOT(load()));*/
	connect(multi_page->destination_multi, SIGNAL(editingFinished()), multi_page, SLOT(destinationTextChanged()));
	connect(multi_page->tab_name, SIGNAL(editingFinished()), multi_page, SLOT(multitabNameChanged()));
	connect(multi_page->vars_multi, SIGNAL(released()), multi_page, SLOT(varsDialogue()));
	
	multi_page->tab_name->setText(multi_tabWidget->tabText(multi_tabWidget->indexOf(multi_page)));
	
	multi_tabWidget->setCurrentIndex(multi_tabWidget->indexOf(multi_page));
    
	return multi_page;
}

void MainWindow::addSource()
{
	MultisyncPage * multi_page = (MultisyncPage *) multi_tabWidget->currentWidget();
    QListWidgetItem * item = new QListWidgetItem();
	QString path = QFileDialog::getExistingDirectory(this,
                "Choose a directory",
                QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty()) return;
    QMapIterator<QString, QString> i(multi_page->vars_map);
    while (i.hasNext()) {
        i.next();
        if (path.startsWith(i.value())) {
            path.replace(0, i.value().length(), i.key() + "/");
        }
    }
	path.replace("\\", "/");
	path.replace("//", "/");
	item->setText(path);
	item->setCheckState(Qt::Checked);
	multi_page->list_multi->addItem(item);
}

void MainWindow::removeSource()
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
	leaveAnalyse();
	if (destination_multi->text()=="") { addTableItem(tr("%1    Synchronisation failed: Choose a destination first").arg(QTime().currentTime().toString("hh:mm:ss")), "", "", QBrush(Qt::red), QBrush(Qt::white)); return 0; }
	bool found = false;
	for (int n = 0; n < list_multi->count(); ++n) {
		if (list_multi->item(n)->checkState()==Qt::Checked) {
			found = true; break;
		}
	}
	if (!found) { QMessageBox::information(mp_parent, tr("Synkron"), tr("No sources selected.")); return 0; }
	setSyncEnabled(false); QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
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
	collided.clear();
	dir_filters = QDir::NoDotAndDotDot | QDir::Files;
    if (sync_hidden->isChecked()) { dir_filters |= QDir::Hidden; }
    if (!sync_nosubdirs->isChecked()) { dir_filters |= QDir::AllDirs; }

    addTableItem(tr("%1	Synchronisation started").arg(QTime().currentTime().toString("hh:mm:ss")), "", "", QBrush(Qt::yellow));
	QStringList pathlist; QString path;
	QDir destination; QDir syncfolder; int all_synced_files = 0;
	for (int i = 0; i < list_multi->count(); ++i) {
		if (list_multi->item(i)->checkState()==Qt::Unchecked) continue;
		destination.setPath(destination_multi->text());
		if (!destination.exists()) {
            if (!QDir().mkpath(destination.path())) {
                addTableItem(tr("%1	Synchronisation failed: Failed to create directory %2").arg(QTime().currentTime().toString("hh:mm:ss")).arg(destination.path()), "", "", QBrush(Qt::red), QBrush(Qt::white));
                setSyncEnabled(true); QApplication::restoreOverrideCursor(); return 0;
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
					setSyncEnabled(true); QApplication::restoreOverrideCursor(); return 0;
				}
				destination.cd(pathlist.at(v));
			}
		}
		path = list_multi->item(i)->text();
		QMapIterator<QString, QString> iter(vars_map);
        while (iter.hasNext()) {
            iter.next();
            if (path.startsWith(iter.key())) {
                path.replace(0, iter.key().length(), iter.value());
            }
        }
        path.replace("//", "/");
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
            QString sync_folder; //QStringList prop_files_list;
            for (int i = 0; i < 2; ++i) {
                if (i == 0) sync_folder = syncfolder.absolutePath();
                else sync_folder = destination.absolutePath();
                //prop_files_list = getFolderDatabase(sync_folder);
                folder_prop_list_map.insert(sync_folder, getFolderDatabase(sync_folder));
            }
        }
    	update_time = (QDateTime::currentDateTime()).toString("yyyy.MM.dd-hh.mm.ss");
        if (move->isChecked()) {
            status_table_item->setText(tr("Moving")); qApp->processEvents();
            moveContents(syncfolder, destination);
        } else {
            status_table_item->setText(tr("Searching for changes")); qApp->processEvents();
            subSync(syncfolder, destination, false);
        }
        if (alert_collisions->isChecked() && collided.count()) displayCollisions();
		countExtsBl();
		if (propagate_deletions->isChecked()) {
            saveFolderDatabase(syncfolder.absolutePath());
            saveFolderDatabase(destination.absolutePath());
        }
        all_synced_files += synced_files;
		addTableItem(tr("%1	%2: %3 file(s) %4").arg(QTime().currentTime().toString("hh:mm:ss")).arg(list_multi->item(i)->text()).arg(synced_files).arg(move->isChecked() ? tr("moved") : tr("synchronised")), "", "", QBrush(Qt::green));
        last_sync = QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate);
        status_table_item->setText(tr("Last synced on %1").arg(last_sync)); qApp->processEvents();
        collided.clear();
	}
	mp_parent->saveSettings();
	syncing = false;
    setSyncEnabled(true); QApplication::restoreOverrideCursor();
	if (!mp_parent->syncingAll) {
	    mp_parent->showTrayMessage(tr("Synchronisation complete"), tr("%1 file(s) %2").arg(all_synced_files).arg(move->isChecked() ? tr("moved") : tr("synchronised")));
	}
	if (mp_parent->actionShut_down_after_sync->isChecked()) {
        if (!mp_parent->isSyncing()) {
            mp_parent->shutDownComputer();
        }
    }
    if (mp_parent->actionQuit_after_sync->isChecked()) {
        if (!mp_parent->isSyncing()) {
            mp_parent->closeApp();
        }
    }
	return all_synced_files;
}

void MultisyncPage::setSyncEnabled(bool enable)
{
	list_multi->setEnabled(enable);
	add_multi->setEnabled(enable);
	remove_multi->setEnabled(enable);
	destination_multi->setEnabled(enable);
	browse_multi->setEnabled(enable);
	/*load_multi->setEnabled(enable);
	saveas_multi->setEnabled(enable);
	save_multi->setEnabled(enable);*/
	search_multi->setEnabled(enable);
	vars_multi->setEnabled(enable);
	advanced->setEnabled(enable);
	syncing = !enable;
	stop_sync_btn->setVisible(!enable);
    sync_multi->setVisible(enable);
    mp_parent->actionClose_sync->setEnabled(enable);
    qApp->processEvents();
}

void MultisyncPage::multitabNameChanged()
{
    if (tab_name->text() == mp_parent->multi_tabWidget->tabText(mp_parent->multi_tabWidget->indexOf(this))) return;
	QMapIterator<QTableWidgetItem*, SyncSchedule*>  i(mp_parent->item_sched_map);
    while (i.hasNext()) {
		i.next();
		for (int n = 0; n < i.value()->sched_multitab_list.count(); ++n) {
			if (mp_parent->multi_tabWidget->tabText(mp_parent->multi_tabWidget->indexOf(this))==i.value()->sched_multitab_list.at(n)) {
				i.value()->sched_multitab_list[n] = tab_name->text();
			}
		}
	}
	if (propagate_deletions->isChecked()) {
        changeTabNameInDatabase(tab_name->text(), mp_parent->multi_tabWidget->tabText(mp_parent->multi_tabWidget->indexOf(this)));
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

QStringList MultisyncPage::syncFoldersList()
{
    QStringList paths; QString path;
    QString destination = QDir(destination_multi->text()).absolutePath();
    for (int i = 0; i < list_multi->count(); ++i) {
        path = list_multi->item(i)->text();
        QMapIterator<QString, QString> iter(vars_map);
        while (iter.hasNext()) {
            iter.next();
            if (path.startsWith(iter.key())) {
                path.replace(0, iter.key().length(), iter.value());
            }
        }
		path.replace("//", "/");
		paths << path;
		paths << QString("%1/%2").arg(destination).arg(list_multi->item(i)->text().remove(":"));
    }
    return paths;
}

void MultisyncPage::varsDialogue()
{
    QDialog * vars_dialogue = new QDialog (this, Qt::Dialog);
	vars_dialogue->setWindowModality(Qt::WindowModal);
	vars_dialogue->setWindowTitle(tr("Variables"));
	QVBoxLayout * vars_hlayout = new QVBoxLayout(vars_dialogue);
	vars_hlayout->setMargin(4); vars_hlayout->setSpacing(10);

	vars_tree = new QTreeWidget(vars_dialogue);
    vars_tree->setColumnCount(0);
    QStringList labels;
    labels << tr("Variable name");
    labels << tr("Folder");
    vars_tree->setHeaderLabels(labels);
    QMapIterator<QString, QString> i(vars_map);
    while (i.hasNext()) {
        i.next();
        QTreeWidgetItem * tree_item = new QTreeWidgetItem(vars_tree);
        tree_item->setText(0, i.key());
        tree_item->setText(1, i.value());
    }
    vars_hlayout->addWidget(vars_tree);

    QHBoxLayout * button_layout = new QHBoxLayout;
    QPushButton * add_var = new QPushButton(vars_dialogue);
    add_var->setText(tr("Add"));
    connect(add_var, SIGNAL(released()), this, SLOT(addVariable()));
    connect(add_var, SIGNAL(released()), vars_dialogue, SLOT(close()));
    button_layout->addWidget(add_var);
    QPushButton * rem_var = new QPushButton(vars_dialogue);
    rem_var->setText(tr("Remove"));
    connect(rem_var, SIGNAL(released()), this, SLOT(removeVariable()));
    button_layout->addWidget(rem_var);
    button_layout->addStretch();
    QPushButton * close_dial = new QPushButton(tr("Close"));
    connect(close_dial, SIGNAL(released()), vars_dialogue, SLOT(close()));
    button_layout->addWidget(close_dial);
    vars_hlayout->addLayout(button_layout);
    vars_dialogue->show();
}

void MultisyncPage::addVariable()
{
    QDialog * add_var_dialogue = new QDialog (this, Qt::Dialog);
	add_var_dialogue->setWindowModality(Qt::WindowModal);
	//cl_dialogue->setAttribute(Qt::WA_DeleteOnClose);
	add_var_dialogue->setWindowTitle(tr("Variables"));
	QVBoxLayout * add_var_hlayout = new QVBoxLayout(add_var_dialogue);
	add_var_hlayout->setMargin(4); add_var_hlayout->setSpacing(10);

	QVBoxLayout * name_hlayout = new QVBoxLayout;
	QLabel * name_lbl = new QLabel(tr("Variable name:"));
	name_hlayout->addWidget(name_lbl);
	QLineEdit * name_le = new QLineEdit(add_var_dialogue);
	name_hlayout->addWidget(name_le);
	add_var_hlayout->addLayout(name_hlayout);

    QLabel * path_lbl = new QLabel(tr("Folder:"));
    add_var_hlayout->addWidget(path_lbl);
    QLineEdit * path_le = new QLineEdit(add_var_dialogue);
	add_var_hlayout->addWidget(path_le);

    QDialogButtonBox * bb = new QDialogButtonBox(add_var_dialogue);
    bb->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QObject::connect(bb, SIGNAL(accepted()), add_var_dialogue, SLOT(accept()));
    QObject::connect(bb, SIGNAL(rejected()), add_var_dialogue, SLOT(reject()));
    add_var_hlayout->addWidget(bb);
    
    add_var_dialogue->show();
    QString path = QFileDialog::getExistingDirectory(
                    add_var_dialogue,
                    "Choose a directory",
                    QDir::homePath(),
                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (path != "") {
        path_le->setText(path);
    }

    QMap<QString, QString> old_vars_map = vars_map;
    switch (add_var_dialogue->exec()) {
		case 0: // Cancel
			break;	
		case 1: // OK
			if (name_le->text().isEmpty() || path_le->text().isEmpty()) break;
			vars_map.insert(name_le->text(), path_le->text());
			break;
	}
	resetSourcePaths(old_vars_map);
	varsDialogue();
}

void MultisyncPage::removeVariable()
{
    if (vars_tree->currentItem() == NULL) return;
    QMap<QString, QString> old_vars_map = vars_map;

    vars_map.remove(vars_tree->currentItem()->text(0));
    delete vars_tree->currentItem();

    resetSourcePaths(old_vars_map);
}

void MultisyncPage::resetSourcePaths(QMap<QString, QString> old_vars_map)
{
    QString path;
    for (int i = 0; i < list_multi->count(); ++i) {
        path = list_multi->item(i)->text();
        QMapIterator<QString, QString> old_iter(old_vars_map);
        while (old_iter.hasNext()) {
            old_iter.next();
            if (path.startsWith(old_iter.key())) {
                path.replace(0, old_iter.key().length(), old_iter.value() + "/");
            }
        }
        path.replace("//", "/");

        QMapIterator<QString, QString> iter(vars_map);
        while (iter.hasNext()) {
            iter.next();
            if (path.startsWith(iter.value())) {
                path.replace(0, iter.value().length(), iter.key() + "/");
            }
        }
	    path.replace("\\", "/");
        path.replace("//", "/");
        list_multi->item(i)->setText(path);
    }
}

QString MultisyncPage::variablesToString()
{
    QString vars;
    QMapIterator<QString, QString> iter(vars_map);
    while (iter.hasNext()) {
        iter.next();
        vars.append(iter.key());
        vars.append(";");
        vars.append(iter.value());
        if (iter.hasNext()) vars.append("<:?:>");
    }
    return vars;
}

void MultisyncPage::variablesFromString(QString vars_str)
{
    QStringList vars = vars_str.split("<:?:>");
    for (int i = 0; i < vars.count(); ++i) {
        if (vars.at(i).split(";").count() >= 2) {
            vars_map.insert(vars.at(i).split(";").first(), vars.at(i).split(";").at(1));
        }
    }
}
