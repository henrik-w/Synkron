#include "main_window.h"

SyncPage * MainWindow::addSyncTab()
{
    SyncPage * page = new SyncPage(this);
    page->tab = new QWidget (tabWidget);
    QString title;
    int n = 1; gen_title:
    title = tr("Sync #%1").arg(n); bool ok = true;
    for (int i = 0; i < tabWidget->count(); ++i) {
        if (tabWidget->tabText(i) == title) { ok = false; }
    }
    if (!ok) { n++; goto gen_title; }
    tabWidget->addTab(page->tab, QIcon(QString::fromUtf8(":/new/prefix1/images/Synkron128.png")), title);
    QGridLayout * mainglayout = new QGridLayout (page->tab);
    mainglayout->setMargin(4); mainglayout->setSpacing(6);
    QGridLayout * hlayout0 = new QGridLayout (page->tab);
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout0->addItem(spacerItem, 0, 0);
    page->tab_name = new QLineEdit (page->tab);
    page->tab_name->setStatusTip(tr("Set sync name"));
    page->tab_name->setText(tabWidget->tabText(tabWidget->indexOf(page->tab)));
    QObject::connect(page->tab_name, SIGNAL(editingFinished()), this, SLOT(tabNameChanged()));
    hlayout0->addWidget(page->tab_name, 0, 1);
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout0->addItem(spacerItem, 0, 2);
    mainglayout->addLayout(hlayout0, 0, 0);
    page->sync_text = new QLabel (page->tab);
    page->sync_text->setText(tr("<b>Sync folders:</b>"));
    mainglayout->addWidget(page->sync_text, 1, 0); // ###################################
    
    QGridLayout * hlayout1 = new QGridLayout (page->tab);
    page->sync_folder_1 = new QLineEdit (page->tab);
    page->sync_folder_1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    page->sync_folder_1->setStatusTip(tr("Set the first synchronisation folder"));
    connect(page->sync_folder_1, SIGNAL(editingFinished()), page, SLOT(folder1TextChanged()));
    hlayout1->addWidget(page->sync_folder_1, 0, 0);
    page->browse_1 = new QPushButton (tr("Browse"), page->tab);
    page->browse_1->setStatusTip(tr("Browse"));
    page->browse_1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    page->browse_1->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/folder_16.png")));
    hlayout1->addWidget(page->browse_1, 0, 1);
    mainglayout->addLayout(hlayout1, 2, 0); // ###################################
    
    QGridLayout * hlayout2 = new QGridLayout (page->tab);
    page->sync_folder_2 = new QLineEdit (page->tab);
    page->sync_folder_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    page->sync_folder_2->setStatusTip(tr("Set the second synchronisation folder"));
    connect(page->sync_folder_2, SIGNAL(editingFinished()), page, SLOT(folder2TextChanged()));
    hlayout2->addWidget(page->sync_folder_2, 0, 0);
    page->browse_2 = new QPushButton (tr("Browse"), page->tab);
    page->browse_2->setStatusTip(tr("Browse"));
    page->browse_2->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    page->browse_2->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/folder_16.png")));
    hlayout2->addWidget(page->browse_2, 0, 1);
    mainglayout->addLayout(hlayout2, 3, 0); // ###################################
    
    QButtonGroup * browse = new QButtonGroup (page->tab);
    browse->addButton(page->browse_1); browse->addButton(page->browse_2);
    QObject::connect(browse, SIGNAL(buttonReleased(QAbstractButton *)), this, SLOT(browse(QAbstractButton *)));
    
    QGridLayout * hlayout3 = new QGridLayout (page->tab);
	QLabel * sync_text_2 = new QLabel (page->tab);
    sync_text_2->setText(tr("<b>Sync log:</b>"));
    hlayout3->addWidget(sync_text_2, 0, 0);
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout3->addItem(spacerItem, 0, 1);
    page->log_search = new ExtendedLineEdit(page->tab);
    page->log_search->setStatusTip(tr("Search sync log"));
    page->log_search->setText(tr("Search"));
    QObject::connect(page->log_search, SIGNAL(textEdited(const QString &)), this, SLOT(searchTw(const QString)));
    hlayout3->addWidget(page->log_search, 0, 3);
    mainglayout->addLayout(hlayout3, 4, 0);// #################################
    page->tw = new QTableWidget (0, 2, page->tab);
    page->tw->setHorizontalHeaderLabels(QStringList() << tr("Source") << tr("Destination"));
    page->tw->verticalHeader()->hide();
    page->tw->setShowGrid(false);
    page->tw->setStatusTip(tr("List of synchronised files and folders"));
    page->tw->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    mainglayout->addWidget(page->tw, 5, 0); // ###################################
    
    QGridLayout * hlayout4 = new QGridLayout (page->tab);
    page->periodical_sync = new QCheckBox (tr("Synchronise periodically, every "), page->tab);
    page->periodical_sync->setStatusTip(tr("Synchronise periodically"));
    page->periodical_start = new QPushButton (page->tab);
    page->periodical_stop = new QPushButton (page->tab);
    page->periodical_sync->setChecked(false);
	page->periodical_start->setEnabled(false); 
	page->periodical_stop->setEnabled(false);
    hlayout4->addWidget(page->periodical_sync, 0, 0); // #######################
    page->sync_interval = new QSpinBox (page->tab);
    page->sync_interval->setStatusTip(tr("Set sync interval"));
    page->sync_interval->setMaximum(1440);
    page->sync_interval->setValue(30);
    page->sync_interval->setEnabled(false);
    hlayout4->addWidget(page->sync_interval, 0, 1); // #########################
    page->min_text = new QLabel (page->tab);
    page->min_text->setText(tr(" minute(s)"));
    hlayout4->addWidget(page->min_text, 0, 2); // ###################################
    page->periodical_start->setText(tr("Start"));
    page->periodical_start->setStatusTip(tr("Start periodical synchronisation"));
    connect(page->periodical_sync, SIGNAL(toggled(bool)), page->periodical_start, SLOT(setEnabled(bool)));
    connect(page->periodical_start, SIGNAL(released()), this, SLOT(startPeriodical()));
    hlayout4->addWidget(page->periodical_start, 0, 3); // ###################################
    page->periodical_stop->setText(tr("Stop"));
    page->periodical_stop->setStatusTip(tr("Stop periodical synchronisation"));
    connect(page->periodical_stop, SIGNAL(released()), this, SLOT(stopPeriodical()));
    hlayout4->addWidget(page->periodical_stop, 0, 4); // ###################################
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout4->addItem(spacerItem, 0, 5); // ###################################
    page->sync_btn = new QPushButton (tr("Sync"), page->tab);
    page->sync_btn->setStatusTip(tr("Synchronise"));
    page->sync_btn->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/sync22.png")));
    QObject::connect(page->sync_btn, SIGNAL(released()), page, SLOT(sync()));
    QObject::connect(page, SIGNAL(sigsync(QWidget *)), page, SLOT(sync()));
    hlayout4->addWidget(page->sync_btn, 0, 6);
    page->stop_sync_btn = new QPushButton (tr("Stop sync"), page->tab);
    page->stop_sync_btn->setStatusTip(tr("Stop synchronisation"));
    page->stop_sync_btn->setVisible(false);
    hlayout4->addWidget(page->stop_sync_btn, 0, 7);
    QObject::connect(page->stop_sync_btn, SIGNAL(released()), page, SLOT(stopSync()));
    mainglayout->addLayout(hlayout4, 6, 0); // ###################################
    connect(page->periodical_sync, SIGNAL(toggled(bool)), page->sync_interval, SLOT(setEnabled(bool)));
    
    page->advanced = new MTAdvancedGroupBox(page->tab);
    page->advanced->setStatusTip(tr("Show advanced options"));
    
    page->sync_hidden = new QCheckBox (page->advanced);
    page->sync_hidden->setChecked(false);
    page->sync_hidden->setStatusTip(tr("Synchronise hidden files and folders"));
    page->sync_hidden->setText(tr("Synchronise hidden files and folders"));
    page->advanced->addWidget(page->sync_hidden, 0, 0);
    
    page->sync_nosubdirs = new QCheckBox (page->advanced);
    page->sync_nosubdirs->setChecked(false);
    page->sync_nosubdirs->setStatusTip(tr("Do not synchronise subdirectories"));
    page->sync_nosubdirs->setText(tr("Do not synchronise subdirectories"));
    page->advanced->addWidget(page->sync_nosubdirs, 1, 0);
    
	page->ignore_blacklist = new QCheckBox (page->advanced);
    page->ignore_blacklist->setChecked(false);
    page->ignore_blacklist->setStatusTip(tr("Ignore blacklist"));
    page->ignore_blacklist->setText(tr("Ignore blacklist"));
    page->advanced->addWidget(page->ignore_blacklist, 2, 0);
    
    page->symlinks = new QCheckBox;
#ifdef Q_WS_WIN
    page->symlinks->setChecked(false);
#else
	page->symlinks->setParent(page->advanced);
	page->symlinks->setChecked(false);
	page->symlinks->setStatusTip(tr("Follow symbolic links"));
    page->symlinks->setText(tr("Follow symbolic links"));
    page->advanced->addWidget(page->symlinks, 3, 0);
#endif
    
    QLabel * folder1_label = new QLabel (page->advanced);
    folder1_label->setText(tr("<b>Folder 1:</b>"));
    page->advanced->addWidget(folder1_label, 4, 0);
    QHBoxLayout * folder1_layout = new QHBoxLayout (page->advanced);
	page->backup_folder_1 = new QCheckBox (tr("Do not backup updated files"), page->advanced);
	page->backup_folder_1->setStatusTip(tr("Do not backup updated files"));
	folder1_layout->addWidget(page->backup_folder_1);
	folder1_layout->addItem(new QSpacerItem(10, 5, QSizePolicy::Fixed, QSizePolicy::Fixed));
	page->update_only_1 = new QCheckBox (tr("Update existing files only"), page->advanced);
	page->update_only_1->setStatusTip(tr("Update existing files only"));
	folder1_layout->addWidget(page->update_only_1);
	page->advanced->addLayout(folder1_layout, 5, 0);
	page->move = new QCheckBox (tr("Move contents to folder 2, leaving folder 1 empty"), page->advanced);
	page->move->setStatusTip(tr("Move contents to folder 2, leaving folder 1 empty"));
	connect(page->move, SIGNAL(stateChanged(int)), page, SLOT(moveChecked(int)));
	page->advanced->addWidget(page->move, 6, 0);
	QLabel * folder2_label = new QLabel (page->advanced);
    folder2_label->setText(tr("<b>Folder 2:</b>"));
    page->advanced->addWidget(folder2_label, 7, 0);
    QHBoxLayout * folder2_layout = new QHBoxLayout (page->advanced);
	page->backup_folder_2 = new QCheckBox (tr("Do not backup updated files"), page->advanced);
	page->backup_folder_2->setStatusTip(tr("Do not backup updated files"));
	folder2_layout->addWidget(page->backup_folder_2);
	folder2_layout->addItem(new QSpacerItem(10, 5, QSizePolicy::Fixed, QSizePolicy::Fixed));
	page->update_only_2 = new QCheckBox (tr("Update existing files only"), page->advanced);
	page->update_only_2->setStatusTip(tr("Update existing files only"));
	folder2_layout->addWidget(page->update_only_2);
	page->advanced->addLayout(folder2_layout, 8, 0);
	page->advanced->gridLayout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 1);
	page->filters = new QGroupBox(tr("Filters"), page->advanced);
	page->filters->setCheckable(true);
	page->filters->setChecked(false);
#ifdef Q_WS_WIN
	page->filters->setMaximumSize(150, 185);
#elif defined Q_WS_MAC
	page->filters->setMaximumSize(150, 220);
#else
	page->filters->setMaximumSize(150, 200);
#endif
	page->filters->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	QVBoxLayout * vlayout_filters = new QVBoxLayout (page->filters);
#ifndef Q_WS_MAC
	vlayout_filters->setContentsMargins(9, 6, 9, 9);
#endif
	page->lw_filters = new QListWidget(page->filters);
	page->lw_filters->setMinimumSize(90, 80);
	QListWidgetItem * item;
    for (int f = 0; f < filter_list->count(); ++f) {
		item = new QListWidgetItem (filter_list->item(f)->text());
		item->setCheckState(Qt::Unchecked);
		page->lw_filters->addItem(item);
	}
	vlayout_filters->addWidget(page->lw_filters);
	page->advanced->addWidget(page->filters, 0, 2, 9, 1);
	mainglayout->addWidget(page->advanced, 7, 0);
    
    page->sync_timer = new QTimer();
    page->sync_timer->connect(page->sync_timer, SIGNAL(timeout()), page->sync_timer, SLOT(stop()));
    page->sync_timer->connect(page->sync_timer, SIGNAL(timeout()), page, SLOT(syncPage()));
	tabs.insert(page->tab, page);
	
	tabWidget->setCurrentIndex(tabWidget->indexOf(page->tab));

    return page;
}

void MainWindow::browse(QAbstractButton * btn)
{
    SyncPage * page = tabs.value(tabWidget->currentWidget());
    if (btn == page->browse_1) {
    	page->sync_folder_1->setText(QFileDialog::getExistingDirectory(
                this,
                "Choose a directory",
                QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
    } else if (btn == page->browse_2) {
    	page->sync_folder_2->setText(QFileDialog::getExistingDirectory(
                this,
                "Choose a directory",
                QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
    }
}

void MainWindow::sync(QWidget* syncTab)
{
    tabs.value(syncTab)->sync();
}

int SyncPage::sync()
{
    if (syncing) return 0;
    synced_files = 0;
    QString directory1 = sync_folder_1->text();
    QString directory2 = sync_folder_2->text();
    if ((directory1.isEmpty()) || (directory2.isEmpty()))
    {
        addTableItem(tr("%1	Synchronisation failed: Choose the synchronization folders first").arg(QTime().currentTime().toString("hh:mm:ss")), "", "", QBrush(Qt::red), QBrush(Qt::white));
        return 0;
    }
    setFoldersEnabled(false);
    QDir d1 (directory1); QDir d2 (directory2);
    if (!d1.exists()) {
        if (!QDir().mkpath(d1.path())) {
            addTableItem(tr("%1	Synchronisation failed: Failed to create directory %2").arg(QTime().currentTime().toString("hh:mm:ss")).arg(d1.path()), "", "", QBrush(Qt::red), QBrush(Qt::white));
            if (periodical_sync->isChecked()) {
			    sync_timer->stop(); sync_timer->start(sync_interval->value()*60000);
			    setFoldersEnabled(false);
		    	setPeriodicalEnabled(false);
		    } else {
		        setFoldersEnabled(true);
		        setPeriodicalEnabled(true);
		    }
            return 0;
        } else {
            addTableItem(tr("%1	Directory %2 created").arg(QTime().currentTime().toString("hh:mm:ss")).arg(d1.path()), "", "", QBrush(Qt::darkBlue), QBrush(Qt::white));
        }
    } if (!d2.exists()) {
        if (!QDir().mkpath(d2.path())) {
            addTableItem(tr("%1	Synchronisation failed: Failed to create directory %2").arg(QTime().currentTime().toString("hh:mm:ss")).arg(d2.path()), "", "", QBrush(Qt::darkBlue), QBrush(Qt::white));
            if (periodical_sync->isChecked()) {
			    sync_timer->stop(); sync_timer->start(sync_interval->value()*60000);
			    setFoldersEnabled(false);
			    setPeriodicalEnabled(false);
		    } else {
		        setFoldersEnabled(true);
		        setPeriodicalEnabled(true);
		    }
            return 0;
        } else {
            addTableItem(tr("%1	Directory %2 created").arg(QTime().currentTime().toString("hh:mm:ss")).arg(d2.path()), "", "", QBrush(Qt::darkBlue), QBrush(Qt::white));
        }
    }
    if (d1.path() == d2.path()) {
       QMessageBox::warning(mp_parent, tr("Synkron - %1").arg(mp_parent->tabWidget->tabText(mp_parent->tabWidget->indexOf(tab))), tr("Directories with the same path selected."));
       setFoldersEnabled(true); setPeriodicalEnabled(true); return 0;
    }
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
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
	} syncing = true;
    if (move->isChecked()) moveContents(d1, d2);
	else subSync(d1, d2, false);
	extensions.clear();
    QApplication::restoreOverrideCursor();
    if (periodical_sync->isChecked()) {
		setSyncingOn(false);
		sync_timer->stop(); sync_timer->start(sync_interval->value()*60000);
		setPeriodicalEnabled(false);
	}
    else {
		setFoldersEnabled(true);
		sync_timer->stop();
		setPeriodicalEnabled(true);
		periodical_start->setEnabled(false);
	}
	mp_parent->saveSettings();
    addTableItem(tr("%1	Synchronisation complete: %2 file(s) %3").arg(QTime().currentTime().toString("hh:mm:ss")).arg(synced_files).arg(move->isChecked() ? tr("moved") : tr("synchronised")), "", "", QBrush(Qt::green));
	if (!mp_parent->syncingAll) {
		mp_parent->showTrayMessage(tr("Synchronisation complete"), tr("%1 files %2").arg(synced_files).arg(move->isChecked() ? tr("moved") : tr("synchronised")));
	}
	return synced_files;
}

void AbstractSyncPage::subSync(QDir& d1, QDir& d2, bool repeated)
{
	qApp->processEvents();
	if (!syncing) return;
	if (!ignore_blacklist->isChecked()) {
        if (mp_parent->folders_blacklist.contains(d1.path(), Qt::CaseInsensitive)) {
            addTableItem(tr("Folder %1 blacklisted, skipped").arg(d1.path()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
            return;
        } if (mp_parent->folders_blacklist.contains(d2.path(), Qt::CaseInsensitive)) return;
    }
	QFileInfoList d1_entries; QFileInfoList d2_entries;
	QDir::Filters filters;
	filters = QDir::NoDotAndDotDot | QDir::Files;
	if (sync_hidden->isChecked()) { filters |= QDir::Hidden; }
	if (!sync_nosubdirs->isChecked()) { filters |= QDir::AllDirs; }
	if (extensions.count()==0) {
		d1_entries = d1.entryInfoList(filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
    	d2_entries = d2.entryInfoList(filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
	} else {
		d1_entries = d1.entryInfoList(extensions.toList(), filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
    	d2_entries = d2.entryInfoList(extensions.toList(), filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
	}
	QString update_time = (QDateTime::currentDateTime()).toString("yyyy.MM.dd-hh.mm.ss");
    QString buffer;
    bool found = false; MTFile * file; QDir * temp = new QDir (QDir::homePath());
    /*for (int i = 0; i < d1_entries.count(); ++i) {
        if (!d1_entries.at(i).isDir()) {
            QMessageBox::warning(this, tr("Synkron"), tr("%1").arg(d1_entries.at(i).fileName()));
        }
    }
    for (int n = 0; n < d2_entries.count(); ++n) {
        if (!d2_entries.at(n).isDir()) {
            QMessageBox::warning(this, tr("Synkron"), tr("%1").arg(d2_entries.at(n).fileName()));
        }
    }*/
    for (int i = 0; i < d1_entries.count(); ++i) {
		if (!syncing) return;
		if (!ignore_blacklist->isChecked()) {
		    if (d1_entries.at(i).isDir()) {
		    	if (mp_parent->folders_blacklist.contains(d1_entries.at(i).absoluteFilePath(), Qt::CaseInsensitive)) {
		    		addTableItem(tr("Folder %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    		continue;
		    	}
		    } else {
		    	if (mp_parent->files_blacklist.contains(d1_entries.at(i).absoluteFilePath(), Qt::CaseInsensitive)) {
		    		addTableItem(tr("File %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    		continue;
		    	}
		    }
		}
        found = false;
        for (int n = 0; n < d2_entries.count(); ++n) {
        	if (!syncing) return;
        	if (d1_entries.at(i).fileName() == d2_entries.at(n).fileName()) {
				found = true; if (repeated) continue;
				if (!ignore_blacklist->isChecked()) {
				    if (d2_entries.at(n).isDir()) {
					    if (mp_parent->folders_blacklist.contains(d2_entries.at(n).absoluteFilePath(), Qt::CaseInsensitive)) {
					    	continue;
					    }
				    } else {
				    	if (mp_parent->files_blacklist.contains(d2_entries.at(n).absoluteFilePath(), Qt::CaseInsensitive)) {
				    		continue;
				    	}
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
						if (!syncing) return;
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
            	else if (d1_entries.at(i).lastModified() < d2_entries.at(n).lastModified()) {
					file = new MTFile (d1_entries.at(i).absoluteFilePath());
					if (d1.path().startsWith(syncFolder1Text(), Qt::CaseInsensitive) && backup_folder_1->isChecked()) { goto copying1; }
                    else if (d1.path().startsWith(syncFolder2Text(), Qt::CaseInsensitive) && backup_folder_2->isChecked()) { goto copying1; }
					if (!file->open(QIODevice::ReadOnly)) {
                       unknownError(d1_entries.at(i).fileName(), tr("file"), tr("open"), ":/new/prefix1/images/file.png"); delete file; continue;
                    }
					temp->mkpath(QString(".Synkron/%2").arg(update_time));
                    if (!(file->copy(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d1_entries.at(i).fileName()).arg(synced_files)))) {
                       unknownError(d1_entries.at(i).fileName(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png", tr(" to temp")); delete file; continue;
                    }
                    mp_parent->synchronised << d1_entries.at(i).fileName() << update_time << d1_entries.at(i).absoluteFilePath();
                    mp_parent->synchronised << QString("%1/.Synkron/%2/%3.%4").arg(QDir::QDir::homePath()).arg(update_time).arg(d1_entries.at(i).fileName()).arg(synced_files);
                    copying1:
                    file->remove(); delete file;
                    file = new MTFile (d2_entries.at(n).absoluteFilePath());
                    if (!file->open(QIODevice::ReadOnly)) {
                       unknownError(d2_entries.at(n).fileName(), tr("file"), tr("open"), ":/new/prefix1/images/file.png"); delete file; continue;
                    }
                    buffer.clear(); buffer = d1_entries.at(i).absoluteFilePath();
                    if (!file->copy(buffer)) {
                       unknownError(d1_entries.at(i).fileName(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png"); delete file; continue;
                    }
                    addTableItem(d2_entries.at(n).absoluteFilePath(), d1_entries.at(i).absoluteFilePath(), QString::fromUtf8(":/new/prefix1/images/file.png"));
                    file->close();
                    synced_files++; delete file;
            	}
                else if (d1_entries.at(i).lastModified() > d2_entries.at(n).lastModified()) {
                    file = new MTFile (d2_entries.at(n).absoluteFilePath());
                    if (d2.path().startsWith(syncFolder1Text(), Qt::CaseInsensitive) && backup_folder_1->isChecked()) { goto copying2; }
                    else if (d2.path().startsWith(syncFolder2Text(), Qt::CaseInsensitive) && backup_folder_2->isChecked()) { goto copying2; }
                    if (!file->open(QIODevice::ReadOnly)) {
                       unknownError(d2_entries.at(n).fileName(), tr("file"), tr("open"), ":/new/prefix1/images/file.png"); delete file; continue;
                    }
					temp->mkpath(QString(".Synkron/%2").arg(update_time));
                    if (!(file->copy(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d2_entries.at(n).fileName()).arg(synced_files)))) {
                       unknownError(d2_entries.at(n).fileName(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png", tr(" to temp")); delete file; continue;
                    }
                    mp_parent->synchronised << d2_entries.at(n).fileName() << update_time << d2_entries.at(n).absoluteFilePath();
                    mp_parent->synchronised << QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d2_entries.at(n).fileName()).arg(synced_files);
					copying2:
                    file->remove(); delete file;
                    file = new MTFile (d1_entries.at(i).absoluteFilePath());
                    if (!file->open(QIODevice::ReadOnly)) {
                       unknownError(d1_entries.at(i).fileName(), tr("file"), tr("open"), ":/new/prefix1/images/file.png"); delete file; continue;
                    }
                    buffer.clear(); buffer = d2_entries.at(n).absoluteFilePath();
                    if (!file->copy(buffer)) {
                       unknownError(d2_entries.at(n).fileName(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png"); delete file; continue;
                    }
                    addTableItem(d1_entries.at(i).absoluteFilePath(), d2_entries.at(n).absoluteFilePath(), QString::fromUtf8(":/new/prefix1/images/file.png"));
                    file->close();
                    synced_files++; delete file;
                }
                break;
        }
    }

        if (found == false) {
            if (!syncing) return;
            bool symfound = false;
            if (update_only_1->isChecked() && QString("%1/%2").arg(d2.path()).arg(d1_entries.at(i).fileName()).startsWith(syncFolder1Text())) goto end;
            if (update_only_2->isChecked() && QString("%1/%2").arg(d2.path()).arg(d1_entries.at(i).fileName()).startsWith(syncFolder2Text())) goto end;
            file = new MTFile (d1_entries.at(i).absoluteFilePath());
            if (!d1_entries.at(i).isDir()) {
                if (!file->open(QIODevice::ReadOnly)) {
                     unknownError(d1_entries.at(i).absoluteFilePath(), tr("file"), tr("open"), ":/new/prefix1/images/file.png"); delete file; continue;
                }
            }
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
					subSync(subDir1, subDir2, true);
				} else if (d1_entries.at(i).isSymLink() && !followSymlinks()) symfound = true;
			}
            if (!d1_entries.at(i).isDir() || symfound)
			{
				if (!file->copy(buffer)) {
                	unknownError(d1_entries.at(i).fileName(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png"); delete file; continue;
         		}
         		else {
					addTableItem(d1_entries.at(i).absoluteFilePath(), buffer, QString::fromUtf8(":/new/prefix1/images/file.png"));
            	}
            }
            file->close();
            synced_files++; delete file;
            end: synced_files = synced_files;
        }
    }
    if (!repeated) {
        subSync (d2, d1, true);
    }
}

void AbstractSyncPage::moveContents(QDir& d1, QDir& d2)
{
	qApp->processEvents();
	if (!syncing) return;
	if (!ignore_blacklist->isChecked()) {
        if (mp_parent->folders_blacklist.contains(d1.path(), Qt::CaseInsensitive)) {
            addTableItem(tr("Folder %1 blacklisted, skipped").arg(d1.path()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
            return;
        } if (mp_parent->folders_blacklist.contains(d2.path(), Qt::CaseInsensitive)) {
		    addTableItem(tr("Folder %1 blacklisted, skipped").arg(d2.path()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    return;
	    }
    }
	QFileInfoList d1_entries; QFileInfoList d2_entries;
	QDir::Filters filters;
	filters = QDir::NoDotAndDotDot | QDir::Files;
	if (sync_hidden->isChecked()) { filters |= QDir::Hidden; }
	if (!sync_nosubdirs->isChecked()) { filters |= QDir::AllDirs; }
	if (extensions.count()==0) {
		d1_entries = d1.entryInfoList(filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
    	d2_entries = d2.entryInfoList(filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
	} else {
		d1_entries = d1.entryInfoList(extensions.toList(), filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
    	d2_entries = d2.entryInfoList(extensions.toList(), filters, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
	}
	QString update_time = (QDateTime::currentDateTime()).toString("yyyy.MM.dd-hh.mm.ss");
    QString buffer;
    bool found = false; MTFile * file; QDir *temp = new QDir (QDir::homePath());
    for (int i = 0; i < d1_entries.count(); ++i) {
		if (!syncing) return;
		if (!ignore_blacklist->isChecked()) {
		    if (d1_entries.at(i).isDir()) {
		    	if (mp_parent->folders_blacklist.contains(d1_entries.at(i).absoluteFilePath(), Qt::CaseInsensitive)) {
		    		addTableItem(tr("Folder %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    		continue;
		    	}
		    } else {
		    	if (mp_parent->files_blacklist.contains(d1_entries.at(i).absoluteFilePath(), Qt::CaseInsensitive)) {
		    		addTableItem(tr("File %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		    		continue;
		    	}
		    }
		}
        found = false;
        for (int n = 0; n < d2_entries.count(); ++n) {
        	if (!syncing) return;
        	if (d1_entries.at(i).fileName() == d2_entries.at(n).fileName()) {
				found = true;
				if (!ignore_blacklist->isChecked()) {
				    if (d2_entries.at(n).isDir()) {
					    if (mp_parent->folders_blacklist.contains(d2_entries.at(n).absoluteFilePath(), Qt::CaseInsensitive)) {
					    	addTableItem(tr("Folder %1 blacklisted, skipped").arg(d2_entries.at(n).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
					    	continue;
					    }
				    } else {
				    	if (mp_parent->files_blacklist.contains(d2_entries.at(n).absoluteFilePath(), Qt::CaseInsensitive)) {
				    		addTableItem(tr("File %1 blacklisted, skipped").arg(d2_entries.at(n).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
				    		continue;
				    	}
				    }
				}
				if (d1_entries.at(i).isDir() && d2_entries.at(n).isDir()) {
					if (d2_entries.at(n).isSymLink() || d1_entries.at(i).isSymLink()) {
						if (followSymlinks()) {
								QDir subDir1 (d1_entries.at(i).isSymLink() ? d1_entries.at(i).symLinkTarget() : d1_entries.at(i).absoluteFilePath());
                				QDir subDir2 (d2_entries.at(n).isSymLink() ? d2_entries.at(n).symLinkTarget() : d2_entries.at(n).absoluteFilePath());
                				moveContents(subDir1, subDir2);
                				if (d1_entries.at(i).isSymLink()) QFile(d1_entries.at(i).absoluteFilePath()).remove();
                				else d1.rmdir(d1_entries.at(i).fileName());
								continue;
						} else if (!d1_entries.at(i).isSymLink() || !d2_entries.at(n).isSymLink()) {
							addTableItem(tr("A file or a folder and a symbolic link with the same name have been found. Unable to synchronise these files. (%1, %2)").arg(d1_entries.at(i).fileName()).arg(d2_entries.at(n).fileName()), "",
							    QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::red), QBrush(Qt::white));
                    		continue;
						}
					}
					else {
						if (!syncing) return;
						QDir subDir1 (d1_entries.at(i).absoluteFilePath());
                		QDir subDir2 (d2_entries.at(n).absoluteFilePath());
                		moveContents(subDir1, subDir2);
                		d1.rmdir(d1_entries.at(i).fileName());
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
            	else if (d1_entries.at(i).lastModified() < d2_entries.at(n).lastModified()) {
					file = new MTFile (d1_entries.at(i).absoluteFilePath());
					if (d1.path().startsWith(syncFolder1Text(), Qt::CaseInsensitive) && backup_folder_1->isChecked()) { goto copying1; }
                    else if (d1.path().startsWith(syncFolder2Text(), Qt::CaseInsensitive) && backup_folder_2->isChecked()) { goto copying1; }
					if (!file->open(QIODevice::ReadOnly)) {
                       unknownError(d1_entries.at(i).fileName(), tr("file"), tr("open"), ":/new/prefix1/images/file.png"); delete file; continue;
                    }
					temp->mkpath(QString(".Synkron/%2").arg(update_time));
                    if (!(file->copy(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d1_entries.at(i).fileName()).arg(synced_files)))) {
                       unknownError(d1_entries.at(i).fileName(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png", tr(" to temp")); delete file; continue;
                    }
                    mp_parent->synchronised << d1_entries.at(i).fileName() << update_time << d1_entries.at(i).absoluteFilePath();
                    mp_parent->synchronised << QString("%1/.Synkron/%2/%3.%4").arg(QDir::QDir::homePath()).arg(update_time).arg(d1_entries.at(i).fileName()).arg(synced_files);
					addTableItem(tr("Older file %1 backed up and deleted").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"));
					copying1:
                    file->remove(); delete file;
                    synced_files++;
            	}
                else if (d1_entries.at(i).lastModified() > d2_entries.at(n).lastModified()) {
                    file = new MTFile (d2_entries.at(n).absoluteFilePath());
                    if (d2.path().startsWith(syncFolder1Text(), Qt::CaseInsensitive) && backup_folder_1->isChecked()) { goto copying2; }
                    else if (d2.path().startsWith(syncFolder2Text(), Qt::CaseInsensitive) && backup_folder_2->isChecked()) { goto copying2; }
                    if (!file->open(QIODevice::ReadOnly)) {
                       unknownError(d2_entries.at(n).fileName(), tr("file"), tr("open"), ":/new/prefix1/images/file.png"); delete file; continue;
                    }
					temp->mkpath(QString(".Synkron/%2").arg(update_time));
                    if (!(file->copy(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d2_entries.at(n).fileName()).arg(synced_files)))) {
                       unknownError(d2_entries.at(n).fileName(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png", tr(" to temp")); delete file; continue;
                    }
                    mp_parent->synchronised << d2_entries.at(n).fileName() << update_time << d2_entries.at(n).absoluteFilePath();
                    mp_parent->synchronised << QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d2_entries.at(n).fileName()).arg(synced_files);
                    copying2:
                    file->remove(); delete file;
                    file = new MTFile (d1_entries.at(i).absoluteFilePath());
                    if (!file->open(QIODevice::ReadOnly)) {
                       unknownError(d1_entries.at(i).fileName(), tr("file"), tr("open"), ":/new/prefix1/images/file.png"); delete file; continue;
                    }
                    buffer.clear(); buffer = d2_entries.at(n).absoluteFilePath();
                    if (!file->copy(buffer)) {
                       unknownError(d2_entries.at(n).fileName(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png"); delete file; continue;
                    }
                    file->remove();
					addTableItem(d1_entries.at(i).absoluteFilePath(), d2_entries.at(n).absoluteFilePath(), QString::fromUtf8(":/new/prefix1/images/file.png"));
                    synced_files++; delete file;
                }
                else if (d1_entries.at(i).lastModified() == d2_entries.at(n).lastModified()) {
					file = new MTFile (d1_entries.at(i).absoluteFilePath());
					file->remove(); synced_files++; delete file;
				}
                break;
        }
    }

        if (found == false) {
			if (!syncing) return;
			bool symfound = false;
			//if (page->updateOnly1() && QString("%1/%2").arg(d2.path()).arg(d1_entries.at(i).fileName()).startsWith(page->syncFolder1Text())) goto end;
            if (update_only_2->isChecked()/* && QString("%1/%2").arg(d2.path()).arg(d1_entries.at(i).fileName()).startsWith(page->syncFolder2Text())*/) goto end;
			file = new MTFile (d1_entries.at(i).absoluteFilePath());
            if (!d1_entries.at(i).isDir()) {
                if (!file->open(QIODevice::ReadOnly)) {
                     unknownError(d1_entries.at(i).fileName(), tr("file"), tr("open"), ":/new/prefix1/images/file.png"); delete file; continue;
                }
                
            }
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
					if (d1_entries.at(i).isSymLink()) QFile(d1_entries.at(i).absoluteFilePath()).remove();
                	else d1.rmdir(d1_entries.at(i).fileName());
				} else if (d1_entries.at(i).isSymLink() && !followSymlinks()) symfound = true;
			}
            if (!d1_entries.at(i).isDir() || symfound)
			{
				if (!file->copy(buffer)) {
                	unknownError(d1_entries.at(i).fileName(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png"); delete file; continue;
         		}
         		else {
					addTableItem(d1_entries.at(i).absoluteFilePath(), buffer, QString::fromUtf8(":/new/prefix1/images/file.png"));
            	}
            }
            file->remove();
            synced_files++; delete file;
            end: synced_files = synced_files;
        }
    }
}

void MainWindow::syncAll()
{
    syncingAll = true; int all_synced_files = 0;
    for (int i = 0; i<(tabWidget->count()); ++i) {
        all_synced_files += tabs.value(tabWidget->widget(i))->sync();
    }
	showTrayMessage(tr("Synchronisation complete"), tr("%1 files synchronised").arg(all_synced_files));
    syncingAll = false;
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
	SyncPage * page = tabs.value(tabWidget->currentWidget());
	QMapIterator<QTableWidgetItem*, SyncSchedule*>  i(item_sched_map);
	while (i.hasNext()) {
		i.next();
		for (int n = 0; n < i.value()->sched_tab_list.count(); ++n) {
			if (tabWidget->tabText(tabWidget->indexOf(page->tab))==i.value()->sched_tab_list.at(n)) {
				i.value()->sched_tab_list[n] = page->tab_name->text();
			}
		}
	}
	tabWidget->setTabText(tabWidget->indexOf(page->tab), page->tab_name->text());
}

void MainWindow::startPeriodical(QWidget* syncTab)
{
	SyncPage * page = 0;
	if (syncTab==0) page = tabs.value(tabWidget->currentWidget());
	else page = tabs.value(syncTab);
	page->sync_timer->stop(); page->sync_timer->start(page->sync_interval->value()*60000);
	page->setFoldersEnabled(false);
	page->setSyncingOn(false);
	page->setPeriodicalEnabled(false);
}

void MainWindow::stopPeriodical()
{
	SyncPage * page = tabs.value(tabWidget->currentWidget());
	page->sync_timer->stop();
	page->setFoldersEnabled(true);
	page->setPeriodicalEnabled(true);
}

void SyncPage::setPeriodicalEnabled( bool enable)
{
	periodical_stop->setEnabled(!enable);
	periodical_start->setEnabled(enable);
	sync_btn->setEnabled(enable);
}

void SyncPage::setFoldersEnabled(bool enable)
{
	sync_folder_1->setEnabled(enable);
	sync_folder_2->setEnabled(enable);
	browse_1->setEnabled(enable);
	browse_2->setEnabled(enable);
	log_search->setEnabled(enable);
	//sync_btn->setEnabled(enable);
	sync_hidden->setEnabled(enable);
	tab_name->setEnabled(enable);
	periodical_sync->setEnabled(enable);
	if (periodical_sync->isChecked()) sync_interval->setEnabled(enable);
	min_text->setEnabled(enable);
	advanced->setEnabled(enable);
	periodical_start->setEnabled(enable);
	periodical_stop->setEnabled(enable);
	setSyncingOn(!enable);
}

void SyncPage::setSyncingOn(bool on)
{
    syncing = on;
    stop_sync_btn->setVisible(on);
    sync_btn->setVisible(!on);
}

void SyncPage::moveChecked(int state)
{
	if ((Qt::CheckState)state==Qt::Checked){
		sync_btn->setText(tr("Move"));
		sync_btn->setStatusTip(tr("Move contents to folder 2, leaving folder 1 empty"));
	} else {
		sync_btn->setText(tr("Sync"));
		sync_btn->setStatusTip(tr("Synchronise"));
	}
}
