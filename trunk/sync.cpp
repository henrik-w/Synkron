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

SyncPage * MainWindow::addSyncTab()
{
    SyncPage * page = new SyncPage(this);
    page->tab_stw = new QStackedWidget(tabWidget);
    page->tab = new QWidget ();
    page->blacklist = new QWidget ();
    
    page->tab_stw->addWidget(page->tab);
    page->tab_stw->addWidget(page->blacklist);
    
    QString title;
    int n = 1; gen_title:
    title = tr("Sync #%1").arg(n); bool ok = true;
    for (int i = 0; i < tabWidget->count(); ++i) {
        if (tabWidget->tabText(i) == title) { ok = false; }
    }
    if (!ok) { n++; goto gen_title; }
    tabWidget->addTab(page->tab_stw, QIcon(QString::fromUtf8(":/new/prefix1/images/Synkron128.png")), title);
    
    page->setSyncWidget();
    page->setBlacklistWidget();
    
    tabs.insert(page->tab_stw, page);
	tabWidget->setCurrentIndex(tabWidget->indexOf(page->tab_stw));

    return page;
}

void SyncPage::setSyncWidget()
{
    QGridLayout * mainglayout = new QGridLayout (tab);
    mainglayout->setMargin(4); mainglayout->setSpacing(6);
    QGridLayout * hlayout0 = new QGridLayout;
    QSpacerItem * spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout0->addItem(mp_parent->spacerItem, 0, 0);
    tab_name = new QLineEdit (tab);
    tab_name->setStatusTip(tr("Set sync name"));
    tab_name->setText(mp_parent->tabWidget->tabText(mp_parent->tabWidget->indexOf(tab_stw)));
    QObject::connect(tab_name, SIGNAL(editingFinished()), mp_parent, SLOT(tabNameChanged()));
    hlayout0->addWidget(tab_name, 0, 1);
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout0->addItem(spacerItem, 0, 2);
    mainglayout->addLayout(hlayout0, 0, 0);
    QHBoxLayout * folders_hlayout = new QHBoxLayout;
    QToolButton * add_folder_btn = new QToolButton (this);
    add_folder_btn->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/add.png")));
    add_folder_btn->setStatusTip(tr("Add"));
    folders_hlayout->addWidget(add_folder_btn);
    show_sync_folders = new QCheckBox (tab);
    show_sync_folders->setStatusTip(tr("Show/hide sync folders"));
    show_sync_folders->setChecked(true);
    folders_hlayout->addWidget(show_sync_folders);
    QLabel * sync_text = new QLabel (tab);
    sync_text->setText(tr("<b>Sync folders:</b>"));
    folders_hlayout->addWidget(sync_text);
    folders_hlayout->addStretch();
    mainglayout->addLayout(folders_hlayout, 1, 0); // ###################################
    
    sync_folders = new SyncFolders (this);
    connect(sync_folders, SIGNAL(sigfolderschanged()), this, SLOT(syncFoldersChanged()));
    connect(show_sync_folders, SIGNAL(clicked(bool)), sync_folders, SLOT(setVisible(bool)));
    connect(show_sync_folders, SIGNAL(clicked(bool)), add_folder_btn, SLOT(setVisible(bool)));
    connect(add_folder_btn, SIGNAL(released()), sync_folders, SLOT(addFolder()));
    mainglayout->addWidget(sync_folders, 2, 0);
    
    QGridLayout * hlayout3 = new QGridLayout;
	sync_log_label = new QLabel (tab);
    sync_log_label->setText(tr("<b>Sync log:</b>"));
    hlayout3->addWidget(sync_log_label, 0, 0);
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout3->addItem(spacerItem, 0, 1);
    log_search = new ExtendedLineEdit(tab);
    log_search->setStatusTip(tr("Search sync log"));
    log_search->setText(tr("Search"));
    QObject::connect(log_search, SIGNAL(textEdited(const QString &)), mp_parent, SLOT(searchTw(const QString)));
    QObject::connect(log_search, SIGNAL(returnPressed()), this, SLOT(searchAnalyseTree()));
    hlayout3->addWidget(log_search, 0, 3);
    mainglayout->addLayout(hlayout3, 4, 0);// #################################
    tw = new QTableWidget (0, 2, tab);
    tw->setHorizontalHeaderLabels(QStringList() << tr("Source") << tr("Destination"));
    tw->verticalHeader()->hide();
    tw->setMinimumSize(0, 50);
    tw->setShowGrid(false);
    tw->setStatusTip(tr("List of synchronised files and folders"));
    tw->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tw->setLayoutDirection(Qt::LeftToRight);
    
    analyse_tree = new ExtendedTreeWidget (this);
    connect(analyse_tree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(analyseTreeItemClicked(QTreeWidgetItem *, int)));
    connect(analyse_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(analyseTreeItemDoubleClicked(QTreeWidgetItem *, int)));
    connect(analyse_tree, SIGNAL(sigconmenu(QPoint)), this, SLOT(analyseTreeConMenu(QPoint)));
    connect(analyse_tree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(analyseTreeItemExpanded(QTreeWidgetItem *)));
    
    logs_stw = new QStackedWidget (tab);
    logs_stw->addWidget(tw);
    logs_stw->addWidget(analyse_tree);
    
    mainglayout->addWidget(logs_stw, 5, 0); // ###################################
    
    QHBoxLayout * hlayout4 = new QHBoxLayout;
    QCheckBox * chb_advanced = new QCheckBox(tab);
    hlayout4->addWidget(chb_advanced);
    hlayout4->addStretch(); // ###################################
    
    go_to_analyse = new QPushButton (tr("Analyse"), tab);
    go_to_analyse->setStatusTip(tr("Analyse"));
    go_to_analyse->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/analyse_16.png")));
    connect(go_to_analyse, SIGNAL(released()), this, SLOT(goToAnalyse()));
    hlayout4->addWidget(go_to_analyse);
    
    sync_btn = new QPushButton (tr("Sync"), tab);
    sync_btn->setStatusTip(tr("Synchronise"));
    sync_btn->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/sync22.png")));
    QObject::connect(sync_btn, SIGNAL(released()), this, SLOT(sync()));
    QObject::connect(this, SIGNAL(sigsync(QWidget *)), this, SLOT(sync()));
    hlayout4->addWidget(sync_btn);
    
    stop_sync_btn = new QPushButton (tr("Stop sync"), tab);
    stop_sync_btn->setStatusTip(tr("Stop synchronisation"));
    stop_sync_btn->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/stop_sync22.png")));
    stop_sync_btn->setVisible(false);
    hlayout4->addWidget(stop_sync_btn);
    QObject::connect(stop_sync_btn, SIGNAL(released()), this, SLOT(stopSync()));
    mainglayout->addLayout(hlayout4, 6, 0); // ##################################
    
    //Advanced -----------------------------------------------------------------
    advanced = new MTAdvancedGroupBox(chb_advanced, tab);
    advanced->setStatusTip(tr("Show advanced options"));
    QVBoxLayout * column1_layout  = new QVBoxLayout;
    QVBoxLayout * column2_layout  = new QVBoxLayout;
    
	sync_hidden = new QCheckBox (advanced);
    sync_hidden->setChecked(false);
    sync_hidden->setStatusTip(tr("Synchronise hidden files and folders"));
    sync_hidden->setText(tr("Synchronise hidden files and folders"));
	column1_layout->addWidget(sync_hidden);
	
    propagate_deletions = new QCheckBox (advanced);
    propagate_deletions->setChecked(false);
    propagate_deletions->setStatusTip(tr("Propagate deletions"));
    propagate_deletions->setText(tr("Propagate deletions"));
    connect(propagate_deletions, SIGNAL(clicked(bool)), this, SLOT(propagatedStateChanged(bool)));
    connect(propagate_deletions, SIGNAL(clicked(bool)), this, SLOT(propagatedClicked(bool)));
	column2_layout->addWidget(propagate_deletions);
    
	sync_nosubdirs = new QCheckBox (advanced);
    sync_nosubdirs->setChecked(false);
    sync_nosubdirs->setStatusTip(tr("Do not synchronise subdirectories"));
    sync_nosubdirs->setText(tr("Do not synchronise subdirectories"));
	column1_layout->addWidget(sync_nosubdirs);
	
    QHBoxLayout * bl_layout = new QHBoxLayout;
    ignore_blacklist = new QCheckBox (advanced);
    ignore_blacklist->setChecked(false);
    ignore_blacklist->setStatusTip(tr("Ignore blacklist"));
    ignore_blacklist->setText(tr("Ignore blacklist"));
    connect(ignore_blacklist, SIGNAL(clicked(bool)), this, SLOT(ignoreBlacklistClicked(bool)));
    bl_layout->addWidget(ignore_blacklist);
    bl_layout->addItem(new QSpacerItem(10, 5, QSizePolicy::Fixed, QSizePolicy::Fixed));
    
    edit_blacklist = new QToolButton(advanced);
    edit_blacklist->setText(tr("Edit blacklist"));
    edit_blacklist->setStatusTip(tr("Edit blacklist for this tab"));
    connect(edit_blacklist, SIGNAL(released()), this, SLOT(editBlacklist()));
    bl_layout->addWidget(edit_blacklist);
	column2_layout->addLayout(bl_layout);
	
	backup_folders = new QCheckBox (tr("Do not backup updated files"), advanced);
	backup_folders->setStatusTip(tr("Do not backup updated files"));
	connect(backup_folders, SIGNAL(clicked(bool)), this, SLOT(backupFoldersStateChanged(bool)));
	column1_layout->addWidget(backup_folders);
	
    update_only = new QCheckBox (tr("Update existing files only"), advanced);
	update_only->setStatusTip(tr("Update existing files only"));
	connect(update_only, SIGNAL(clicked(bool)), this, SLOT(updateOnlyStateChanged(bool)));
	column2_layout->addWidget(update_only);
    
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
	
	filters = new QGroupBox(tr("Filters"), advanced);
	filters->setCheckable(true);
	filters->setChecked(false);
	filters->setMaximumWidth(150);
	QVBoxLayout * vlayout_filters = new QVBoxLayout (filters);
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
	vlayout_filters->addWidget(lw_filters);
	
	QVBoxLayout * advanced_vlayout = new QVBoxLayout;
    QHBoxLayout * columns_1_and_2_hlayout = new QHBoxLayout;
	columns_1_and_2_hlayout->addLayout(column1_layout);
	columns_1_and_2_hlayout->addLayout(column2_layout);
	columns_1_and_2_hlayout->addStretch();
	advanced_vlayout->addLayout(columns_1_and_2_hlayout);
	
	folder1_gb = new QGroupBox(tr("Folder 1"));
	QGridLayout * folder1_grid = new QGridLayout(folder1_gb);
	folder1_grid->setContentsMargins(9, 9, 9, 9);
	
	backup_folder_1 = new QCheckBox (tr("Do not backup updated files"), advanced);
	backup_folder_1->setStatusTip(tr("Do not backup updated files from Folder 1"));
	connect(backup_folder_1, SIGNAL(clicked(bool)), this, SLOT(backupOneFolderStateChanged(bool)));
	folder1_grid->addWidget(backup_folder_1, 0, 0);
	
    update_only_1 = new QCheckBox (tr("Update existing files only"), advanced);
	update_only_1->setStatusTip(tr("Update existing files in Folder 1 only"));
	connect(update_only_1, SIGNAL(clicked(bool)), this, SLOT(updateOnlyOneFolderStateChanged(bool)));
	folder1_grid->addWidget(update_only_1, 0, 1);
    
    move = new QCheckBox (tr("Move contents to folder 2, leaving folder 1 empty"), advanced);
	move->setStatusTip(tr("Move contents to folder 2, leaving folder 1 empty"));
	connect(move, SIGNAL(clicked(bool)), this, SLOT(moveStateChanged(bool)));
	folder1_grid->addWidget(move, 1, 0);
	
    folder1_grid->addItem(new QSpacerItem(10, 5, QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 2);
	advanced_vlayout->addWidget(folder1_gb);
	
	folder2_gb = new QGroupBox(tr("Folder 2"));
	QGridLayout * folder2_grid = new QGridLayout(folder2_gb);
	folder2_grid->setContentsMargins(9, 9, 9, 9);
	
	backup_folder_2 = new QCheckBox (tr("Do not backup updated files"), advanced);
	backup_folder_2->setStatusTip(tr("Do not backup updated files from Folder 2"));
	connect(backup_folder_2, SIGNAL(clicked(bool)), this, SLOT(backupOneFolderStateChanged(bool)));
	folder2_grid->addWidget(backup_folder_2, 0, 0);
	
    update_only_2 = new QCheckBox (tr("Update existing files only"), advanced);
	update_only_2->setStatusTip(tr("Update existing files in Folder 2 only"));
	connect(update_only_2, SIGNAL(clicked(bool)), this, SLOT(updateOnlyOneFolderStateChanged(bool)));
	folder2_grid->addWidget(update_only_2, 0, 1);
	
	clone_folder1 = new QCheckBox (tr("Clone folder 1"), advanced);
	clone_folder1->setStatusTip(tr("Clone folder 1"));
	connect(clone_folder1, SIGNAL(clicked(bool)), this, SLOT(cloneStateChanged(bool)));
	folder2_grid->addWidget(clone_folder1, 1, 0);
    
    folder2_grid->addItem(new QSpacerItem(10, 5, QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 2);
	advanced_vlayout->addWidget(folder2_gb);
	
	QGroupBox * analyse_gb = new QGroupBox(tr("Analysis"));
	QGridLayout * analyse_grid = new QGridLayout(analyse_gb);
	analyse_grid->setContentsMargins(9, 9, 9, 9);
	
	fast_analyse = new QCheckBox (tr("Fast analysis"), advanced);
    fast_analyse->setStatusTip(tr("Fast analysis"));
    analyse_grid->addWidget(fast_analyse, 0, 0);
    
    analyse_special_only = new QCheckBox (tr("List files which need to be synchronised only"), advanced);
    analyse_special_only->setStatusTip(tr("List files which need to be synchronised only"));
    analyse_grid->addWidget(analyse_special_only, 0, 1);
    
    analyse_grid->addItem(new QSpacerItem(10, 5, QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 2);
	advanced_vlayout->addWidget(analyse_gb);
	
	clone_folder1->setMinimumSize(move->sizeHint().width(), 0);
	backup_folders->setMinimumSize(move->sizeHint().width() + 10, 0);
	fast_analyse->setMinimumSize(move->sizeHint().width(), 0);
	
	//advanced->addLayout(column1_layout, 0, 1);
	//advanced->addLayout(column2_layout, 0, 2);
	advanced->addLayout(advanced_vlayout, 0, 1);
	//advanced->addItem(new QSpacerItem(10, 5, QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 2);
	advanced->addWidget(filters, 0, 3);
	mainglayout->addWidget(advanced, 7, 0);
}

void AbstractSyncPage::setBlacklistWidget()
{
    QVBoxLayout * bl_vlayout = new QVBoxLayout (blacklist);
    bl_vlayout->setMargin(4); bl_vlayout->setSpacing(6);
    QLabel * files = new QLabel (blacklist);
    files->setText(tr("<b>Files in the blacklist</b>"));
    bl_vlayout->addWidget(files);
    blacklist_fileslist = new QListWidget (blacklist);
    blacklist_fileslist->setStatusTip(tr("List of files in the blacklist"));
    bl_vlayout->addWidget(blacklist_fileslist);
    
    QHBoxLayout * files_btn_layout = new QHBoxLayout;
    blacklist_addfile = new QPushButton (blacklist);
    blacklist_addfile->setText(tr("Add"));
    blacklist_addfile->setStatusTip(tr("Add file to black list"));
    connect(blacklist_addfile, SIGNAL(released()), this, SLOT(addFileToBlacklist()));
    files_btn_layout->addWidget(blacklist_addfile);
    blacklist_removefile = new QPushButton (blacklist);
    blacklist_removefile->setText(tr("Remove"));
    blacklist_removefile->setStatusTip(tr("Remove file from black list"));
    connect(blacklist_removefile, SIGNAL(released()), this, SLOT(removeFileFromBlacklist()));
    files_btn_layout->addWidget(blacklist_removefile);
    files_btn_layout->addStretch();
    bl_vlayout->addLayout(files_btn_layout);
    
    QLabel * folders = new QLabel (blacklist);
    folders->setText(tr("<b>Folders in the blacklist</b>"));
    bl_vlayout->addWidget(folders);
    blacklist_folderslist = new QListWidget (blacklist);
    blacklist_folderslist->setStatusTip(tr("List of folders in the blacklist"));
    bl_vlayout->addWidget(blacklist_folderslist);
    
    QHBoxLayout * folders_btn_layout = new QHBoxLayout;
    blacklist_addfolder = new QPushButton (blacklist);
    blacklist_addfolder->setText(tr("Add"));
    blacklist_addfolder->setStatusTip(tr("Add folder to black list"));
    connect(blacklist_addfolder, SIGNAL(released()), this, SLOT(addFolderToBlacklist()));
    folders_btn_layout->addWidget(blacklist_addfolder);
    blacklist_removefolder = new QPushButton (blacklist);
    blacklist_removefolder->setText(tr("Remove"));
    blacklist_removefolder->setStatusTip(tr("Remove folder from black list"));
    connect(blacklist_removefolder, SIGNAL(released()), this, SLOT(removeFolderFromBlacklist()));
    folders_btn_layout->addWidget(blacklist_removefolder);
    folders_btn_layout->addStretch();
    bl_vlayout->addLayout(folders_btn_layout);
    
    QLabel * exts = new QLabel (blacklist);
    exts->setText(tr("<b>Extensions in the blacklist</b>"));
    bl_vlayout->addWidget(exts);
    blacklist_extslist = new QListWidget (blacklist);
    blacklist_extslist->setStatusTip(tr("List of extensions in the blacklist"));
    bl_vlayout->addWidget(blacklist_extslist);
    
    QHBoxLayout * exts_btn_layout = new QHBoxLayout;
    blacklist_addext = new QPushButton (blacklist);
    blacklist_addext->setText(tr("Add"));
    blacklist_addext->setStatusTip(tr("Add extension to black list"));
    connect(blacklist_addext, SIGNAL(released()), this, SLOT(addExtToBlacklist()));
    exts_btn_layout->addWidget(blacklist_addext);
    blacklist_removeext = new QPushButton (blacklist);
    blacklist_removeext->setText(tr("Remove"));
    blacklist_removeext->setStatusTip(tr("Remove extension from black list"));
    connect(blacklist_removeext, SIGNAL(released()), this, SLOT(removeExtFromBlacklist()));
    exts_btn_layout->addWidget(blacklist_removeext);
    exts_btn_layout->addStretch();
    bl_vlayout->addLayout(exts_btn_layout);
    
    QHBoxLayout * back_btn_layout = new QHBoxLayout;
    back_btn_layout->addStretch();
    blacklist_back = new QPushButton (blacklist);
    blacklist_back->setText(tr("Back"));
    blacklist_back->setStatusTip(tr("Go back"));
    connect(blacklist_back, SIGNAL(released()), this, SLOT(backFromBlacklist()));
    back_btn_layout->addWidget(blacklist_back);
    bl_vlayout->addLayout(back_btn_layout);
}

bool MainWindow::isSyncing()
{
    if (syncingAll) return true;
    for (int i = 0; i < tabWidget->count(); ++i) {
        if (tabs.value(tabWidget->widget(i))->syncing) {
            return true;
        }
    }
    for (int i = 0; i < multi_tabWidget->count(); ++i) {
        if (((MultisyncPage *) multi_tabWidget->widget(i))->syncing) {
            return true;
        }
    }
    return false;
}

void MainWindow::sync(QWidget* syncTab)
{
    tabs.value(syncTab)->sync();
}

int SyncPage::sync(MTStringSet sync_folders_set)
{
    if (syncing) return 0;
    leaveAnalyse();
    //MTStringSet sync_folders_set;
    if (sync_folders_set.count() == 0) {
        for (int i = 0; i < sync_folders->count(); ++i) {
            if (sync_folders->at(i)->path() == "") continue;
            QDir sync_dir (sync_folders->at(i)->path());
            if (sync_dir.exists()) {
                sync_folders_set << sync_dir.path();
            } else {
                if (!QDir().mkpath(sync_dir.path())) {
                    addTableItem(tr("%1	Failed to create directory %2").arg(QTime().currentTime().toString("hh:mm:ss")).arg(sync_dir.path()), "", "", QBrush(Qt::red), QBrush(Qt::white));
                } else {
                    addTableItem(tr("%1	Directory %2 created").arg(QTime().currentTime().toString("hh:mm:ss")).arg(sync_dir.path()), "", "", QBrush(Qt::darkBlue), QBrush(Qt::white));
                    sync_folders_set << sync_dir.path();
                }
            }
        }
    }
    if (sync_folders_set.count() < 2) {
        addTableItem(tr("%1	Synchronisation failed: Not enough valid directories specified").arg(QTime().currentTime().toString("hh:mm:ss")), "", "", QBrush(Qt::red), QBrush(Qt::white));
        return 0;
    }
    setSyncEnabled(false);
    exts_bl_map.clear();
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
    if (propagate_deletions->isChecked()) {
        folder_prop_list_map.clear();
        for (int i = 0; i < sync_folders_set.count(); ++i) {
            QStringList prop_files_list;
            QFile file(QString("%1/%2").arg(sync_folders_set.at(i)).arg(".synkron.syncdb"));
            if (!file.exists()) continue;
            if (!file.open(QFile::ReadOnly | QFile::Text)) {
		        //QMessageBox::critical(this, tr("Save database"), tr("Cannot write file %1:\n%2.").arg(db_file_name).arg(file.errorString()));
		        continue;
            }
            QTextStream in(&file);
            in.setCodec("UTF-8");
            while (!in.atEnd()) { prop_files_list << in.readLine(); }
            folder_prop_list_map.insert(sync_folders_set.at(i), prop_files_list);
        }
    }
    
    if (sync_folders_set.count() == 2) {
        QDir d1 (sync_folders_set.at(0)); QDir d2 (sync_folders_set.at(1));
        if (d1.path() == d2.path()) {
            addTableItem(tr("%1	Synchronisation failed: Directories with the same path selected").arg(QTime().currentTime().toString("hh:mm:ss")), "", "", QBrush(Qt::red), QBrush(Qt::white));
            setSyncEnabled(true); return 0;
        }
        
        if (move->isChecked()) moveContents(d1, d2);
        else subSync(d1, d2, false);
        countExtsBl();
        if (propagate_deletions->isChecked()) {
            saveFolderDatabase(sync_folders_set.at(0));
            saveFolderDatabase(sync_folders_set.at(1));
        }
        
    } else if (sync_folders_set.count() > 2) {
        
        subGroupSync(sync_folders_set);
        countExtsBl();
        if (propagate_deletions->isChecked()) saveAllFolderDatabases();
        
    }
    
    extensions.clear();
	setSyncEnabled(true);
	mp_parent->saveSettings();
    addTableItem(tr("%1	Synchronisation complete: %2 file(s) %3").arg(QTime().currentTime().toString("hh:mm:ss")).arg(synced_files).arg(move->isChecked() ? tr("moved") : tr("synchronised")), "", "", QBrush(Qt::green));
	if (!mp_parent->syncingAll) {
		mp_parent->showTrayMessage(tr("Synchronisation complete"), tr("%1 files %2").arg(synced_files).arg(move->isChecked() ? tr("moved") : tr("synchronised")));
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
	return synced_files;
}

void AbstractSyncPage::subSync(QDir& d1, QDir& d2, bool repeated)
{
	qApp->processEvents();
	if (!syncing) return;
	if (!ignore_blacklist->isChecked()) {
        /*if (folders_blacklist.contains(d1.path(), Qt::CaseInsensitive)) {
            addTableItem(tr("Folder %1 blacklisted, skipped").arg(d1.path()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
            return;
        } if (folders_blacklist.contains(d2.path(), Qt::CaseInsensitive)) return;*/
         for (int b = 0; b < folders_blacklist.count(); ++b) {
            if (QRegExp(folders_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(d1.path())) {
                addTableItem(tr("Folder %1 blacklisted, skipped").arg(d1.path()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
                return;
            }
            if (QRegExp(folders_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(d2.path())) {
                return;
            }
        }
    }
	QFileInfoList d1_entries; QFileInfoList d2_entries;
	if (extensions.count()==0) {
		d1_entries = d1.entryInfoList(dir_filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
    	d2_entries = d2.entryInfoList(dir_filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
	} else {
		d1_entries = d1.entryInfoList(extensions.toList(), dir_filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
    	d2_entries = d2.entryInfoList(extensions.toList(), dir_filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
	}
	bool d1_is_d1 = true;
	if (!d1.path().startsWith(syncFolder1Text(), Qt::CaseInsensitive)) d1_is_d1 = false;
    QString buffer;
    bool found = false; MTFile * file; QDir * temp = new QDir (QDir::homePath());
    for (int i = 0; i < d1_entries.count(); ++i) {
		if (!syncing) return;
		if (d1_entries.at(i).fileName() == ".synkron.syncdb") continue;
		found = false;
        if (!ignore_blacklist->isChecked()) {
            for (int e = 0; e < exts_blacklist.count(); ++e) {
                if (d1_entries.at(i).absoluteFilePath().endsWith(exts_blacklist.at(e))) {
                    //addTableItem(tr("File %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
                    exts_bl_map.insert(exts_blacklist.at(e), exts_bl_map.value(exts_blacklist.at(e), 0) + 1);
                    found = true;
                    break;
                }
            }
            if (found) continue;
		    if (d1_entries.at(i).isDir()) {
		    	/*if (folders_blacklist.contains(d1_entries.at(i).absoluteFilePath(), Qt::CaseInsensitive)) {
		    		addTableItem(tr("Folder %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    		continue;
		    	}*/
		    	for (int b = 0; b < folders_blacklist.count(); ++b) {
                    if (QRegExp(folders_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(d1_entries.at(i).absoluteFilePath())) {
                        addTableItem(tr("Folder %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    		    found = true;
                        break;
                    }
                } if (found) continue;
		    } else {
		    	/*if (files_blacklist.contains(d1_entries.at(i).absoluteFilePath(), Qt::CaseInsensitive)) {
		    		addTableItem(tr("File %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    		continue;
		    	}*/
		    	for (int b = 0; b < files_blacklist.count(); ++b) {
                    if (QRegExp(files_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(d1_entries.at(i).absoluteFilePath())) {
                        addTableItem(tr("File %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    		    found = true;
                        break;
                    }
                } if (found) continue;
		    }
		}
        for (int n = 0; n < d2_entries.count(); ++n) {
        	if (!syncing) return;
        	if (d1_entries.at(i).fileName() == d2_entries.at(n).fileName()) {
				found = true; if (repeated) continue;
				if (!ignore_blacklist->isChecked()) {
				    if (d2_entries.at(n).isDir()) {
					    /*if (folders_blacklist.contains(d2_entries.at(n).absoluteFilePath(), Qt::CaseInsensitive)) {
					    	continue;
					    }*/
					    for (int b = 0; b < folders_blacklist.count(); ++b) {
                            if (QRegExp(folders_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(d2_entries.at(n).absoluteFilePath())) {
                                found = false;
                                break;
                            }
                        } if (!found) continue;
				    } else {
				    	/*if (files_blacklist.contains(d2_entries.at(n).absoluteFilePath(), Qt::CaseInsensitive)) {
				    		continue;
				    	}*/
				    	for (int b = 0; b < files_blacklist.count(); ++b) {
                            if (QRegExp(files_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(d2_entries.at(n).absoluteFilePath())) {
                                found = false;
                                break;
                            }
                        } if (!found) continue;
				    	/*for (int e = 0; e < exts_blacklist.count(); ++e) {
                            if (d2_entries.at(n).absoluteFilePath().endsWith(exts_blacklist.at(e))) {
                                exts_bl_map.insert(exts_blacklist.at(e), exts_bl_map.value(exts_blacklist.at(e), 0) + 1);
                                continue;
                            }
                        }*/
				    }
				}
				if (d1_entries.at(i).isDir() && d2_entries.at(n).isDir()) {
					if (d2_entries.at(n).isSymLink() || d1_entries.at(i).isSymLink()) {
						if (followSymlinks()) {
							QDir subDir1 (d1_entries.at(i).isSymLink() ? d1_entries.at(i).symLinkTarget() : d1_entries.at(i).absoluteFilePath());
                			QDir subDir2 (d2_entries.at(n).isSymLink() ? d2_entries.at(n).symLinkTarget() : d2_entries.at(n).absoluteFilePath());
                			subSync(subDir1, subDir2, false);
							continue;
						} else if (!d1_entries.at(i).isSymLink() || !d2_entries.at(n).isSymLink()) {
							if (!repeated) {
								addTableItem(tr("A file or a folder and a symbolic link with the same name have been found. Unable to synchronise these files. (%1, %2)").arg(d1_entries.at(i).fileName()).arg(d2_entries.at(n).fileName()), "",
								    QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::red), QBrush(Qt::white));
                   			}
                    		continue;
						}
					}
					else {
						if (repeated) continue;
						QDir subDir1 (d1_entries.at(i).absoluteFilePath());
                		QDir subDir2 (d2_entries.at(n).absoluteFilePath());
                		subSync(subDir1, subDir2, false);
                		continue;
					}
            	}
            	if ((d1_entries.at(i).isDir() && !d2_entries.at(n).isDir())) {
                	if (!repeated) {
						addTableItem(tr("A folder (%1) and a file (%2) with the same name have been found. Unable to synchronise these files.").arg(d1_entries.at(i).fileName()).arg(d2_entries.at(n).fileName()), "",
						    QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::red), QBrush(Qt::white));
                    }
                    continue;
            	}
            	else if ((d2_entries.at(n).isDir() && !d1_entries.at(i).isDir())) {
               	if (!repeated) {
					    addTableItem(tr("A file (%1) and a folder (%2) with the same name have been found. Unable to synchronise these files.").arg(d1_entries.at(i).fileName()).arg(d2_entries.at(n).fileName()), "",
						    QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::red), QBrush(Qt::white));
                    }
                    continue;
            	}
            	else if (MTFileInfo(d1_entries.at(i)).lastModified() < MTFileInfo(d2_entries.at(n)).lastModified()) {
					file = new MTFile (d1_entries.at(i).absoluteFilePath());
                    bool skipped_temp = false;
					if (backup_folder_1->isChecked() && d1_is_d1/*d1.path().startsWith(syncFolder1Text(), Qt::CaseInsensitive)*/) { goto copying1; skipped_temp = true; }
                    else if (backup_folder_2->isChecked() && !d1_is_d1/*d1.path().startsWith(syncFolder2Text(), Qt::CaseInsensitive)*/) { goto copying1; skipped_temp = true; }
					temp->mkpath(QString(".Synkron/%2").arg(update_time));
                    if (!(file->copy(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d1_entries.at(i).fileName()).arg(synced_files)))) {
                       unknownError(d1_entries.at(i).absoluteFilePath(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png", tr(" to temp")); delete file; continue;
                    }
                    saveBackedUpFile(d1_entries.at(i));
                    //mp_parent->synchronised << d1_entries.at(i).fileName() << update_time << d1_entries.at(i).absoluteFilePath();
                    //mp_parent->synchronised << QString("%1/.Synkron/%2/%3.%4").arg(QDir::QDir::homePath()).arg(update_time).arg(d1_entries.at(i).fileName()).arg(synced_files);
                    copying1:
                    file->remove();
                    delete file;
                    file = new MTFile (d2_entries.at(n).absoluteFilePath());
                    buffer.clear(); buffer = d1_entries.at(i).absoluteFilePath();
                    if (!file->copy(buffer)) {
                       unknownError(d2_entries.at(n).absoluteFilePath(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png"); delete file;
                        if (!skipped_temp) {
                            if (mp_parent->restoreFile(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d1_entries.at(i).fileName()).arg(synced_files),
                                    d1_entries.at(i).absoluteFilePath(), update_time)) {
                                addTableItem(tr("File %1 restored").arg(d1_entries.at(i).absoluteFilePath()), "", "", QBrush(Qt::darkBlue), QBrush(Qt::white));
                            }
                        } continue;
                    }
                    addTableItem(d2_entries.at(n).absoluteFilePath(), d1_entries.at(i).absoluteFilePath(), QString::fromUtf8(":/new/prefix1/images/file.png"));
                    synced_files++; delete file;
            	}
                else if (MTFileInfo(d1_entries.at(i)).lastModified() > MTFileInfo(d2_entries.at(n)).lastModified()) {
                    file = new MTFile (d2_entries.at(n).absoluteFilePath(), qApp);
                    bool skipped_temp = false;
                    if (backup_folder_1->isChecked() && !d1_is_d1/*d2.path().startsWith(syncFolder1Text(), Qt::CaseInsensitive)*/) { goto copying2; skipped_temp = true; }
                    else if (backup_folder_2->isChecked() && d1_is_d1/*d2.path().startsWith(syncFolder2Text(), Qt::CaseInsensitive)*/) { goto copying2; skipped_temp = true; }
					temp->mkpath(QString(".Synkron/%2").arg(update_time));
                    if (!(file->copy(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d2_entries.at(n).fileName()).arg(synced_files)))) {
                       unknownError(d2_entries.at(n).absoluteFilePath(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png", tr(" to temp")); delete file; continue;
                    }
                    saveBackedUpFile(d2_entries.at(n));
                    //mp_parent->synchronised << d2_entries.at(n).fileName() << update_time << d2_entries.at(n).absoluteFilePath();
                    //mp_parent->synchronised << QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d2_entries.at(n).fileName()).arg(synced_files);
					copying2:
                    file->remove();
                    delete file;
                    file = new MTFile (d1_entries.at(i).absoluteFilePath());
                    buffer.clear(); buffer = d2_entries.at(n).absoluteFilePath();
                    if (!file->copy(buffer)) {
                       unknownError(d1_entries.at(i).absoluteFilePath(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png"); delete file;
                        if (!skipped_temp) {
                            if (mp_parent->restoreFile(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d2_entries.at(n).fileName()).arg(synced_files),
                                    d2_entries.at(n).absoluteFilePath(), update_time)) {
                                addTableItem(tr("File %1 restored").arg(d2_entries.at(n).absoluteFilePath()), "", "", QBrush(Qt::darkBlue), QBrush(Qt::white));
                            }
                        } continue;
                    }
                    addTableItem(d1_entries.at(i).absoluteFilePath(), d2_entries.at(n).absoluteFilePath(), QString::fromUtf8(":/new/prefix1/images/file.png"));
                    synced_files++; delete file;
                }
                break;
        }
    }

        if (found == false) {
            if (!syncing) return;
            bool symfound = false;
            buffer.clear();
            buffer = QString("%1/%2").arg(d2.path()).arg(d1_entries.at(i).fileName());
            if (propagate_deletions->isChecked()) {
                if (isInDatabase(buffer)) {
                    if (d1_entries.at(i).isDir() && !d1_entries.at(i).isSymLink()) {
                        if ((backup_folder_1->isChecked() && d1_is_d1) || (backup_folder_2->isChecked() && !d1_is_d1)) {
                            backupAndRemoveDir(d1_entries.at(i).absoluteFilePath(), false);
                        } else {
                            backupAndRemoveDir(d1_entries.at(i).absoluteFilePath());
                        }
                    } else {
                        if ((backup_folder_1->isChecked() && d1_is_d1) || (backup_folder_2->isChecked() && !d1_is_d1)) {
                            backupAndRemoveFile(d1_entries.at(i), false);
                        } else {
                            backupAndRemoveFile(d1_entries.at(i));
                        }
                    }
                    continue;
                }
            }
            if (clone_folder1->isChecked() && d1_entries.at(i).absoluteFilePath().startsWith(syncFolder2Text())) {
                QString cloned_file = d1_entries.at(i).absoluteFilePath();
                if (d1_entries.at(i).isDir() && !d1_entries.at(i).isSymLink()) {
                    if ((backup_folder_1->isChecked() && d1_is_d1) || (backup_folder_2->isChecked() && !d1_is_d1)) {
                        backupAndRemoveDir(d1_entries.at(i).absoluteFilePath(), false);
                    } else {
                        backupAndRemoveDir(d1_entries.at(i).absoluteFilePath());
                    }
                } else {
                    if ((backup_folder_1->isChecked() && d1_is_d1) || (backup_folder_2->isChecked() && !d1_is_d1)) {
                        backupAndRemoveFile(d1_entries.at(i), false);
                    } else {
                        backupAndRemoveFile(d1_entries.at(i));
                    }
                }
                continue;
            }
            if (update_only_1->isChecked() && !d1_is_d1/*QString("%1/%2").arg(d2.path()).arg(d1_entries.at(i).fileName()).startsWith(syncFolder1Text())*/) goto end;
            if (update_only_2->isChecked() && d1_is_d1/*QString("%1/%2").arg(d2.path()).arg(d1_entries.at(i).fileName()).startsWith(syncFolder2Text())*/) goto end;
            file = new MTFile (d1_entries.at(i).absoluteFilePath());
            if (d1_entries.at(i).isDir()) {
                if (!d1_entries.at(i).isSymLink() || (d1_entries.at(i).isSymLink() && followSymlinks())) {
					if (!(d2.mkdir(d1_entries.at(i).fileName()))) {
                    	unknownError(d1_entries.at(i).absoluteFilePath(), tr("folder"), tr("copy"), ":/new/prefix1/images/folder_16.png"); delete file; continue;
                	}
                	QDir subDir1 (d1_entries.at(i).isSymLink() ? d1_entries.at(i).symLinkTarget() : d1_entries.at(i).absoluteFilePath());
                	QDir subDir2 (QString("%1/%2").arg(d2.absolutePath()).arg(d1_entries.at(i).fileName()));
                	addTableItem(d1_entries.at(i).absoluteFilePath(), buffer, QString::fromUtf8(":/new/prefix1/images/folder_16.png"));
					subSync(subDir1, subDir2, true);
				} else if (d1_entries.at(i).isSymLink() && !followSymlinks()) symfound = true;
			}
            if (!d1_entries.at(i).isDir() || symfound)
			{
				if (!file->copy(buffer)) {
                	unknownError(d1_entries.at(i).absoluteFilePath(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png"); delete file; continue;
         		}
         		else {
					addTableItem(d1_entries.at(i).absoluteFilePath(), buffer, QString::fromUtf8(":/new/prefix1/images/file.png"));
            	}
            }
            synced_files++; delete file;
            end: synced_files = synced_files;
        }
    }
    if (!repeated) {
        subSync (d2, d1, true);
    }
    delete temp;
}

void AbstractSyncPage::moveContents(QDir& d1, QDir& d2)
{
	qApp->processEvents();
	if (!syncing) return;
	if (!ignore_blacklist->isChecked()) {
        /*if (folders_blacklist.contains(d1.path(), Qt::CaseInsensitive)) {
            addTableItem(tr("Folder %1 blacklisted, skipped").arg(d1.path()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
            return;
        } if (folders_blacklist.contains(d2.path(), Qt::CaseInsensitive)) {
		    addTableItem(tr("Folder %1 blacklisted, skipped").arg(d2.path()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    return;
	    }*/
	    for (int b = 0; b < folders_blacklist.count(); ++b) {
            if (QRegExp(folders_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(d1.path())) {
                addTableItem(tr("Folder %1 blacklisted, skipped").arg(d1.path()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
                return;
            }
            if (QRegExp(folders_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(d2.path())) {
                addTableItem(tr("Folder %1 blacklisted, skipped").arg(d2.path()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
                return;
            }
        }
    }
	QFileInfoList d1_entries; QFileInfoList d2_entries;
	/*QDir::Filters filters;
	filters = QDir::NoDotAndDotDot | QDir::Files;
	if (sync_hidden->isChecked()) { filters |= QDir::Hidden; }
	if (!sync_nosubdirs->isChecked()) { filters |= QDir::AllDirs; }*/
	if (extensions.count()==0) {
		d1_entries = d1.entryInfoList(dir_filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
    	d2_entries = d2.entryInfoList(dir_filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
	} else {
		d1_entries = d1.entryInfoList(extensions.toList(), dir_filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
    	d2_entries = d2.entryInfoList(extensions.toList(), dir_filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
	}
    QString buffer;
    bool found = false; MTFile * file; QDir *temp = new QDir (QDir::homePath());
    for (int i = 0; i < d1_entries.count(); ++i) {
		if (!syncing) return;
        found = false;
		if (d1_entries.at(i).fileName() == ".synkron.syncdb") continue;
		if (!ignore_blacklist->isChecked()) {
		    if (d1_entries.at(i).isDir()) {
		    	/*if (folders_blacklist.contains(d1_entries.at(i).absoluteFilePath(), Qt::CaseInsensitive)) {
		    		addTableItem(tr("Folder %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    		continue;
		    	}*/
		    	for (int b = 0; b < folders_blacklist.count(); ++b) {
                    if (QRegExp(folders_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(d1_entries.at(i).absoluteFilePath())) {
                        addTableItem(tr("Folder %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    		    found = true;
                        break;
                    }
                } if (found) continue;
		    } else {
		    	for (int e = 0; e < exts_blacklist.count(); ++e) {
                    if (d1_entries.at(i).absoluteFilePath().endsWith(exts_blacklist.at(e))) {
                        //addTableItem(tr("File %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
                        exts_bl_map.insert(exts_blacklist.at(e), exts_bl_map.value(exts_blacklist.at(e), 0) + 1);
                        found = true;
                        break;
                    }
                }
                if (found) continue;
                /*if (files_blacklist.contains(d1_entries.at(i).absoluteFilePath(), Qt::CaseInsensitive)) {
		    		addTableItem(tr("File %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    		continue;
		    	}*/
		    	for (int b = 0; b < files_blacklist.count(); ++b) {
                    if (QRegExp(files_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(d1_entries.at(i).absoluteFilePath())) {
                        addTableItem(tr("File %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    		    found = true;
                        break;
                    }
                } if (found) continue;
		    	/*for (int e = 0; e < exts_blacklist.count(); ++e) {
                    if (d1_entries.at(i).absoluteFilePath().endsWith(exts_blacklist.at(e))) {
                        exts_bl_map.insert(exts_blacklist.at(e), exts_bl_map.value(exts_blacklist.at(e), 0) + 1);
                        //addTableItem(tr("File %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    		    found = true;
                        break;
                    }
                }*/
		    }
		}
        //if (found) continue;
        for (int n = 0; n < d2_entries.count(); ++n) {
        	if (!syncing) return;
        	if (d1_entries.at(i).fileName() == d2_entries.at(n).fileName()) {
				found = true;
				if (!ignore_blacklist->isChecked()) {
				    if (d2_entries.at(n).isDir()) {
					    /*if (folders_blacklist.contains(d2_entries.at(n).absoluteFilePath(), Qt::CaseInsensitive)) {
					    	addTableItem(tr("Folder %1 blacklisted, skipped").arg(d2_entries.at(n).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
					    	continue;
					    }*/
					    for (int b = 0; b < folders_blacklist.count(); ++b) {
                            if (QRegExp(folders_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(d2_entries.at(n).absoluteFilePath())) {
                                addTableItem(tr("Folder %1 blacklisted, skipped").arg(d2_entries.at(n).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
					    	    found = false;
                                break;
                            }
                        } if (!found) continue;
				    } else {
				    	/*if (files_blacklist.contains(d2_entries.at(n).absoluteFilePath(), Qt::CaseInsensitive)) {
				    		addTableItem(tr("File %1 blacklisted, skipped").arg(d2_entries.at(n).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
				    		continue;
				    	}*/
				    	for (int b = 0; b < files_blacklist.count(); ++b) {
                            if (QRegExp(files_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(d2_entries.at(n).absoluteFilePath())) {
                                addTableItem(tr("File %1 blacklisted, skipped").arg(d2_entries.at(n).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
                                found = false;
                                break;
                            }
                        } if (!found) continue;
				    	/*for (int e = 0; e < exts_blacklist.count(); ++e) {
                            if (d2_entries.at(n).absoluteFilePath().endsWith(exts_blacklist.at(e))) {
                                exts_bl_map.insert(exts_blacklist.at(e), exts_bl_map.value(exts_blacklist.at(e), 0) + 1);
                                //addTableItem(tr("File %1 blacklisted, skipped").arg(d2_entries.at(n).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    		            continue;
                            }
                        }*/
				    }
				}
				if (d1_entries.at(i).isDir() && d2_entries.at(n).isDir()) {
					if (d2_entries.at(n).isSymLink() || d1_entries.at(i).isSymLink()) {
						if (followSymlinks()) {
							QDir subDir1 (d1_entries.at(i).isSymLink() ? d1_entries.at(i).symLinkTarget() : d1_entries.at(i).absoluteFilePath());
                			QDir subDir2 (d2_entries.at(n).isSymLink() ? d2_entries.at(n).symLinkTarget() : d2_entries.at(n).absoluteFilePath());
                			moveContents(subDir1, subDir2);
                			if (d1_entries.at(i).isSymLink()) {
                                QString file_name = d1_entries.at(i).absoluteFilePath();
                                if (QFile(d1_entries.at(i).absoluteFilePath()).remove())
                                    addTableItem(tr("File %1 deleted").arg(file_name), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
                                else
                                    addTableItem(tr("Error removing file %1").arg(file_name), "", ":/new/prefix1/images/file.png", QBrush(Qt::red), QBrush(Qt::white));
                            }
                			else {
                                QString file_name = d1_entries.at(i).absoluteFilePath();
                                if (d1.rmdir(d1_entries.at(i).fileName()))
                                    addTableItem(tr("Folder %1 deleted").arg(file_name), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
                                else
                                    addTableItem(tr("Error removing directory %1").arg(file_name), "", ":/new/prefix1/images/folder_16.png", QBrush(Qt::red), QBrush(Qt::white));
                            }
							continue;
						} else if (!d1_entries.at(i).isSymLink() || !d2_entries.at(n).isSymLink()) {
							addTableItem(tr("A file or a folder and a symbolic link with the same name have been found. Unable to synchronise these files. (%1, %2)").arg(d1_entries.at(i).fileName()).arg(d2_entries.at(n).fileName()), "",
							    QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::red), QBrush(Qt::white));
                    		continue;
						}
					}
					else {
						QDir subDir1 (d1_entries.at(i).absoluteFilePath());
                		QDir subDir2 (d2_entries.at(n).absoluteFilePath());
                		moveContents(subDir1, subDir2);
                		QString file_name = d1_entries.at(i).absoluteFilePath();
                        if (d1.rmdir(d1_entries.at(i).fileName()))
                            addTableItem(tr("Folder %1 deleted").arg(file_name), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
                        else
                            addTableItem(tr("Error removing directory %1").arg(file_name), "", ":/new/prefix1/images/folder_16.png", QBrush(Qt::red), QBrush(Qt::white));
                		continue;
					}
            	}
            	if (d1_entries.at(i).isDir() && !d2_entries.at(n).isDir()) {
					addTableItem(tr("A folder (%1) and a file (%2) with the same name have been found. Unable to synchronise these files.").arg(d1_entries.at(i).fileName()).arg(d2_entries.at(n).fileName()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::red), QBrush(Qt::white));
                    continue;
            	}
            	else if (d2_entries.at(n).isDir() && !d1_entries.at(i).isDir()) {
					addTableItem(tr("A file (%1) and a folder (%2) with the same name have been found. Unable to synchronise these files.").arg(d1_entries.at(i).fileName()).arg(d2_entries.at(n).fileName()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::red), QBrush(Qt::white));
                    continue;
            	}
            	else if (MTFileInfo(d1_entries.at(i)).lastModified() < MTFileInfo(d2_entries.at(n)).lastModified()) {
					file = new MTFile (d1_entries.at(i).absoluteFilePath());
					if (d1.path().startsWith(syncFolder1Text(), Qt::CaseInsensitive) && backup_folder_1->isChecked()) { goto copying1; }
                    else if (d1.path().startsWith(syncFolder2Text(), Qt::CaseInsensitive) && backup_folder_2->isChecked()) { goto copying1; }
					temp->mkpath(QString(".Synkron/%2").arg(update_time));
                    if (!(file->copy(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d1_entries.at(i).fileName()).arg(synced_files)))) {
                       unknownError(d1_entries.at(i).absoluteFilePath(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png", tr(" to temp")); delete file; continue;
                    }
                    saveBackedUpFile(d1_entries.at(i));
                    //mp_parent->synchronised << d1_entries.at(i).fileName() << update_time << d1_entries.at(i).absoluteFilePath();
                    //mp_parent->synchronised << QString("%1/.Synkron/%2/%3.%4").arg(QDir::QDir::homePath()).arg(update_time).arg(d1_entries.at(i).fileName()).arg(synced_files);
					addTableItem(tr("Older file %1 backed up and deleted").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"));
					copying1:
                    QString file_name = d1_entries.at(i).absoluteFilePath();
                    if (file->remove())
                        addTableItem(tr("File %1 deleted").arg(file_name), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
                    else
                        addTableItem(tr("Error removing file %1").arg(file_name), "", ":/new/prefix1/images/file.png", QBrush(Qt::red), QBrush(Qt::white));
                    delete file;
                    synced_files++;
            	}
                else if (MTFileInfo(d1_entries.at(i)).lastModified() > MTFileInfo(d2_entries.at(n)).lastModified()) {
                    file = new MTFile (d2_entries.at(n).absoluteFilePath());
                    bool skipped_temp = false;
                    if (d2.path().startsWith(syncFolder1Text(), Qt::CaseInsensitive) && backup_folder_1->isChecked()) { goto copying2; skipped_temp = true; }
                    else if (d2.path().startsWith(syncFolder2Text(), Qt::CaseInsensitive) && backup_folder_2->isChecked()) { goto copying2; skipped_temp = true; }
					temp->mkpath(QString(".Synkron/%2").arg(update_time));
                    if (!(file->copy(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d2_entries.at(n).fileName()).arg(synced_files)))) {
                       unknownError(d2_entries.at(n).absoluteFilePath(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png", tr(" to temp")); delete file; continue;
                    }
                    saveBackedUpFile(d2_entries.at(n));
                    //mp_parent->synchronised << d2_entries.at(n).fileName() << update_time << d2_entries.at(n).absoluteFilePath();
                    //mp_parent->synchronised << QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d2_entries.at(n).fileName()).arg(synced_files);
                    copying2:
                    QString file_name = d2_entries.at(n).absoluteFilePath();
                    if (file->remove())
                        addTableItem(tr("File %1 deleted").arg(file_name), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
                    else
                        addTableItem(tr("Error removing file %1").arg(file_name), "", ":/new/prefix1/images/file.png", QBrush(Qt::red), QBrush(Qt::white));
                    delete file;
                    file = new MTFile (d1_entries.at(i).absoluteFilePath());
                    buffer.clear(); buffer = d2_entries.at(n).absoluteFilePath();
                    if (!file->copy(buffer)) {
                       unknownError(d1_entries.at(i).absoluteFilePath(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png"); delete file;
                        if (!skipped_temp) {
                            if (mp_parent->restoreFile(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d2_entries.at(n).fileName()).arg(synced_files),
                                    d2_entries.at(n).absoluteFilePath(), update_time)) {
                                addTableItem(tr("File %1 restored").arg(d2_entries.at(n).absoluteFilePath()), "", "", QBrush(Qt::darkBlue), QBrush(Qt::white));
                            }
                        } continue;
                    }
                    file_name = d1_entries.at(i).absoluteFilePath();
                    if (file->remove())
                        addTableItem(tr("File %1 deleted").arg(file_name), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
                    else
                        addTableItem(tr("Error removing file %1").arg(file_name), "", ":/new/prefix1/images/file.png", QBrush(Qt::red), QBrush(Qt::white));
					addTableItem(d1_entries.at(i).absoluteFilePath(), d2_entries.at(n).absoluteFilePath(), QString::fromUtf8(":/new/prefix1/images/file.png"));
                    synced_files++; delete file;
                }
                else if (MTFileInfo(d1_entries.at(i)).lastModified() == MTFileInfo(d2_entries.at(n)).lastModified()) {
					file = new MTFile (d1_entries.at(i).absoluteFilePath());
					QString file_name = d1_entries.at(i).absoluteFilePath();
                    if (file->remove())
                        addTableItem(tr("File %1 deleted").arg(file_name), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
                    else
                        addTableItem(tr("Error removing file %1").arg(file_name), "", ":/new/prefix1/images/file.png", QBrush(Qt::red), QBrush(Qt::white));
					synced_files++; delete file;
				}
                break;
        }
    }

        if (found == false) {
			if (!syncing) return;
			bool symfound = false;
			QString file_name;
			//if (page->updateOnly1() && QString("%1/%2").arg(d2.path()).arg(d1_entries.at(i).fileName()).startsWith(page->syncFolder1Text())) goto end;
            if (update_only_2->isChecked()/* && QString("%1/%2").arg(d2.path()).arg(d1_entries.at(i).fileName()).startsWith(page->syncFolder2Text())*/) goto end;
			file = new MTFile (d1_entries.at(i).absoluteFilePath());
            buffer.clear();
            buffer = QString("%1/%2").arg(d2.path()).arg(d1_entries.at(i).fileName());
            if (d1_entries.at(i).isDir()) {
                if (!d1_entries.at(i).isSymLink() || (d1_entries.at(i).isSymLink() && followSymlinks())) {
					if (!(d2.mkdir(d1_entries.at(i).fileName()))) {
                    	unknownError(d1_entries.at(i).absoluteFilePath(), tr("folder"), tr("copy"), ":/new/prefix1/images/folder_16.png"); delete file; continue;
                	}
                	QDir subDir1 (d1_entries.at(i).isSymLink() ? d1_entries.at(i).symLinkTarget() : d1_entries.at(i).absoluteFilePath());
                	QDir subDir2 (QString("%1/%2").arg(d2.absolutePath()).arg(d1_entries.at(i).fileName()));
                	addTableItem(d1_entries.at(i).absoluteFilePath(), buffer, QString::fromUtf8(":/new/prefix1/images/folder_16.png"));
					moveContents(subDir1, subDir2);
					if (d1_entries.at(i).isSymLink()) {
                        file_name = d1_entries.at(i).absoluteFilePath();
                        if (file->remove())
                            addTableItem(tr("File %1 deleted").arg(file_name), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
                        else
                            addTableItem(tr("Error removing file %1").arg(file_name), "", ":/new/prefix1/images/file.png", QBrush(Qt::red), QBrush(Qt::white));
                    }
                	else {
                        file_name = d1_entries.at(i).absoluteFilePath();
                        if (d1.rmdir(d1_entries.at(i).fileName()))
                            addTableItem(tr("Folder %1 deleted").arg(file_name), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
                        else
                            addTableItem(tr("Error removing directory %1").arg(file_name), "", ":/new/prefix1/images/folder_16.png", QBrush(Qt::red), QBrush(Qt::white));
                    }
				} else if (d1_entries.at(i).isSymLink() && !followSymlinks()) symfound = true;
			}
            if (!d1_entries.at(i).isDir() || symfound)
			{
				if (!file->copy(buffer)) {
                	unknownError(d1_entries.at(i).absoluteFilePath(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png"); delete file; continue;
         		}
         		else {
					addTableItem(d1_entries.at(i).absoluteFilePath(), buffer, QString::fromUtf8(":/new/prefix1/images/file.png"));
            	}
            }
            file_name = d1_entries.at(i).absoluteFilePath();
            if (file->remove())
                addTableItem(tr("File %1 deleted").arg(file_name), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
            else
                addTableItem(tr("Error removing file %1").arg(file_name), "", ":/new/prefix1/images/file.png", QBrush(Qt::red), QBrush(Qt::white));
            synced_files++; delete file;
            end: synced_files = synced_files;
        }
    }
}

void SyncPage::subGroupSync(MTStringSet sync_folders_set, MTStringSet rel_paths)
{
    if (!syncing) return;
    if (rel_paths.count() == 0) {
        for (int i = 0; i < sync_folders_set.count(); ++i) {
            //Creating a set of all file names ---------------------------------
            QDir sync_dir = sync_folders_set.at(i);
            if (!sync_dir.exists()) continue;
            QFileInfoList entries;
            if (extensions.count()==0) {
                entries = sync_dir.entryInfoList(dir_filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
            } else {
                entries = sync_dir.entryInfoList(extensions.toList(), dir_filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
            }
            for (int n = 0; n < entries.count(); ++n) {
                //if (!entries.at(n).absoluteFilePath().startsWith(sync_folders_set.at(i))) continue;
                rel_paths << entries.at(n).fileName();
            }
        }
    }
    
    MTStringSet sync_folders_set2;
    MTFileInfo * file_info = 0;
    MTFileInfo * file_info2 = 0;
    MTFile * file;
    
    for (int i = 0; i < rel_paths.count(); ++i) {
        if (!syncing) return;
        if (rel_paths.at(i) == ".synkron.syncdb") continue;
        if (!ignore_blacklist->isChecked()) {
            bool found_ext = false;
            for (int e = 0; e < exts_blacklist.count(); ++e) {
                if (rel_paths.at(i).endsWith(exts_blacklist.at(e))) {
                    exts_bl_map.insert(exts_blacklist.at(e), exts_bl_map.value(exts_blacklist.at(e), 0) + 1);
                    found_ext = true;
                    break;
                }
            }
            if (found_ext) continue;
        }
        sync_folders_set2.clear();
        release(file_info);
        for (int n = 0; n < sync_folders_set.count(); ++n) {
            // Obtaining absolute paths for the file names ---------------------
            release(file_info2);
            file_info2 = new MTFileInfo (QString("%1/%2").arg(sync_folders_set.at(n)).arg(rel_paths.at(i)));
            if (!file_info2->exists()) {
                sync_folders_set2 << file_info2->absoluteFilePath();
                continue;
            }
            if (file_info2->isDir()) {
			    if (file_info && !file_info->isDir()) {
                    addTableItem(tr("A file and a folder with the same name (%1) have been found. Unable to synchronise these files.").arg(file_info->fileName()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::red), QBrush(Qt::white));
                    release(file_info);
                    break;
                }
                if (!ignore_blacklist->isChecked()) {
                    /*if (folders_blacklist.contains(file_info2->absoluteFilePath(), Qt::CaseInsensitive)) {
			    	    addTableItem(tr("Folder %1 blacklisted, skipped").arg(file_info2->absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
			    	    release(file_info);
                        break;
                    }*/
                    for (int b = 0; b < folders_blacklist.count(); ++b) {
                        if (QRegExp(folders_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(file_info2->absoluteFilePath())) {
                            addTableItem(tr("Folder %1 blacklisted, skipped").arg(file_info2->absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
			    	        release(file_info); release(file_info2);
                            break;
                        }
                    }
                    if (file_info2 == 0) break;
                }
            } else {
                if (file_info && file_info->isDir()) {
                    addTableItem(tr("A file and a folder with the same name (%1) have been found. Unable to synchronise these files.").arg(file_info->fileName()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::red), QBrush(Qt::white));
                    release(file_info);
                    break;
                }
                if (!ignore_blacklist->isChecked()) {
                    /*if (files_blacklist.contains(file_info2->absoluteFilePath(), Qt::CaseInsensitive)) {
                        addTableItem(tr("File %1 blacklisted, skipped").arg(file_info2->absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
                        release(file_info);
                        break;
                    }*/
                    for (int b = 0; b < files_blacklist.count(); ++b) {
                        if (QRegExp(files_blacklist.at(b), Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(file_info2->absoluteFilePath())) {
                            addTableItem(tr("File %1 blacklisted, skipped").arg(file_info2->absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
                            release(file_info); release(file_info2);
                            break;
                        }
                    }
                    if (file_info2 == 0) break;
                    /*bool found_ext = false;
                    for (int e = 0; e < exts_blacklist.count(); ++e) {
                        if (file_info2->absoluteFilePath().endsWith(exts_blacklist.at(e))) {
                            exts_bl_map.insert(exts_blacklist.at(e), exts_bl_map.value(exts_blacklist.at(e), 0) + 1);
                            //addTableItem(tr("File %1 blacklisted, skipped").arg(file_info->absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
                            found_ext = true;
                            break;
                        }
                    }
                    if (found_ext) break;*/
                }
            }
            if (!file_info) { file_info = new MTFileInfo (file_info2->absoluteFilePath()); }
            sync_folders_set2 << file_info2->absoluteFilePath();
        }
        release(file_info2);
        
        if (!syncing) return;
        if (sync_folders_set2.count() > 1 && file_info) {
            if (file_info->isDir() && !file_info->isSymLink()) { //Is dir ------
                MTStringSet sync_folders_set3;
                for (int n = 0; n < sync_folders_set2.count(); ++n) {
                    // Obtaining a set of available dirs -----------------------
                    file_info2 = new MTFileInfo (sync_folders_set2.at(n));
                    if (!file_info2->exists()) {
                        if (propagate_deletions->isChecked()) {
                            //Propagated deletions -----------------------------
                            if (isInGroupDatabase(file_info2->absoluteFilePath())) {
                                MTFileInfo * file_info3;
                                release(file_info2);
                                for (int m = 0; m < sync_folders_set2.count(); ++m) {
                                    if (!syncing) return;
                                    file_info3 = new MTFileInfo (sync_folders_set2.at(m));
                                    if (file_info3->exists()) backupAndRemoveDir(file_info3->absoluteFilePath(), !backup_folders->isChecked());
                                    delete file_info3;
                                }
                                break;
                            }
                        }
                        //Creating folder --------------------------------------
                        if (!update_only->isChecked()) {
                            if (QDir().mkpath(file_info2->absoluteFilePath())) {
                                addTableItem(file_info->absoluteFilePath(), file_info2->absoluteFilePath(), QString::fromUtf8(":/new/prefix1/images/folder_16.png"));
                                sync_folders_set3 << file_info2->absoluteFilePath();
                                synced_files++;
                            } else {
                                addTableItem(tr("Unknown error creating folder: %1").arg(file_info2->absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::red), QBrush(Qt::white));
                            }
                        }
                    } else {
                        sync_folders_set3 << file_info2->absoluteFilePath();
                    }
                    release(file_info2);
                }
                if (sync_folders_set3.count() > 1) {
                    subGroupSync(sync_folders_set3);
                }
            } else { //Is file or symlink --------------------------------------
                for (int n = 0; n < sync_folders_set2.count(); ++n) {
                    file_info2 = new MTFileInfo (sync_folders_set2.at(n));
                    if (file_info2->exists() && (file_info2->lastModified() > file_info->lastModified())) {
                        release(file_info);
                        file_info = new MTFileInfo (file_info2->absoluteFilePath());
                    }
                    release(file_info2);
                }
                for (int n = 0; n < sync_folders_set2.count(); ++n) {
                    release(file_info2);
                    file_info2 = new MTFileInfo (sync_folders_set2.at(n));
                    if (!file_info2->exists()) {
                        if (propagate_deletions->isChecked()) {
                            //Propagated deletions -----------------------------
                            if (isInGroupDatabase(file_info2->absoluteFilePath())) {
                                MTFileInfo * file_info3;
                                for (int m = 0; m < sync_folders_set2.count(); ++m) {
                                    if (!syncing) return;
                                    file_info3 = new MTFileInfo (sync_folders_set2.at(m));
                                    if (file_info3->exists()) backupAndRemoveFile(*file_info3, !backup_folders->isChecked());
                                    delete file_info3;
                                }
                                break;
                            }
                        }
                        //Copying file -----------------------------------------
                        if (!update_only->isChecked()) {
                            file = new MTFile (file_info->absoluteFilePath());
                            if (!file->copy(file_info2->absoluteFilePath())) {
                                unknownError(file_info->absoluteFilePath(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png");
                                //file->close();
                            } else {
                                addTableItem(file_info->absoluteFilePath(), file_info2->absoluteFilePath(), QString::fromUtf8(":/new/prefix1/images/file.png"));
                                synced_files++;
                            }
                            delete file;
                        }
                    }
                    else if (file_info->lastModified() > file_info2->lastModified()) {
                        //Overwriting older file -------------------------------
                        file = new MTFile (file_info2->absoluteFilePath());
                        if (!backup_folders->isChecked()) {
                            QDir::home().mkpath(QString(".Synkron/%2").arg(update_time));
                            if (!file->copy(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(file_info2->fileName()).arg(synced_files))) {
                                unknownError(file_info2->absoluteFilePath(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png", tr(" to temp"));
                                //file->close();
                                delete file;
                                continue;
                            }
                            saveBackedUpFile(*file_info2);
                            //mp_parent->synchronised << file_info2->fileName() << update_time << file_info2->absoluteFilePath();
                            //mp_parent->synchronised << QString("%1/.Synkron/%2/%3.%4").arg(QDir::QDir::homePath()).arg(update_time).arg(file_info2->fileName()).arg(synced_files);
                        }
                        file->remove();
                        delete file;
                        file = new MTFile (file_info->absoluteFilePath());
                        if (!file->copy(file_info2->absoluteFilePath())) {
                            unknownError(file_info->absoluteFilePath(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png");
                            //file->close();
                            if (mp_parent->restoreFile(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(file_info2->fileName()).arg(synced_files),
                                    file_info2->absoluteFilePath(), update_time)) {
                                addTableItem(tr("File %1 restored").arg(file_info2->absoluteFilePath()), "", "", QBrush(Qt::darkBlue), QBrush(Qt::white));
                            }
                        } else {
                            addTableItem(file_info->absoluteFilePath(), file_info2->absoluteFilePath(), QString::fromUtf8(":/new/prefix1/images/file.png"));
                            synced_files++;
                        }
                        delete file;
                    }
                }
                release(file_info2);
            }
        }
    }
    release(file_info);
}

void AbstractSyncPage::countExtsBl()
{
    QMapIterator<QString, int> i(exts_bl_map);
 	while (i.hasNext()) {
    	i.next();
    	if (i.value() != 0) {
			addTableItem(tr("%1 %2 files skipped").arg(i.value()).arg(i.key()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		}
	}
    exts_bl_map.clear();
}

void MainWindow::syncAll()
{
    syncingAll = true; int all_synced_files = 0;
    for (int i = 0; i<(tabWidget->count()); ++i) {
        all_synced_files += tabs.value(tabWidget->widget(i))->sync();
    }
	showTrayMessage(tr("Synchronisation complete"), tr("%1 files synchronised").arg(all_synced_files));
    syncingAll = false;
    if (actionShut_down_after_sync->isChecked()) {
        if (!isSyncing()) {
            shutDownComputer();
        }
    }
    if (actionQuit_after_sync->isChecked()) {
        if (!isSyncing()) {
            this->closeApp();
        }
    }
}

void AbstractSyncPage::addTableItem(QString source, QString destination, QString icon, QBrush background, QBrush foreground)
{
	tableWidget()->insertRow(tableWidget()->rowCount());
	QTableWidgetItem *item = new QTableWidgetItem (source);
	if (icon!="") item->setIcon(QIcon(icon));
	if (destination=="") {
		item->setBackground(background);
		item->setForeground(foreground);
        tableWidget()->setSpan(tableWidget()->rowCount() - 1, 0, 1, 2);
	}
	tableWidget()->setItem(tableWidget()->rowCount() - 1, 0, item);
	if (destination!="") {
		item = new QTableWidgetItem(destination);
        if (icon!="") item->setIcon(QIcon(icon));
        tableWidget()->setItem(tableWidget()->rowCount() - 1, 1, item);
	}
	tableWidget()->setRowHeight(tableWidget()->rowCount() - 1, 16);
	tableWidget()->setCurrentCell(tableWidget()->rowCount() - 1, 0);
	tableWidget()->clearSelection();
	qApp->processEvents();
}

void AbstractSyncPage::unknownError(QString fileName, QString type, QString action, QString icon, QString toTemp)
{
	addTableItem(tr("Unknown error %1ing %2: %3%4").arg(action).arg(type).arg(fileName).arg(toTemp), "", icon, QBrush(Qt::red), QBrush(Qt::white));
}

void SyncPage::syncPage()
{
     emit sigsync(tab);
}

void MainWindow::tabNameChanged()
{
	if (tabWidget->count() == 0) return;
    SyncPage * page = tabs.value(tabWidget->currentWidget());
	QMapIterator<QTableWidgetItem*, SyncSchedule*>  i(item_sched_map);
	while (i.hasNext()) {
		i.next();
		for (int n = 0; n < i.value()->sched_tab_list.count(); ++n) {
			if (tabWidget->tabText(tabWidget->indexOf(page->tab_stw))==i.value()->sched_tab_list.at(n)) {
				i.value()->sched_tab_list[n] = page->tab_name->text();
			}
		}
	}
	tabWidget->setTabText(tabWidget->indexOf(page->tab_stw), page->tab_name->text());
}

void SyncPage::setSyncEnabled(bool enable)
{
	sync_folders->setEnabled(enable);
	log_search->setEnabled(enable);
	tab_name->setEnabled(enable);
	advanced->setEnabled(enable);
	syncing = !enable;
    stop_sync_btn->setVisible(!enable);
    sync_btn->setVisible(enable);
    go_to_analyse->setEnabled(enable);
    mp_parent->actionClose_sync->setEnabled(enable);
    if (!enable) {
        synced_files = 0;
        update_time = (QDateTime::currentDateTime()).toString("yyyy.MM.dd-hh.mm.ss");
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    } else {
        QApplication::restoreOverrideCursor();
    }
    qApp->processEvents();
}

void AbstractSyncPage::moveStateChanged(bool checked)
{
    update_only_1->setEnabled(true);
    if (update_only_1->isChecked() != checked) update_only_1->click();
    update_only_1->setEnabled(!checked);
    propagate_deletions->setEnabled(!checked);
    clone_folder1->setEnabled(!checked);
    if (checked) clone_folder1->setChecked(false);
    moveChecked(checked);
}

void SyncPage::moveChecked(bool checked)
{
	if (checked){
		sync_btn->setText(tr("Move"));
		sync_btn->setStatusTip(tr("Move contents to folder 2, leaving folder 1 empty"));
	} else {
		sync_btn->setText(tr("Sync"));
		sync_btn->setStatusTip(tr("Synchronise"));
	}
}

void AbstractSyncPage::cloneStateChanged(bool checked)
{
    if (checked) {
        update_only_2->setChecked(false);
        move->setChecked(false);
    }
    update_only_1->setEnabled(!checked);
    update_only_1->setChecked(checked);
    update_only_2->setEnabled(!checked);
    if (propagate_deletions->isChecked()) return;
    move->setEnabled(!checked);
    cloneChecked(checked);
}

void SyncPage::cloneChecked(bool checked)
{
	if (checked){
        update_only->setChecked(false);
		sync_btn->setText(tr("Clone"));
		sync_btn->setStatusTip(tr("Clone folder 1"));
	} else {
		sync_btn->setText(tr("Sync"));
		sync_btn->setStatusTip(tr("Synchronise"));
	}
	update_only->setEnabled(!checked);
}

void SyncPage::syncFoldersChanged()
{
    bool enable = true;
    if (sync_folders->count() > 2) enable = false;
    if (!enable) {
        symlinks->setChecked(false);
        backup_folder_1->setChecked(false);
        backup_folder_2->setChecked(false);
        update_only_1->setChecked(false);
        update_only_2->setChecked(false);
        move->setChecked(false);
        clone_folder1->setChecked(false);
    }
    folder1_gb->setEnabled(enable);
    folder2_gb->setEnabled(enable);
}

void SyncPage::showThisPage()
{
    mp_parent->tabWidget->setCurrentWidget(tab_stw);
    mp_parent->actionSynchronise->trigger();
}

void AbstractSyncPage::saveFolderDatabase(QString dir_path)
{
    QDir dir (dir_path);
    QFile file(QString("%1/%2").arg(dir.absolutePath()).arg(".synkron.syncdb"));
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
		addTableItem(tr("Cannot write file %1: %2").arg(file.fileName()).arg(file.errorString()), "", ":/new/prefix1/images/file.png", QBrush(Qt::red), QBrush(Qt::white));
        return;
    }
    QTextStream sfile(&file);
	sfile.setCodec("UTF-8");
	QStringList entry_list = getEntryList(dir.absolutePath(), dir.absolutePath());
    for (int i = 0; i < entry_list.count(); ++i) {
        sfile << entry_list.at(i) << endl;
    }
}

void SyncPage::saveAllFolderDatabases()
{
    for (int i = 0; i < sync_folders->count(); ++i) {
        saveFolderDatabase(sync_folders->at(i)->path());
    }
}

void AbstractSyncPage::deleteFolderDatabase(QString dir_path)
{
    QDir dir (dir_path);
    QFile(QString("%1/%2").arg(dir.absolutePath()).arg(".synkron.syncdb")).remove();
}

void SyncPage::deleteAllFolderDatabases()
{
    for (int i = 0; i < sync_folders->count(); ++i) {
        deleteFolderDatabase(sync_folders->at(i)->path());
    }
}

QStringList AbstractSyncPage::getEntryList(QString dir_path, QString parent_dir)
{
    QFileInfoList entries = QDir(dir_path).entryInfoList(QDir::NoDotAndDotDot | QDir::Hidden | QDir::Files | QDir::AllDirs, QDir::Name | QDir::DirsFirst | QDir::IgnoreCase);
    QStringList entry_list;
    QDir dir (parent_dir);
    for (int i = 0; i < entries.count(); ++i) {
        if (entries.at(i).fileName() == ".synkron.syncdb") continue;
        entry_list << dir.relativeFilePath(entries.at(i).absoluteFilePath());
        if (entries.at(i).isDir()) {
            entry_list << getEntryList(entries.at(i).absoluteFilePath(), parent_dir);
        }
    }
    return entry_list;
}

bool SyncPage::isInGroupDatabase(QString file_name)
{
    QString sync_folder_path;
    for (int i = 0; i < sync_folders->count(); ++i) {
        if (file_name.startsWith(sync_folders->at(i)->path())) {
            sync_folder_path = sync_folders->at(i)->path();
            break;
        }
    }
    if (sync_folder_path == "") return false;
    QStringList prop_files_list = folder_prop_list_map.value(sync_folder_path);
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
    QStringList prop_files_list = folder_prop_list_map.value(dir.path());
    if (prop_files_list.contains(dir.relativeFilePath(file_name))) return true;
    return false;
}

void AbstractSyncPage::propagatedStateChanged(bool checked)
{
    if (checked) {
        if (move->isChecked()) move->click();
        if (clone_folder1->isChecked()) clone_folder1->click();
    }
    if (countSyncFolders() == 2) {
        move->setEnabled(!checked);
        clone_folder1->setEnabled(!checked);
    }
}

void AbstractSyncPage::propagatedClicked(bool checked)
{
    if (checked) {
        saveAllFolderDatabases();
    } else {
        deleteAllFolderDatabases();
    }
}

void SyncPage::updateOnlyStateChanged(bool checked)
{
    if (!move->isChecked()) update_only_1->setChecked(checked);
    update_only_2->setChecked(checked);
}

void SyncPage::backupFoldersStateChanged(bool checked)
{
    backup_folder_1->setChecked(checked);
    backup_folder_2->setChecked(checked);
}

void SyncPage::updateOnlyOneFolderStateChanged(bool)
{
    if (update_only_1->isChecked() && update_only_2->isChecked()) update_only->setChecked(true);
    else update_only->setChecked(false);
}

void SyncPage::backupOneFolderStateChanged(bool)
{
    if (backup_folder_1->isChecked() && backup_folder_2->isChecked()) backup_folders->setChecked(true);
    else backup_folders->setChecked(false);
}

void AbstractSyncPage::saveBackedUpFile(QFileInfo file_info)
{
    mp_parent->temp_settings->beginGroup(update_time);
    mp_parent->temp_settings->beginGroup(QString("%1.%2").arg(file_info.fileName()).arg(synced_files));
    mp_parent->temp_settings->setValue("file_name", file_info.fileName());
    mp_parent->temp_settings->setValue("old_file_path", file_info.absoluteFilePath());
    mp_parent->temp_settings->setValue("temp_file_path", QString("%1/.Synkron/%2/%3.%4").arg(QDir::QDir::homePath()).arg(update_time).arg(file_info.fileName()).arg(synced_files));
    mp_parent->temp_settings->endGroup();
    mp_parent->temp_settings->endGroup();
}
