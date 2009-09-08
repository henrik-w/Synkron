/*******************************************************************
 This file is part of Synkron
 Copyright (C) 2005-2009 Matus Tomlein (matus.tomlein@gmail.com)

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
    
        f_ver = 1.6;
        ver = "1.6.0";
    
    if (tr("LTR") == "RTL") { qApp->setLayoutDirection(Qt::RightToLeft); }
    
#ifdef Q_WS_MAC
	actionBrushedMetalStyle = new QAction(tr("Use the brushed metal style"), this);
	actionBrushedMetalStyle->setStatusTip(tr("Use the brushed metal style"));
	actionBrushedMetalStyle->setCheckable(true);
	menuOptions->addAction(actionBrushedMetalStyle);
    
    actionQuit->setMenuRole(QAction::QuitRole);
    actionAbout->setMenuRole(QAction::AboutRole);
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

    temp_path = QString("%1/.Synkron").arg(QDir::homePath());

#ifndef Q_WS_WIN
    tw_schedules->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    actionShut_down_after_sync->setVisible(false);
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
    
    QActionGroup * options_actgrp = new QActionGroup(this);
    options_actgrp->addAction(actionShut_down_after_sync);
    options_actgrp->addAction(actionQuit_after_sync);
    options_actgrp->addAction(actionSync_at_launch);
    options_actgrp->setExclusive(false);

    actionShow_icons_only = new QAction(tr("Show icons only"), this);
    actionShow_icons_only->setCheckable(true);

    QTranslator translator; translator.load(":/i18n/Synkron-i18n.qm");
    synkron_i18n.insert("English", "English");
    synkron_i18n.insert(translator.translate("LanguageNames", "Slovak"), "Slovak");
    synkron_i18n.insert(translator.translate("LanguageNames", "German"), "German");
    synkron_i18n.insert(translator.translate("LanguageNames", "Japanese"), "Japanese");
    synkron_i18n.insert(translator.translate("LanguageNames", "Arabic"), "Arabic");
    synkron_i18n.insert(translator.translate("LanguageNames", "Russian"), "Russian");
    synkron_i18n.insert(translator.translate("LanguageNames", "Spanish"), "Spanish");
    synkron_i18n.insert(translator.translate("LanguageNames", "Brazilian Portuguese"), "Brazilian Portuguese");
    synkron_i18n.insert(translator.translate("LanguageNames", "Polish"), "Polish");
    synkron_i18n.insert(translator.translate("LanguageNames", "Italian"), "Italian");
    synkron_i18n.insert(translator.translate("LanguageNames", "French"), "French");
    synkron_i18n.insert(translator.translate("LanguageNames", "Valencian"), "Valencian");
    synkron_i18n.insert(translator.translate("LanguageNames", "Finnish"), "Finnish");
    synkron_i18n.insert(translator.translate("LanguageNames", "Czech"), "Czech");
    synkron_i18n.insert(translator.translate("LanguageNames", "Chinese"), "Chinese");
    synkron_i18n.insert(translator.translate("LanguageNames", "Dutch"), "Dutch");

    connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(actionNew_sync, SIGNAL(triggered()), this, SLOT(addTab()));
    connect(actionClose_sync, SIGNAL(triggered()), this, SLOT(closeTab()));
    connect(actgrpView, SIGNAL(triggered(QAction*)), this, SLOT(switchView(QAction*)));
    connect(options_actgrp, SIGNAL(triggered(QAction*)), this, SLOT(optionClicked(QAction*)));
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
    connect(timing_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(timingTabIndexChanged(int)));
    connect(actionSave_tab, SIGNAL(triggered()), this, SLOT(saveTab()));
    connect(actionSave_tab_as, SIGNAL(triggered()), this, SLOT(saveTabAs()));
    connect(actionLoad_tab, SIGNAL(triggered()), this, SLOT(loadTab()));
    connect(actionChange_temp, SIGNAL(triggered()), this, SLOT(changeTemp()));
    connect(actionShow_icons_only, SIGNAL(toggled(bool)), this, SLOT(showIconsOnly(bool)));
    connect(actionSync, SIGNAL(triggered()), this, SLOT(syncCurrentTab()));
    connect(actionAnalyse, SIGNAL(triggered()), this, SLOT(analyseCurrentTab()));
    connect(menuTab, SIGNAL(aboutToShow()), this, SLOT(aboutToShowTabMenu()));
    
    setCleanGB();
    setSelectGB();
    setSchedDatesGB();
    tabWidget->removeTab(0);
    multi_tabWidget->removeTab(0);

    sync_settings = s;
    readSettings();
    loadTempSettings();
    if (tabWidget->count()==0) addSyncTab();
    if (multi_tabWidget->count()==0) addMultiTab();

    QSettings settings ("Matus Tomlein", "Synkron");
    tcp_server = new QTcpServer(this);
    QObject::connect(tcp_server, SIGNAL(newConnection()), this, SLOT(addConnection()));
    tcp_socket = new QTcpSocket(this);
    //QObject::connect(tcp_socket, SIGNAL(connected()), this, SLOT(sendMessageAndClose()));
    //QObject::connect(tcp_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(initServer(QAbstractSocket::SocketError)));
    tcp_socket->connectToHost("Localhost", settings.value("process_id", 1).toUInt());
    if (tcp_socket->waitForConnected(1000)) {
        sendMessageAndClose();
        skip_close_event = true;
        QTimer::singleShot(0, this, SLOT(close())); }
    else {
        initServer(QAbstractSocket::SocketTimeoutError);
        if (actionSync_at_launch->isChecked()) {
            QTimer::singleShot(0, this, SLOT(syncAll()));
        }
    }
    QTimer::singleShot(2000, this, SLOT(setShownManually()));
}

// +++ Connection +++

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
                    loadTab(file_info.absoluteFilePath());
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

    if ((quint64)c_socket->bytesAvailable() < c_blocksize)
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
            c_parent->loadTab(file_info.absoluteFilePath());
        }
    }

    c_socket->close();
    delete this;
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

// --- Connection ---
// +++ About +++

void MainWindow::about ()
{
    About *about = new About(ver, QVariant(QDate::currentDate().year()).toString());
    about->show();
}

About::About(QString ver, QString year/*, QString qtver*/)
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
	aboutQt.append(tr("This programme uses Qt Open Source Edition version %1.").arg(qVersion()));
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

// --- About ---
// +++ Languages +++


void MainWindow::checkForUpdates()
{
	delete http_buffer; http_buffer = new QBuffer(this);
    http->setHost("synkron.sourceforge.net");
	http->get("/current-version", http_buffer);
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

// --- Languages ---

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
