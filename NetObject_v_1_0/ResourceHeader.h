#ifndef RESOURCEHEADER_H
#define RESOURCEHEADER_H

#include <QObject>
#include <QString>
#include <QDataStream>

namespace KCloud{

    class ResourceHeader : public QObject{
        Q_OBJECT

    public:
        explicit ResourceHeader(QObject *parent = 0);

    signals:

    public slots:

    };
}

#endif // RESOURCEHEADER_H
