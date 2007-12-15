#include "main_window.h"

void MainWindow::toMultisync()
{
	
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
	list_multi->addItem(item);
}

void MainWindow::removeMultisync()
{
	bool found = false;
	for (int i = list_multi->count()-1; i>=0; ++i) {
		if (list_multi->item(i)->checkState()==Qt::Checked) {
			found = true; break;
		}
	}
	if (!found) { QMessageBox::information(this, tr("Synkron"), tr("No files selected.")); return; }
	QMessageBox msgBox; msgBox.setText(tr("Are you sure you want to remove the selected directories from the list?"));
	msgBox.setWindowTitle(QString("Synkron")); msgBox.setIcon(QMessageBox::Question);
 	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No); 
 	switch (msgBox.exec()) {
 	case QMessageBox::Yes:
   		for (int i = list_multi->count()-1; i>=0; --i) {
			if (list_multi->item(i)->checkState() == Qt::Checked) {
				delete list_multi->item(i);
			}
		}
   		break;
 	case QMessageBox::No:
     	return;
	default:
   		break;
 	}
}

void MainWindow::browseMultiDestination()
{
	destination_multi->setText(QFileDialog::getExistingDirectory(
                this,
                "Choose a directory",
                QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}

void MainWindow::multisync()
{
	if (destination_multi->text()=="") { QMessageBox::information(this, tr("Synkron"), tr("Choose a destination first.")); return; }
	bool found = false;
	for (int n = 0; n < list_multi->count(); ++n) {
		if (list_multi->item(n)->checkState()==Qt::Checked) {
			found = true; break;
		}
	}
	if (!found) { QMessageBox::information(this, tr("Synkron"), tr("No files selected.")); return; }
	setMultisyncEnabled(false); QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QStringList pathlist; QString path;
	QDir destination; QDir syncfolder; syncFiles = 0;
	for (int i = 0; i < list_multi->count(); ++i) {
		if (list_multi->item(i)->checkState()==Qt::Unchecked) continue;
		destination.setPath(destination_multi->text());
		pathlist = list_multi->item(i)->text().split("/");
		QString s = pathlist.at(0);
		pathlist[0] = s.remove(":");
		for (int v = 0; v < pathlist.count(); ++v) {
			if (!destination.cd(pathlist.at(v))) {
				if (!destination.mkdir(pathlist.at(v))) { QMessageBox::critical(this, tr("Synkron"), tr("Error creating directory.")); setMultisyncEnabled(true); QApplication::restoreOverrideCursor(); return; }
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
		if ((!syncfolder.exists()) || (!destination.exists())) {
			addTableItem(0, tr("%1	Synchronisation failed: Directory not found").arg(QTime().currentTime().toString("hh:mm:ss")), "", "", QBrush(Qt::red), QBrush(Qt::white));
		}
    	int x = syncFiles;
		subSync(syncfolder, destination, 0, false);
		addTableItem(0, tr("%1	%2: %3 file(s) synchronised").arg(QTime().currentTime().toString("hh:mm:ss")).arg(list_multi->item(i)->text()).arg(syncFiles-x), "", "", QBrush(Qt::green));
	}
	saveSettings(); setMultisyncEnabled(true); QApplication::restoreOverrideCursor();
	trayIcon->showMessage(tr("Synchronisation complete"), tr("%1 files synchronised").arg(syncFiles)); syncFiles = 0;
}

void MainWindow::saveMultisync()
{
	QSettings sync_settings(QSettings::IniFormat, QSettings::UserScope, "Matus Tomlein", "Synkron");
	if (sync_settings.value("slist_path").toString() == "") {
		saveAsMultisync(); return;
	}
	saveAsMultisync(sync_settings.value("slist_path").toString());
}

void MainWindow::saveAsMultisync()
{
	QString file_name = QFileDialog::getSaveFileName(this,
                tr("Synkron - Save Multisync"),
                QString("%1/multisync.slist").arg(destination_multi->text()),
                tr("Synkron Multisyncs (*.slist)"));
    saveAsMultisync(file_name);
}

void MainWindow::saveAsMultisync(QString file_name)
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
    QFile file(file_name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::critical(this, tr("Synkron"), tr("Cannot write file %1:\n%2.").arg(file_name).arg(file.errorString()));
		return;
	}
	QTextStream out(&file);
	out << domdoc.toString();
	QSettings sync_settings(QSettings::IniFormat, QSettings::UserScope, "Matus Tomlein", "Synkron");
	sync_settings.setValue("slist_path", file_name);
}

void MainWindow::loadMultisync()
{
	QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                QDir::homePath(),
                                                tr("Synkron Multisyncs (*.slist)"));
    loadMultisync(file_name);
}

void MainWindow::loadMultisync(QString file_name)
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
    if (source_list.count()!=0) list_multi->clear();
    QListWidgetItem * item;
	for (int i = 0; i < source_list.count(); ++i) {
		item = new QListWidgetItem (source_list.at(i));
		item->setCheckState(el_sources.attribute("checkstates").at(i) == '+' ? Qt::Checked : Qt::Unchecked);
		list_multi->addItem(item);
	}
	QDomElement el_destination = el_sync.firstChildElement("destination");
	if (el_destination.attribute("data")!="") {
		destination_multi->setText(el_destination.attribute("data"));
	}
	QSettings sync_settings(QSettings::IniFormat, QSettings::UserScope, "Matus Tomlein", "Synkron");
	sync_settings.setValue("slist_path", file_name);
}

void MainWindow::setMultisyncEnabled(bool enable)
{
	list_multi->setEnabled(enable);
	add_multi->setEnabled(enable);
	remove_multi->setEnabled(enable);
	destination_multi->setEnabled(enable);
	browse_multi->setEnabled(enable);
	load_multi->setEnabled(enable);
	saveas_multi->setEnabled(enable);
	save_multi->setEnabled(enable);
	sync_multi->setEnabled(enable);
}
