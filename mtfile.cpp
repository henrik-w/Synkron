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
	if (this->QFile::copy(dest)) {
	    QStringList arguments; QProcess touch;
		arguments << "-cf" << "-r" << fileName() << dest;
	    if (touch.execute("touch", arguments) != 0) { return false; }
		if (QFileInfo(fileName()).lastModified() == QFileInfo(dest).lastModified())
		{ return true; } else { return false; }
	} else { return false; }
	return false;
}

#endif
