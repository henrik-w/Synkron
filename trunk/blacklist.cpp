#include "main_window.h"

void MainWindow::toBlacklist()
{
	blacklist_fileslist->clear(); blacklist_folderslist->clear();
	QListWidgetItem * item;
	for (int i = 0; i < files_blacklist.count(); ++i) {
		item = new QListWidgetItem(files_blacklist.at(i));
		blacklist_fileslist->addItem(item);
	}
	for (int i = 0; i < folders_blacklist.count(); ++i) {
		item = new QListWidgetItem(folders_blacklist.at(i));
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
    blacklist_fileslist->addItem(item);
}

void MainWindow::removeFileFromBlacklist()
{
	if (blacklist_fileslist->currentItem()==0) { QMessageBox::warning(this, tr("Synkron"), tr("No file selected.")); return; }
	QMessageBox msgBox; msgBox.setText(tr("Are you sure you want to remove the selected file from blacklist?"));
	msgBox.setWindowTitle(QString("Synkron")); msgBox.setIcon(QMessageBox::Question);
 	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
 	switch (msgBox.exec()) {
 	case QMessageBox::Yes:
		if (files_blacklist.contains(blacklist_fileslist->currentItem()->text())) {
				files_blacklist.removeAt(files_blacklist.indexOf(blacklist_fileslist->currentItem()->text()));
		}
		delete blacklist_fileslist->currentItem();
   		break;
 	case QMessageBox::No:
		break;
	default:
   		break;
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
    blacklist_folderslist->addItem(item);
}

void MainWindow::removeFolderFromBlacklist()
{
	if (blacklist_fileslist->currentItem()==0) { QMessageBox::warning(this, tr("Synkron"), tr("No folder selected.")); return; }
	QMessageBox msgBox; msgBox.setText(tr("Are you sure you want to remove the selected folder from blacklist?"));
	msgBox.setWindowTitle(QString("Synkron")); msgBox.setIcon(QMessageBox::Question);
 	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
 	switch (msgBox.exec()) {
 	case QMessageBox::Yes:
		if (folders_blacklist.contains(blacklist_folderslist->currentItem()->text())) {
				folders_blacklist.removeAt(folders_blacklist.indexOf(blacklist_folderslist->currentItem()->text()));
		}
		delete blacklist_folderslist->currentItem();
   		break;
 	case QMessageBox::No:
		break;
	default:
   		break;
 	}
}
