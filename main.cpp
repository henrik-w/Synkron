#include "main_window.h"

#include <QApplication>

MainWindow::MainWindow()
{
    setupUi(this);
    
    /*trayIcon = new QSystemTrayIcon (QIcon(QString::fromUtf8(":/new/prefix1/images/Synkron16.png")), this);
    QMenu *trayMenu = new QMenu ("Synkron");
    QMenu subMenu ("Menu");
    QAction * actionSync = new QAction ("&Sync", this);
    //trayMenu.addAction(QIcon(QString::fromUtf8(":/new/prefix1/images/Synkron16.png")), "actionSync", this, SLOT(sync()));
    connect(actionSync, SIGNAL(triggered()), this, SLOT(sync()));
    trayMenu->popup(QPoint(0,0), actionSync);
    trayIcon->setContextMenu(trayMenu);*/
    
    createActions();
    createTrayIcon();
    trayIcon->show();
    trayIconVisible(true);
    syncingAll = false;
    
    QTimer::singleShot(200, this, SLOT(updateGeometry()));
    
    actgrpView = new QActionGroup(this);
    actgrpView->addAction(actionSynchronise);
    actgrpView->addAction(actionRestore);
    actgrpView->addAction(actionBlacklist);
    
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
    
    tabWidget->removeTab(0);
    readSettings();
}

SyncPage * MainWindow::addTab() { return addTab("", "", "", true, false, 5); }

//SyncPage * MainWindow::addTab(bool p, bool h) { return addTab("", "", "", p, h); }

SyncPage * MainWindow::addTab(QString name, QString folder1, QString folder2, bool periodical, bool hidden, int interval)
{
    SyncPage * page = new SyncPage;
    page->tab = new QWidget (tabWidget);
    QString title;
    if (name.isEmpty()) {
       int n = 1; gen_title:
       title = tr("Sync #%1").arg(n); bool ok = true;
       for (int i = 0; i < tabWidget->count(); ++i) {
           if (tabWidget->tabText(i) == title) { ok = false; }
       }
       if (!ok) { n++; goto gen_title; }
    }
    else { title = name; }
    tabWidget->addTab(page->tab, QIcon(QString::fromUtf8(":/new/prefix1/images/Synkron128.png")), title);
    QGridLayout * mainglayout = new QGridLayout (page->tab);
    mainglayout->setMargin(0); mainglayout->setSpacing(0);
    page->stacked_widget = new QStackedWidget (page->tab);
    mainglayout->addWidget(page->stacked_widget, 0, 0);
    page->stacked_page_1 = new QWidget (page->tab);
    page->stacked_widget->addWidget(page->stacked_page_1);
    page->stacked_page_2 = new QWidget (page->tab);
    page->stacked_widget->addWidget(page->stacked_page_2);
    /*page->stacked_page_3 = new QWidget (page->tab);
    page->stacked_widget->addWidget(page->stacked_page_3);*/
    page->stacked_widget->setCurrentIndex(0);
    
    QGridLayout * glayout = new QGridLayout (page->stacked_page_1);
    glayout->setMargin(10); glayout->setSpacing(6);
    QSpacerItem * spacerItem = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    
    glayout->addItem(spacerItem, 0, 0); // ###################################
    page->icon_label = new QLabel (page->stacked_page_1);
    page->icon_label->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/Synkron128.png")));
    page->icon_label->setAlignment(Qt::AlignCenter);
    
    glayout->addWidget(page->icon_label, 1, 0); // #############################
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    glayout->addItem(spacerItem, 3, 0); // ###################################
    QGridLayout * hlayout0 = new QGridLayout (page->stacked_page_1);
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout0->addItem(spacerItem, 0, 0);
    /*QLabel * tab_name_text = new QLabel (page->stacked_page_1);
    tab_name_text->setText(tr("Sync name:"));
    hlayout0->addWidget(tab_name_text, 0, 0);*/
    page->tab_name = new QLineEdit (page->stacked_page_1);
    page->tab_name->setStatusTip("Set sync name");
    page->tab_name->setText(tabWidget->tabText(tabWidget->indexOf(page->tab)));
    QObject::connect(page->tab_name, SIGNAL(editingFinished()), this, SLOT(tabNameEdited()));
    hlayout0->addWidget(page->tab_name, 0, 1);
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout0->addItem(spacerItem, 0, 2);
    glayout->addLayout(hlayout0, 4, 0);
    QLabel * sync_text = new QLabel (page->stacked_page_1);
    sync_text->setText(tr("Sync folders:"));
    glayout->addWidget(sync_text, 5, 0); // ###################################
    
    QGridLayout * hlayout1 = new QGridLayout (page->stacked_page_1);
    page->sync_folder_1 = new QLineEdit (page->stacked_page_1);
    page->sync_folder_1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    page->sync_folder_1->setStatusTip("Set the first synchronisation folder");
    page->sync_folder_1->setText(folder1);
    hlayout1->addWidget(page->sync_folder_1, 0, 0);
    page->browse_1 = new QPushButton (tr("Browse"), page->stacked_page_1);
    page->browse_1->setStatusTip("Browse");
    page->browse_1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    page->browse_1->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/folder.png")));
    page->browse_1->setIconSize(QSize(24, 24));
    hlayout1->addWidget(page->browse_1, 0, 1);
    glayout->addLayout(hlayout1, 6, 0); // ###################################
    
    QGridLayout * hlayout2 = new QGridLayout (page->stacked_page_1);
    page->sync_folder_2 = new QLineEdit (page->stacked_page_1);
    page->sync_folder_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    page->sync_folder_2->setStatusTip("Set the second synchronisation folder");
    page->sync_folder_2->setText(folder2);
    hlayout2->addWidget(page->sync_folder_2, 0, 0);
    page->browse_2 = new QPushButton (tr("Browse"), page->stacked_page_1);
    page->browse_2->setStatusTip("Browse");
    page->browse_2->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    page->browse_2->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/folder.png")));
    page->browse_2->setIconSize(QSize(24, 24));
    hlayout2->addWidget(page->browse_2, 0, 1);
    glayout->addLayout(hlayout2, 7, 0); // ###################################
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    glayout->addItem(spacerItem, 8, 0); // ###################################
    QButtonGroup * browse = new QButtonGroup (page->stacked_page_1);
    browse->addButton(page->browse_1); browse->addButton(page->browse_2);
    QObject::connect(browse, SIGNAL(buttonReleased(QAbstractButton *)), this, SLOT(browse(QAbstractButton *)));
    
    QGridLayout * hlayout3 = new QGridLayout (page->stacked_page_1);
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout3->addItem(spacerItem, 0, 0);
    page->sync = new QPushButton (tr("Sync"), page->stacked_page_1);
    page->sync->setStatusTip("Synchronise");
    page->sync->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/sync.png")));
    page->sync->setIconSize(QSize(24, 24));
    QObject::connect(page->sync, SIGNAL(released()), this, SLOT(sync()));
    QObject::connect(page, SIGNAL(sigsync(QWidget *)), this, SLOT(sync(QWidget *)));
    hlayout3->addWidget(page->sync, 0, 1);
    glayout->addLayout(hlayout3, 9, 0); // ###################################
    
    QGridLayout * glayout2 = new QGridLayout (page->stacked_page_2);
    glayout2->setMargin(10); glayout2->setSpacing(6);
    QLabel * sync_text_2 = new QLabel (page->stacked_page_2);
    sync_text_2->setText(tr("Synchronised files:"));
    glayout2->addWidget(sync_text_2, 0, 0); // #################################
    page->lw = new QListWidget (page->stacked_page_2);
    page->lw->setStatusTip("List of synchronised files and folders");
    glayout2->addWidget(page->lw, 1, 0); // ###################################
    QGridLayout * hlayout5 = new QGridLayout (page->stacked_page_2);
    page->periodical_sync = new QCheckBox (tr("Synchronise periodically, every "), page->stacked_page_2);
    page->periodical_sync->setStatusTip("Synchronise periodically");
    if (periodical==true) page->periodical_sync->setChecked(true);
    else page->periodical_sync->setChecked(false);
    hlayout5->addWidget(page->periodical_sync, 0, 0); // #######################
    page->sync_interval = new QSpinBox (page->stacked_page_2);
    page->sync_interval->setStatusTip("Set sync interval");
    page->sync_interval->setValue(interval);
    if (!page->periodical_sync->isChecked()) page->sync_interval->setEnabled(false);
    hlayout5->addWidget(page->sync_interval, 0, 1); // #########################
    QLabel * min_text = new QLabel (page->stacked_page_2);
    min_text->setText(tr(" minute(s)"));
    hlayout5->addWidget(min_text, 0, 2); // ###################################
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout5->addItem(spacerItem, 0, 3); // ###################################
    glayout2->addLayout(hlayout5, 2, 0); // ###################################
    connect(page->periodical_sync, SIGNAL(toggled(bool)), page->sync_interval, SLOT(setEnabled(bool)));
    page->sync_hidden = new QCheckBox (page->stacked_page_2);
    if (hidden==false) page->sync_hidden->setChecked(false);
    else page->sync_hidden->setChecked(true);
    page->sync_hidden->setText(tr("Synchronise hidden files and folders"));
    glayout2->addWidget(page->sync_hidden, 3, 0);
        
    QGridLayout * hlayout6 = new QGridLayout (page->stacked_page_2);
    page->back = new QPushButton (tr("Back"), page->stacked_page_2);
    page->back->setStatusTip("Go back");
    page->back->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/back.png")));
    page->back->setIconSize(QSize(24, 24));
    hlayout6->addWidget(page->back, 0, 0);
    QObject::connect(page->back, SIGNAL(released()), this, SLOT(goBack()));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout6->addItem(spacerItem, 0, 1);
    page->resync = new QPushButton (tr("Resync"), page->stacked_page_2);
    page->resync->setStatusTip("Synchronise again");
    page->resync->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/sync.png")));
    page->resync->setIconSize(QSize(24, 24));
    hlayout6->addWidget(page->resync, 0, 2);
    QObject::connect(page->resync, SIGNAL(released()), this, SLOT(sync()));
    glayout2->addLayout(hlayout6, 4, 0); // ###################################
    
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
    QDir d1 (directory1); QDir d2 (directory2);
    if ((!d1.exists()) || (!d2.exists())) {
		QListWidgetItem * item = new QListWidgetItem(tr("%1	Synchronisation failed: Directory not found").arg(QTime(QTime().currentTime()).toString("hh:mm:ss")));
    	item->setBackground(QBrush(Qt::red));
		page->lw->addItem(item);
		page->stacked_widget->setCurrentIndex(1);
		return;
	}
    /*if ((!d1.exists()) && (!d2.exists())) {
		QMessageBox msgBox; msgBox.setText(tr("Directories %1 and %2 not found. Create?").arg(d1.dirName()).arg(d2.dirName()));
		msgBox.setWindowTitle(QString("Synkron - %1").arg(tabWidget->tabText(tabWidget->indexOf(page->tab)))); msgBox.setIcon(QMessageBox::Question);
 		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
 		switch (msgBox.exec()) {
 		case QMessageBox::Yes:
     		if (!d1.mkpath(d1.path())) { QMessageBox::critical(this, tr("Synkron - %1").arg(tabWidget->tabText(tabWidget->indexOf(page->tab))), tr("Error creating directory %1.").arg(d1.dirName())); return; }
     		if (!d2.mkpath(d2.path())) { QMessageBox::critical(this, tr("Synkron - %1").arg(tabWidget->tabText(tabWidget->indexOf(page->tab))), tr("Error creating directory %1.").arg(d2.dirName())); return; }
     		break;
 		case QMessageBox::No:
     		return;
		 default:
     		break;
 		}
    }
    else if (!d1.exists()) {
		QMessageBox msgBox; msgBox.setText(tr("Directory %1 not found. Create?").arg(d1.dirName()));
		msgBox.setWindowTitle(QString("Synkron - %1").arg(tabWidget->tabText(tabWidget->indexOf(page->tab)))); msgBox.setIcon(QMessageBox::Question);
 		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
 		switch (msgBox.exec()) {
 		case QMessageBox::Yes:
     		if (!d1.mkpath(d1.path())) { QMessageBox::critical(this, tr("Synkron - %1").arg(tabWidget->tabText(tabWidget->indexOf(page->tab))), tr("Error creating directory %1.").arg(d1.dirName())); return; }
     		break;
 		case QMessageBox::No:
     		return;
		 default:
     		break;
 		}
    }
    else if (!d2.exists()) {
		QMessageBox msgBox; msgBox.setText(tr("Directory %1 not found. Create?").arg(d2.dirName()));
		msgBox.setWindowTitle(QString("Synkron - %1").arg(tabWidget->tabText(tabWidget->indexOf(page->tab)))); msgBox.setIcon(QMessageBox::Question);
 		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
 		switch (msgBox.exec()) {
 		case QMessageBox::Yes:
     		if (!d2.mkpath(d2.path())) { QMessageBox::critical(this, tr("Synkron - %1").arg(tabWidget->tabText(tabWidget->indexOf(page->tab))), tr("Error creating directory %1.").arg(d2.dirName())); return; }
     		break;
 		case QMessageBox::No:
     		return;
		 default:
     		break;
 		}
    }*/
    /*if (d1.count() == 0 && d2.count() == 0) {
       QMessageBox::warning(this, tr("Synkron - %1").arg(tabWidget->tabText(tabWidget->indexOf(page->tab))), tr("The selected directories are empty."));
       return;
    }*/
    if (d1.path() == d2.path()) {
       QMessageBox::warning(this, tr("Synkron - %1").arg(tabWidget->tabText(tabWidget->indexOf(page->tab))), tr("Directories with the same path selected."));
       return;
    }
    bool repeated = false; if (!syncingAll) { syncFiles = 0; }
    bool d1_blacklist = false; bool d2_blacklist = false;
    if (folders_blacklist.contains(d1.path(), Qt::CaseInsensitive)) { d1_blacklist = true; }
    if (folders_blacklist.contains(d2.path(), Qt::CaseInsensitive)) { d2_blacklist = true; }
    page->stacked_widget->setCurrentIndex(1);
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	qApp->processEvents();
    subSync(d1, d2, page, repeated, d1_blacklist, d2_blacklist);
    QApplication::restoreOverrideCursor();
    //if (page->periodical_sync->isChecked()) { sync_timer.stop(); sync_timer.singleShot(((page->sync_interval->value())*60000), page, SLOT(syncPage())); sync_timer.stop(); }
    if (page->periodical_sync->isChecked()) { page->sync_timer->stop(); page->sync_timer->start(page->sync_interval->value()*60000); }
    else { page->sync_timer->stop(); }
    QListWidgetItem * item = new QListWidgetItem(tr("%1	Synchronisation complete: %2 file(s) synchronised").arg(QTime(QTime().currentTime()).toString("hh:mm:ss")).arg(syncFiles-x));
    item->setBackground(QBrush(Qt::green));
	page->lw->addItem(item);
	if (!syncingAll) { trayIcon->showMessage(tr("Synchronisation complete"), tr("%1 files synchronised").arg(syncFiles)); syncFiles = 0; }
}

void MainWindow::subSync(QDir& d1, QDir& d2, SyncPage * page, bool repeated, bool d1_blacklist, bool d2_blacklist)
{
    QFileInfoList d1_entries; QFileInfoList d2_entries;
	if (page->sync_hidden->isChecked()) {
		d1_entries = d1.entryInfoList((QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden), (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
    	d2_entries = d2.entryInfoList((QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden), (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
	} else {
		d1_entries = d1.entryInfoList((QDir::NoDotAndDotDot | QDir::AllEntries), (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
    	d2_entries = d2.entryInfoList((QDir::NoDotAndDotDot | QDir::AllEntries), (QDir::Name | QDir::DirsFirst | QDir::IgnoreCase));
	}
    
    QString update_time = (QDateTime::currentDateTime()).toString("yyyy.MM.dd-hh.mm.ss");
    QString buffer;
    bool found = false; QFile * file; QFileInfo fileInfo;  QListWidgetItem *item; QDir *temp = new QDir (QDir::homePath());
    for (int i = 0; i < d1_entries.count(); ++i) {
        found = false;
        for (int n = 0; n < d2_entries.count(); ++n) {
            if (d1_entries.at(i).fileName() == d2_entries.at(n).fileName() && repeated == false) {
               if (d1_entries.at(i).isDir() && d2_entries.at(n).isDir()) {
                	QDir subDir5 (d1_entries.at(i).absoluteFilePath());
                	QDir subDir6 (d2_entries.at(n).absoluteFilePath());
                	bool subD1_blacklist = false; bool subD2_blacklist = false;
    				if (folders_blacklist.contains(subDir5.path(), Qt::CaseInsensitive)) { subD1_blacklist = true; }
    				if (folders_blacklist.contains(subDir6.path(), Qt::CaseInsensitive)) { subD2_blacklist = true; }
                	subSync(subDir5, subDir6, page, repeated, subD1_blacklist, subD2_blacklist);
                	found = true; continue;
               }
               else if (d1_entries.at(i).isDir()) {
                    if (!repeated) {
                       item = new QListWidgetItem (tr("A folder (%1) and a file (%2) with the same name have been found. Unable to synchronise these files.").arg(d1_entries.at(i).fileName()).arg(d2_entries.at(n).fileName()));
                       item->setBackground(QBrush::QBrush(Qt::red));
                       item->setForeground(QBrush::QBrush(Qt::white));
                       item->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/folder_16.png")));
                       page->lw->addItem(item);
                       qApp->processEvents();
                    }
                    found = true; continue;
               }
               else if (d2_entries.at(n).isDir()) {
                    if (!repeated) {
                       item = new QListWidgetItem (tr("A file (%1) and a folder (%2) with the same name have been found. Unable to synchronise these files.").arg(d1_entries.at(i).fileName()).arg(d2_entries.at(n).fileName()));
                       item->setBackground(QBrush::QBrush(Qt::red));
                       item->setForeground(QBrush::QBrush(Qt::white));
                       item->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/folder_16.png")));
                       page->lw->addItem(item);
                       qApp->processEvents();
                    }
                    found = true; continue;
               }
               else if (d1_entries.at(i).lastModified() < d2_entries.at(n).lastModified() && !d1_blacklist) {
                    if (files_blacklist.contains(d1_entries.at(i).absoluteFilePath(), Qt::CaseInsensitive)) { found = true; continue; }
					file = new QFile (d1_entries.at(i).absoluteFilePath());
                    if (!file->open(QIODevice::ReadOnly)) {
                       unknownError(page, d1_entries.at(i).fileName(), QString("file"), QString("open")); delete file; found = true; continue;
                    }
                    temp->mkpath(QString(".Synkron/%2").arg(update_time));
                    if (!(file->copy(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d1_entries.at(i).fileName()).arg(syncFiles)))) {
                       unknownError(page, d1_entries.at(i).fileName(), QString("file"), QString("copy"), QString(" to temp")); delete file; found = true; continue;
                    }
                    file->remove(); delete file;
                    file = new QFile (d2_entries.at(n).absoluteFilePath());
                    if (!file->open(QIODevice::ReadOnly)) {
                       unknownError(page, d2_entries.at(n).fileName(), QString("file"), QString("open")); delete file; found = true; continue;
                    }
                    buffer.clear(); buffer = d1_entries.at(i).absoluteFilePath();
                    if (!file->copy(buffer)) {
                       unknownError(page, d1_entries.at(i).fileName(), QString("file"), QString("copy")); delete file; found = true; continue;
                    }
                    synchronised << d1_entries.at(i).fileName() << update_time << d1_entries.at(i).absoluteFilePath();
                    synchronised << QString("%1/.Synkron/%2/%3.%4").arg(QDir::QDir::homePath()).arg(update_time).arg(d1_entries.at(i).fileName()).arg(syncFiles);
                    item = new QListWidgetItem (d1_entries.at(i).absoluteFilePath());
                    item->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/file.png")));
                    page->lw->addItem(item);
                    qApp->processEvents();
                    file->close();
                    syncFiles++; delete file;
                }
                else if (d1_entries.at(i).lastModified() > d2_entries.at(n).lastModified() && !d2_blacklist) {
					if (files_blacklist.contains(d2_entries.at(n).absoluteFilePath(), Qt::CaseInsensitive)) { found = true; continue; }
                    file = new QFile (d2_entries.at(n).absoluteFilePath());
                    if (!file->open(QIODevice::ReadOnly)) {
                       unknownError(page, d2_entries.at(n).fileName(), QString("file"), QString("open")); delete file; found = true; continue;
                    }
                    temp->mkpath(QString(".Synkron/%2").arg(update_time));
                    if (!(file->copy(QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d2_entries.at(n).fileName()).arg(syncFiles)))) {
                       unknownError(page, d2_entries.at(n).fileName(), QString("file"), QString("copy"), QString(" to temp")); delete file; found = true; continue;
                    }
                    file->remove();
                    file = new QFile (d1_entries.at(i).absoluteFilePath());
                    if (!file->open(QIODevice::ReadOnly)) {
                       unknownError(page, d1_entries.at(i).fileName(), QString("file"), QString("open")); delete file; found = true; continue;
                    }
                    buffer.clear(); buffer = d2_entries.at(n).absoluteFilePath();
                    if (!file->copy(buffer)) {
                       unknownError(page, d2_entries.at(n).fileName(), QString("file"), QString("copy")); delete file; found = true; continue;
                    }
                    synchronised << d2_entries.at(n).fileName() << update_time << d2_entries.at(n).absoluteFilePath();
                    synchronised << QString("%1/.Synkron/%2/%3.%4").arg(QDir::homePath()).arg(update_time).arg(d2_entries.at(n).fileName()).arg(syncFiles);
                    item = new QListWidgetItem (d2_entries.at(n).absoluteFilePath());
                    item->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/file.png")));
                    page->lw->addItem(item);
                    qApp->processEvents();
                    file->close();
                    syncFiles++; delete file;
                }
                found = true; break;
        }
        else if (d1_entries.at(i).fileName() == d2_entries.at(n).fileName() && repeated == true) { found = true; }
    }

        if (found == false && !d2_blacklist) {
            file = new QFile (d1_entries.at(i).absoluteFilePath());
            fileInfo.setFile(*file);
            if (!fileInfo.isDir()) {
                if (!file->open(QIODevice::ReadOnly)) {
                     unknownError(page, d1_entries.at(i).fileName(), QString("file"), QString("open")); delete file; continue;
                }
                
            }
            buffer.clear();
            buffer = QString("%1/%2").arg(d2.path()).arg(d1_entries.at(i).fileName());
            if (!file->copy(buffer)) {
                if (d1_entries.at(i).isDir()) {
                    if (!(d2.mkdir(d1_entries.at(i).fileName()))) {
                       unknownError(page, d1_entries.at(i).fileName(), QString("folder"), QString("copy")); delete file; continue;
                    }
                    QDir subDir1 (d1_entries.at(i).absoluteFilePath());
                    QDir subDir2 (QString("%1/%2").arg(d2.absolutePath()).arg(d1_entries.at(i).fileName()));
                    subSync(subDir1, subDir2, page, repeated, false, false);
                    item = new QListWidgetItem (d1_entries.at(i).absoluteFilePath());
                    item->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/folder_16.png")));
                    page->lw->addItem(item);
                }
                else {
                     unknownError(page, d1_entries.at(i).fileName(), QString("file"), QString("copy")); delete file; continue;
                }
            }
            else {
                 item = new QListWidgetItem (d1_entries.at(i).absoluteFilePath());
                 item->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/file.png")));
                 page->lw->addItem(item);
            }
            file->close();
            syncFiles++; delete file;
        }
    }
    if (!repeated) {
        subSync (d2, d1, page, true, d2_blacklist, d1_blacklist);
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

void MainWindow::unknownError(SyncPage * page, QString fileName, QString type, QString action, QString toTemp)
{
	QListWidgetItem *item = new QListWidgetItem (tr("Unknown error %1ing %2: %3%4").arg(action).arg(type).arg(fileName).arg(toTemp));
	item->setBackground(QBrush::QBrush(Qt::red));
	item->setForeground(QBrush::QBrush(Qt::white));
	if (type == "file") {
		item->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/file.png")));
	}
	else {
		item->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/folder_16.png")));
	}
	page->lw->addItem(item);
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    updateGeometry();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    QSettings settings ("Matus Tomlein", "Synkron");
    QSettings sync_settings(QSettings::IniFormat, QSettings::UserScope, "Matus Tomlein", "Synkron");
    QStringList recentsyncs;
    for (int i = 0; i<tabWidget->count(); i++) {
        recentsyncs << tabWidget->tabText(i);
        recentsyncs << tabs.value(tabWidget->widget(i))->sync_folder_1->text();
        recentsyncs << tabs.value(tabWidget->widget(i))->sync_folder_2->text();
        if (tabs.value(tabWidget->widget(i))->periodical_sync->isChecked()) recentsyncs << "checked";
        else recentsyncs << "unchecked";
        if (tabs.value(tabWidget->widget(i))->sync_hidden->isChecked()) recentsyncs << "checked";
        else recentsyncs << "unchecked";
        recentsyncs << QVariant(tabs.value(tabWidget->widget(i))->sync_interval->value()).toString();
    }
    sync_settings.setValue("recentsyncs", recentsyncs);
    sync_settings.setValue("synchronised", synchronised);
    sync_settings.setValue("files_blacklist", files_blacklist);
    sync_settings.setValue("folders_blacklist", folders_blacklist);
    settings.setValue("pos", this->pos());
    settings.setValue("size", this->size());
    trayIcon->hide();
}

void MainWindow::readSettings ()
{
	QSettings settings ("Matus Tomlein", "Synkron");
	QSettings sync_settings(QSettings::IniFormat, QSettings::UserScope, "Matus Tomlein", "Synkron");
    QStringList recentsyncs = sync_settings.value("recentsyncs").toStringList();
    QString name; QString sync1; QString sync2; bool periodical; bool hidden;
	if (recentsyncs.count() == 0 && settings.value("recentsyncs").toStringList().count() != 0) {
		recentsyncs = settings.value("recentsyncs").toStringList();
		for (int i = 0; i < recentsyncs.count(); ++i) {
       		if (i % 3 == 0 || i == 0) { name = recentsyncs.at(i); }
        	if (i % 3 == 1) { sync1 = recentsyncs.at(i); }
    		if (i % 3 == 2) { sync2 = recentsyncs.at(i); addTab(name, sync1, sync2, true, false, 5); }
    	}
	}
    else if (recentsyncs.count() == 0) { addTab(); return; }
    else {
		for (int i = 0; i < recentsyncs.count(); ++i) {
     		if (i % 6 == 0 || i == 0) { name = recentsyncs.at(i); }
    		if (i % 6 == 1) { sync1 = recentsyncs.at(i); }
    		if (i % 6 == 2) { sync2 = recentsyncs.at(i); }
    		if (i % 6 == 3) {
				if (recentsyncs.at(i)=="unchecked") periodical = false;
				else periodical = true;
			}
			if (i % 6 == 4) {
				if (recentsyncs.at(i)=="checked") hidden = true;
				else hidden = false;
			}
			if (i % 6 == 5) {
				addTab(name, sync1, sync2, periodical, hidden, recentsyncs.at(i).toInt());
			}
   		}
	}
    synchronised = sync_settings.value("synchronised").toStringList();
    files_blacklist = sync_settings.value("files_blacklist").toStringList();
	folders_blacklist = sync_settings.value("folders_blacklist").toStringList();
    this->move(settings.value("pos", this->pos()).toPoint());
	this->resize(settings.value("size", this->size()).toSize());
}

void MainWindow::updateGeometry()
{
    int h = statusBar()->height() + toolBar->height() + menubar->height();
    mainGridLayout->setGeometry(0, 0, this->geometry().width(), this->geometry().height()-h);
    tabWidget->setGeometry(0, 0, mainStackedWidget->width(), mainStackedWidget->height());
    restoreGridLayout->setGeometry(QRect(0, 0, mainStackedWidget->width(), mainStackedWidget->height()));
    blacklistGridLayout->setGeometry(QRect(0, 0, mainStackedWidget->width(), mainStackedWidget->height()));
}

void MainWindow::about ()
{
    About *about = new About;
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

void SyncPage::syncPage()
{
     emit sigsync(tab);
}

void MainWindow::toRestorePage()
{
	date_of_sync->setDateTime(QDateTime(QDate(2000, 01, 01), QTime(00, 00, 00)));
	path_of_syncfile->clear();
	to_black_list->setCheckState(Qt::Unchecked);
	//page->stacked_widget->setCurrentIndex(2);
 	QListWidgetItem * item;
 	restore_list->clear();
 	for (int i = 0; i < synchronised.count(); i+=4) {
		item = new QListWidgetItem (synchronised.at(i));
		item->setCheckState(Qt::Unchecked);
		restore_list->addItem(item);
		QStringList list;
		list << synchronised.at(i);
		list << synchronised.at(i+1);
		list << synchronised.at(i+2);
		list << synchronised.at(i+3);
		//item_temp.insert(item, synchronised.at(i+3));
		item->setData(Qt::UserRole, list);
		// QStringList list = item->data(Qt::UserRole).toStringList();
    }
}

void MainWindow::restoreItemChanged(QListWidgetItem * item, QListWidgetItem *) //yyyy.MM.dd-hh.mm
{
     if (!item) { return; }
     QStringList item_list = item->data(Qt::UserRole).toStringList();
     bool found = false;
     for (int i = 0; i < files_blacklist.count(); ++i) {
		if (files_blacklist.at(i) == item_list.at(2)) {
			to_black_list->setCheckState(Qt::Checked); found = true;
		}
	 }
	 if (!found) {
			to_black_list->setCheckState(Qt::Unchecked);
		}
	 //int k = (synchronised.indexOf(item_temp.value(page->restore_list->currentItem())))-3;
	 QStringList date_time = item_list.at(1).split("-");
	 QStringList date = date_time.at(0).split(".");
	 QStringList time = date_time.at(1).split(".");
	 date_of_sync->setDate(QDate(date.at(0).toInt(), date.at(1).toInt(), date.at(2).toInt()));
	 date_of_sync->setTime(QTime(time.at(0).toInt(), time.at(1).toInt(), time.at(2).toInt()));
	 path_of_syncfile->setText(item_list.at(2));
}

void MainWindow::restoreFiles()
{
	QFile * file; QFile * old_file;
	int restored_items = 0; int errors = 0; /*QStringList restored_items_list;*/ int restored [restore_list->count()];
	for (int c = 0; c < restore_list->count(); ++c) {
		if (restore_list->item(c)->checkState() == Qt::Checked) {
			QStringList item_list = restore_list->item(c)->data(Qt::UserRole).toStringList();
			file = new QFile (item_list.at(3));
			if (!file->open(QIODevice::ReadOnly)) { QMessageBox::critical(this, tr("Synkron"), tr("Unknown error opening file: %1.").arg(item_list.at(0))); errors++; delete file; continue; }
			old_file = new QFile (item_list.at(2));
			if (old_file->exists()) {
				if (!file->copy(QString("%1.res").arg(item_list.at(2)))) { QMessageBox::critical(this, tr("Synkron"), tr("Unknown error copying file: %1.").arg(item_list.at(0))); errors++; delete file; delete old_file; continue; }
				QString file_name = old_file->fileName();
				if (!old_file->remove()) { QMessageBox::critical(this, tr("Synkron"), tr("Unknown error copying file: %1.").arg(item_list.at(0))); errors++; delete file; delete old_file; continue; }
				delete file; file = new QFile (QString("%1.res").arg(item_list.at(2)));
				if (!file->rename(file_name)) { QMessageBox::critical(this, tr("Synkron"), tr("Unknown error copying file: %1.").arg(item_list.at(0))); errors++; delete file; delete old_file; continue; }
				delete file; file = new QFile (item_list.at(3));
			} else {
				if (!file->copy(item_list.at(2))) { QMessageBox::critical(this, tr("Synkron"), tr("Unknown error copying file: %1.").arg(item_list.at(0))); errors++; delete file; delete old_file; continue; }
			}
			file->remove();
			delete file; delete old_file;
			restored [restored_items] = restore_list->row(restore_list->item(c)); restored_items++;
			for (int o = 0; o < synchronised.count(); ++o) {
				if (synchronised.at(o) == item_list.at(3)) {
					synchronised.removeAt(o);
					synchronised.removeAt(o-1);
					synchronised.removeAt(o-2);
					synchronised.removeAt(o-3);
					break;
				}
			}
		}
	}
	for (int l = restored_items; l > 0; --l) {
		delete restore_list->item(restored[l-1]);
	}
	if (restored_items == 0 && errors == 0) {
		QMessageBox::warning(this, tr("Synkron"), tr("No files selected."));
	} else {
		QMessageBox::information(this, tr("Synkron"), tr("%1 files restored.").arg(restored_items));
	}
}

void MainWindow::addToBlackList(int state)
{
	if (!restore_list->currentItem()) { to_black_list->setCheckState(Qt::Unchecked); return; }
	QStringList item_list = restore_list->currentItem()->data(Qt::UserRole).toStringList();
	if (item_list.count() != 0) {
		if (state ==  Qt::Checked) {
			bool found = false;
			for (int i = 0; i < files_blacklist.count(); ++i) {
				if (files_blacklist.at(i) == item_list.at(2)) {
					found = true;
				}
			}
			if (!found) {
				files_blacklist << item_list.at(2);
			}
		}
		else if (state == Qt::Unchecked) {
			for (int i = files_blacklist.count(); i > 0; --i) {
				if (files_blacklist.at(i-1) == item_list.at(2)) {
					files_blacklist.removeAt(i-1);
				}
			}
		}
	}
}

void MainWindow::switchView(QAction * action)
{
	if (action == actionSynchronise) {
		mainStackedWidget->setCurrentIndex(0);
		actionNew_sync->setEnabled(true);
		actionClose_sync->setEnabled(true);
	}
	else if (action == actionRestore) {
		toRestorePage();
		actionNew_sync->setDisabled(true);
		actionClose_sync->setDisabled(true);
		mainStackedWidget->setCurrentIndex(1);
	}
	else if (action == actionBlacklist) {
		toBlacklist();
		actionNew_sync->setDisabled(true);
		actionClose_sync->setDisabled(true);
		mainStackedWidget->setCurrentIndex(2);
	}
}

void MainWindow::toBlacklist()
{
	blacklist_fileslist->clear(); blacklist_folderslist->clear();
	QListWidgetItem * item;
	for (int i = 0; i < files_blacklist.count(); ++i) {
		item = new QListWidgetItem(files_blacklist.at(i));
		item->setCheckState(Qt::Unchecked);
		blacklist_fileslist->addItem(item);
	}
	for (int i = 0; i < folders_blacklist.count(); ++i) {
		item = new QListWidgetItem(folders_blacklist.at(i));
		item->setCheckState(Qt::Unchecked);
		blacklist_folderslist->addItem(item);
	}
}

void MainWindow::addFileToBlacklist()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Choose a file"), QDir::homePath());
	if (file == 0) { return; }
    files_blacklist << file;
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(file);
    item->setCheckState(Qt::Unchecked);
    blacklist_fileslist->addItem(item);
}

void MainWindow::removeFileFromBlacklist()
{
	int removed = 0;
	for (int c = 0; c < blacklist_fileslist->count();) {
		if (blacklist_fileslist->item(c)->checkState() == Qt::Checked) {
			if (files_blacklist.contains(blacklist_fileslist->item(c)->text())) {
				files_blacklist.removeAt(files_blacklist.indexOf(blacklist_fileslist->item(c)->text()));
			}
			delete blacklist_fileslist->item(c);
			removed++;
		} else { c++; }
	}
	if (removed == 0) {
		QMessageBox::warning(this, tr("Synkron"), tr("No files selected."));
	}
	else {
		QMessageBox::information(this, tr("Synkron"), tr("%1 files removed.").arg(removed));
	}
}

void MainWindow::addFolderToBlacklist()
{
	QString folder = QFileDialog::getExistingDirectory(
                    this,
                    "Choose a directory",
                    QDir::homePath(),
                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (folder == 0) { return; }
    folders_blacklist << folder;
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(folder);
    item->setCheckState(Qt::Unchecked);
    blacklist_folderslist->addItem(item);
}

void MainWindow::removeFolderFromBlacklist()
{
	int removed = 0;
	for (int c = 0; c < blacklist_folderslist->count();) {
		if (blacklist_folderslist->item(c)->checkState() == Qt::Checked) {
			if (folders_blacklist.contains(blacklist_folderslist->item(c)->text())) {
				folders_blacklist.removeAt(folders_blacklist.indexOf(blacklist_folderslist->item(c)->text()));
			}
			delete blacklist_folderslist->item(c);
			removed++;
		} else { c++; }
	}
	if (removed == 0) {
		QMessageBox::warning(this, tr("Synkron"), tr("No folders selected."));
	}
	else {
		QMessageBox::information(this, tr("Synkron"), tr("%1 folders removed.").arg(removed));
	}
}

void MainWindow::tabNameEdited()
{
	SyncPage * page = tabs.value(tabWidget->currentWidget());
	tabWidget->setTabText(tabWidget->indexOf(page->tab), page->tab_name->text());
}

void MainWindow::delTmpAll()
{
	if (restore_list->count()==0) { QMessageBox::information(this, tr("Synkron"), tr("There are no temporary files.")); return; }
	QMessageBox msgBox; msgBox.setText(tr("Are you sure you want to delete all the temporary files?"));
	msgBox.setWindowTitle(QString("Synkron")); msgBox.setIcon(QMessageBox::Question);
 	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
 	switch (msgBox.exec()) {
 	case QMessageBox::Yes:
		QFile * file;
   		for (int i = restore_list->count()-1; i>=0; --i) {
			file = new QFile (restore_list->item(i)->data(Qt::UserRole).toStringList().at(3));
			if (file->exists()) {
				file->remove();
			}
			/*if (synchronised.contains(restore_list->item(i)->data(Qt::UserRole).toStringList().at(3))) {
				int n = synchronised.indexOf(restore_list->item(i)->data(Qt::UserRole).toStringList().at(3)) - 3;
				synchronised.removeAt(n); synchronised.removeAt(n+1); synchronised.removeAt(n+2); synchronised.removeAt(n+3);
			}*/
			delete restore_list->item(i);
		} synchronised.clear();
   		break;
 	case QMessageBox::No:
     	return;
	default:
   		break;
 	}
}

void MainWindow::delTmpSel()
{
	bool a = false;
	for (int i = restore_list->count()-1; i>=0; --i) {
		if (restore_list->item(i)->checkState() == Qt::Checked) {
			a = true;
		}
	}
	if (!a) { QMessageBox::information(this, tr("Synkron"), tr("No files selected.")); return; }
	QMessageBox msgBox; msgBox.setText(tr("Are you sure you want to delete the selected temporary files?"));
	msgBox.setWindowTitle(QString("Synkron")); msgBox.setIcon(QMessageBox::Question);
 	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No); 
 	switch (msgBox.exec()) {
 	case QMessageBox::Yes:
		QFile * file;
   		for (int i = restore_list->count()-1; i>=0; --i) {
			if (restore_list->item(i)->checkState() == Qt::Checked) {
				file = new QFile (restore_list->item(i)->data(Qt::UserRole).toStringList().at(3));
				if (file->exists()) {
					file->remove();
				}
				if (synchronised.contains(restore_list->item(i)->data(Qt::UserRole).toStringList().at(3))) {
					int n = synchronised.indexOf(restore_list->item(i)->data(Qt::UserRole).toStringList().at(3));
					synchronised.removeAt(n); synchronised.removeAt(n-1); synchronised.removeAt(n-2); synchronised.removeAt(n-3);
				}
				delete restore_list->item(i);
			}
		}
		if (synchronised.count() == 0) { synchronised.clear(); }
   		break;
 	case QMessageBox::No:
     	return;
	default:
   		break;
 	}
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow *window = new MainWindow;

    window->show();
    return app.exec();
}
