#ifndef MTSTRINGSET_H
#define MTSTRINGSET_H

#include <QStringList>

class MTStringSet : public QStringList
{
public:
    MTStringSet(): QStringList() {};
    MTStringSet(const QString & str): QStringList(str) {};
    MTStringSet(const QStringList & other): QStringList(other) {};
    MTStringSet(const QList<QString> & other): QStringList(other) {};
    virtual ~MTStringSet() { this->QStringList::~QStringList(); };

    void append(const QString & str) {
        if (!contains(str)) { this->QStringList::append(str); }
    };

    inline QStringList & operator<<(const QString & str)
        { append(str); return *this; };
};

#endif // MTSTRINGSET_H
