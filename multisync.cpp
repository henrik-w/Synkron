#include "main_window.h"

MultisyncPage::MultisyncPage(MainWindow *parent) : AbstractSyncPage(parent)
{
	setupUi(this);
	
	stop_sync_btn->setVisible(false);
	
	setAdvancedGB();
}

void MultisyncPage::setAdvancedGB()
{
	sync_hidden = new QCheckBox(advanced);
    sync_hidden->setChecked(true);
    sync_hidden->setStatusTip(tr("Synchronise hidden files and folders"));
    sync_hidden->setText(tr("Synchronise hidden files and folders"));
    advanced->addWidget(sync_hidden, 0, 0);
    
    sync_nosubdirs = new QCheckBox(advanced);
    sync_nosubdirs->setChecked(false);
    sync_nosubdirs->setStatusTip(tr("Do not synchronise subdirectories"));
    sync_nosubdirs->setText(tr("Do not synchronise subdirectories"));
    advanced->addWidget(sync_nosubdirs, 1, 0);
    
	ignore_blacklist = new QCheckBox(advanced);
    ignore_blacklist->setChecked(false);
    ignore_blacklist->setStatusTip(tr("Ignore blacklist"));
    ignore_blacklist->setText(tr("Ignore blacklist"));
    advanced->addWidget(ignore_blacklist, 2, 0);
    
    symlinks = new QCheckBox;
#ifdef Q_WS_WIN
    symlinks->setChecked(false);
#else
	symlinks->setParent(advanced);
	symlinks->setChecked(false);
    symlinks->setStatusTip(tr("Follow symbolic links"));
    symlinks->setText(tr("Follow symbolic links"));
    advanced->addWidget(symlinks, 3, 0);
#endif
    
    QLabel * folder1_label = new QLabel(advanced);
    folder1_label->setText(tr("<b>Sources:</b>"));
	advanced->addWidget(folder1_label, 4, 0);
    QHBoxLayout * folder1_layout = new QHBoxLayout(advanced);
	backup_folder_1 = new QCheckBox (tr("Do not backup updated files"), advanced);
	backup_folder_1->setStatusTip(tr("Do not backup updated files"));
	folder1_layout->addWidget(backup_folder_1);
	folder1_layout->addItem(new QSpacerItem(10, 5, QSizePolicy::Fixed, QSizePolicy::Fixed));
	update_only_1 = new QCheckBox (tr("Update existing files only"), advanced);
	update_only_1->setStatusTip(tr("Update existing files only"));
	folder1_layout->addWidget(update_only_1);
	advanced->addLayout(folder1_layout, 5, 0);
	move = new QCheckBox (tr("Move contents to destination, leaving sources empty"), advanced);
	move->setStatusTip(tr("Move contents to destination, leaving sources empty"));
	connect(move, SIGNAL(stateChanged(int)), this, SLOT(moveStateChanged(int)));
	advanced->addWidget(move, 6, 0);
	QLabel * folder2_label = new QLabel(advanced);
    folder2_label->setText(tr("<b>Destination:</b>"));
    advanced->addWidget(folder2_label, 7, 0);
    QHBoxLayout * folder2_layout = new QHBoxLayout(advanced);
	backup_folder_2 = new QCheckBox (tr("Do not backup updated files"), advanced);
	backup_folder_2->setStatusTip(tr("Do not backup updated files"));
	folder2_layout->addWidget(backup_folder_2);
	folder2_layout->addItem(new QSpacerItem(10, 5, QSizePolicy::Fixed, QSizePolicy::Fixed));
	update_only_2 = new QCheckBox (tr("Update existing files only"), advanced);
	update_only_2->setStatusTip(tr("Update existing files only"));
	folder2_layout->addWidget(update_only_2);
	advanced->addLayout(folder2_layout, 8, 0);
	clone_folder1 = new QCheckBox (tr("Clone sources"), advanced);
	clone_folder1->setStatusTip(tr("Clone sources"));
	connect(clone_folder1, SIGNAL(stateChanged(int)), this, SLOT(cloneStateChanged(int)));
	advanced->addWidget(clone_folder1, 9, 0);/*
	advanced->gridLayout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 1);
	filters = new QGroupBox(tr("Filters"), advanced);
	filters->setCheckable(true);
	filters->setChecked(false);
#ifdef Q_WS_WIN
	filters->setMaximumSize(150, 185);
#elif defined Q_WS_MAC
	filters->setMaximumSize(150, 220);
#else
	filters->setMaximumSize(150, 200);
#endif
	filters->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	QVBoxLayout * vlayout_filters = new QVBoxLayout(filters);
#ifndef Q_WS_MAC
	vlayout_filters->setContentsMargins(9, 6, 9, 9);
#endif
	lw_filters = new QListWidget(filters);
	lw_filters->setMinimumSize(90, 80);
	QListWidgetItem * item;
    for (int f = 0; f < mp_parent->filter_list->count(); ++f) {
		item = new QListWidgetItem (mp_parent->filter_list->item(f)->text());
		item->setCheckState(Qt::Unchecked);
		lw_filters->addItem(item);
	}
	vlayout_filters->addWidget(lw_filters);
	advanced->addWidget(filters, 0, 2, 9, 1);*/
}

MultisyncPage * MainWindow::addMultiTab()
{
	MultisyncPage * multi_page = new MultisyncPage(this);
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
	if (destination_multi->text()=="") { QMessageBox::information(this, tr("Synkron"), tr("Choose a destination first.")); return 0; }
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
	QStringList pathlist; QString path;
	QDir destination; QDir syncfolder; int all_synced_files = 0;
	for (int i = 0; i < list_multi->count(); ++i) {
		if (list_multi->item(i)->checkState()==Qt::Unchecked) continue;
		destination.setPath(destination_multi->text());
		if (!destination.exists()) {
            if (!QDir().mkpath(destination.path())) {
                addTableItem(tr("%1	Synchronisation failed: Failed to create directory %2").arg(QTime().currentTime().toString("hh:mm:ss")).arg(destination.path()), "", "", QBrush(Qt::darkBlue), QBrush(Qt::white));
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
    	update_time = (QDateTime::currentDateTime()).toString("yyyy.MM.dd-hh.mm.ss");
		if (move->isChecked()) moveContents(syncfolder, destination);
		else subSync(syncfolder, destination, false);
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

void MultisyncPage::moveChecked(int state)
{
	if ((Qt::CheckState)state==Qt::Checked) {
		sync_multi->setText(tr("Move contents"));
		sync_multi->setStatusTip(tr("Move contents to destination, leaving sources empty"));
	} else {
		sync_multi->setText(tr("Multisync"));
		sync_multi->setStatusTip(tr("Start multisync"));
	}
}

void MultisyncPage::cloneChecked(int state)
{
	if ((Qt::CheckState)state==Qt::Checked) {
		sync_multi->setText(tr("Clone sources"));
		sync_multi->setStatusTip(tr("Clone sources"));
	} else {
		sync_multi->setText(tr("Multisync"));
		sync_multi->setStatusTip(tr("Start multisync"));
	}
}
