#ifndef EXTENDEDLISTWIDGET_H
#define EXTENDEDLISTWIDGET_H

#include <QContextMenuEvent>
#include <QListWidget>

class ExtendedListWidget : public QListWidget
{
    Q_OBJECT

public:
    ExtendedListWidget(QWidget * parent = 0):
    QListWidget(parent) {};

signals:
    void sigconmenu(QPoint);

protected:
    void contextMenuEvent(QContextMenuEvent * event) {
        emit sigconmenu(event->globalPos());
    };
};

#endif // EXTENDEDLISTWIDGET_H
