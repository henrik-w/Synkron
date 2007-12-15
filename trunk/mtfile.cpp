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
		if (QFileInfo(fileName()).lastModified() == QFileInfo(dest).lastModified())
		{ return true; } else { return false; }
	} else { return false; }
	return false;
}

#endif

bool MTFile::touch(QApplication * app)
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
