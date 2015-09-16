//#ifndef CLIENT_H
//#define CLIENT_H

//#include <QCoreApplication>
//#include <QTextStream>
//#include <QAbstractSocket>
//#include <QObject>
//#include <QThread>
//#include <QStringList>

//#include "netobject.h"

//class ClientServerApplication : public QThread{
//		Q_OBJECT
//	public:
//		ClientServerApplication(QObject *parent);

//	signals:
//		void dataReceived();
//		void dataSended();

//	public slots:
//		void sendFile(const QString path);
//		void receiveFile(const QString path);
//		//void receive
//	private:
//};

//class Client : public ClientServerApplication{
//		Q_OBJECT
//	public:
//		Client(QObject *parent = 0);

//	signals:

//	public slots:

//	private:

//};

//class Server : public ClientServerApplication{
//		Q_OBJECT
//	public:
//		Server(QObject *parent = 0);

//	signals:

//	public slots:

//	private:
//};

//class WorkDispatcher : public QThread{
//	Q_OBJECT
//	public:
//		WorkDispatcher(QObject * parent = 0);
//	protected:
//		void run();
//	private:
//		enum Mode{
//			Client,
//			Server
//		};
//		Mode						workMode;
//		QTextStream					consoleOut;
//		QTextStream					consoleIn;
//		QCoreApplication *			coreApplication;
//		ClientServerApplication *	frontEndApplication;

//};

//#endif // CLIENT_H
