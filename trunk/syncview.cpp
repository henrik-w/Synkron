#include "main_window.h"

SyncViewItem::SyncViewItem(AbstractSyncPage * page)
{
	setupUi(this);
	
	connect(start_btn, SIGNAL(released()), this, SLOT(startSync()));
    connect(stop_btn, SIGNAL(released()), page, SLOT(stopSync()));
    connect(edit_btn, SIGNAL(released()), page, SLOT(showThisPage()));
    
    stop_btn->setEnabled(false);
    
    parent_page = page;
}

void MainWindow::toSyncView()
{
    refreshSyncs();
    refreshMultisyncs();
}

void MainWindow::refreshSyncs()
{
    for (int i = 0; i < syncs_syncview->rowCount();) {
        delete syncs_syncview->takeItem(i, 0);
        syncs_syncview->removeRow(i);
    }
    /*QMapIterator<SyncPage *, SyncViewItem *> i(sv_sync_items_map);
    switch (msgBox.exec()) {
    while (i.hasNext()) {
        delete sv_sync_items_map.take(i.value());
    }*/
    
    SyncViewItem * item;
    /*QMapIterator<QWidget *, SyncPage *> i(tabs);
    while (i.hasNext()) {*/
    SyncPage * page;
    for (int m = 0; m < tabWidget->count(); ++m) {
        page = tabs.value(tabWidget->widget(m));
        //i.next();
        item = new SyncViewItem (page);
        item->setName(page->tab_name->text());
        multisyncs_syncview->setRowHeight(m, item->sizeHint().height());
        //item->setMinimumSize(item->sizeHint());
        /*connect(item->start_btn, SIGNAL(released()), i.value(), SLOT(sync()));
        connect(item->stop_btn, SIGNAL(released()), i.value(), SLOT(stopSync()));*/
        //sv_sync_items_map.setValue(i.value(), item);
        syncs_syncview->setRowCount(syncs_syncview->rowCount() + 1);
        syncs_syncview->setCellWidget(syncs_syncview->rowCount() - 1, 0, item);
    }
}

void MainWindow::refreshMultisyncs()
{
    for (int i = 0; i < multisyncs_syncview->rowCount();) {
        delete multisyncs_syncview->takeItem(i, 0);
        multisyncs_syncview->removeRow(i);
    }
    /*QMapIterator<MultisyncPage *, SyncViewItem *> i(sv_multisync_items_map);
    switch (msgBox.exec()) {
    while (i.hasNext()) {
        delete sv_multisync_items_map.take(i.value());
    }*/
    
    SyncViewItem * item;
    MultisyncPage * page;
    for (int m = 0; m < multi_tabWidget->count(); ++m) {
        page = (MultisyncPage *) multi_tabWidget->widget(m);
        item = new SyncViewItem (page);
        item->setName(page->tab_name->text());
        //item->setMinimumSize(item->sizeHint());
        multisyncs_syncview->setRowHeight(m, item->sizeHint().height());
        //sv_multisync_items_map.setValue(page, item);
        multisyncs_syncview->insertRow(multisyncs_syncview->rowCount());
        multisyncs_syncview->setCellWidget(multisyncs_syncview->rowCount() - 1, 0, item);
    }
}

void SyncViewItem::startSync()
{
    start_btn->setEnabled(false);
    stop_btn->setEnabled(true);
    
    int synced_files = parent_page->sync();
    sync_name_lbl->setText(QString("%1<span style=\"color: rgb(128, 128, 128)\"> - %2 %3</span>")
            .arg(parent_page->tabNameText()).arg(synced_files).arg(tr("file(s) synchronised")));
    
    start_btn->setEnabled(true);
    stop_btn->setEnabled(false);
}
