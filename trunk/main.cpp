#include "main_window.h"

MainWindow::MainWindow()
{
	f_ver = 1.1;
	ver = "1.1.0";
	
    setupUi(this);
    if (tr("LTR") == "RTL")
    {
    	qApp->setLayoutDirection(Qt::RightToLeft);
    	
    }
    
#ifdef Q_WS_MAC
	actionBrushedMetalStyle = new QAction(tr("Use the brushed metal style"), this);
	actionBrushedMetalStyle->setStatusTip(tr("Use the brushed metal style"));
	actionBrushedMetalStyle->setCheckable(true);
	menuOptions->addAction(actionBrushedMetalStyle);
#endif
    
    http = new QHttp(this);
	http_buffer = new QBuffer(this);
    
    createActions();
    createTrayIcon();
    trayIcon->show();
    trayIconVisible(true);
    syncingAll = false;
    sched_removed = false;
    run_hidden = false;
	no_closedialogue = false;
    
    tw_schedules->setHorizontalHeaderLabels(QStringList() << tr("Schedule name") << tr("Status"));
    tw_schedules->verticalHeader()->hide();
    tw_schedules->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    
    actgrpView = new QActionGroup(this);
    actgrpView->addAction(actionSynchronise);
    actgrpView->addAction(actionRestore);
    actgrpView->addAction(actionBlacklist);
    actgrpView->addAction(actionMultisync);
    actgrpView->addAction(actionScheduler);
    actgrpView->addAction(actionFilters);
    
    synkron_i18n.insert(tr("Arabic"), "Arabic");
	synkron_i18n.insert(tr("English"), "English");
    synkron_i18n.insert(tr("Slovak"), "Slovak");
    synkron_i18n.insert(tr("German"), "German");
    
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(actionNew_sync, SIGNAL(triggered()), this, SLOT(addTab()));
    connect(actionClose_sync, SIGNAL(triggered()), this, SLOT(closeTab()));
    connect(actgrpView, SIGNAL(triggered(QAction*)), this, SLOT(switchView(QAction*)));
    connect(restore_list, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(restoreItemChanged(QListWidgetItem *, QListWidgetItem *)));
    connect(to_black_list, SIGNAL(stateChanged(int)), this, SLOT(addToBlackList(int)));
    connect(restore_files, SIGNAL(released()), this, SLOT(restoreFiles()));
    connect(blacklist_addfile, SIGNAL(released()), this, SLOT(addFileToBlacklist()));
    connect(blacklist_removefile, SIGNAL(released()), this, SLOT(removeFileFromBlacklist()));
    connect(blacklist_addfolder, SIGNAL(released()), this, SLOT(addFolderToBlacklist()));
    connect(blacklist_removefolder, SIGNAL(released()), this, SLOT(removeFolderFromBlacklist()));
    connect(selTmpAllBtn, SIGNAL(released()), this, SLOT(selTmpAll()));
    connect(actionRun_hidden, SIGNAL(toggled(bool)), this, SLOT(setRunHidden(bool)));
    connect(actionSync_all, SIGNAL(triggered()), this, SLOT(syncAll()));
	connect(actionCheck_for_updates, SIGNAL(triggered()), this, SLOT(checkForUpdates()));
	connect(http, SIGNAL(done(bool)), this, SLOT(httpRequestFinished(bool)));
	connect(add_schedule, SIGNAL(released()), this, SLOT(addSchedule()));
	connect(remove_schedule, SIGNAL(released()), this, SLOT(removeSchedule()));
	connect(tw_schedules, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(scheduleActivated(int, int, int, int)));
	connect(sched_name, SIGNAL(textEdited(const QString)), this, SLOT(setSchedName(const QString)));
	connect(sched_add_time, SIGNAL(released()), this, SLOT(addSchedTime()));
	connect(sched_remove_time, SIGNAL(released()), this, SLOT(removeSchedTime()));
	connect(sched_start, SIGNAL(released()), this, SLOT(startSchedule()));
	connect(sched_stop, SIGNAL(released()), this, SLOT(stopSchedule()));
	connect(restore_search, SIGNAL(textEdited(const QString)), this, SLOT(searchLw(const QString)));
	connect(startall_schedules, SIGNAL(released()), this, SLOT(startAllSchedules()));
	connect(stopall_schedules, SIGNAL(released()), this, SLOT(stopAllSchedules()));
	connect(actionChange_language, SIGNAL(triggered()), this, SLOT(changeLanguage()));
	connect(filter_add, SIGNAL(released()), this, SLOT(addFilter()));
	connect(filter_remove, SIGNAL(released()), this, SLOT(removeFilter()));
	connect(filter_add_extension, SIGNAL(released()), this, SLOT(addFilterExtension()));
	connect(filter_remove_extension, SIGNAL(released()), this, SLOT(removeFilterExtension()));
	connect(filter_list, SIGNAL(itemSelectionChanged()), this, SLOT(filterChanged()));
	connect(sched_tab_lw, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(schedTabClicked(QListWidgetItem*)));
	connect(sched_time_lw, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(schedTimeClicked(QListWidgetItem*)));
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(closeApp()));
	connect(actionSave_log, SIGNAL(triggered()), this, SLOT(saveSyncLog()));
    
    setCleanGB();
    tabWidget->removeTab(0);
    multi_tabWidget->removeTab(0);
    readSettings();
    if (tabWidget->count()==0) addSyncTab();
    if (multi_tabWidget->count()==0) addMultiTab();
	
    if (qApp->arguments().count() > 1) {
        QFileInfo file_info (qApp->arguments().at(1));
        if (file_info.exists()) {
			addMultiTab()->loadMultisync(file_info.absoluteFilePath());
			multi_tabWidget->setCurrentIndex(multi_tabWidget->count()-1);
            actionMultisync->trigger();
        }
    }
}

bool MainWindow::closeDialogue()
{
	QSettings settings ("Matus Tomlein", "Synkron");
    if (settings.value("dont_ask_on_quit").toBool()==true) {
		if (settings.value("minimise_on_quit").toBool()==true) {
			minimizeAction->trigger();
			return false;
		} else if (settings.value("minimise_on_quit").toBool()==false) {
			return true;
		}
	}
	QDialog * cl_dialogue = new QDialog (this, Qt::Dialog);
	cl_dialogue->setWindowModality(Qt::WindowModal);
	//cl_dialogue->setAttribute(Qt::WA_DeleteOnClose);
	cl_dialogue->setWindowTitle(tr("Quit Synkron"));
	QGridLayout * cl_glayout = new QGridLayout (cl_dialogue);
	cl_glayout->setMargin(4); cl_glayout->setSpacing(10);
	QLabel * cl_label = new QLabel (cl_dialogue);
	cl_label->setText(tr("Are you sure you want to quit?"));
	cl_label->setAlignment(Qt::AlignHCenter);
	cl_glayout->addWidget(cl_label, 0, 0);
	QCheckBox * rm_minimise = new QCheckBox(cl_dialogue);
	rm_minimise->setText(tr("Do not ask me again"));
	rm_minimise->setChecked(true);
	cl_glayout->addWidget(rm_minimise, 1, 0);
	QHBoxLayout * hlayout = new QHBoxLayout (cl_dialogue);
	hlayout->addStretch();
	QPushButton * close_btn = new QPushButton (cl_dialogue);
	close_btn->setText(tr("&Quit"));
	connect(close_btn, SIGNAL(released()), cl_dialogue, SLOT(accept()));
	hlayout->addWidget(close_btn);
	QPushButton * minimise_btn = new QPushButton (cl_dialogue);
	minimise_btn->setText(tr("&Minimise"));
	connect(minimise_btn, SIGNAL(released()), cl_dialogue, SLOT(reject()));
	hlayout->addWidget(minimise_btn);
	hlayout->addStretch();
	cl_glayout->addLayout(hlayout, 2, 0);
	switch (cl_dialogue->exec()) {
		case 0: // Minimise
			settings.setValue("dont_ask_on_quit", rm_minimise->isChecked());
			settings.setValue("minimise_on_quit", true);
			minimizeAction->trigger();
			return false;
			break;	
		case 1: // Quit
			settings.setValue("dont_ask_on_quit", rm_minimise->isChecked());
			settings.setValue("minimise_on_quit", false);
			return true;
			break;
	}
	return true;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
	if (!no_closedialogue) {
		if (!closeDialogue()) {
			event->ignore();
			return;
		}
	}
	saveSettings();
    trayIcon->hide();
    QSettings sync_settings(QSettings::IniFormat, QSettings::UserScope, "Matus Tomlein", "Synkron");
}

void MainWindow::saveSettings()
{
	QSettings settings ("Matus Tomlein", "Synkron");
    QSettings sync_settings(QSettings::IniFormat, QSettings::UserScope, "Matus Tomlein", "Synkron");
    sync_settings.clear();
    QStringList tabs_list;
    for (int i = 0; i < tabWidget->count(); ++i) {
		tabs_list << tabWidget->tabText(i);
		sync_settings.setValue(QString("tab_%1_%2/folder1").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->sync_folder_1->text());
		sync_settings.setValue(QString("tab_%1_%2/folder2").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->sync_folder_2->text());
		sync_settings.setValue(QString("tab_%1_%2/periodical").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->periodical_sync->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("tab_%1_%2/sync_hidden").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->sync_hidden->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("tab_%1_%2/sync_interval").arg(tabWidget->tabText(i)).arg(i), QVariant(tabs.value(tabWidget->widget(i))->sync_interval->value()).toString());
		sync_settings.setValue(QString("tab_%1_%2/periodical_stop").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->periodical_stop->isEnabled() ? "on" : "off");
		sync_settings.setValue(QString("tab_%1_%2/advanced").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->advanced->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("tab_%1_%2/backup_folder_1").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->backup_folder_1->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("tab_%1_%2/backup_folder_2").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->backup_folder_2->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("tab_%1_%2/update_only_1").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->update_only_1->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("tab_%1_%2/update_only_2").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->update_only_2->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("tab_%1_%2/sync_nosubdirs").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->sync_nosubdirs->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("tab_%1_%2/ignore_blacklist").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->ignore_blacklist->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("tab_%1_%2/move").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->move->isChecked() ? "checked" : "unchecked");
	#ifndef Q_WS_WIN
		sync_settings.setValue(QString("tab_%1_%2/symlinks").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->symlinks->isChecked() ? "checked" : "unchecked");
	#endif
		sync_settings.setValue(QString("tab_%1_%2/filters_gb").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->filters->isChecked() ? "checked" : "unchecked");
		QStringList flist;
		for (int n = 0; n < tabs.value(tabWidget->widget(i))->lw_filters->count(); ++n) {
			if (tabs.value(tabWidget->widget(i))->lw_filters->item(n)->checkState()==Qt::Checked) {
				flist << tabs.value(tabWidget->widget(i))->lw_filters->item(n)->text();
			}
		}
		sync_settings.setValue(QString("tab_%1_%2/flist").arg(tabWidget->tabText(i)).arg(i), flist);
	}
	sync_settings.setValue("tabs", tabs_list);
	QStringList filters;
	for (int i = 0; i < filter_list->count(); ++i) {
		filters << filter_list->item(i)->text();
		sync_settings.setValue(QString("filter_%1_%2/extensions").arg(filter_list->item(i)->text()).arg(i), ((Filter *)filter_list->item(i))->extensions);
	}
	sync_settings.setValue("filters", filters);
	QStringList multitabs_list; MultisyncPage * multi_page;
    for (int i = 0; i < multi_tabWidget->count(); ++i) {
		multi_page = (MultisyncPage *) multi_tabWidget->widget(i);
		multitabs_list << multi_tabWidget->tabText(i);
		QStringList sources;
		for (int s = 0; s < multi_page->list_multi->count(); ++s) {
			sources << multi_page->list_multi->item(s)->text();
			if (multi_page->list_multi->item(s)->checkState() == Qt::Checked) sources << "checked";
			else sources << "unchecked";
		}
		sync_settings.setValue(QString("multitab_%1_%2/sources").arg(multi_tabWidget->tabText(i)).arg(i), sources);
		sync_settings.setValue(QString("multitab_%1_%2/destination").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->destination_multi->text());
		sync_settings.setValue(QString("multitab_%1_%2/advanced").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->advanced->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("multitab_%1_%2/sync_hidden").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->sync_hidden->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("multitab_%1_%2/backup_folder_1").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->backup_folder_1->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("multitab_%1_%2/backup_folder_2").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->backup_folder_2->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("multitab_%1_%2/update_only_1").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->update_only_1->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("multitab_%1_%2/update_only_2").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->update_only_2->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("multitab_%1_%2/sync_nosubdirs").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->sync_nosubdirs->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("multitab_%1_%2/ignore_blacklist").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->ignore_blacklist->isChecked() ? "checked" : "unchecked");
		sync_settings.setValue(QString("multitab_%1_%2/move").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->move->isChecked() ? "checked" : "unchecked");
	#ifndef Q_WS_WIN
		sync_settings.setValue(QString("multitab_%1_%2/symlinks").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->symlinks->isChecked() ? "checked" : "unchecked");
	#endif
		/*sync_settings.setValue(QString("multitab_%1_%2/filters_gb").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->filters->isChecked() ? "checked" : "unchecked");
		QStringList flist;
		for (int n = 0; n < multi_page->lw_filters->count(); ++n) {
			if (multi_page->lw_filters->item(n)->checkState()==Qt::Checked) {
				flist << multi_page->lw_filters->item(n)->text();
			}
		}
		sync_settings.setValue(QString("multitab_%1_%2/flist").arg(multi_tabWidget->tabText(i)).arg(i), flist);*/
	}
	sync_settings.setValue("multitabs", multitabs_list);
    sync_settings.setValue("synchronised", synchronised);
    sync_settings.setValue("restore_clean_selected", restore_clean_selected->isChecked());
    sync_settings.setValue("restore_clean_by_date", restore_clean_by_date->isChecked());
    sync_settings.setValue("restore_clean_repeated", restore_clean_repeated->isChecked());
    sync_settings.setValue("restore_clean_date", restore_clean_date->value());
    sync_settings.setValue("restore_clean_files", restore_clean_files->value());
    sync_settings.setValue("files_blacklist", files_blacklist);
    sync_settings.setValue("folders_blacklist", folders_blacklist);
    sync_settings.setValue("run_hidden", run_hidden);
    sync_settings.setValue("current_synctab", tabWidget->currentIndex());
    sync_settings.setValue("current_multitab", multi_tabWidget->currentIndex());
    sync_settings.setValue("disable_tray_messages", actionDisable_tray_messages->isChecked());
    QStringList schedules; SyncSchedule * schedule;
	for (int i = 0; i < tw_schedules->rowCount(); ++i) {
		schedule = item_sched_map.value(tw_schedules->item(i, 0));
		schedules << tw_schedules->item(i, 0)->text();
		if (schedule->scheduling) schedules << "on";
		else schedules << "off";
		sync_settings.setValue(QString("sched_%1_%2_tabs").arg(tw_schedules->item(i, 0)->text()).arg(QVariant(i).toString()), schedule->sched_tab_list);
		sync_settings.setValue(QString("sched_%1_%2_times").arg(tw_schedules->item(i, 0)->text()).arg(QVariant(i).toString()), schedule->sched_time_list);
		sync_settings.setValue(QString("sched_%1_%2_checkedtimes").arg(tw_schedules->item(i, 0)->text()).arg(QVariant(i).toString()), schedule->sched_checked_time_list);
	}
	sync_settings.setValue("schedules", schedules);
    settings.setValue("pos", this->pos());
    settings.setValue("size", this->size());
    settings.setValue("ver", QVariant(f_ver).toString());
#ifdef Q_WS_MAC
	settings.setValue("macx_brushedmetalstyle", actionBrushedMetalStyle->isChecked());
#endif
}

void MainWindow::readSettings()
{
	QSettings settings ("Matus Tomlein", "Synkron");
#ifdef Q_WS_MAC
	actionBrushedMetalStyle->setChecked(settings.value("macx_brushedmetalstyle", false).toBool());
	if (actionBrushedMetalStyle->isChecked()) this->setAttribute(Qt::WA_MacBrushedMetal);
	this->setUnifiedTitleAndToolBarOnMac(!actionBrushedMetalStyle->isChecked());
#endif
	QSettings sync_settings(QSettings::IniFormat, QSettings::UserScope, "Matus Tomlein", "Synkron");
	QStringList filters = sync_settings.value("filters").toStringList();
	for (int i = 0; i < filters.count(); ++i) {
		addFilter(filters.at(i), sync_settings.value(QString("filter_%1_%2/extensions").arg(filters.at(i)).arg(i)).toStringList());
	}
	SyncPage * page = 0;
	QStringList tabs_list = sync_settings.value("tabs").toStringList();
	if (tabs_list.count()==0) {
		QStringList recentsyncs = sync_settings.value("recentsyncs").toStringList();
		if (recentsyncs.count() != 0) {
			bool okey; recentsyncs.last().toInt(&okey);
    		if ((recentsyncs.count()-1) % 6 == 5 && okey) {
				for (int i = 0; i < recentsyncs.count(); ++i) {
     				if (i % 6 == 0 || i == 0) { page = addSyncTab();
						QDir dirpath(recentsyncs.at(i));
						page->tab_name->setText(dirpath.path());
						tabWidget->setTabText(tabWidget->indexOf(page->tab), dirpath.path()); }
  		  			if (i % 6 == 1) page->sync_folder_1->setText(recentsyncs.at(i));
    				if (i % 6 == 2) page->sync_folder_2->setText(recentsyncs.at(i));
	    			if (i % 6 == 3) page->periodical_sync->setChecked(recentsyncs.at(i)=="checked");
					if (i % 6 == 4) page->sync_hidden->setChecked(recentsyncs.at(i)=="checked");
					if (i % 6 == 5) page->sync_interval->setValue(recentsyncs.at(i).toInt());
  				}
			}
			else {
				for (int i = 0; i < recentsyncs.count(); ++i) {
     				if (i % 12 == 0 || i == 0) { page = addSyncTab();
						QDir dirpath(recentsyncs.at(i));
						page->tab_name->setText(dirpath.path());
						tabWidget->setTabText(tabWidget->indexOf(page->tab), dirpath.path()); }
   			 		if (i % 12 == 1) page->sync_folder_1->setText(recentsyncs.at(i));
    				if (i % 12 == 2) page->sync_folder_2->setText(recentsyncs.at(i));
    				if (i % 12 == 3) page->periodical_sync->setChecked(recentsyncs.at(i)=="checked");
					if (i % 12 == 4) page->sync_hidden->setChecked(recentsyncs.at(i)=="checked");
					if (i % 12 == 5) page->sync_interval->setValue(recentsyncs.at(i).toInt());
					if (i % 12 == 6) {
						if (recentsyncs.at(i)=="on") startPeriodical(tabWidget->widget(tabWidget->indexOf(page->tab)));
						else  page->periodical_stop->setEnabled(false);
					}
					if (i % 12 == 7) {
						showAdvancedGroupBox((recentsyncs.at(i)=="checked"), page);
					}
					if (i % 12 == 8) {
						page->backup_folder_1->setChecked((recentsyncs.at(i)=="checked"));
					}
					if (i % 12 == 9) {
						page->backup_folder_2->setChecked((recentsyncs.at(i)=="checked"));
					}
					if (i % 12 == 10) {
						page->update_only_1->setChecked((recentsyncs.at(i)=="checked"));
					}
					if (i % 12 == 11) {
						page->update_only_2->setChecked((recentsyncs.at(i)=="checked"));
					}
   				}
			}
		} else {
			recentsyncs = settings.value("recentsyncs").toStringList();
			for (int i = 0; i < recentsyncs.count(); ++i) {
       			if (i % 3 == 0 || i == 0) { page = addSyncTab();
					QDir dirpath(recentsyncs.at(i));
					page->tab_name->setText(dirpath.path());
					tabWidget->setTabText(tabWidget->indexOf(page->tab), dirpath.path()); }
        		if (i % 3 == 1) page->sync_folder_1->setText(recentsyncs.at(i));
    			if (i % 3 == 2) page->sync_folder_2->setText(recentsyncs.at(i));
    		}
		}
	}
	for (int i = 0; i < tabs_list.count(); ++i) {
		page = addSyncTab();
		page->tab_name->setText(tabs_list.at(i));
		tabWidget->setTabText(tabWidget->indexOf(page->tab), tabs_list.at(i));
		page->sync_folder_1->setText(sync_settings.value(QString("tab_%1_%2/folder1").arg(tabs_list.at(i)).arg(i)).toString());
		page->sync_folder_2->setText(sync_settings.value(QString("tab_%1_%2/folder2").arg(tabs_list.at(i)).arg(i)).toString());
		page->periodical_sync->setChecked(sync_settings.value(QString("tab_%1_%2/periodical").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		page->sync_hidden->setChecked(sync_settings.value(QString("tab_%1_%2/sync_hidden").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		page->sync_interval->setValue(sync_settings.value(QString("tab_%1_%2/sync_interval").arg(tabs_list.at(i)).arg(i)).toInt());
		if (sync_settings.value(QString("tab_%1_%2/periodical_stop").arg(tabs_list.at(i)).arg(i)).toString()=="on") startPeriodical(tabWidget->widget(tabWidget->indexOf(page->tab)));
		else  page->periodical_stop->setEnabled(false);
		showAdvancedGroupBox(sync_settings.value(QString("tab_%1_%2/advanced").arg(tabs_list.at(i)).arg(i)).toString()=="checked", page);
		page->backup_folder_1->setChecked(sync_settings.value(QString("tab_%1_%2/backup_folder_1").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		page->backup_folder_2->setChecked(sync_settings.value(QString("tab_%1_%2/backup_folder_2").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		page->update_only_1->setChecked(sync_settings.value(QString("tab_%1_%2/update_only_1").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		page->update_only_2->setChecked(sync_settings.value(QString("tab_%1_%2/update_only_2").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		page->sync_nosubdirs->setChecked(sync_settings.value(QString("tab_%1_%2/sync_nosubdirs").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		page->ignore_blacklist->setChecked(sync_settings.value(QString("tab_%1_%2/ignore_blacklist").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		page->move->setChecked(sync_settings.value(QString("tab_%1_%2/move").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
	#ifndef Q_WS_WIN
		page->symlinks->setChecked(sync_settings.value(QString("tab_%1_%2/symlinks").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
	#endif
		page->filters->setChecked(sync_settings.value(QString("tab_%1_%2/filters_gb").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		QStringList flist = sync_settings.value(QString("tab_%1_%2/flist").arg(tabs_list.at(i)).arg(i)).toStringList();
		for (int n = 0; n < flist.count(); ++n) {
			for (int f = 0; f < page->lw_filters->count(); ++f) {
				if (page->lw_filters->item(f)->text() == flist.at(n)) {
					page->lw_filters->item(f)->setCheckState(Qt::Checked);
				}
			}
		}
	}
    synchronised = sync_settings.value("synchronised").toStringList();
    restore_clean_selected->setChecked(sync_settings.value("restore_clean_selected", true).toBool());
    restore_clean_by_date->setChecked(sync_settings.value("restore_clean_by_date").toBool());
    restore_clean_repeated->setChecked(sync_settings.value("restore_clean_repeated").toBool());
    restore_clean_date->setValue(sync_settings.value("restore_clean_date", 7).toInt());
    restore_clean_files->setValue(sync_settings.value("restore_clean_files", 3).toInt());
    files_blacklist = sync_settings.value("files_blacklist").toStringList();
	folders_blacklist = sync_settings.value("folders_blacklist").toStringList();
	QStringList multitabs_list = sync_settings.value("multitabs").toStringList();
	MultisyncPage * multi_page;
	if (multitabs_list.count()==0) {
		QStringList multisyncs_items = sync_settings.value("multisyncs_items").toStringList();
		QListWidgetItem * item = 0;
		multi_page = addMultiTab();
		for (int i = 0; i < multisyncs_items.count(); ++i) {
			if (i % 2 == 0) {
				item = new QListWidgetItem();
				item->setText(multisyncs_items.at(i));
			}
			else if (i % 2 == 1) {
				if (multisyncs_items.at(i) == "unchecked") item->setCheckState(Qt::Unchecked);
				else item->setCheckState(Qt::Checked);
				multi_page->list_multi->addItem(item);
			}
		}
		multi_page->destination_multi->setText(sync_settings.value("destination_multi").toString());
		multi_page->destinationTextChanged();
	}
	for (int i = 0; i < multitabs_list.count(); ++i) {
		multi_page = addMultiTab();
		multi_page->tab_name->setText(multitabs_list.at(i));
		multi_tabWidget->setTabText(multi_tabWidget->indexOf(multi_page), multitabs_list.at(i));
		QStringList sources = sync_settings.value(QString("multitab_%1_%2/sources").arg(multitabs_list.at(i)).arg(i)).toStringList();
		QListWidgetItem * item = 0;
		for (int s = 0; s < sources.count(); ++s) {
			if (s % 2 == 0) {
				item = new QListWidgetItem();
				item->setText(sources.at(s));
			}
			else if (s % 2 == 1) {
				if (sources.at(s) == "unchecked") item->setCheckState(Qt::Unchecked);
				else item->setCheckState(Qt::Checked);
				multi_page->list_multi->addItem(item);
			}
		}
		multi_page->destination_multi->setText(sync_settings.value(QString("multitab_%1_%2/destination").arg(multitabs_list.at(i)).arg(i)).toString());
		multi_page->showAdvancedGroupBox(sync_settings.value(QString("multitab_%1_%2/advanced").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		multi_page->sync_hidden->setChecked(!(sync_settings.value(QString("multitab_%1_%2/sync_hidden").arg(multitabs_list.at(i)).arg(i)).toString()=="unchecked"));
		multi_page->backup_folder_1->setChecked(sync_settings.value(QString("multitab_%1_%2/backup_folder_1").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		multi_page->backup_folder_2->setChecked(sync_settings.value(QString("multitab_%1_%2/backup_folder_2").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		multi_page->update_only_1->setChecked(sync_settings.value(QString("multitab_%1_%2/update_only_1").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		multi_page->update_only_2->setChecked(sync_settings.value(QString("multitab_%1_%2/update_only_2").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		multi_page->sync_nosubdirs->setChecked(sync_settings.value(QString("multitab_%1_%2/sync_nosubdirs").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		multi_page->ignore_blacklist->setChecked(sync_settings.value(QString("multitab_%1_%2/ignore_blacklist").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		multi_page->move->setChecked(sync_settings.value(QString("multitab_%1_%2/move").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
	#ifndef Q_WS_WIN
		multi_page->symlinks->setChecked(sync_settings.value(QString("multitab_%1_%2/symlinks").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
	#endif
		/*multi_page->filters->setChecked(sync_settings.value(QString("multitab_%1_%2/filters_gb").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		QStringList flist = sync_settings.value(QString("multitab_%1_%2/flist").arg(multitabs_list.at(i)).arg(i)).toStringList();
		for (int n = 0; n < flist.count(); ++n) {
			for (int f = 0; f < multi_page->lw_filters->count(); ++f) {
				if (multi_page->lw_filters->item(f)->text() == flist.at(n)) {
					multi_page->lw_filters->item(f)->setCheckState(Qt::Checked);
				}
			}
		}*/
	}
	run_hidden = sync_settings.value("run_hidden", false).toBool();
	actionDisable_tray_messages->setChecked(sync_settings.value("disable_tray_messages").toBool());
	QStringList schedules = sync_settings.value("schedules").toStringList(); QString schedule;
	for (int i = 0; i < schedules.count(); ++i) {
		if (i % 2 == 0) {
			addSchedule(sync_settings.value(QString("sched_%1_%2_tabs").arg(schedules.at(i)).arg(i/2)).toStringList(),
						sync_settings.value(QString("sched_%1_%2_times").arg(schedules.at(i)).arg(i/2)).toStringList(),
						sync_settings.value(QString("sched_%1_%2_checkedtimes").arg(schedules.at(i)).arg(i/2)).toStringList())->setText(schedules.at(i));
		}
		if (i % 2 == 1) {
			if (schedules.at(i)=="on") {
				setScheduleStatusOn(true, tw_schedules->item(tw_schedules->rowCount()-1, 1));
				startSchedule(tw_schedules->item(tw_schedules->rowCount()-1, 0));
			}
		}
	}
	tabWidget->setCurrentIndex(sync_settings.value("current_synctab", 0).toInt());
	multi_tabWidget->setCurrentIndex(sync_settings.value("current_multitab", 0).toInt());
    this->move(settings.value("pos", this->pos()).toPoint());
	this->resize(settings.value("size", this->size()).toSize());
	actionRun_hidden->setChecked(run_hidden);
	trayIconVisible(!run_hidden);
}

void MainWindow::about ()
{
    About *about = new About(ver, QVariant(QDate::currentDate().year()).toString(), "4.3");
    about->show();
}

void MainWindow::createActions()
{
     minimizeAction = new QAction(tr("&Hide"), this);
     connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
     connect(minimizeAction, SIGNAL(triggered()), this, SLOT(minimizeTrayIcon()));

     maximizeAction = new QAction(tr("S&how"), this);
     connect(maximizeAction, SIGNAL(triggered()), this, SLOT(show()));
     connect(maximizeAction, SIGNAL(triggered()), this, SLOT(maximizeTrayIcon()));

     syncAction = new QAction(tr("Sync &current tab"), this);
     connect(syncAction, SIGNAL(triggered()), this, SLOT(sync()));
     
     syncAllAction = new QAction(tr("Sync &all tabs"), this);
     connect(syncAllAction, SIGNAL(triggered()), this, SLOT(syncAll()));

     quitAction = new QAction(tr("&Quit"), this);
     connect(quitAction, SIGNAL(triggered()), this, SLOT(closeApp()));
}

void MainWindow::createTrayIcon()
{
     QMenu * trayIconMenu = new QMenu(this);
     trayIconMenu->addAction(syncAction);
     trayIconMenu->addAction(syncAllAction);
     trayIconMenu->addSeparator();
     trayIconMenu->addAction(minimizeAction);
     trayIconMenu->addAction(maximizeAction);
     trayIconMenu->addSeparator();
     trayIconMenu->addAction(quitAction);

     trayIcon = new QSystemTrayIcon(this);
     trayIcon->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/Synkron16.png")));
     trayIcon->setContextMenu(trayIconMenu);
     connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::trayIconVisible(bool visible)
{
     minimizeAction->setEnabled(visible);
     maximizeAction->setEnabled(!visible);
     syncAction->setEnabled(true);
     syncAllAction->setEnabled(true);
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
	case QSystemTrayIcon::MiddleClick:
	case QSystemTrayIcon::DoubleClick:
		if (this->isHidden()) {
			this->show();
		}
		else {
			this->hide();
		}
		break;
    case QSystemTrayIcon::Context:
        trayIcon->contextMenu()->show();
        break;
    default:
        break;
    }
}

void MainWindow::switchView(QAction * action)
{
	if (mainStackedWidget->currentIndex()==0) tabNameChanged();
	if (action == actionSynchronise) {
		actionNew_sync->setEnabled(true);
		actionClose_sync->setEnabled(true);
		actionSave_log->setEnabled(true);
		mainStackedWidget->setCurrentIndex(0);
	}
	else if (action == actionRestore) {
		toRestorePage();
		actionNew_sync->setDisabled(true);
		actionClose_sync->setDisabled(true);
		actionSave_log->setEnabled(false);
		mainStackedWidget->setCurrentIndex(1);
	}
	else if (action == actionBlacklist) {
		toBlacklist();
		actionNew_sync->setDisabled(true);
		actionClose_sync->setDisabled(true);
		actionSave_log->setEnabled(false);
		mainStackedWidget->setCurrentIndex(2);
	}
	else if (action == actionMultisync) {
		actionNew_sync->setEnabled(true);
		actionClose_sync->setEnabled(true);
		actionSave_log->setEnabled(true);
		mainStackedWidget->setCurrentIndex(3);
	}
	else if (action == actionScheduler) {
		actionNew_sync->setDisabled(true);
		actionClose_sync->setDisabled(true);
		actionSave_log->setEnabled(false);
		if (tw_schedules->currentItem()!=0) activateSchedule();
		mainStackedWidget->setCurrentIndex(4);
	}
	else if (action == actionFilters) {
		actionNew_sync->setDisabled(true);
		actionClose_sync->setDisabled(true);
		actionSave_log->setEnabled(false);
		mainStackedWidget->setCurrentIndex(5);
	}
}

void MainWindow::checkForUpdates()
{
	delete http_buffer; http_buffer = new QBuffer(this);
    http->setHost("synkron.sourceforge.net");
	http->get("/current-version", http_buffer);
}

void MainWindow::httpRequestFinished(bool error)
{
	httpRequestFinished_start:
	if (error) {
		switch (QMessageBox::critical(this, tr("Synkron"), tr("Failed to check for updates."), tr("&Try again"), tr("Cancel"), 0, 1)) {
			case 0: // Try again
				checkForUpdates(); return; break;
			case 1: // Cancel
				return; break;
		}
	}
	QString str(http_buffer->data()); QTextStream in(&str);
	if (in.readLine() != "[Synkron.current-version]") { error = true; goto httpRequestFinished_start; }
	QString current_ver = in.readLine();
	if (in.readLine() != "[Synkron.current-version.float]") { error = true; goto httpRequestFinished_start; }
	float f_current_ver = in.readLine().toFloat();
	if (in.readLine() != "[Synkron.release-notes]") { error = true; goto httpRequestFinished_start; }
	QString release_notes;
	while (!in.atEnd()) { release_notes.append(in.readLine()); }
	if (f_current_ver <= f_ver) {
		QMessageBox::information(this, tr("Synkron"), tr("Your Synkron is up-to-date."));
	} else {
		QString info; QTextStream out(&info);
		out << "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body><p>" << endl;
		out << "<b>" << tr("Synkron %1 is available now.").arg(current_ver) << "</b><br><br>" << endl;
		out << release_notes << endl << "</p></body></html>";
		QMessageBox::information(this, tr("Synkron"), info);
	}
}

void MainWindow::changeLanguage()
{
	QWidget * lang_widget = new QWidget(this, Qt::Dialog);
	lang_widget->setWindowModality(Qt::WindowModal);
	lang_widget->setAttribute(Qt::WA_DeleteOnClose);
	lang_widget->setWindowTitle(tr("Change language - Synkron"));
	QGridLayout * lang_glayout = new QGridLayout(lang_widget);
	lang_glayout->setMargin(6); lang_glayout->setSpacing(6);
	QLabel * lang_label = new QLabel(lang_widget);
	lang_label->setText(tr("Select your preferred language"));
	lang_glayout->addWidget(lang_label, 0, 0);
	langComboBox = new QComboBox(lang_widget);
	QStringList langs(synkron_i18n.values()); langs.sort();
	for (int i = 0; i < langs.count(); ++i) {
		langComboBox->addItem(tr(QByteArray().append(langs.at(i)).constData()), lang_widget);
		if (langs.at(i) == "English") { langComboBox->setCurrentIndex(i); }
	}
	lang_glayout->addWidget(langComboBox, 1, 0);
	QDialogButtonBox * lang_buttonbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, lang_widget);
	QObject::connect(lang_buttonbox, SIGNAL(accepted()), this, SLOT(langChanged()));
	QObject::connect(lang_buttonbox, SIGNAL(rejected()), lang_widget, SLOT(close()));
	lang_glayout->addWidget(lang_buttonbox, 2, 0);
	lang_widget->show();
}

void MainWindow::langChanged()
{
	if (langComboBox == NULL) { return; }
	QString lang = synkron_i18n.value(langComboBox->currentText(), langComboBox->currentText());
	QSettings settings("Matus Tomlein", "Synkron");
	QString current_lang = settings.value("lang", "English").toString();
	if (current_lang != lang) {
		settings.setValue("lang", lang);
		QMessageBox::information(this, tr("Synkron"), tr("You need to restart Synkron for the changes to apply."));
	}
	if (langComboBox->parent() == NULL) { return; }
	QWidget * lang_widget = (QWidget *)langComboBox->parent();
	lang_widget->close();
}

void MainWindow::searchTw(const QString text)
{
	QTableWidget * tw = NULL;
	if (mainStackedWidget->currentIndex()==0) {
		SyncPage * page = tabs.value(tabWidget->currentWidget());
		tw = page->tw;
	} else if (mainStackedWidget->currentIndex()==3) {
		MultisyncPage * page = (MultisyncPage *) multi_tabWidget->currentWidget();
		tw = page->tw_multi;
	} else return;
	if (tw == NULL) return;
	for (int i = 0; i < tw->rowCount(); ++i) {
		if (tw->item(i, 0)->text().contains(text, Qt::CaseInsensitive)) tw->showRow(i);
		else if (tw->columnSpan(i, 0)!=2) {
			if (tw->item(i, 1)->text().contains(text, Qt::CaseInsensitive)) tw->showRow(i);
			else tw->hideRow(i);
		}
		else tw->hideRow(i);
	}
}

void MainWindow::searchLw(const QString text)
{
	for (int i = 0; i < restore_list->count(); ++i) {
		if (!restore_list->item(i)->text().contains(text, Qt::CaseInsensitive)) restore_list->item(i)->setHidden(true);
		else restore_list->item(i)->setHidden(false);
	}
}

void MainWindow::addTab()
{
	if (mainStackedWidget->currentIndex()==0) {
		addSyncTab();
	} else if (mainStackedWidget->currentIndex()==3) {
		addMultiTab();
	}
}

void MainWindow::closeTab()
{
	if (mainStackedWidget->currentIndex()==0) {
		tabWidget->removeTab(tabWidget->currentIndex());
	} else if (mainStackedWidget->currentIndex()==3) {
		multi_tabWidget->removeTab(multi_tabWidget->currentIndex());
	}
}

bool MainWindow::showTrayMessage(QString title, QString message)
{
	if (!actionDisable_tray_messages->isChecked()) {
		trayIcon->showMessage(title, message);
		return true;
	} else return false;
}

void MainWindow::saveSyncLog()
{
    AbstractSyncPage * page; QString tab_text;
    if (mainStackedWidget->currentIndex()==0) {
		page = tabs.value(tabWidget->currentWidget());
		tab_text = tabWidget->tabText(tabWidget->currentIndex());
	} else if (mainStackedWidget->currentIndex()==3) {
		page = (MultisyncPage *) multi_tabWidget->currentWidget();
		tab_text = multi_tabWidget->tabText(multi_tabWidget->currentIndex());
	} else return;
    QString path = QFileDialog::getSaveFileName(this,
                tr("Synkron - Save Multisync"),
                QString("%1/%2.html").arg(QDir::homePath()).arg(tab_text),
                tr("Sync logs (*.html)"));
    if (path.isNull() || path.isEmpty()) { return; }
    QFile file(path);
	if (!file.open(QFile::WriteOnly | QFile::Text)) 
	{
		QMessageBox::critical(this, tr("Export sync log"), tr("Cannot write file %1:\n%2.").arg(path).arg(file.errorString()));
		return;
	}
	QTextStream sfile(&file);
	sfile.setCodec("UTF-8");
	sfile << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>" << endl;
	sfile << tr("Synkron sync log") << " - " << tab_text << " - ";
	sfile << endl << "</title></head><body><table width=\"100%\" style=\"font-family:sans-serif;\">" << endl;
	for (int i = 0; i < page->tableWidget()->rowCount(); ++i) {
		sfile << "<tr>";
		if (page->tableWidget()->columnSpan(i, 0) == 2) {
		    sfile << "<td colspan=\"2\" style=\"background-color: rgb(";
		    sfile << page->tableWidget()->item(i, 0)->background().color().red() << ", ";
		    sfile << page->tableWidget()->item(i, 0)->background().color().green() << ", ";
		    sfile << page->tableWidget()->item(i, 0)->background().color().blue() << "); ";
		    sfile << "color: rgb(";
		    sfile << page->tableWidget()->item(i, 0)->foreground().color().red() << ", ";
		    sfile << page->tableWidget()->item(i, 0)->foreground().color().green() << ", ";
		    sfile << page->tableWidget()->item(i, 0)->foreground().color().blue() << ")\">";
		    sfile << endl << page->tableWidget()->item(i, 0)->text() << "\n</td>" << endl;
		} else {
		    sfile << "<td width=\"50%\">";
		    sfile << endl << page->tableWidget()->item(i, 0)->text() << "\n</td>" << endl;
		    
		    sfile << "<td width=\"50%\">";
		    sfile << endl << page->tableWidget()->item(i, 1)->text() << "\n</td>" << endl;
		}
		sfile << "</tr>";
	}
	sfile << "</table></body></html>" << endl;
}

About::About(QString ver, QString year, QString qtver)
{
	setupUi(this);
	
	QString about = "<p style=\"font-family: sans-serif; font-style:italic;\"><span style=\"font-size:12pt;\">Synkron</span><br>";
	about.append("<span style=\"font-size:8pt;\">");
    about.append(tr("Version"));
    about.append(QString(" %1</span></p><p></p>").arg(ver));
    about.append("http://synkron.sourceforge.net/");
	about.append("<p style=\"font-family: sans-serif; font-size:8pt;\">");
    about.append(tr("Synkron is a simple Qt application designed for easy folder synchronisation."));
    about.append("</p><p></p>");
	about.append("<p style=\"font-family: sans-serif; font-size:8pt;\">");
    about.append(tr("This programme is distributed under the terms of the GPL v2."));
	about.append("</p><p></p>");
	about.append(QString::fromUtf8("<p style=\"font-family: sans-serif; font-size:8pt;\">2005-%1 <span style=\"font-style:italic;\">Mat\303\272\305\241 Tomlein</span> (matus.tomlein@gmail.com)</p>").arg(year));
	about.append("<p></p><p style=\"font-family: sans-serif; font-size:8pt;\">");
	about.append(tr("The programme is provided AS IS with ABSOLUTELY NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE."));
	about.append("</p>");
#ifdef Q_WS_MAC
	about.remove("font-family: sans-serif;");
	about.replace("font-size:12pt;", "font-size:14pt;");
	about.replace("font-size:8pt;", "font-size:10pt;");
#endif
    about_synkron_tb->setHtml(about);
	QString aboutQt = "<p style=\"font-family: sans-serif; font-style:italic;\"><span style=\"font-size:12pt;\">";
    aboutQt.append(tr("About Qt"));
	aboutQt.append("</span></p><p></p><p style=\"font-family: sans-serif; font-size:8pt; font-style:italic;\">");
	aboutQt.append(tr("This programme uses Qt Open Source Edition version %1.").arg(qtver));
	aboutQt.append("</p><p></p><p style=\"font-family: sans-serif; font-size:8pt;\">");
	aboutQt.append(tr("Qt is a C++ toolkit for cross-platform application development."));
	aboutQt.append("</p><p></p><p style=\"font-family: sans-serif; font-size:8pt;\">");
	aboutQt.append(tr("Qt provides single-source portability across MS Windows, Mac OS X, Linux and all major commercial Unix variants. Qt is also available for embedded devices as Qtopia Core."));
	aboutQt.append("</p><p></p><p style=\"font-family: sans-serif; font-size:8pt;\">");
	aboutQt.append(tr("Qt is a Trolltech product. See <span style=\"font-style:italic;\">http://www.trolltech.com/qt/</span> for more information."));
	aboutQt.append("</p>");
#ifdef Q_WS_MAC
	aboutQt.remove("font-family: sans-serif;");
	aboutQt.replace("font-size:12pt;", "font-size:14pt;");
	aboutQt.replace("font-size:8pt;", "font-size:10pt;");
#endif
    about_qt_tb->setHtml(aboutQt);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QSettings settings("Matus Tomlein", "Synkron");
	QString lang = settings.value("lang").toString();
	if (lang.isEmpty()) {
		lang = QLocale::languageToString(QLocale::system().language());
		settings.setValue("lang", lang);
	}
	if (lang == "C") { lang = "English"; settings.setValue("lang", lang); }
	if (lang != "English") {
		QTranslator * translator = new QTranslator;
		translator->load(QString(":/i18n/Synkron-%1.qm").arg(lang.replace(" ", "_")));
		app.installTranslator(translator);
	}
    
    MainWindow *window = new MainWindow;
	if (window->runHidden() && app.arguments().count() <= 1) window->hide();
	else window->show();
    return app.exec();
}
