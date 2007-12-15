#ifndef MTFILE_H
#define MTFILE_H

#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QProcess>
#include <QDateTime>
#include <QApplication>

class MTFile : public QFile
{
public:
	MTFile(QString);
	MTFile(QString, QObject *);
	bool copy(QString);
	bool touch(QApplication * = 0);
};

#endif // MTFILE_H
