#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QProcess>
#include <QDateTime>

class MTFile : public QFile
{
public:
	MTFile(QString);
	MTFile(QString, QObject *);
	bool copy(QString);
};
