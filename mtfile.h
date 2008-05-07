/*******************************************************************
 This file is part of Synkron
 Copyright (C) 2005-2008 Matus Tomlein (matus.tomlein@gmail.com)

 Synkron is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public Licence
 as published by the Free Software Foundation; either version 2
 of the Licence, or (at your option) any later version.

 Synkron is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public Licence for more details.

 You should have received a copy of the GNU General Public Licence
 along with Synkron; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************/

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
	bool touch(QApplication *);
};

class MTEvenDateTime : public QDateTime
{
public:
    MTEvenDateTime(): QDateTime() { makeEven(); };
    MTEvenDateTime(const QDate & date): QDateTime(date) { makeEven(); };
    MTEvenDateTime(const QDate & date, const QTime & time, Qt::TimeSpec spec = Qt::LocalTime): QDateTime(date, time, spec) { makeEven(); };
    MTEvenDateTime(const QDateTime & other): QDateTime(other) { makeEven(); };

    void setTime(const QTime & time) { QDateTime::setTime(time); makeEven(); };
    void setTime_t(uint seconds) { QDateTime::setTime_t(seconds); makeEven(); };
protected:
    void makeEven() {
        if (time().second() % 2 != 0) { setTime(time().addSecs(1)); }
    };
};

class MTFileInfo : public QFileInfo
{
public:
    MTFileInfo(): QFileInfo() {};
    MTFileInfo(const QString & file): QFileInfo(file) {};
    MTFileInfo(const QFile & file): QFileInfo(file) {};
    MTFileInfo(const QDir & dir, const QString & file): QFileInfo(dir, file) {};
    MTFileInfo(const QFileInfo & fileinfo): QFileInfo(fileinfo) {};

    MTEvenDateTime lastModified() const { return MTEvenDateTime(QFileInfo::lastModified()); };
    MTEvenDateTime lastRead() const { return MTEvenDateTime(QFileInfo::lastRead()); };
};

#endif // MTFILE_H
