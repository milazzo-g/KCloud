#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>
#include <QDataStream>
#include <QCryptographicHash>

namespace KCloud{

    class User : public QObject{
        Q_OBJECT

    public:
        explicit User(QObject *parent = 0);

    signals:

    public slots:

    private:
        bool        m_state;
        qint64      m_space;
        QString     m_email;
        QString     m_password;
    };

}

#endif // USER_H
