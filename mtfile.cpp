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

#include "mtfile.h"

MTFile::MTFile(QString s):
QFile(s)
{}

MTFile::MTFile(QString s, QObject * o):
QFile(s, o)
{}

#ifndef USE_UNIX_TOUCH_COMMAND
#ifndef Q_WS_MAC

bool MTFile::copy(QString dest)
{
    return this->QFile::copy(dest);
}

#endif
#else

bool MTFile::copy(QString dest)
{
	bool ok = false;
	if (QFile::symLinkTarget(fileName()).isEmpty()) { ok = this->QFile::copy(dest); }
	else {
		QStringList arguments; QProcess cp;
		arguments << "-R" << fileName() << dest;
		ok = cp.execute("cp", arguments) == 0;
	}
	if (ok) {
		QStringList arguments; QProcess touch;
		arguments << "-cf" << "-r" << fileName() << dest;
		if (touch.execute("touch", arguments) != 0) { return false; }
		return QFileInfo(fileName()).lastModified() == QFileInfo(dest).lastModified();
	} else { return false; }
	return false;
}

#endif

bool MTFile::touch(QApplication *
#ifdef Q_WS_WIN
    app
#endif
    )
{
    QStringList arguments; QProcess touch;
#ifdef Q_WS_WIN
    touch.setWorkingDirectory(QFileInfo(app->arguments().at(0)).absolutePath());
#endif
	arguments << "-cmf" << fileName();
	if (touch.execute("touch", arguments) == 0) {
		return true;
	} else { return false; }
	return false;
}
