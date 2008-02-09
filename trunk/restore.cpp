#include "main_window.h"

void MainWindow::toRestorePage()
{
	date_of_sync->setDateTime(QDateTime(QDate(2000, 01, 01), QTime(00, 00, 00)));
	path_of_syncfile->clear();
 	QListWidgetItem * item;
 	restore_list->clear();
 	QString last_time = "";
 	QString last_colour = "grey";
 	for (int i = 0; i < synchronised.count(); i+=4) {
		item = new QListWidgetItem (synchronised.at(i));
		item->setCheckState(Qt::Unchecked);
		QStringList list;
		if (synchronised.at(i+1) == last_time) {
            if (last_colour == "grey") {
                item->setBackground(QBrush(QColor(226, 226, 226)));
            }
        } else {
            last_time = synchronised.at(i+1);
            if (last_colour == "grey") {
                last_colour = "white";
            } else {
                item->setBackground(QBrush(QColor(226, 226, 226)));
                last_colour = "grey";
            }
        }
		list << synchronised.at(i);
		list << synchronised.at(i+1);
		list << synchronised.at(i+2);
		list << synchronised.at(i+3);
		item->setData(Qt::UserRole, list);
		item->setToolTip(synchronised.at(i+2));
		restore_list->addItem(item);
    }
    to_black_list->setCheckState(Qt::Unchecked);
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
	//MTFile * file; MTFile * old_file;
	int restored_items = 0; int errors = 0;// int restored [restore_list->count()];
	for (int c = restore_list->count()-1; c >= 0; --c) {
		if (restore_list->item(c)->checkState() == Qt::Checked) {
			if (restoreItem(restore_list->item(c))) restored_items++;
			else errors++;
		}
	}
	/*for (int l = restored_items; l > 0; --l) {
		delete restore_list->item(restored[l-1]);
	}*/
	if (restored_items == 0 && errors == 0) {
		QMessageBox::warning(this, tr("Synkron"), tr("No files selected."));
	} else {
		QMessageBox::information(this, tr("Synkron"), tr("%1 files restored.").arg(restored_items));
	}
}

void MainWindow::restoreCurrentItem()
{
	if (restore_list->currentItem()!=0) restoreItem(restore_list->currentItem());
}

bool MainWindow::restoreItem(QListWidgetItem * item)
{
	QStringList item_list = item->data(Qt::UserRole).toStringList();
	MTFile * file = new MTFile (item_list.at(3));
	if (!file->open(QIODevice::ReadOnly)) { QMessageBox::critical(this, tr("Synkron"), tr("Unknown error opening file: %1.").arg(item_list.at(0))); delete file; return false; }
	MTFile * old_file = new MTFile (item_list.at(2));
	if (old_file->exists()) {
		if (!file->copy(QString("%1.res").arg(item_list.at(2)))) { QMessageBox::critical(this, tr("Synkron"), tr("Unknown error copying file: %1.").arg(item_list.at(0))); delete file; delete old_file; return false; }
		QString file_name = old_file->fileName();
		if (!old_file->remove()) { QMessageBox::critical(this, tr("Synkron"), tr("Unknown error copying file: %1.").arg(item_list.at(0))); delete file; delete old_file; return false; }
		delete file; file = new MTFile (QString("%1.res").arg(item_list.at(2)));
		if (!file->rename(file_name)) { QMessageBox::critical(this, tr("Synkron"), tr("Unknown error copying file: %1.").arg(item_list.at(0))); delete file; delete old_file; return false; }
		delete file; file = new MTFile (item_list.at(3));
	} else {
		if (!file->copy(item_list.at(2))) { QMessageBox::critical(this, tr("Synkron"), tr("Unknown error copying file: %1.").arg(item_list.at(0))); delete file; delete old_file; return false; }
	}
	file->remove(); delete file;
	file = new MTFile (item_list.at(2));
	file->touch(qApp);
	delete file; delete old_file;
	//restored[restored_items] = restore_list->row(item);
	for (int o = 0; o < synchronised.count(); ++o) {
		if (synchronised.at(o) == item_list.at(3)) {
			synchronised.removeAt(o);
			synchronised.removeAt(o-1);
			synchronised.removeAt(o-2);
			synchronised.removeAt(o-3);
			break;
		}
	}
	delete item;
	return true;
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

void MainWindow::selTmpAll()
{
	for (int i = 0; i < restore_list->count(); ++i) {
		if (!restore_list->item(i)->isHidden()) {
		    restore_list->item(i)->setCheckState(Qt::Checked);
		}
	}
}

void MainWindow::setCleanGB()
{
    restore_clean_gb->setTitle(tr("Clean temporary files"));
    restore_clean_selected = new QCheckBox(restore_clean_gb);
    restore_clean_selected->setChecked(true);
    restore_clean_selected->setText(tr("Delete selected files only"));
    restore_clean_selected->setStatusTip(tr("Delete selected files only"));
    restore_clean_gb->addWidget(restore_clean_selected, 0, 0);
    
    QHBoxLayout * clean_date_layout = new QHBoxLayout (restore_clean_gb);
    restore_clean_by_date = new QCheckBox(restore_clean_gb);
    restore_clean_by_date->setChecked(false);
    restore_clean_by_date->setText(tr("Delete files older than "));
    restore_clean_by_date->setStatusTip(tr("Delete files older than"));
    clean_date_layout->addWidget(restore_clean_by_date);
    restore_clean_date = new QSpinBox (restore_clean_gb);
    //page->sync_interval->setStatusTip(tr("Set"));
    restore_clean_date->setMaximum(1440);
    restore_clean_date->setValue(30);
    restore_clean_date->setEnabled(false);
    clean_date_layout->addWidget(restore_clean_date);
    connect(restore_clean_by_date, SIGNAL(toggled(bool)), restore_clean_date, SLOT(setEnabled(bool)));
    QLabel * days_lbl = new QLabel (restore_clean_gb);
    days_lbl->setText(tr(" day(s)"));
    clean_date_layout->addWidget(days_lbl);
    clean_date_layout->addStretch();
    restore_clean_gb->addLayout(clean_date_layout, 1, 0);
    
    QHBoxLayout * clean_repeated_layout = new QHBoxLayout(restore_clean_gb);
    restore_clean_repeated = new QCheckBox(restore_clean_gb);
    restore_clean_repeated->setChecked(false);
    restore_clean_repeated->setText(tr("Delete older versions of files, keep "));
    restore_clean_repeated->setStatusTip(tr("Delete older versions of files"));
    clean_repeated_layout->addWidget(restore_clean_repeated);
    restore_clean_files = new QSpinBox (restore_clean_gb);
    //page->sync_interval->setStatusTip(tr("Set"));
    restore_clean_files->setMaximum(1440);
    restore_clean_files->setValue(30);
    restore_clean_files->setEnabled(false);
    connect(restore_clean_repeated, SIGNAL(toggled(bool)), restore_clean_files, SLOT(setEnabled(bool)));
    clean_repeated_layout->addWidget(restore_clean_files);
    QLabel * versions_lbl = new QLabel (restore_clean_gb);
    versions_lbl->setText(tr(" latest versions"));
    clean_repeated_layout->addWidget(versions_lbl);
    clean_repeated_layout->addStretch();
    restore_clean_gb->addLayout(clean_repeated_layout, 2, 0);
    
    QHBoxLayout * clean_layout = new QHBoxLayout (restore_clean_gb);
    clean_layout->addStretch();
    restore_clean = new QPushButton(restore_clean_gb);
    restore_clean->setText(tr("Clean"));
    restore_clean->setStatusTip(tr("Clean temporary files"));
    restore_clean->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/clean_temp_16.png")));
    connect(restore_clean, SIGNAL(released()), this, SLOT(cleanTemporary()));
    clean_layout->addWidget(restore_clean);
    restore_clean_gb->addLayout(clean_layout, 3, 0);
}

void MainWindow::cleanTemporary()
{
    if (restore_clean_selected->isChecked()) {
        bool a = false;
	    for (int i = restore_list->count()-1; i>=0; --i) {
	    	if (restore_list->item(i)->checkState() == Qt::Checked) {
	    		a = true;
	    	}
	    }
	    if (!a) { QMessageBox::information(this, tr("Synkron"), tr("No files selected.")); return; }
    }
    QMap<QString, int> repeated_map;
	QMessageBox msgBox; msgBox.setText(tr("Are you sure you want to delete temporary files?"));
	msgBox.setWindowTitle(QString("Synkron")); msgBox.setIcon(QMessageBox::Question);
 	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No); 
 	switch (msgBox.exec()) {
 	case QMessageBox::Yes:
 	    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		//MTFile * file;
   		for (int i = restore_list->count()-1; i>=0; --i) {
            if (restore_clean_selected->isChecked() && restore_list->item(i)->checkState() == Qt::Unchecked) continue;
            bool straight_delete = false; bool repeated_only = false;
            if (restore_clean_by_date->isChecked()) {
                QStringList date = restore_list->item(i)->data(Qt::UserRole).toStringList().at(1).split("-")
                                                                            .at(0).split(".");
                int days_to = QDate(date.at(0).toInt(), date.at(1).toInt(), date.at(2).toInt())
                                                                .daysTo(QDate::currentDate());
                if (days_to < restore_clean_date->value()) repeated_only = true;
                else straight_delete = true;
            }
            if (restore_clean_repeated->isChecked() && !straight_delete) {
                if (repeated_map.value(restore_list->item(i)->data(Qt::UserRole).toStringList().at(2)) >= restore_clean_files->value()) {
                    deleteTempFile(restore_list->item(i)->data(Qt::UserRole).toStringList().at(3));
					/*file = new MTFile (restore_list->item(i)->data(Qt::UserRole).toStringList().at(3));
                    if (file->exists()) {
                        file->remove();
                    }
                    if (synchronised.contains(restore_list->item(i)->data(Qt::UserRole).toStringList().at(3))) {
                        int x = synchronised.indexOf(restore_list->item(i)->data(Qt::UserRole).toStringList().at(3));
                        synchronised.removeAt(x); synchronised.removeAt(x-1); synchronised.removeAt(x-2); synchronised.removeAt(x-3);
                    }*/
                    delete restore_list->item(i);
                } else {
                    repeated_map.insert(restore_list->item(i)->data(Qt::UserRole).toStringList().at(2), repeated_map.value(restore_list->item(i)->data(Qt::UserRole).toStringList().at(2))+1);
                }
                continue;
            }
            if (!repeated_only) {
                deleteTempFile(restore_list->item(i)->data(Qt::UserRole).toStringList().at(3));
				/*file = new MTFile (restore_list->item(i)->data(Qt::UserRole).toStringList().at(3));
                if (file->exists()) {
                    file->remove();
                }
                if (synchronised.contains(restore_list->item(i)->data(Qt::UserRole).toStringList().at(3))) {
			    	int n = synchronised.indexOf(restore_list->item(i)->data(Qt::UserRole).toStringList().at(3));
			    	synchronised.removeAt(n); synchronised.removeAt(n-1); synchronised.removeAt(n-2); synchronised.removeAt(n-3);
                }*/
                delete restore_list->item(i);
            }
		}
		if (synchronised.count() == 0) { synchronised.clear(); }
   		QApplication::restoreOverrideCursor();
   		break;
 	case QMessageBox::No:
     	return;
	default:
   		break;
 	}
}

void MainWindow::deleteTempFile(QString path)
{
	MTFile * file = new MTFile (path);
    if (file->exists()) {
		file->remove();
		QFileInfo info(path);
		QDir dir = info.dir();
		QString dirname = dir.dirName();
		dir.cdUp();
		dir.rmdir(dirname);
    }
    if (synchronised.contains(path)) {
		int n = synchronised.indexOf(path);
		synchronised.removeAt(n); synchronised.removeAt(n-1); synchronised.removeAt(n-2); synchronised.removeAt(n-3);
    }
}

void MainWindow::deleteRestoreItem()
{
	if (restore_list->currentItem()==0) return;
	QMessageBox msgBox; msgBox.setText(tr("Are you sure you want to delete the selected temporary file?"));
	msgBox.setWindowTitle(QString("Synkron")); msgBox.setIcon(QMessageBox::Question);
 	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
 	switch (msgBox.exec()) {
 	case QMessageBox::Yes:
 	    deleteTempFile(restore_list->currentItem()->data(Qt::UserRole).toStringList().at(3));
    	delete restore_list->currentItem();
   		break;
 	case QMessageBox::No:
     	return;
	default:
   		break;
 	}
}

void MainWindow::checkRestoreItem()
{
	if (restore_list->currentItem()==0) return;
	restore_list->currentItem()->setCheckState(restore_list->currentItem()->checkState()==Qt::Checked ? Qt::Unchecked : Qt::Checked);
}

void MainWindow::blacklistRestoreItem()
{
	if (restore_list->currentItem()==0) return;
	to_black_list->setCheckState(Qt::Checked);
}

void MainWindow::restoreListConMenu(QPoint pos)
{
	QMenu * contextMenu = new QMenu(this);
	contextMenu->addAction(checkRestoreItemAction);
	contextMenu->addAction(restoreAction);
	contextMenu->addAction(deleteRestoreItemAction);
	contextMenu->addAction(blacklistRestoreItemAction);
	contextMenu->move(pos);
	contextMenu->show();
}
