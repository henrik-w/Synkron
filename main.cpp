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

MainWindow::MainWindow(QSettings * s)
{
    setupUi(this);
    
	f_ver = 1.4;
	ver = "1.4.0";
    
    if (tr("LTR") == "RTL") { qApp->setLayoutDirection(Qt::RightToLeft); }
    
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
	skip_close_event = false;
    shown_manually = false;
    
    tw_schedules->setHorizontalHeaderLabels(QStringList() << tr("Schedule name") << tr("Status"));
    tw_schedules->verticalHeader()->hide();

#ifndef Q_WS_WIN
    tw_schedules->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    tw_schedules->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
#endif
    syncs_syncview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    syncs_syncview->horizontalHeader()->hide();
    syncs_syncview->verticalHeader()->hide();
    multisyncs_syncview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    multisyncs_syncview->horizontalHeader()->hide();
    multisyncs_syncview->verticalHeader()->hide();
    
    actgrpView = new QActionGroup(this);
    actgrpView->addAction(actionSynchronise);
    actgrpView->addAction(actionRestore);
    actgrpView->addAction(actionBlacklist);
    actgrpView->addAction(actionMultisync);
    actgrpView->addAction(actionScheduler);
    actgrpView->addAction(actionFilters);
    actgrpView->addAction(actionSyncView);
    
    QTranslator translator; translator.load(":/i18n/Synkron-i18n.qm");
    synkron_i18n.insert("English", "English");
    synkron_i18n.insert(translator.translate("LanguageNames", "Slovak"), "Slovak");
    synkron_i18n.insert(translator.translate("LanguageNames", "German"), "German");
    synkron_i18n.insert(translator.translate("LanguageNames", "Japanese"), "Japanese");
    synkron_i18n.insert(translator.translate("LanguageNames", "Arabic"), "Arabic");
    synkron_i18n.insert(translator.translate("LanguageNames", "Russian"), "Russian");
    synkron_i18n.insert(translator.translate("LanguageNames", "Spanish"), "Spanish");
    
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
    connect(blacklist_addext, SIGNAL(released()), this, SLOT(addExtToBlacklist()));
    connect(blacklist_removeext, SIGNAL(released()), this, SLOT(removeExtFromBlacklist()));
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
	connect(sched_multitab_lw, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(schedMultitabClicked(QListWidgetItem*)));
	connect(sched_time_lw, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(schedTimeClicked(QListWidgetItem*)));
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(closeApp()));
	connect(actionSave_log, SIGNAL(triggered()), this, SLOT(saveSyncLog()));
	connect(restore_list, SIGNAL(sigconmenu(QPoint)), this, SLOT(restoreListConMenu(QPoint)));
	connect(sched_interval_spin, SIGNAL(valueChanged(int)), this, SLOT(schedIntervalChanged(int)));
	connect(timing_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(timngTabIndexChanged(int)));
    
    setCleanGB();
    setSelectGB();
    tabWidget->removeTab(0);
    multi_tabWidget->removeTab(0);
    
    sync_settings = s;
    readSettings();
    if (tabWidget->count()==0) addSyncTab();
    if (multi_tabWidget->count()==0) addMultiTab();
    
    QSettings settings ("Matus Tomlein", "Synkron");
	tcp_server = new QTcpServer(this);
	QObject::connect(tcp_server, SIGNAL(newConnection()), this, SLOT(addConnection()));
	tcp_socket = new QTcpSocket(this);
	//QObject::connect(tcp_socket, SIGNAL(connected()), this, SLOT(sendMessageAndClose()));
	//QObject::connect(tcp_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(initServer(QAbstractSocket::SocketError)));
	tcp_socket->connectToHost("Localhost", settings.value("process_id", 1).toUInt());
	if (tcp_socket->waitForConnected(1000)) { sendMessageAndClose();
        skip_close_event = true;
        QTimer::singleShot(0, this, SLOT(close())); }
    else { initServer(QAbstractSocket::SocketTimeoutError); }
    QTimer::singleShot(2000, this, SLOT(setShownManually()));
}

void MainWindow::initServer(QAbstractSocket::SocketError)
{
    if (tcp_server->isListening()) { return; }
    if (!tcp_server->listen() && !tcp_server->isListening()) {
        tcp_server->close();
    } else {
        QSettings settings ("Matus Tomlein", "Synkron");
        settings.setValue("process_id", tcp_server->serverPort());
        for (int i = 1; i < qApp->arguments().count(); ++i) {
            if (qApp->arguments().at(i) == "-delete" && i < qApp->arguments().count() - 1) {
                globalDelete(qApp->arguments().at(i + 1));
                i++;
            } else if (qApp->arguments().at(i) == "-rename" && i < qApp->arguments().count() - 1) {
                bool ok = false;
                QString file_name = QFileInfo(qApp->arguments().at(i + 1)).fileName();
                QString new_name = QInputDialog::getText(this, tr("Synkron - Rename file"), tr("Type a new name for \"%1\":").arg(qApp->arguments().at(i + 1)), QLineEdit::Normal, file_name, &ok);
                if (ok) globalRename(qApp->arguments().at(i + 1), new_name);
                i++;
            } else {
                QFileInfo file_info (qApp->arguments().at(i));
                //QMessageBox::information(this, tr("Synkron"), tr("%1").arg(qApp->arguments().at(i)));
                if (file_info.exists()) {
                    addMultiTab()->loadMultisync(file_info.absoluteFilePath());
                    multi_tabWidget->setCurrentIndex(multi_tabWidget->count()-1);
                    actionMultisync->trigger();
                }
            }
        }
    }
}

void MainWindow::sendMessageAndClose()
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << (quint64)0;
    if (qApp->arguments().count() < 2) {
        out << QString("[Synkron raise];") << QString("\n");
    } else {
        for (int i = 1; i < qApp->arguments().count(); ++i) {
            if (qApp->arguments().at(i) == "-delete" && i < qApp->arguments().count() - 1) {
                out << QString("[Synkron globalDelete \"%1\"];").arg(qApp->arguments().at(i + 1)) << QString("\n");
                i++;
            } else if (qApp->arguments().at(i) == "-rename" && i < qApp->arguments().count() - 1) {
                out << QString("[Synkron globalRename \"%1\"];").arg(qApp->arguments().at(i + 1)) << QString("\n");
                i++;
            } else {
                QFileInfo file_info (qApp->arguments().at(i));
                if (file_info.exists()) {
                    out << QString("[Synkron loadMultisync \"%1\"];").arg(qApp->arguments().at(i)) << QString("\n");
                    //QMessageBox::information(this, tr("Synkron"), tr("%1").arg(qApp->arguments().at(i)));
                }
            }
        }
    }
    out.device()->seek(0);
    out << (quint64)(ba.size() - sizeof(quint64));
    tcp_socket->write(ba);
    tcp_socket->disconnectFromHost();
    qApp->processEvents();
}

void MainWindow::addConnection()
{
    new ClientConnection(this, tcp_server->nextPendingConnection());
}

ClientConnection::ClientConnection(MainWindow * parent, QTcpSocket * socket)
{
    c_parent = parent;
    c_socket = socket;
    c_blocksize = 0;
    QObject::connect(c_socket, SIGNAL(readyRead()), this, SLOT(read()));
    read();
}

void ClientConnection::read()
{
    QDataStream in(c_socket);
    in.setVersion(QDataStream::Qt_4_2);

    if (c_blocksize == 0) {
        if (c_socket->bytesAvailable() < (int)sizeof(quint64))
            return;

        in >> c_blocksize;
    }

    if (c_socket->bytesAvailable() < c_blocksize)
        return;

    QString received_string; QString buffer;
    do {
        in >> buffer; received_string.append(buffer);
    } while (!in.atEnd());

    QTextStream input(&received_string); QStringList bufferlist;
    while (!input.atEnd()) {
        buffer = input.readLine();
        if (!buffer.startsWith("[Synkron")) continue;
        buffer.truncate(buffer.count() - 2);
        bufferlist = buffer.split("\"");
        
        if (bufferlist.at(0) == "[Synkron raise];") c_parent->show();
        else if (bufferlist.at(0) == "[Synkron globalRename ") {
            c_parent->show();
            bool ok = false;
            QString file_name = QFileInfo(bufferlist.at(1)).fileName();
            QString new_name = QInputDialog::getText(c_parent, tr("Synkron - Rename file"), tr("Type a new name for \"%1\":").arg(bufferlist.at(1)), QLineEdit::Normal, file_name, &ok);
            if (ok) c_parent->globalRename(bufferlist.at(1), new_name);
        }
        else if (bufferlist.at(0) == "[Synkron globalDelete ") { c_parent->show(); c_parent->globalDelete(bufferlist.at(1)); }
        else if (bufferlist.at(0) == "[Synkron loadMultisync ") {
            c_parent->show();
            QFileInfo file_info (bufferlist.at(1));
            c_parent->addMultiTab()->loadMultisync(file_info.absoluteFilePath());
            c_parent->multi_tabWidget->setCurrentIndex(c_parent->multi_tabWidget->count()-1);
            c_parent->actionMultisync->trigger();
        }
    }

    c_socket->close();
    delete this;
}

bool MainWindow::closeDialogue()
{
	QSettings settings ("Matus Tomlein", "Synkron");
    if (sync_settings->value("dont_ask_on_quit", settings.value("dont_ask_on_quit")).toBool()==true) {
		if (sync_settings->value("minimise_on_quit", settings.value("minimise_on_quit")).toBool()==true) {
			minimizeAction->trigger();
			return false;
		} else if (sync_settings->value("minimise_on_quit", settings.value("minimise_on_quit")).toBool()==false) {
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
			sync_settings->setValue("dont_ask_on_quit", rm_minimise->isChecked());
			sync_settings->setValue("minimise_on_quit", true);
			minimizeAction->trigger();
			return false;
			break;	
		case 1: // Quit
			sync_settings->setValue("dont_ask_on_quit", rm_minimise->isChecked());
			sync_settings->setValue("minimise_on_quit", false);
			return true;
			break;
	}
	return true;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
	if (skip_close_event) {
        trayIcon->hide();
        event->accept();
        return;
    }
    if (!no_closedialogue) {
#ifdef Q_WS_MAC
        event->ignore();
        this->hide();
        return;
#else
		if (!closeDialogue()) {
			event->ignore();
			return;
		}
#endif
	}
	saveSettings();
    trayIcon->hide();
}

void MainWindow::saveSettings()
{
    QString lang = sync_settings->value("lang", "English").toString();
    bool dont_ask_on_quit = sync_settings->value("dont_ask_on_quit").toBool();
    bool minimise_on_quit = sync_settings->value("minimise_on_quit").toBool();
    sync_settings->clear();
    sync_settings->setValue("lang", lang);
    sync_settings->setValue("dont_ask_on_quit", dont_ask_on_quit);
    sync_settings->setValue("minimise_on_quit", minimise_on_quit);
    QStringList tabs_list;
    for (int i = 0; i < tabWidget->count(); ++i) {
		tabs_list << tabWidget->tabText(i);
		sync_settings->setValue(QString("tab_%1_%2/folders").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->sync_folders->pathsList());
		//sync_settings->setValue(QString("tab_%1_%2/folder1").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->sync_folder_1->text());
		//sync_settings->setValue(QString("tab_%1_%2/folder2").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->sync_folder_2->text());
		sync_settings->setValue(QString("tab_%1_%2/sync_hidden").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->sync_hidden->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("tab_%1_%2/advanced").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->advanced->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("tab_%1_%2/backup_folder_1").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->backup_folder_1->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("tab_%1_%2/backup_folder_2").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->backup_folder_2->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("tab_%1_%2/update_only_1").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->update_only_1->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("tab_%1_%2/update_only_2").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->update_only_2->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("tab_%1_%2/sync_nosubdirs").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->sync_nosubdirs->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("tab_%1_%2/ignore_blacklist").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->ignore_blacklist->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("tab_%1_%2/move").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->move->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("tab_%1_%2/clone_folder1").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->clone_folder1->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("tab_%1_%2/propagate_deletions").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->propagate_deletions->isChecked() ? "checked" : "unchecked");
        sync_settings->setValue(QString("tab_%1_%2/backup_folders").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->backup_folders->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("tab_%1_%2/update_only").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->update_only->isChecked() ? "checked" : "unchecked");
	#ifndef Q_WS_WIN
		sync_settings->setValue(QString("tab_%1_%2/symlinks").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->symlinks->isChecked() ? "checked" : "unchecked");
	#endif
		sync_settings->setValue(QString("tab_%1_%2/filters_gb").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->filters->isChecked() ? "checked" : "unchecked");
		QStringList flist;
		for (int n = 0; n < tabs.value(tabWidget->widget(i))->lw_filters->count(); ++n) {
			if (tabs.value(tabWidget->widget(i))->lw_filters->item(n)->checkState()==Qt::Checked) {
				flist << tabs.value(tabWidget->widget(i))->lw_filters->item(n)->text();
			}
		}
		sync_settings->setValue(QString("tab_%1_%2/flist").arg(tabWidget->tabText(i)).arg(i), flist);
        sync_settings->setValue(QString("tab_%1_%2/files_blacklist").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->files_blacklist);
        sync_settings->setValue(QString("tab_%1_%2/folders_blacklist").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->folders_blacklist);
        sync_settings->setValue(QString("tab_%1_%2/exts_blacklist").arg(tabWidget->tabText(i)).arg(i), tabs.value(tabWidget->widget(i))->exts_blacklist);
	}
	sync_settings->setValue("tabs", tabs_list);
	QStringList filters;
	for (int i = 0; i < filter_list->count(); ++i) {
		filters << filter_list->item(i)->text();
		sync_settings->setValue(QString("filter_%1_%2/extensions").arg(filter_list->item(i)->text()).arg(i), ((Filter *)filter_list->item(i))->extensions);
	}
	sync_settings->setValue("filters", filters);
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
		sync_settings->setValue(QString("multitab_%1_%2/sources").arg(multi_tabWidget->tabText(i)).arg(i), sources);
		sync_settings->setValue(QString("multitab_%1_%2/destination").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->destination_multi->text());
		sync_settings->setValue(QString("multitab_%1_%2/advanced").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->advanced->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("multitab_%1_%2/sync_hidden").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->sync_hidden->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("multitab_%1_%2/backup_folder_1").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->backup_folder_1->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("multitab_%1_%2/backup_folder_2").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->backup_folder_2->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("multitab_%1_%2/update_only_1").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->update_only_1->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("multitab_%1_%2/update_only_2").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->update_only_2->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("multitab_%1_%2/sync_nosubdirs").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->sync_nosubdirs->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("multitab_%1_%2/ignore_blacklist").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->ignore_blacklist->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("multitab_%1_%2/move").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->move->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("multitab_%1_%2/clone_folder1").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->clone_folder1->isChecked() ? "checked" : "unchecked");
		sync_settings->setValue(QString("multitab_%1_%2/propagate_deletions").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->propagate_deletions->isChecked() ? "checked" : "unchecked");
        //sync_settings->setValue(QString("multitab_%1_%2/backup_folders").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->backup_folders->isChecked() ? "checked" : "unchecked");
		//sync_settings->setValue(QString("multitab_%1_%2/update_only").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->update_only->isChecked() ? "checked" : "unchecked");
    #ifndef Q_WS_WIN
		sync_settings->setValue(QString("multitab_%1_%2/symlinks").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->symlinks->isChecked() ? "checked" : "unchecked");
	#endif
        /*sync_settings->setValue(QString("multitab_%1_%2/filters_gb").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->filters->isChecked() ? "checked" : "unchecked");
		QStringList flist;
		for (int n = 0; n < multi_page->lw_filters->count(); ++n) {
			if (multi_page->lw_filters->item(n)->checkState()==Qt::Checked) {
				flist << multi_page->lw_filters->item(n)->text();
			}
		}
		sync_settings->setValue(QString("multitab_%1_%2/flist").arg(multi_tabWidget->tabText(i)).arg(i), flist);*/
        sync_settings->setValue(QString("multitab_%1_%2/files_blacklist").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->files_blacklist);
        sync_settings->setValue(QString("multitab_%1_%2/folders_blacklist").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->folders_blacklist);
        sync_settings->setValue(QString("multitab_%1_%2/exts_blacklist").arg(multi_tabWidget->tabText(i)).arg(i), multi_page->exts_blacklist);
	}
	sync_settings->setValue("multitabs", multitabs_list);
    sync_settings->setValue("synchronised", synchronised);
    sync_settings->setValue("restore_clean_selected", restore_clean_selected->isChecked());
    sync_settings->setValue("restore_clean_by_date", restore_clean_by_date->isChecked());
    sync_settings->setValue("restore_clean_repeated", restore_clean_repeated->isChecked());
    sync_settings->setValue("restore_clean_date", restore_clean_date->value());
    sync_settings->setValue("restore_clean_files", restore_clean_files->value());
    sync_settings->setValue("files_blacklist", files_blacklist);
    sync_settings->setValue("folders_blacklist", folders_blacklist);
    sync_settings->setValue("exts_blacklist", exts_blacklist);
    sync_settings->setValue("run_hidden", run_hidden);
    sync_settings->setValue("current_synctab", tabWidget->currentIndex());
    sync_settings->setValue("current_multitab", multi_tabWidget->currentIndex());
    sync_settings->setValue("disable_tray_messages", actionDisable_tray_messages->isChecked());
    QStringList schedules; SyncSchedule * schedule;
	for (int i = 0; i < tw_schedules->rowCount(); ++i) {
		schedule = item_sched_map.value(tw_schedules->item(i, 0));
		schedules << tw_schedules->item(i, 0)->text();
		if (schedule->scheduling) schedules << "on";
		else schedules << "off";
		sync_settings->setValue(QString("sched_%1_%2_tabs").arg(tw_schedules->item(i, 0)->text()).arg(i), schedule->sched_tab_list);
		sync_settings->setValue(QString("sched_%1_%2_multitabs").arg(tw_schedules->item(i, 0)->text()).arg(i), schedule->sched_multitab_list);
		sync_settings->setValue(QString("sched_%1_%2_times").arg(tw_schedules->item(i, 0)->text()).arg(i), schedule->sched_time_list);
		sync_settings->setValue(QString("sched_%1_%2_checkedtimes").arg(tw_schedules->item(i, 0)->text()).arg(i), schedule->sched_checked_time_list);
		sync_settings->setValue(QString("sched_%1_%2_periodical_interval").arg(tw_schedules->item(i, 0)->text()).arg(i), schedule->periodical_interval);
		sync_settings->setValue(QString("sched_%1_%2_tab_index").arg(tw_schedules->item(i, 0)->text()).arg(i), schedule->timing_tab_index);
	}
	sync_settings->setValue("schedules", schedules);
    sync_settings->setValue("pos", this->pos());
    sync_settings->setValue("size", this->size());
    sync_settings->setValue("ver", QVariant(f_ver).toString());
#ifdef Q_WS_MAC
	sync_settings->setValue("macx_brushedmetalstyle", actionBrushedMetalStyle->isChecked());
#endif
}

void MainWindow::readSettings()
{
	QSettings settings ("Matus Tomlein", "Synkron");
#ifdef Q_WS_MAC
	actionBrushedMetalStyle->setChecked(sync_settings->value("macx_brushedmetalstyle", settings.value("macx_brushedmetalstyle", false)).toBool());
	if (actionBrushedMetalStyle->isChecked()) this->setAttribute(Qt::WA_MacBrushedMetal);
	this->setUnifiedTitleAndToolBarOnMac(!actionBrushedMetalStyle->isChecked());
#endif
	QStringList filters = sync_settings->value("filters").toStringList();
	for (int i = 0; i < filters.count(); ++i) {
		addFilter(filters.at(i), sync_settings->value(QString("filter_%1_%2/extensions").arg(filters.at(i)).arg(i)).toStringList());
	}
	SyncPage * page = 0;
	QStringList tabs_list = sync_settings->value("tabs").toStringList();
    if (tabs_list.count()==0) {
		QStringList recentsyncs = sync_settings->value("recentsyncs").toStringList();
		if (recentsyncs.count() != 0) {
			bool okey; recentsyncs.last().toInt(&okey);
    		if ((recentsyncs.count()-1) % 6 == 5 && okey) {
				for (int i = 0; i < recentsyncs.count(); ++i) {
     				if (i % 6 == 0 || i == 0) { page = addSyncTab();
						page->sync_folders->removeAllFolders();
                        QDir dirpath(recentsyncs.at(i));
						page->tab_name->setText(dirpath.path());
						tabWidget->setTabText(tabWidget->indexOf(page->tab_stw), dirpath.path()); }
  		  			if (i % 6 == 1) {
                        page->sync_folders->addFolder()->setPath(recentsyncs.at(i));
                        page->sync_folders->addToFolders(2);
                    }
    				if (i % 6 == 2) page->sync_folders->addFolder()->setPath(recentsyncs.at(i));
					if (i % 6 == 4) page->sync_hidden->setChecked(recentsyncs.at(i)=="checked");
  				}
			}
			else {
				for (int i = 0; i < recentsyncs.count(); ++i) {
     				if (i % 12 == 0 || i == 0) { page = addSyncTab();
     				    page->sync_folders->removeAllFolders();
						QDir dirpath(recentsyncs.at(i));
						page->tab_name->setText(dirpath.path());
						tabWidget->setTabText(tabWidget->indexOf(page->tab_stw), dirpath.path()); }
   			 		if (i % 12 == 1) {
                        page->sync_folders->addFolder()->setPath(recentsyncs.at(i));
                        page->sync_folders->addToFolders(2);
                    }
    				if (i % 12 == 2) page->sync_folders->addFolder()->setPath(recentsyncs.at(i));
					if (i % 12 == 4) page->sync_hidden->setChecked(recentsyncs.at(i)=="checked");
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
					page->sync_folders->removeAllFolders();
                    QDir dirpath(recentsyncs.at(i));
					page->tab_name->setText(dirpath.path());
					tabWidget->setTabText(tabWidget->indexOf(page->tab_stw), dirpath.path()); }
        		if (i % 3 == 1) page->sync_folders->addFolder()->setPath(recentsyncs.at(i));
    			if (i % 3 == 2) {
                    page->sync_folders->addFolder()->setPath(recentsyncs.at(i));
                    page->sync_folders->addToFolders(2);
                }
    		}
		}
	}
	for (int i = 0; i < tabs_list.count(); ++i) {
		page = addSyncTab();
		page->tab_name->setText(tabs_list.at(i));
		tabWidget->setTabText(tabWidget->indexOf(page->tab_stw), tabs_list.at(i));
		QStringList folders = sync_settings->value(QString("tab_%1_%2/folders").arg(tabs_list.at(i)).arg(i)).toStringList();
		if (folders.count()==0) {
            folders << sync_settings->value(QString("tab_%1_%2/folder1").arg(tabs_list.at(i)).arg(i)).toString();
            folders << sync_settings->value(QString("tab_%1_%2/folder2").arg(tabs_list.at(i)).arg(i)).toString();
        }
        page->sync_folders->removeAllFolders();
        for (int f = 0; f < folders.count(); ++f) {
            if (folders.at(f) != "")
                page->sync_folders->addFolder()->setPath(folders.at(f));
        }
        page->sync_folders->addToFolders(2);
        //page->sync_folder_1->setText(sync_settings->value(QString("tab_%1_%2/folder1").arg(tabs_list.at(i)).arg(i)).toString());
		//page->sync_folder_2->setText(sync_settings->value(QString("tab_%1_%2/folder2").arg(tabs_list.at(i)).arg(i)).toString());
		showAdvancedGroupBox(sync_settings->value(QString("tab_%1_%2/advanced").arg(tabs_list.at(i)).arg(i)).toString()=="checked", page);
		page->sync_hidden->setChecked(sync_settings->value(QString("tab_%1_%2/sync_hidden").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		page->sync_nosubdirs->setChecked(sync_settings->value(QString("tab_%1_%2/sync_nosubdirs").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		page->ignore_blacklist->setChecked(sync_settings->value(QString("tab_%1_%2/ignore_blacklist").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		if (sync_settings->value(QString("tab_%1_%2/backup_folder_1").arg(tabs_list.at(i)).arg(i)).toString()=="checked") if (!page->backup_folder_1->isChecked()) page->backup_folder_1->click();
		if (sync_settings->value(QString("tab_%1_%2/backup_folder_2").arg(tabs_list.at(i)).arg(i)).toString()=="checked") if (!page->backup_folder_2->isChecked()) page->backup_folder_2->click();
		if (sync_settings->value(QString("tab_%1_%2/update_only_1").arg(tabs_list.at(i)).arg(i)).toString()=="checked") if (!page->update_only_1->isChecked()) page->update_only_1->click();
        if (sync_settings->value(QString("tab_%1_%2/update_only_2").arg(tabs_list.at(i)).arg(i)).toString()=="checked") if (!page->update_only_2->isChecked()) page->update_only_2->click();
		if (sync_settings->value(QString("tab_%1_%2/move").arg(tabs_list.at(i)).arg(i)).toString()=="checked") if (!page->move->isChecked()) page->move->click();
		if (sync_settings->value(QString("tab_%1_%2/clone_folder1").arg(tabs_list.at(i)).arg(i)).toString()=="checked") if (!page->clone_folder1->isChecked()) page->clone_folder1->click();
        if (sync_settings->value(QString("tab_%1_%2/propagate_deletions").arg(tabs_list.at(i)).arg(i)).toString()=="checked") {
            page->propagate_deletions->setChecked(true); page->propagatedStateChanged(true); }
		if (sync_settings->value(QString("tab_%1_%2/backup_folders").arg(tabs_list.at(i)).arg(i)).toString()=="checked") if (!page->backup_folders->isChecked()) page->backup_folders->click();
		if (sync_settings->value(QString("tab_%1_%2/update_only").arg(tabs_list.at(i)).arg(i)).toString()=="checked") if (!page->update_only->isChecked()) page->update_only->click();
        //page->backup_folder_1->setChecked(sync_settings->value(QString("tab_%1_%2/backup_folder_1").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		//page->backup_folder_2->setChecked(sync_settings->value(QString("tab_%1_%2/backup_folder_2").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		//page->update_only_1->setChecked(sync_settings->value(QString("tab_%1_%2/update_only_1").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		//page->update_only_2->setChecked(sync_settings->value(QString("tab_%1_%2/update_only_2").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		//page->move->setChecked(sync_settings->value(QString("tab_%1_%2/move").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		//page->clone_folder1->setChecked(sync_settings->value(QString("tab_%1_%2/clone_folder1").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
	    //page->propagate_deletions->setChecked(sync_settings->value(QString("tab_%1_%2/propagate_deletions").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
        //page->backup_folders->setChecked(sync_settings->value(QString("tab_%1_%2/backup_folders").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		//page->update_only->setChecked(sync_settings->value(QString("tab_%1_%2/update_only").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		if (sync_settings->value(QString("tab_%1_%2/periodical").arg(tabs_list.at(i)).arg(i)).toString()=="checked") {
	        SyncSchedule * sync_sched = addSchedule(tabs_list.at(i));
	        sync_sched->periodical_interval = sync_settings->value(QString("tab_%1_%2/sync_interval").arg(tabs_list.at(i)).arg(i), 1).toInt();
	        sync_sched->timing_tab_index = 1;
	        sync_sched->sched_tab_list << tabs_list.at(i);
	        if (sync_settings->value(QString("tab_%1_%2/periodical_stop").arg(tabs_list.at(i)).arg(i)).toString()=="on") {
	            startSchedule(item_sched_map.key(sync_sched));
	        }
	    }
	#ifndef Q_WS_WIN
		page->symlinks->setChecked(sync_settings->value(QString("tab_%1_%2/symlinks").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
	#endif
		page->filters->setChecked(sync_settings->value(QString("tab_%1_%2/filters_gb").arg(tabs_list.at(i)).arg(i)).toString()=="checked");
		QStringList flist = sync_settings->value(QString("tab_%1_%2/flist").arg(tabs_list.at(i)).arg(i)).toStringList();
		for (int n = 0; n < flist.count(); ++n) {
			for (int f = 0; f < page->lw_filters->count(); ++f) {
				if (page->lw_filters->item(f)->text() == flist.at(n)) {
					page->lw_filters->item(f)->setCheckState(Qt::Checked);
				}
			}
		}
        page->files_blacklist = sync_settings->value(QString("tab_%1_%2/files_blacklist").arg(tabs_list.at(i)).arg(i), sync_settings->value("files_blacklist")).toStringList();
        page->folders_blacklist = sync_settings->value(QString("tab_%1_%2/folders_blacklist").arg(tabs_list.at(i)).arg(i), sync_settings->value("folders_blacklist")).toStringList();
        page->exts_blacklist = sync_settings->value(QString("tab_%1_%2/exts_blacklist").arg(tabs_list.at(i)).arg(i), sync_settings->value("exts_blacklist")).toStringList();
	    
    }
    synchronised = sync_settings->value("synchronised").toStringList();
    restore_clean_selected->setChecked(sync_settings->value("restore_clean_selected", true).toBool());
    restore_clean_by_date->setChecked(sync_settings->value("restore_clean_by_date").toBool());
    restore_clean_repeated->setChecked(sync_settings->value("restore_clean_repeated").toBool());
    restore_clean_date->setValue(sync_settings->value("restore_clean_date", 7).toInt());
    restore_clean_files->setValue(sync_settings->value("restore_clean_files", 3).toInt());
    files_blacklist = sync_settings->value("files_blacklist").toStringList();
	folders_blacklist = sync_settings->value("folders_blacklist").toStringList();
	exts_blacklist = sync_settings->value("exts_blacklist").toStringList();
	QStringList multitabs_list = sync_settings->value("multitabs").toStringList();
	MultisyncPage * multi_page;
	if (multitabs_list.count()==0) {
		QStringList multisyncs_items = sync_settings->value("multisyncs_items").toStringList();
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
		multi_page->destination_multi->setText(sync_settings->value("destination_multi").toString());
		multi_page->destinationTextChanged();
	}
	for (int i = 0; i < multitabs_list.count(); ++i) {
		multi_page = addMultiTab();
		multi_page->tab_name->setText(multitabs_list.at(i));
		multi_tabWidget->setTabText(multi_tabWidget->indexOf(multi_page), multitabs_list.at(i));
		QStringList sources = sync_settings->value(QString("multitab_%1_%2/sources").arg(multitabs_list.at(i)).arg(i)).toStringList();
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
		multi_page->destination_multi->setText(sync_settings->value(QString("multitab_%1_%2/destination").arg(multitabs_list.at(i)).arg(i)).toString());
		multi_page->showAdvancedGroupBox(sync_settings->value(QString("multitab_%1_%2/advanced").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		multi_page->sync_hidden->setChecked(!(sync_settings->value(QString("multitab_%1_%2/sync_hidden").arg(multitabs_list.at(i)).arg(i)).toString()=="unchecked"));
		multi_page->backup_folder_1->setChecked(sync_settings->value(QString("multitab_%1_%2/backup_folder_1").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		multi_page->backup_folder_2->setChecked(sync_settings->value(QString("multitab_%1_%2/backup_folder_2").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		multi_page->update_only_1->setChecked(sync_settings->value(QString("multitab_%1_%2/update_only_1").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		multi_page->update_only_2->setChecked(sync_settings->value(QString("multitab_%1_%2/update_only_2").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		multi_page->sync_nosubdirs->setChecked(sync_settings->value(QString("multitab_%1_%2/sync_nosubdirs").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		multi_page->ignore_blacklist->setChecked(sync_settings->value(QString("multitab_%1_%2/ignore_blacklist").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		multi_page->move->setChecked(sync_settings->value(QString("multitab_%1_%2/move").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		multi_page->clone_folder1->setChecked(sync_settings->value(QString("multitab_%1_%2/clone_folder1").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
        multi_page->propagate_deletions->setChecked(sync_settings->value(QString("multitab_%1_%2/propagate_deletions").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
        //multi_page->backup_folders->setChecked(sync_settings->value(QString("multitab_%1_%2/backup_folders").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		//multi_page->update_only->setChecked(sync_settings->value(QString("multitab_%1_%2/update_only").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
	#ifndef Q_WS_WIN
		multi_page->symlinks->setChecked(sync_settings->value(QString("multitab_%1_%2/symlinks").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
	#endif
		/*
        multi_page->filters->setChecked(sync_settings->value(QString("multitab_%1_%2/filters_gb").arg(multitabs_list.at(i)).arg(i)).toString()=="checked");
		QStringList flist = sync_settings->value(QString("multitab_%1_%2/flist").arg(multitabs_list.at(i)).arg(i)).toStringList();
		for (int n = 0; n < flist.count(); ++n) {
			for (int f = 0; f < multi_page->lw_filters->count(); ++f) {
				if (multi_page->lw_filters->item(f)->text() == flist.at(n)) {
					multi_page->lw_filters->item(f)->setCheckState(Qt::Checked);
				}
			}
		}*/
        multi_page->files_blacklist = sync_settings->value(QString("multitab_%1_%2/files_blacklist").arg(multitabs_list.at(i)).arg(i), sync_settings->value("files_blacklist")).toStringList();
        multi_page->folders_blacklist = sync_settings->value(QString("multitab_%1_%2/folders_blacklist").arg(multitabs_list.at(i)).arg(i), sync_settings->value("folders_blacklist")).toStringList();
        multi_page->exts_blacklist = sync_settings->value(QString("multitab_%1_%2/exts_blacklist").arg(multitabs_list.at(i)).arg(i), sync_settings->value("exts_blacklist")).toStringList();
	}
	run_hidden = sync_settings->value("run_hidden", false).toBool();
	actionDisable_tray_messages->setChecked(sync_settings->value("disable_tray_messages").toBool());
	QStringList schedules = sync_settings->value("schedules").toStringList(); QString schedule;
	SyncSchedule * sync_sched = 0;
	for (int i = 0; i < schedules.count(); ++i) {
		if (i % 2 == 0) {
		    sync_sched = addSchedule(schedules.at(i));
		    sync_sched->sched_tab_list = sync_settings->value(QString("sched_%1_%2_tabs").arg(schedules.at(i)).arg(i/2), QStringList()).toStringList();
		    sync_sched->sched_multitab_list = sync_settings->value(QString("sched_%1_%2_multitabs").arg(schedules.at(i)).arg(i/2), QStringList()).toStringList();
		    sync_sched->sched_time_list = sync_settings->value(QString("sched_%1_%2_times").arg(schedules.at(i)).arg(i/2), QStringList()).toStringList();
		    sync_sched->sched_checked_time_list = sync_settings->value(QString("sched_%1_%2_checkedtimes").arg(schedules.at(i)).arg(i/2), QStringList()).toStringList();
		    sync_sched->periodical_interval = sync_settings->value(QString("sched_%1_%2_periodical_interval").arg(schedules.at(i)).arg(i/2), 1).toInt();
		    sync_sched->timing_tab_index = sync_settings->value(QString("sched_%1_%2_tab_index").arg(schedules.at(i)).arg(i/2), 0).toInt();
		}
		if (i % 2 == 1) {
			if (schedules.at(i)=="on") {
				startSchedule(item_sched_map.key(sync_sched));
			}
		}
	}
	tabWidget->setCurrentIndex(sync_settings->value("current_synctab", 0).toInt());
	multi_tabWidget->setCurrentIndex(sync_settings->value("current_multitab", 0).toInt());
    this->move(sync_settings->value("pos", (settings.value("pos", this->pos()))).toPoint());
	this->resize(sync_settings->value("size", (settings.value("size", this->size()))).toSize());
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
     //connect(minimizeAction, SIGNAL(triggered()), this, SLOT(minimizeTrayIcon()));

     maximizeAction = new QAction(tr("S&how"), this);
     connect(maximizeAction, SIGNAL(triggered()), this, SLOT(show()));
     //connect(maximizeAction, SIGNAL(triggered()), this, SLOT(maximizeTrayIcon()));

     syncAction = new QAction(tr("Sync &current tab"), this);
     connect(syncAction, SIGNAL(triggered()), this, SLOT(sync()));
     
     syncAllAction = new QAction(tr("Sync &all tabs"), this);
     connect(syncAllAction, SIGNAL(triggered()), this, SLOT(syncAll()));

     quitAction = new QAction(tr("&Quit"), this);
     connect(quitAction, SIGNAL(triggered()), this, SLOT(closeApp()));
     
     checkRestoreItemAction = new QAction(tr("&Check/Uncheck"), this);
	 connect(checkRestoreItemAction, SIGNAL(triggered()), this, SLOT(checkRestoreItem()));
     
     restoreAction = new QAction(tr("&Restore"), this);
     connect(restoreAction, SIGNAL(triggered()), this, SLOT(restoreCurrentItem()));
     
	 deleteRestoreItemAction = new QAction(tr("&Remove"), this);
	 connect(deleteRestoreItemAction, SIGNAL(triggered()), this, SLOT(deleteRestoreItem()));
	 
	 blacklistRestoreItemAction = new QAction(tr("Add to &blacklist"), this);
	 connect(blacklistRestoreItemAction, SIGNAL(triggered()), this, SLOT(blacklistRestoreItem()));
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
     trayIcon->setToolTip("Synkron");
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
	else if (action == actionSyncView) {
        toSyncView();
		actionNew_sync->setDisabled(true);
		actionClose_sync->setDisabled(true);
		actionSave_log->setEnabled(false);
		mainStackedWidget->setCurrentIndex(6);
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
	QStringList langs(synkron_i18n.keys()); langs.sort();
	for (int i = 0; i < langs.count(); ++i) {
		langComboBox->addItem(langs.at(i));
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
	QString current_lang = sync_settings->value("lang", "English").toString();
	if (current_lang != lang) {
		sync_settings->setValue("lang", lang);
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
        if (page->logs_stw->currentIndex()==1) return;
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
		tabs.remove(tabWidget->currentWidget());
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

bool MainWindow::removeDir(QString path)
{
	QDir dir (path);
	if (!dir.exists()) return false;
	QFileInfoList info_list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files | QDir::Hidden, (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
	for (int i = 0; i < info_list.count(); ++i) {
		if (info_list.at(i).isSymLink() || !info_list.at(i).isDir()) {
			QFile file (info_list.at(i).absoluteFilePath());
			if (!file.remove()) return false;
			continue;
		}
		if (!removeDir(info_list.at(i).absoluteFilePath())) return false;
	}
	QString dirname = dir.dirName();
	dir.cdUp();
	if (!dir.rmdir(dirname)) return false;
	return true;
}

bool MainWindow::removeFile(QString path)
{
    QFileInfo file_info (path);
    if (!file_info.exists()) return false;
    else if (file_info.isSymLink() || !file_info.isDir()) {
        if (!QFile(file_info.absoluteFilePath()).remove()) {
            QMessageBox::critical(this, tr("Synkron"), tr("Error removing file %1").arg(file_info.path()));
            return false;
        }
    } else {
        QDir dir (path);
        if (!removeDir(dir.path())) {
            QMessageBox::critical(this, tr("Synkron"), tr("Error removing directory %1").arg(dir.path()));
            return false;
        }
    }
    return true;
}

void MainWindow::globalDelete(QString path)
{
	if (path == "") return;
	QMessageBox msgBox; msgBox.setText(tr("Are you sure you want to remove \"%1\" from every synced location?").arg(path));
	msgBox.setWindowTitle(QString("Synkron")); msgBox.setIcon(QMessageBox::Question);
 	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    QMapIterator<QWidget *, SyncPage *> i(tabs);
    MultisyncPage * multi_page;
    QProgressDialog progress (this);
    QFileInfo file_info (path);
 	
 	switch (msgBox.exec()) {
 	case QMessageBox::Yes:
        progress.setLabelText(tr("Removing files..."));
        progress.setMinimum(0);
        progress.setMaximum(tabWidget->count() /*+ multi_tabWidget->count()*/ + 1);
        progress.setMinimumDuration(0);
        progress.setWindowModality(Qt::WindowModal);
	    
	    path = file_info.absoluteFilePath();
	    if (progress.wasCanceled()) return;
	    removeFile(path);
	    progress.setValue(progress.value()+1);
	    qApp->processEvents();
	    
	    while (i.hasNext()) {
	    	i.next();
	    	if (progress.wasCanceled()) return;
	    	QString path2;
	    	for (int n = 0; n < i.value()->sync_folders->count(); ++n) {
                if (path.startsWith(i.value()->sync_folders->syncFolder(n)->path())) {
                    path2 = path;
	    		    path2.remove(i.value()->sync_folders->syncFolder(n)->path());
	    		    break;
                }
            }
            if (path2 == "" || path2 == ".") continue;
            for (int n = 0; n < i.value()->sync_folders->count(); ++n) {
                QString path3 = path2;
                path3.insert(0, i.value()->sync_folders->syncFolder(n)->path());
                if (path3 == path) continue;
                if (removeFile(path3)) {
                    i.value()->addTableItem(tr("File %1 deleted").arg(path3), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
                }
            }
	    	/*if (path.startsWith(i.value()->syncFolder1Text())) {
	    		path2 = path;
	    		path2.replace(i.value()->syncFolder1Text(), i.value()->syncFolder2Text());
	    	} else if (path.startsWith(i.value()->syncFolder2Text())) {
	    		path2 = path;
	    		path2.replace(i.value()->syncFolder2Text(), i.value()->syncFolder1Text());
	    	} else {
	    		continue;
	    	}
	    	if (path2 == "" || path2 == ".") continue;
	    	if (removeFile(path2)) {
                i.value()->addTableItem(tr("File %1 deleted").arg(path2), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
            }*/
	    	progress.setValue(progress.value()+1);
	    	qApp->processEvents();
	    }
	    
        for (int m = 0; m < multi_tabWidget->count(); ++m) {
            multi_page = (MultisyncPage *)multi_tabWidget->widget(m);
            if (path.startsWith(multi_page->destination_multi->text())) {
                QString path2 = path;
                path2.remove(multi_page->destination_multi->text());
                if (path2.startsWith('/')) {
                    path2.remove(0, 1);
                }
                for (int s = 0; s < multi_page->list_multi->count(); ++s) {
                    if (path2.startsWith(multi_page->list_multi->item(s)->text())) {
                        if (path2.startsWith("HOMEPATH", Qt::CaseSensitive)) {
                            path2.replace("HOMEPATH", QDir::homePath());
                        } else if (path2.startsWith("ROOTPATH", Qt::CaseSensitive)) {
                            path2.replace("ROOTPATH", QDir::rootPath());
                        }
                        if (removeFile(path2)) {
                            multi_page->addTableItem(tr("File %1 deleted").arg(path2), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
                        }
                        break;
                    }
                }
            } else {
                for (int s = 0; s < multi_page->list_multi->count(); ++s) {
                    QString path3 = multi_page->list_multi->item(s)->text();
                    if (path3.startsWith("HOMEPATH", Qt::CaseSensitive)) {
                        path3.replace("HOMEPATH", QDir::homePath());
                    } else if (path3.startsWith("ROOTPATH", Qt::CaseSensitive)) {
                        path3.replace("ROOTPATH", QDir::rootPath());
                    }
                    if (path.startsWith(path3)) {
                        QString path2 = QString("%1/%2%3").arg(multi_page->destination_multi->text())
                                                        .arg(multi_page->list_multi->item(s)->text())
                                                        .arg(QString(path).remove(0, path3.count()));
                        if (removeFile(path2)) {
                            multi_page->addTableItem(tr("File %1 deleted").arg(path2), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
                        }
                        break;
                    }
                }
            }
            progress.setValue(progress.value()+1);
            qApp->processEvents();
        }
        break;
 	case QMessageBox::No:
     	break;
	default:
   		break;
 	}
}

bool MainWindow::renameFile(QString path, QString name)
{
    QFileInfo file_info (path);
	path = file_info.absoluteFilePath();
	if (!file_info.exists()) return false;
	else if (file_info.isSymLink() || !file_info.isDir()) {
		QFile file (path);
		if (!file.rename(QString("%1/%2").arg(file_info.dir().path()).arg(name))) {
			QMessageBox::critical(this, tr("Synkron"), tr("Error renaming file %1").arg(path));
			return false;
		}
	} else {
		QDir dir (path);
		QString dirname = dir.dirName();
		dir.cdUp();
		if (!dir.rename(dirname, name)) {
			QMessageBox::critical(this, tr("Synkron"), tr("Error renaming directory %1").arg(path));
			return false;
		}
	}
	return true;
}

void MainWindow::globalRename(QString path, QString name)
{
	if (path == "" || name == "") return;
	QFileInfo file_info (path);
	path = file_info.absoluteFilePath();
	
	renameFile(path, name);
	
	QMapIterator<QWidget *, SyncPage *> i(tabs);
	while (i.hasNext()) {
		i.next();
		QString path2;
		for (int n = 0; n < i.value()->sync_folders->count(); ++n) {
            if (path.startsWith(i.value()->sync_folders->syncFolder(n)->path())) {
                path2 = path;
	    		path2.remove(i.value()->sync_folders->syncFolder(n)->path());
	    		break;
            }
        }
        if (path2 == "" || path2 == ".") continue;
        for (int n = 0; n < i.value()->sync_folders->count(); ++n) {
            QString path3 = path2;
            path3.insert(0, i.value()->sync_folders->syncFolder(n)->path());
            if (path3 == path) continue;
            if (renameFile(path3, name)) {
                i.value()->addTableItem(tr("File %1 renamed").arg(path3), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
            }
        }
        
		/*if (path.startsWith(i.value()->syncFolder1Text())) {
			path2 = path;
			path2.replace(i.value()->syncFolder1Text(), i.value()->syncFolder2Text());
		} else if (path.startsWith(i.value()->syncFolder2Text())) {
			path2 = path;
			path2.replace(i.value()->syncFolder2Text(), i.value()->syncFolder1Text());
		} else {
			continue;
		}
		if (path2 == "" || path2 == ".") continue;
		
		if (renameFile(path2, name)) {
            i.value()->addTableItem(tr("File %1 renamed").arg(path2), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
        }*/
	}
	
	MultisyncPage * multi_page;
	for (int m = 0; m < multi_tabWidget->count(); ++m) {
        multi_page = (MultisyncPage *)multi_tabWidget->widget(m);
        if (path.startsWith(multi_page->destination_multi->text())) {
            QString path2 = path;
            path2.remove(multi_page->destination_multi->text());
            if (path2.startsWith('/')) {
                path2.remove(0, 1);
            }
            for (int s = 0; s < multi_page->list_multi->count(); ++s) {
                if (path2.startsWith(multi_page->list_multi->item(s)->text())) {
                    if (path2.startsWith("HOMEPATH", Qt::CaseSensitive)) {
                        path2.replace("HOMEPATH", QDir::homePath());
                    } else if (path2.startsWith("ROOTPATH", Qt::CaseSensitive)) {
                        path2.replace("ROOTPATH", QDir::rootPath());
                    }
                    
                    if (renameFile(path2, name)) {
                        multi_page->addTableItem(tr("File %1 renamed").arg(path2), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
                    }
                }
            }
        } else {
            for (int s = 0; s < multi_page->list_multi->count(); ++s) {
                QString path3 = multi_page->list_multi->item(s)->text();
                if (path3.startsWith("HOMEPATH", Qt::CaseSensitive)) {
                    path3.replace("HOMEPATH", QDir::homePath());
                } else if (path3.startsWith("ROOTPATH", Qt::CaseSensitive)) {
                    path3.replace("ROOTPATH", QDir::rootPath());
                }
                if (path.startsWith(path3)) {
                    QString path2 = QString("%1/%2%3").arg(multi_page->destination_multi->text())
                                                        .arg(multi_page->list_multi->item(s)->text())
                                                        .arg(QString(path).remove(0, path3.count()));
                    
                    if (renameFile(path2, name)) {
                        multi_page->addTableItem(tr("File %1 renamed").arg(path2), "", QString::fromUtf8(":/new/prefix1/images/file.png"), QBrush(Qt::darkMagenta), QBrush(Qt::white));
                    }
                }
            }
        }
    }
}

void MainWindow::showEvent(QShowEvent *)
{
    shown_manually = true;
    trayIconVisible(true);
}

void MainWindow::hideEvent(QHideEvent *)
{
    trayIconVisible(false);
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
    MTApplication app(argc, argv);
    app.setOrganizationName("Matus Tomlein");
    app.setApplicationName("Synkron");
    
    QFileInfo file_info (app.arguments().at(0));
    QDir dir (file_info.dir()); QSettings * sync_settings;
#ifdef PORTABLE_APP
    dir.cdUp(); dir.cdUp();
    dir.cd("Data"); dir.cd("settings");
#endif
    if (dir.exists("Synkron.ini")) {
        sync_settings = new QSettings (dir.absoluteFilePath("Synkron.ini"), QSettings::IniFormat);
    } else {
        sync_settings = new QSettings (QSettings::IniFormat, QSettings::UserScope, "Matus Tomlein", "Synkron");
    }
	QString lang = sync_settings->value("lang").toString();
	if (lang.isEmpty()) {
		lang = QLocale::languageToString(QLocale::system().language());
		sync_settings->setValue("lang", lang);
	}
	if (lang == "C") { lang = "English"; sync_settings->setValue("lang", lang); }
	if (lang != "English") {
		QTranslator * translator = new QTranslator;
		translator->load(QString(":/i18n/Synkron-%1.qm").arg(lang.replace(" ", "_")));
		app.installTranslator(translator);
	}
    
    MainWindow *window = new MainWindow (sync_settings);
	if (window->runHidden() && app.arguments().count() <= 1) window->hide();
	else window->show();
    app.setAppMainWindow(window);
    return app.exec();
}
