#include "main_window.h"

#include <QApplication>

MainWindow::MainWindow()
{
	f_ver = 1.0;
	ver = "1.0.0";
	
    setupUi(this);
    
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
    
    syncFiles = 0;
    
    tw_multi->setHorizontalHeaderLabels(QStringList() << tr("Source") << tr("Destination"));
    tw_multi->verticalHeader()->hide();
    tw_multi->setShowGrid(false);
    tw_multi->setStatusTip(tr("List of synchronised files and folders"));
    
    tw_schedules->setHorizontalHeaderLabels(QStringList() << tr("Schedule name") << tr("Status"));
    tw_schedules->verticalHeader()->hide();
    
    QTimer::singleShot(200, this, SLOT(updateGeometry()));
    
    actgrpView = new QActionGroup(this);
    actgrpView->addAction(actionSynchronise);
    actgrpView->addAction(actionRestore);
    actgrpView->addAction(actionBlacklist);
    actgrpView->addAction(actionMultisync);
    actgrpView->addAction(actionScheduler);
    
	synkron_i18n.insert(tr("English"), "English");
    synkron_i18n.insert(tr("Slovak"), "Slovak");
    
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
    connect(delTmpAllBtn, SIGNAL(released()), this, SLOT(delTmpAll()));
    connect(delTmpSelBtn, SIGNAL(released()), this, SLOT(delTmpSel()));
    connect(actionRun_hidden, SIGNAL(toggled(bool)), this, SLOT(setRunHidden(bool)));
    connect(actionSync_current, SIGNAL(triggered()), this, SLOT(sync()));
    connect(actionSync_all, SIGNAL(triggered()), this, SLOT(syncAll()));
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateGeometry()));
	connect(actionCheck_for_updates, SIGNAL(triggered()), this, SLOT(checkForUpdates()));
	connect(http, SIGNAL(done(bool)), this, SLOT(httpRequestFinished(bool)));
	connect(add_multi, SIGNAL(released()), this, SLOT(addMultisync()));
	connect(remove_multi, SIGNAL(released()), this, SLOT(removeMultisync()));
	connect(browse_multi, SIGNAL(released()), this, SLOT(browseMultiDestination()));
	connect(sync_multi, SIGNAL(released()), this, SLOT(multisync()));
	connect(add_schedule, SIGNAL(released()), this, SLOT(addSchedule()));
	connect(remove_schedule, SIGNAL(released()), this, SLOT(removeSchedule()));
	connect(tw_schedules, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(scheduleActivated(int, int, int, int)));
	connect(sched_name, SIGNAL(textEdited(const QString)), this, SLOT(setSchedName(const QString)));
	connect(sched_add_time, SIGNAL(released()), this, SLOT(addSchedTime()));
	connect(sched_remove_time, SIGNAL(released()), this, SLOT(removeSchedTime()));
	connect(sched_start, SIGNAL(released()), this, SLOT(startSchedule()));
	connect(sched_stop, SIGNAL(released()), this, SLOT(stopSchedule()));
	connect(restore_search, SIGNAL(textEdited(const QString)), this, SLOT(restoreSearch(const QString)));
	connect(startall_schedules, SIGNAL(released()), this, SLOT(startAllSchedules()));
	connect(stopall_schedules, SIGNAL(released()), this, SLOT(stopAllSchedules()));
	QObject::connect(actionChange_language, SIGNAL(triggered()), this, SLOT(changeLanguage()));
	connect(save_multi, SIGNAL(released()), this, SLOT(saveMultisync()));
	connect(saveas_multi, SIGNAL(released()), this, SLOT(saveAsMultisync()));
	connect(load_multi, SIGNAL(released()), this, SLOT(loadMultisync()));
    
    tabWidget->removeTab(0);
    readSettings();
	
    if (qApp->arguments().count() > 1) {
        QFileInfo file_info (qApp->arguments().at(1));
        if (file_info.exists()) {
            loadMultisync(file_info.absoluteFilePath());
            actionMultisync->trigger();
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    updateGeometry();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    saveSettings();
    trayIcon->hide();
    QSettings sync_settings(QSettings::IniFormat, QSettings::UserScope, "Matus Tomlein", "Synkron");
    sync_settings.setValue("slist_path", "");
}

void MainWindow::saveSettings()
{
	QSettings settings ("Matus Tomlein", "Synkron");
    QSettings sync_settings(QSettings::IniFormat, QSettings::UserScope, "Matus Tomlein", "Synkron");
    QStringList recentsyncs;
    for (int i = 0; i < tabWidget->count(); i++) {
        recentsyncs << tabWidget->tabText(i);
        recentsyncs << tabs.value(tabWidget->widget(i))->sync_folder_1->text();
        recentsyncs << tabs.value(tabWidget->widget(i))->sync_folder_2->text();
        if (tabs.value(tabWidget->widget(i))->periodical_sync->isChecked()) recentsyncs << "checked";
        else recentsyncs << "unchecked";
        if (tabs.value(tabWidget->widget(i))->sync_hidden->isChecked()) recentsyncs << "checked";
        else recentsyncs << "unchecked";
        recentsyncs << QVariant(tabs.value(tabWidget->widget(i))->sync_interval->value()).toString();
        if (tabs.value(tabWidget->widget(i))->periodical_stop->isEnabled()) recentsyncs << "on";
        else recentsyncs << "off";
        if (tabs.value(tabWidget->widget(i))->advanced->isChecked()) recentsyncs << "checked";
        else recentsyncs << "unchecked";
        if (tabs.value(tabWidget->widget(i))->backup_folder_1->isChecked()) recentsyncs << "checked";
        else recentsyncs << "unchecked";
        if (tabs.value(tabWidget->widget(i))->backup_folder_2->isChecked()) recentsyncs << "checked";
        else recentsyncs << "unchecked";
        if (tabs.value(tabWidget->widget(i))->update_only_1->isChecked()) recentsyncs << "checked";
        else recentsyncs << "unchecked";
        if (tabs.value(tabWidget->widget(i))->update_only_2->isChecked()) recentsyncs << "checked";
        else recentsyncs << "unchecked";
    }
    sync_settings.setValue("recentsyncs", recentsyncs);
	QStringList multisyncs_items;
	for (int i = 0; i < list_multi->count(); ++i) {
		multisyncs_items << list_multi->item(i)->text();
		if (list_multi->item(i)->checkState() == Qt::Checked) multisyncs_items << "checked";
		else multisyncs_items << "unchecked";
	}
	sync_settings.setValue("multisyncs_items", multisyncs_items);
	sync_settings.setValue("destination_multi", destination_multi->text());
    sync_settings.setValue("synchronised", synchronised);
    sync_settings.setValue("files_blacklist", files_blacklist);
    sync_settings.setValue("folders_blacklist", folders_blacklist);
    sync_settings.setValue("run_hidden", run_hidden);
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
	QStringList recentsyncs = sync_settings.value("recentsyncs").toStringList();
	SyncPage * page = 0;
	if (recentsyncs.count() == 0 && settings.value("recentsyncs").toStringList().count() != 0) {
		recentsyncs = settings.value("recentsyncs").toStringList();
		for (int i = 0; i < recentsyncs.count(); ++i) {
       		if (i % 3 == 0 || i == 0) { page = addTab();
			   tabWidget->setTabText(tabWidget->indexOf(page->tab), recentsyncs.at(i)); }
        	if (i % 3 == 1) page->sync_folder_1->setText(recentsyncs.at(i));
    		if (i % 3 == 2) page->sync_folder_2->setText(recentsyncs.at(i));
    	}
	}
    else if (recentsyncs.count() == 0) { addTab(); return; }
	bool okey; recentsyncs.last().toInt(&okey);
    if ((recentsyncs.count()-1) % 6 == 5 && okey) {
		for (int i = 0; i < recentsyncs.count(); ++i) {
     		if (i % 6 == 0 || i == 0) { page = addTab();
				page->tab_name->setText(recentsyncs.at(i)); tabNameEdited(); }
    		if (i % 6 == 1) page->sync_folder_1->setText(recentsyncs.at(i));
    		if (i % 6 == 2) page->sync_folder_2->setText(recentsyncs.at(i));
    		if (i % 6 == 3) page->periodical_sync->setChecked(recentsyncs.at(i)=="checked");
			if (i % 6 == 4) page->sync_hidden->setChecked(recentsyncs.at(i)=="checked");
			if (i % 6 == 5) page->sync_interval->setValue(recentsyncs.at(i).toInt());
   		}
	}
	else {
		for (int i = 0; i < recentsyncs.count(); ++i) {
     		if (i % 12 == 0 || i == 0) { page = addTab();
				page->tab_name->setText(recentsyncs.at(i)); tabNameEdited(); }
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
    synchronised = sync_settings.value("synchronised").toStringList();
    files_blacklist = sync_settings.value("files_blacklist").toStringList();
	folders_blacklist = sync_settings.value("folders_blacklist").toStringList();
	QStringList multisyncs_items = sync_settings.value("multisyncs_items").toStringList();
	QListWidgetItem * item = 0;
	for (int i = 0; i < multisyncs_items.count(); ++i) {
		if (i % 2 == 0) {
			item = new QListWidgetItem();
			item->setText(multisyncs_items.at(i));
		}
		else if (i % 2 == 1) {
			if (multisyncs_items.at(i) == "unchecked") item->setCheckState(Qt::Unchecked);
			else item->setCheckState(Qt::Checked);
			list_multi->addItem(item);
		}
	}
	destination_multi->setText(sync_settings.value("destination_multi").toString());
	run_hidden = sync_settings.value("run_hidden", false).toBool();
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
    this->move(settings.value("pos", this->pos()).toPoint());
	this->resize(settings.value("size", this->size()).toSize());
	actionRun_hidden->setChecked(run_hidden);
	updateGeometry();
}

void MainWindow::updateGeometry()
{
    SyncPage * page = NULL; page = tabs.value(tabWidget->currentWidget());
    tw_multi->setColumnWidth(0, (tw_multi->width() - 25) / 2);
    tw_multi->setColumnWidth(1, (tw_multi->width() - 25) / 2);
    tw_schedules->setColumnWidth(0, (tw_schedules->width() - 25) / 3 * 2);
    tw_schedules->setColumnWidth(1, (tw_schedules->width() - 25) / 3);
	if (page == NULL) return;
	page->tw->setColumnWidth(0, (page->tw->width() - 25) / 2);
	page->tw->setColumnWidth(1, (page->tw->width() - 25) / 2);
}

void MainWindow::about ()
{
    About *about = new About(ver);
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
     connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
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
			updateGeometry();
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
	if (action == actionSynchronise) {
		mainStackedWidget->setCurrentIndex(0);
		actionNew_sync->setEnabled(true);
		actionClose_sync->setEnabled(true);
		actionSync_current->setEnabled(true);
		updateGeometry();
	}
	else if (action == actionRestore) {
		toRestorePage();
		actionNew_sync->setDisabled(true);
		actionClose_sync->setDisabled(true);
		actionSync_current->setEnabled(false);
		mainStackedWidget->setCurrentIndex(1);
	}
	else if (action == actionBlacklist) {
		toBlacklist();
		actionNew_sync->setDisabled(true);
		actionClose_sync->setDisabled(true);
		actionSync_current->setEnabled(false);
		mainStackedWidget->setCurrentIndex(2);
	}
	else if (action == actionMultisync) {
		toMultisync();
		actionNew_sync->setDisabled(true);
		actionClose_sync->setDisabled(true);
		actionSync_current->setEnabled(false);
		mainStackedWidget->setCurrentIndex(3);
		updateGeometry();
	}
	else if (action == actionScheduler) {
		toScheduler();
		actionNew_sync->setDisabled(true);
		actionClose_sync->setDisabled(true);
		actionSync_current->setEnabled(false);
		mainStackedWidget->setCurrentIndex(4);
		updateGeometry();
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
		translator->load(QString(":/i18n/%1.qm").arg(lang.replace(" ", "_")));
		app.installTranslator(translator);
	}
    
    MainWindow *window = new MainWindow;
	
	window->show();
    if (window->runHidden()) window->hide();
    return app.exec();
}
