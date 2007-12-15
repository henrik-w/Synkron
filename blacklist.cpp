#include "main_window.h"

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
	if (file.isEmpty()) { return; }
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
	if (folder.isEmpty()) { return; }
	QDir dir(folder); folder = dir.path();
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
