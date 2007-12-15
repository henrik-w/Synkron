#include "main_window.h"

void MainWindow::toRestorePage()
{
	date_of_sync->setDateTime(QDateTime(QDate(2000, 01, 01), QTime(00, 00, 00)));
	path_of_syncfile->clear();
	to_black_list->setCheckState(Qt::Unchecked);
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
		item->setData(Qt::UserRole, list);
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
	 QStringList date_time = item_list.at(1).split("-");
	 QStringList date = date_time.at(0).split(".");
	 QStringList time = date_time.at(1).split(".");
	 date_of_sync->setDate(QDate(date.at(0).toInt(), date.at(1).toInt(), date.at(2).toInt()));
	 date_of_sync->setTime(QTime(time.at(0).toInt(), time.at(1).toInt(), time.at(2).toInt()));
	 path_of_syncfile->setText(item_list.at(2));
}

void MainWindow::restoreFiles()
{
	MTFile * file; MTFile * old_file;
	int restored_items = 0; int errors = 0; int restored [restore_list->count()];
	for (int c = 0; c < restore_list->count(); ++c) {
		if (restore_list->item(c)->checkState() == Qt::Checked) {
			QStringList item_list = restore_list->item(c)->data(Qt::UserRole).toStringList();
			file = new MTFile (item_list.at(3));
			if (!file->open(QIODevice::ReadOnly)) { QMessageBox::critical(this, tr("Synkron"), tr("Unknown error opening file: %1.").arg(item_list.at(0))); errors++; delete file; continue; }
			old_file = new MTFile (item_list.at(2));
			if (old_file->exists()) {
				if (!file->copy(QString("%1.res").arg(item_list.at(2)))) { QMessageBox::critical(this, tr("Synkron"), tr("Unknown error copying file: %1.").arg(item_list.at(0))); errors++; delete file; delete old_file; continue; }
				QString file_name = old_file->fileName();
				if (!old_file->remove()) { QMessageBox::critical(this, tr("Synkron"), tr("Unknown error copying file: %1.").arg(item_list.at(0))); errors++; delete file; delete old_file; continue; }
				delete file; file = new MTFile (QString("%1.res").arg(item_list.at(2)));
				if (!file->rename(file_name)) { QMessageBox::critical(this, tr("Synkron"), tr("Unknown error copying file: %1.").arg(item_list.at(0))); errors++; delete file; delete old_file; continue; }
				delete file; file = new MTFile (item_list.at(3));
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


void MainWindow::restoreSearch(const QString text)
{
	for (int i = 0; i < restore_list->count(); ++i) {
		if (!restore_list->item(i)->text().contains(text, Qt::CaseInsensitive)) restore_list->item(i)->setHidden(true);
		else restore_list->item(i)->setHidden(false);
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

void MainWindow::delTmpAll()
{
	if (restore_list->count()==0) { QMessageBox::information(this, tr("Synkron"), tr("There are no temporary files.")); return; }
	QMessageBox msgBox; msgBox.setText(tr("Are you sure you want to delete all the temporary files?"));
	msgBox.setWindowTitle(QString("Synkron")); msgBox.setIcon(QMessageBox::Question);
 	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
 	switch (msgBox.exec()) {
 	case QMessageBox::Yes:
		MTFile * file;
   		for (int i = restore_list->count()-1; i>=0; --i) {
			file = new MTFile (restore_list->item(i)->data(Qt::UserRole).toStringList().at(3));
			if (file->exists()) {
				file->remove();
			}
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
		MTFile * file;
   		for (int i = restore_list->count()-1; i>=0; --i) {
			if (restore_list->item(i)->checkState() == Qt::Checked) {
				file = new MTFile (restore_list->item(i)->data(Qt::UserRole).toStringList().at(3));
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
