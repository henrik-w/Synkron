#include "main_window.h"

SyncPage * MainWindow::addTab()
{
    SyncPage * page = new SyncPage;
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
    QObject::connect(page->tab_name, SIGNAL(editingFinished()), this, SLOT(tabNameEdited()));
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
    connect(page->sync_folder_1, SIGNAL(editingFinished()), this, SLOT(folder1TextChanged()));
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
    connect(page->sync_folder_2, SIGNAL(editingFinished()), this, SLOT(folder2TextChanged()));
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
    page->log_search = new QLineEdit(page->tab);
    page->log_search->setStatusTip(tr("Search sync log"));
    page->log_search->setText(tr("Search"));
    QObject::connect(page->log_search, SIGNAL(textEdited(const QString &)), this, SLOT(logSearch(const QString)));
    hlayout3->addWidget(page->log_search, 0, 3);
    mainglayout->addLayout(hlayout3, 4, 0);// #################################
    page->tw = new QTableWidget (0, 2, page->tab);
    page->tw->setHorizontalHeaderLabels(QStringList() << tr("Source") << tr("Destination"));
    page->tw->verticalHeader()->hide();
    page->tw->setShowGrid(false);
    page->tw->setStatusTip(tr("List of synchronised files and folders"));
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
    page->sync_interval->setValue(30);
    if (!page->periodical_sync->isChecked()) page->sync_interval->setEnabled(false);
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
    page->sync = new QPushButton (tr("Sync"), page->tab);
    page->sync->setStatusTip(tr("Synchronise"));
    page->sync->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/sync22.png")));
    QObject::connect(page->sync, SIGNAL(released()), this, SLOT(sync()));
    QObject::connect(page, SIGNAL(sigsync(QWidget *)), this, SLOT(sync(QWidget *)));
    hlayout4->addWidget(page->sync, 0, 6);
    mainglayout->addLayout(hlayout4, 6, 0); // ###################################
    connect(page->periodical_sync, SIGNAL(toggled(bool)), page->sync_interval, SLOT(setEnabled(bool)));
    
    page->advanced = new MTAdvancedGroupBox(page->tab);
    page->advanced->setStatusTip(tr("Show advanced options"));
    
    page->sync_hidden = new QCheckBox (page->advanced);
    page->sync_hidden->setChecked(false);
    page->sync_hidden->setStatusTip(tr("Synchronise hidden files and folders"));
    page->sync_hidden->setText(tr("Synchronise hidden files and folders"));
    page->advanced->addWidget(page->sync_hidden, 0, 0);
    
    QLabel * folder1_label = new QLabel (page->advanced);
    folder1_label->setText(tr("<b>Folder 1:</b>"));
    page->advanced->addWidget(folder1_label, 1, 0);
    QHBoxLayout * folder1_layout = new QHBoxLayout (page->advanced);
	page->backup_folder_1 = new QCheckBox (tr("Do not backup updated files"), page->advanced);
	page->backup_folder_1->setStatusTip(tr("Do not backup updated files"));
	folder1_layout->addWidget(page->backup_folder_1);
	page->update_only_1 = new QCheckBox (tr("Update existing files only"), page->advanced);
	page->update_only_1->setStatusTip(tr("Update existing files only"));
	folder1_layout->addWidget(page->update_only_1);
	page->advanced->addLayout(folder1_layout, 2, 0);
	QLabel * folder2_label = new QLabel (page->advanced);
    folder2_label->setText(tr("<b>Folder 2:</b>"));
    page->advanced->addWidget(folder2_label, 3, 0);
    QHBoxLayout * folder2_layout = new QHBoxLayout (page->advanced);
	page->backup_folder_2 = new QCheckBox (tr("Do not backup updated files"), page->advanced);
	page->backup_folder_2->setStatusTip(tr("Do not backup updated files"));
	folder2_layout->addWidget(page->backup_folder_2);
	page->update_only_2 = new QCheckBox (tr("Update existing files only"), page->advanced);
	page->update_only_2->setStatusTip(tr("Update existing files only"));
	folder2_layout->addWidget(page->update_only_2);
	page->advanced->addLayout(folder2_layout, 4, 0);
	mainglayout->addWidget(page->advanced, 7, 0);
    
    page->sync_timer = new QTimer();
    page->sync_timer->connect(page->sync_timer, SIGNAL(timeout()), page, SLOT(syncPage()));
	tabs.insert(page->tab, page);

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
    SyncPage * page = tabs.value(syncTab); int x = syncFiles;
    QString directory1 = page->sync_folder_1->text();
    QString directory2 = page->sync_folder_2->text();
    if ((directory1.isEmpty()) || (directory2.isEmpty()))
    {
        QMessageBox::warning(this, tr("Synkron - %1").arg(tabWidget->tabText(tabWidget->indexOf(page->tab))), tr("Choose the synchronization folders first."));
        return;
    }
    setFoldersEnabled(page, false);
    QDir d1 (directory1); QDir d2 (directory2);
    if ((!d1.exists()) || (!d2.exists())) {
		addTableItem(page, tr("%1	Synchronisation failed: Directory not found").arg(QTime().currentTime().toString("hh:mm:ss")), "", "", QBrush(Qt::red), QBrush(Qt::white));
		if (page->periodical_sync->isChecked()) {
			page->sync_timer->stop(); page->sync_timer->start(page->sync_interval->value()*60000);
			setFoldersEnabled(page, false);
			setPeriodicalEnabled(page, false);
		} else setFoldersEnabled(page, true);
		return;
	}
    if (d1.path() == d2.path()) {
       QMessageBox::warning(this, tr("Synkron - %1").arg(tabWidget->tabText(tabWidget->indexOf(page->tab))), tr("Directories with the same path selected."));
       setFoldersEnabled(page, true); return;
    }
    bool repeated = false; if (!syncingAll) { syncFiles = 0; }
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	qApp->processEvents();
    subSync(d1, d2, page, repeated);
    QApplication::restoreOverrideCursor();
    if (page->periodical_sync->isChecked()) {
		page->sync_timer->stop(); page->sync_timer->start(page->sync_interval->value()*60000);
		setPeriodicalEnabled(page, false);
	}
    else {
		page->sync_timer->stop();
		setPeriodicalEnabled(page, true);
		page->periodical_start->setEnabled(false);
		setFoldersEnabled(page, true);
	}
	saveSettings();
    addTableItem(page, tr("%1	Synchronisation complete: %2 file(s) synchronised").arg(QTime().currentTime().toString("hh:mm:ss")).arg(syncFiles-x), "", "", QBrush(Qt::green));
	if (!syncingAll) { trayIcon->showMessage(tr("Synchronisation complete"), tr("%1 files synchronised").arg(syncFiles)); syncFiles = 0; }
}

void MainWindow::subSync(QDir& d1, QDir& d2, SyncPage * page, bool repeated)
{
	if (folders_blacklist.contains(d1.path(), Qt::CaseInsensitive)) {
		addTableItem(page, tr("Folder %1 blacklisted, skipped").arg(d1.path()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
		return;
	}
	if (folders_blacklist.contains(d2.path(), Qt::CaseInsensitive)) {
		return;
	}
	QFileInfoList d1_entries; QFileInfoList d2_entries;
	if (page != 0) {
		if (page->sync_hidden->isChecked()) {
			d1_entries = d1.entryInfoList((QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden), (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
    		d2_entries = d2.entryInfoList((QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden), (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
		} else {
			d1_entries = d1.entryInfoList((QDir::NoDotAndDotDot | QDir::AllEntries), (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
    		d2_entries = d2.entryInfoList((QDir::NoDotAndDotDot | QDir::AllEntries), (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
		}
	}
	else {
		d1_entries = d1.entryInfoList((QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden), (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
    	d2_entries = d2.entryInfoList((QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden), (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
	}
    QString update_time = (QDateTime::currentDateTime()).toString("yyyy.MM.dd-hh.mm.ss");
    QString buffer;
    bool found = false; MTFile * file; QFileInfo fileInfo; QDir *temp = new QDir (QDir::homePath());
    for (int i = 0; i < d1_entries.count(); ++i) {
		if (files_blacklist.contains(d1_entries.at(i).absoluteFilePath(), Qt::CaseInsensitive)) {
			addTableItem(page, tr("File %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
			continue;
		}
		if (d1_entries.at(i).isDir()) {
			if (folders_blacklist.contains(d1_entries.at(i).absoluteFilePath(), Qt::CaseInsensitive)) {
				addTableItem(page, tr("Folder %1 blacklisted, skipped").arg(d1_entries.at(i).absoluteFilePath()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::darkGray), QBrush(Qt::white));
				continue;
			}
		}
        found = false;
        for (int n = 0; n < d2_entries.count(); ++n) {
        	if (d1_entries.at(i).fileName() == d2_entries.at(n).fileName() && repeated == false) {
				if (files_blacklist.contains(d2_entries.at(n).absoluteFilePath(), Qt::CaseInsensitive)) {
					found = true; continue;
				} if (d2_entries.at(n).isDir()) {
					if (folders_blacklist.contains(d2_entries.at(n).absoluteFilePath(), Qt::CaseInsensitive)) {
						found = true; continue;
					}
				}
				if (d1_entries.at(i).isDir() && d2_entries.at(n).isDir()) {
                	QDir subDir5 (d1_entries.at(i).absoluteFilePath());
                	QDir subDir6 (d2_entries.at(n).absoluteFilePath());
                	subSync(subDir5, subDir6, page, repeated);
                	found = true; continue;
            	}
            	else if (d1_entries.at(i).isDir()) {
                	if (!repeated) {
						addTableItem(page, tr("A folder (%1) and a file (%2) with the same name have been found. Unable to synchronise these files.").arg(d1_entries.at(i).fileName()).arg(d2_entries.at(n).fileName()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::red), QBrush(Qt::white));
                    }
                    found = true; continue;
            	}
            	else if (d2_entries.at(n).isDir()) {
               		if (!repeated) {
						addTableItem(page, tr("A file (%1) and a folder (%2) with the same name have been found. Unable to synchronise these files.").arg(d1_entries.at(i).fileName()).arg(d2_entries.at(n).fileName()), "", QString::fromUtf8(":/new/prefix1/images/folder_16.png"), QBrush(Qt::red), QBrush(Qt::white));
                    }
                    found = true; continue;
            	}
            	else if (d1_entries.at(i).lastModified() < d2_entries.at(n).lastModified()) {
					file = new MTFile (d1_entries.at(i).absoluteFilePath());
                    if (page!=0) {
						if (d1.path().startsWith(page->sync_folder_1->text(), Qt::CaseInsensitive) && page->backup_folder_1->isChecked()) { goto copying1; }
                    	else if (d1.path().startsWith(page->sync_folder_2->text(), Qt::CaseInsensitive) && page->backup_folder_2->isChecked()) { goto copying1; }
					}
					if (!file->open(QIODevice::ReadOnly)) {
                       unknownError(page, d1_entries.at(i).fileName(), tr("file"), tr("open"), ":/new/prefix1/images/file.png"); delete file; found = true; continue;
                    }
                    temp->mkpath(QString(".Synkron/%2").arg(update_time));
                    if (!(file->copy(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d1_entries.at(i).fileName()).arg(syncFiles)))) {
                       unknownError(page, d1_entries.at(i).fileName(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png", tr(" to temp")); delete file; found = true; continue;
                    }
                    synchronised << d1_entries.at(i).fileName() << update_time << d1_entries.at(i).absoluteFilePath();
                    synchronised << QString("%1/.Synkron/%2/%3.%4").arg(QDir::QDir::homePath()).arg(update_time).arg(d1_entries.at(i).fileName()).arg(syncFiles);
					copying1:
                    file->remove(); delete file;
                    file = new MTFile (d2_entries.at(n).absoluteFilePath());
                    if (!file->open(QIODevice::ReadOnly)) {
                       unknownError(page, d2_entries.at(n).fileName(), tr("file"), tr("open"), ":/new/prefix1/images/file.png"); delete file; found = true; continue;
                    }
                    buffer.clear(); buffer = d1_entries.at(i).absoluteFilePath();
                    if (!file->copy(buffer)) {
                       unknownError(page, d1_entries.at(i).fileName(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png"); delete file; found = true; continue;
                    }
                    addTableItem(page, d2_entries.at(n).absoluteFilePath(), d1_entries.at(i).absoluteFilePath(), QString::fromUtf8(":/new/prefix1/images/file.png"));
                    file->close();
                    syncFiles++; delete file;
            	}
                else if (d1_entries.at(i).lastModified() > d2_entries.at(n).lastModified()) {
                    file = new MTFile (d2_entries.at(n).absoluteFilePath());
                    if (page!=0) {
                    	if (d2.path().startsWith(page->sync_folder_1->text(), Qt::CaseInsensitive) && page->backup_folder_1->isChecked()) { goto copying2; }
                    	else if (d2.path().startsWith(page->sync_folder_2->text(), Qt::CaseInsensitive) && page->backup_folder_2->isChecked()) { goto copying2; }
					}
                    if (!file->open(QIODevice::ReadOnly)) {
                       unknownError(page, d2_entries.at(n).fileName(), tr("file"), tr("open"), ":/new/prefix1/images/file.png"); delete file; found = true; continue;
                    }
                    temp->mkpath(QString(".Synkron/%2").arg(update_time));
                    if (!(file->copy(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d2_entries.at(n).fileName()).arg(syncFiles)))) {
                       unknownError(page, d2_entries.at(n).fileName(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png", tr(" to temp")); delete file; found = true; continue;
                    }
                    synchronised << d2_entries.at(n).fileName() << update_time << d2_entries.at(n).absoluteFilePath();
                    synchronised << QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d2_entries.at(n).fileName()).arg(syncFiles);
                    copying2:
                    file->remove(); delete file;
                    file = new MTFile (d1_entries.at(i).absoluteFilePath());
                    if (!file->open(QIODevice::ReadOnly)) {
                       unknownError(page, d1_entries.at(i).fileName(), tr("file"), tr("open"), ":/new/prefix1/images/file.png"); delete file; found = true; continue;
                    }
                    buffer.clear(); buffer = d2_entries.at(n).absoluteFilePath();
                    if (!file->copy(buffer)) {
                       unknownError(page, d2_entries.at(n).fileName(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png"); delete file; found = true; continue;
                    }
                    addTableItem(page, d1_entries.at(i).absoluteFilePath(), d2_entries.at(n).absoluteFilePath(), QString::fromUtf8(":/new/prefix1/images/file.png"));
                    file->close();
                    syncFiles++; delete file;
                }
                found = true; break;
        }
        else if (d1_entries.at(i).fileName() == d2_entries.at(n).fileName() && repeated == true) { found = true; }
    }

        if (found == false) {
			if (page!=0) {
				if (page->update_only_1->isChecked() && QString("%1/%2").arg(d2.path()).arg(d1_entries.at(i).fileName()).startsWith(page->sync_folder_1->text())) goto end;
            	if (page->update_only_2->isChecked() && QString("%1/%2").arg(d2.path()).arg(d1_entries.at(i).fileName()).startsWith(page->sync_folder_2->text())) goto end;
			}
			file = new MTFile (d1_entries.at(i).absoluteFilePath());
            fileInfo.setFile(*file);
            if (!fileInfo.isDir()) {
                if (!file->open(QIODevice::ReadOnly)) {
                     unknownError(page, d1_entries.at(i).fileName(), tr("file"), tr("open"), ":/new/prefix1/images/file.png"); delete file; continue;
                }
                
            }
            buffer.clear();
            buffer = QString("%1/%2").arg(d2.path()).arg(d1_entries.at(i).fileName());
            if (!file->copy(buffer)) {
                if (d1_entries.at(i).isDir()) {
                    if (!(d2.mkdir(d1_entries.at(i).fileName()))) {
                       unknownError(page, d1_entries.at(i).fileName(), tr("folder"), tr("copy"), ":/new/prefix1/images/folder_16.png"); delete file; continue;
                    }
                    QDir subDir1 (d1_entries.at(i).absoluteFilePath());
                    QDir subDir2 (QString("%1/%2").arg(d2.absolutePath()).arg(d1_entries.at(i).fileName()));
                    subSync(subDir1, subDir2, page, repeated);
                    addTableItem(page, d1_entries.at(i).absoluteFilePath(), buffer, QString::fromUtf8(":/new/prefix1/images/folder_16.png"));
                }
                else {
                     unknownError(page, d1_entries.at(i).fileName(), tr("file"), tr("copy"), ":/new/prefix1/images/file.png"); delete file; continue;
                }
            }
            else {
				addTableItem(page, d1_entries.at(i).absoluteFilePath(), buffer, QString::fromUtf8(":/new/prefix1/images/file.png"));
            }
            file->close();
            syncFiles++; delete file;
            end: syncFiles = syncFiles;
        }
    }
    if (!repeated) {
        subSync (d2, d1, page, true);
    }
}

void MainWindow::syncAll()
{
     syncingAll = true; syncFiles = 0;
     for (int i = 0; i<(tabWidget->count()); ++i) {
         sync(tabWidget->widget(i));
     }
     trayIcon->showMessage(tr("Synchronisation complete"), tr("%1 files synchronised").arg(syncFiles));
     syncingAll = false; syncFiles = 0;
}

void MainWindow::addTableItem(SyncPage * page, QString source, QString destination, QString icon, QBrush background, QBrush foreground)
{
	if (page == 0) tw_multi->insertRow(tw_multi->rowCount());
	else page->tw->insertRow(page->tw->rowCount());
	QTableWidgetItem *item = new QTableWidgetItem (source);
	if (icon!="") item->setIcon(QIcon(icon));
	if (destination=="") {
		item->setBackground(background);
		item->setForeground(foreground);
		if (page == 0) tw_multi->setSpan(tw_multi->rowCount() - 1, 0, 1, 2);
		else page->tw->setSpan(page->tw->rowCount() - 1, 0, 1, 2);
	}
	if (page == 0) tw_multi->setItem(tw_multi->rowCount() - 1, 0, item);
	else page->tw->setItem(page->tw->rowCount() - 1, 0, item);
	if (destination!="") {
		item = new QTableWidgetItem(destination);
        if (icon!="") item->setIcon(QIcon(icon));
        if (page == 0) tw_multi->setItem(tw_multi->rowCount() - 1, 1, item);
        else page->tw->setItem(page->tw->rowCount() - 1, 1, item);
	}
	if (page == 0) {
		tw_multi->setRowHeight(tw_multi->rowCount() - 1, 16);
		tw_multi->setCurrentCell(tw_multi->rowCount() - 1, 0);
		tw_multi->clearSelection();
	} else {
		page->tw->setRowHeight(page->tw->rowCount() - 1, 16);
		page->tw->setCurrentCell(page->tw->rowCount() - 1, 0);
		page->tw->clearSelection();
	}
	qApp->processEvents();
}

void MainWindow::unknownError(SyncPage * page, QString fileName, QString type, QString action, QString icon, QString toTemp)
{
	addTableItem(page, tr("Unknown error %1ing %2: %3%4").arg(action).arg(type).arg(fileName).arg(toTemp), "", icon, QBrush(Qt::red), QBrush(Qt::white));
}

void MainWindow::logSearch(const QString text)
{
	SyncPage * page = tabs.value(tabWidget->currentWidget());
	for (int i = 0; i < page->tw->rowCount(); ++i) {
		if (page->tw->item(i, 0)->text().contains(text, Qt::CaseInsensitive)) page->tw->showRow(i);
		else if (page->tw->columnSpan(i, 0)!=2) {
			if (page->tw->item(i, 1)->text().contains(text, Qt::CaseInsensitive)) page->tw->showRow(i);
			else page->tw->hideRow(i);
		}
		else page->tw->hideRow(i);
	}
}

void SyncPage::syncPage()
{
     emit sigsync(tab);
}

void MainWindow::tabNameEdited()
{
	SyncPage * page = tabs.value(tabWidget->currentWidget());
	tabWidget->setTabText(tabWidget->indexOf(page->tab), page->tab_name->text());
}

void MainWindow::startPeriodical(QWidget* syncTab)
{
	SyncPage * page = 0;
	if (syncTab==0) page = tabs.value(tabWidget->currentWidget());
	else page = tabs.value(syncTab);
	page->sync_timer->stop(); page->sync_timer->start(page->sync_interval->value()*60000);
	setFoldersEnabled(page, false);
	setPeriodicalEnabled(page, false);
}

void MainWindow::stopPeriodical()
{
	SyncPage * page = tabs.value(tabWidget->currentWidget());
	page->sync_timer->stop();
	setFoldersEnabled(page, true);
	setPeriodicalEnabled(page, true);
}

void MainWindow::setPeriodicalEnabled(SyncPage * page, bool enable)
{
	page->periodical_stop->setEnabled(!enable);
	page->periodical_start->setEnabled(enable);
}

void MainWindow::setFoldersEnabled(SyncPage * page, bool enable)
{
	page->sync_folder_1->setEnabled(enable);
	page->sync_folder_2->setEnabled(enable);
	page->sync_text->setEnabled(enable);
	page->browse_1->setEnabled(enable);
	page->browse_2->setEnabled(enable);
	page->sync->setEnabled(enable);
	page->sync_hidden->setEnabled(enable);
	page->tab_name->setEnabled(enable);
	page->periodical_sync->setEnabled(enable);
	if (page->periodical_sync->isChecked()) page->sync_interval->setEnabled(enable);
	page->min_text->setEnabled(enable);
	page->advanced->setEnabled(enable);
}
