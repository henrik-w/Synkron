#include "main_window.h"

void MainWindow::addSchedule()
{
    addSchedule(QStringList(), QStringList(), QStringList());
    tw_schedules->setCurrentCell(tw_schedules->rowCount() - 1, 0);
}

QTableWidgetItem * MainWindow::addSchedule(QStringList tabs, QStringList times, QStringList checked_times)
{
	QTableWidgetItem * item = new QTableWidgetItem;
	setScheduleStatusOn(false, item);
	tw_schedules->insertRow(tw_schedules->rowCount());
	tw_schedules->setRowHeight(tw_schedules->rowCount() - 1, 16);
	tw_schedules->setItem(tw_schedules->rowCount() - 1, 1, item);
	item = new QTableWidgetItem;
	item->setText("Untitled schedule");
	tw_schedules->setItem(tw_schedules->rowCount() - 1, 0, item);
	SyncSchedule * schedule = new SyncSchedule (this);
	if (tabs.count()!=0) schedule->sched_tab_list = tabs;
	if (times.count()!=0) schedule->sched_time_list = times;
	if (checked_times.count()!=0) schedule->sched_checked_time_list = checked_times;
	schedule->scheduling = false;
	QObject::connect(schedule, SIGNAL(sigsched(QWidget *)), this, SLOT(sync(QWidget *)));
	item_sched_map[item] = schedule;
	return item;
}

void MainWindow::removeSchedule()
{
	if(tw_schedules->currentItem()==0) { QMessageBox::warning(this, tr("Synkron"), tr("No schedule selected.")); return; }
	int m = tw_schedules->currentRow(); tw_schedules->clearSelection(); schedGroupBox->setEnabled(false); sched_removed = true;
	QMessageBox msgBox; msgBox.setText(tr("Are you sure you want to remove the selected schedule from the list?"));
	msgBox.setWindowTitle(QString("Synkron")); msgBox.setIcon(QMessageBox::Question);
 	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
 	switch (msgBox.exec()) {
 	case QMessageBox::Yes:
		if (item_sched_map.value(tw_schedules->item(tw_schedules->currentRow(), 0))->scheduling==true) break;
		delete item_sched_map.value(tw_schedules->item(tw_schedules->currentRow(), 0));
		item_sched_map.remove(tw_schedules->item(m, 0));
		tw_schedules->removeRow(m);
   		break;
 	case QMessageBox::No:
     	break;
	default:
   		break;
 	} sched_removed = false;
 	if (tw_schedules->rowCount()!=0) { 
		if (tw_schedules->currentRow()==0) scheduleActivated(0, 0, 0, 0);
		tw_schedules->setCurrentCell(0, 0);
		schedGroupBox->setEnabled(true);
	}
}

void MainWindow::scheduleActivated(int, int, int previousRow, int)
{
	if (sched_removed) return;
	if (schedGroupBox->isEnabled()) saveSchedSettings(previousRow);
	activateSchedule();
}

void MainWindow::activateSchedule()
{
	int row = tw_schedules->currentRow();
	schedGroupBox->setEnabled(true);
	SyncSchedule * schedule = item_sched_map.value(tw_schedules->item(row, 0));
	sched_name->setText(tw_schedules->item(row, 0)->text());
	sched_tab_lw->clear();
	sched_time_lw->clear();
	QListWidgetItem * litem;
	QMapIterator<QWidget *, SyncPage *> i(tabs);
	while (i.hasNext()) {
		i.next();
		litem = new QListWidgetItem (i.value()->tab_name->text());
		if (schedule->sched_tab_list.contains(i.value()->tab_name->text())) litem->setCheckState(Qt::Checked);
		else litem->setCheckState(Qt::Unchecked);
		sched_tab_lw->addItem(litem);
	}
	for (int n = 0; n < schedule->sched_time_list.count(); ++n) {
		litem = new QListWidgetItem(schedule->sched_time_list.at(n));
		if (schedule->sched_checked_time_list.contains(schedule->sched_time_list.at(n))) litem->setCheckState(Qt::Checked);
		else litem->setCheckState(Qt::Unchecked);
		sched_time_lw->addItem(litem);
	}
	enableSchedule(row);
	setScheduleStatusOn(schedule->scheduling, tw_schedules->item(row, 1));
}

void MainWindow::addSchedTime()
{
	SyncSchedule * schedule = item_sched_map.value(tw_schedules->item(tw_schedules->currentRow(), 0));
	if (schedule->sched_time_list.contains(sched_time_edit->time().toString("H:mm"))) return;
	QListWidgetItem * item = new QListWidgetItem (sched_time_edit->time().toString("H:mm"));
	item->setCheckState(Qt::Checked);
	sched_time_lw->addItem(item);
	schedule->sched_time_list << sched_time_edit->time().toString("H:mm");
	schedule->sched_checked_time_list << sched_time_edit->time().toString("H:mm");
}

void MainWindow::removeSchedTime()
{
	if (sched_time_lw->currentItem()==0) { QMessageBox::warning(this, tr("Synkron"), tr("No sync time selected.")); return; }
	SyncSchedule * schedule;
	QMessageBox msgBox; msgBox.setText(tr("Are you sure you want to remove the selected sync time from the list?"));
	msgBox.setWindowTitle(QString("Synkron")); msgBox.setIcon(QMessageBox::Question);
 	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
 	switch (msgBox.exec()) {
 	case QMessageBox::Yes:
		schedule = item_sched_map.value(tw_schedules->item(tw_schedules->currentRow(), 0));
		if (schedule->sched_time_list.contains(sched_time_lw->currentItem()->text())) {
			schedule->sched_time_list.removeAt(schedule->sched_time_list.indexOf(sched_time_lw->currentItem()->text()));
		}
		delete sched_time_lw->currentItem();
   		break;
 	case QMessageBox::No:
		break;
	default:
   		break;
 	}
}

void MainWindow::schedTabClicked(QListWidgetItem * item)
{
	SyncSchedule * schedule = item_sched_map.value(tw_schedules->item(tw_schedules->currentRow(), 0));
	if (item->checkState()==Qt::Checked) {
		if (!schedule->sched_tab_list.contains(item->text())) {
			schedule->sched_tab_list << item->text();
		}
	} else {
		if (schedule->sched_tab_list.contains(item->text())) {
			schedule->sched_tab_list.removeAll(item->text());
		}
	}
}

void MainWindow::schedTimeClicked(QListWidgetItem * item)
{
	SyncSchedule * schedule = item_sched_map.value(tw_schedules->item(tw_schedules->currentRow(), 0));
	if (item->checkState()==Qt::Checked) {
		if (!schedule->sched_checked_time_list.contains(item->text())) {
			schedule->sched_checked_time_list << item->text();
		}
	} else {
		if (schedule->sched_checked_time_list.contains(item->text())) {
			schedule->sched_checked_time_list.removeAll(item->text());
		}
	}
}

void MainWindow::saveSchedSettings(int row)
{
	SyncSchedule * schedule = item_sched_map.value(tw_schedules->item(row, 0));
	schedule->sched_tab_list.clear();
	for (int i = 0; i < sched_tab_lw->count(); ++i) {
		if (sched_tab_lw->item(i)->checkState()!=Qt::Checked) continue;
		schedule->sched_tab_list << sched_tab_lw->item(i)->text();
	}
	schedule->sched_time_list.clear();
	schedule->sched_checked_time_list.clear();
	for (int i = 0; i < sched_time_lw->count(); ++i) {
		if (sched_time_lw->item(i)->checkState()!=Qt::Checked) {
			schedule->sched_time_list << sched_time_lw->item(i)->text();
		} else {
			schedule->sched_time_list << sched_time_lw->item(i)->text();
			schedule->sched_checked_time_list << sched_time_lw->item(i)->text();
		}
	}
}

void MainWindow::setScheduleStatusOn(bool on, QTableWidgetItem * item)
{
	if (!on) {
		item->setText(tr("Off"));
		item->setBackground(Qt::red);
		item->setForeground(Qt::white);
	} else {
		item->setText(tr("On"));
		item->setBackground(Qt::darkGreen);
		item->setForeground(Qt::white);
	}
}

void MainWindow::startSchedule(QTableWidgetItem * item)
{
	if (tw_schedules->currentItem()==item) saveSchedSettings(item->row());
	SyncSchedule * schedule = item_sched_map.value(item);
	if (schedule->sched_tab_list.count()==0) return;
	for (int t = 0; t < schedule->timers.count(); ++t) {
		schedule->timers.at(t)->stop();
		delete schedule->timers.at(t);
	} schedule->timers.clear(); QTime time = QTime::currentTime(); QTimer * timer; bool scheduled = false;
	QDateTime current_datetime = QDateTime::currentDateTime();
	for (int r = 0; r < schedule->sched_checked_time_list.count(); ++r) {
		timer = new QTimer(this);
		QObject::connect(timer, SIGNAL(timeout()), timer, SLOT(stop()));
		QObject::connect(timer, SIGNAL(timeout()), schedule, SLOT(syncSchedule()));
		QObject::connect(timer, SIGNAL(timeout()), schedule, SLOT(checkSchedStatus()));
		int h = time.msecsTo(QTime(schedule->sched_checked_time_list.at(r).split(':').at(0).toInt(), schedule->sched_checked_time_list.at(r).split(':').at(1).toInt()));
		if (h<0) {
			QDateTime datetime (QDate(QDateTime::currentDateTime().date().year(), QDateTime::currentDateTime().date().month(), QDateTime::currentDateTime().date().day()+1),
						QTime(schedule->sched_checked_time_list.at(r).split(':').at(0).toInt(), schedule->sched_checked_time_list.at(r).split(':').at(1).toInt()));
			h = current_datetime.secsTo(datetime)*1000;
		} if (h<0) { delete timer; continue; }
		timer->start(h); scheduled = true;
		schedule->timers << timer;
	}
	if (scheduled) {
		schedule->scheduling = true;
		setScheduleStatusOn(true, tw_schedules->item(item->row(), 1));
		if (tw_schedules->currentIndex().isValid()) enableSchedule(tw_schedules->currentRow());
	} else schedule->scheduling = false;
}

void MainWindow::stopSchedule(QTableWidgetItem * item)
{
	SyncSchedule * schedule = item_sched_map.value(item);
	for (int t = 0; t < schedule->timers.count(); ++t) {
		schedule->timers.at(t)->stop();
		delete schedule->timers.at(t);
	} schedule->timers.clear();
	schedule->scheduling = false;
	if (tw_schedules->currentIndex().isValid()) enableSchedule(tw_schedules->currentRow());
	setScheduleStatusOn(false, tw_schedules->item(item->row(), 1));
}

void MainWindow::startAllSchedules()
{
	for (int i = 0; i < tw_schedules->rowCount(); ++i) {
		startSchedule(tw_schedules->item(i, 0));
	}
}

void MainWindow::stopAllSchedules()
{
	for (int i = 0; i < tw_schedules->rowCount(); ++i) {
		stopSchedule(tw_schedules->item(i, 0));
	}
}

SyncSchedule::SyncSchedule(MainWindow * parent)
{
	sched_parent = parent;
}

void SyncSchedule::syncSchedule()
{
	for (int i = 0; i < sched_tab_list.count(); ++i) {
		for (int x = 0; x < sched_parent->tabWidget->count(); ++x) {
			if (sched_parent->tabWidget->tabText(x)==sched_tab_list.at(i)) {
				emit sigsched(sched_parent->tabWidget->widget(x));
			}
		}
	}
}

void SyncSchedule::checkSchedStatus()
{
	for (int t = 0; t < sched_checked_time_list.count(); ++t) {
		if (QTime(sched_checked_time_list.at(t).split(':').at(0).toInt(), sched_checked_time_list.at(t).split(':').at(1).toInt())>QTime::currentTime()) return;
	} 
	for (int i = 0; i < timers.count(); ++i) {
		delete timers.at(i);
	}
	timers.clear();
	sched_parent->startSchedule(sched_parent->item_sched_map.key(this));
	sched_parent->reloadSchedStatus();
}

void MainWindow::reloadSchedStatus()
{
	QMapIterator<QTableWidgetItem*, SyncSchedule*> i(item_sched_map);
 	while (i.hasNext()) {
    	i.next();
    	if (i.value()->scheduling==false) {
			setScheduleStatusOn(false, tw_schedules->item(i.key()->row(), 1));
		}
	}
	if (tw_schedules->currentIndex().isValid()) enableSchedule(tw_schedules->currentRow());
}

void MainWindow::enableSchedule(int row)
{
	SyncSchedule * schedule = item_sched_map.value(tw_schedules->item(row, 0));
	sched_name->setDisabled(schedule->scheduling);
	sched_tab_lw->setDisabled(schedule->scheduling);
	sched_time_lw->setDisabled(schedule->scheduling);
	sched_time_edit->setDisabled(schedule->scheduling);
	sched_add_time->setDisabled(schedule->scheduling);
	sched_remove_time->setDisabled(schedule->scheduling);
	sched_start->setDisabled(schedule->scheduling);
	sched_stop->setEnabled(schedule->scheduling);
}
