#ifndef FIRSTCONFIGFORM_H
#define FIRSTCONFIGFORM_H

#include "../Client/Client.h"
#include "../Exceptions/Exceptions.h"
#include "../CommandPacket/CommandPacket.h"

#include <QFile>
#include <QMovie>
#include <QDebug>
#include <QLabel>
#include <QRegExp>
#include <QString>
#include <QDialog>
#include <QSettings>
#include <QTabWidget>
#include <QValidator>
#include <QPushButton>
#include <QTextStream>
#include <QAbstractSocket>
#include <QLineEdit>
#include <QCheckBox>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QDir>

namespace Ui {
	class FirstConfigForm;
}

using namespace KCloud;

class FirstConfigForm : public QDialog{
		Q_OBJECT

	public:
		explicit			FirstConfigForm(Client * client, QWidget *parent = 0);
							~FirstConfigForm();
					bool	getResult();

	private slots:



		void on_acceptCheckBox_stateChanged(int arg1);

		void on_goToConnectionButton_clicked();

		void on_serverAddressEdit_textChanged(const QString &arg1);

		void on_serverPortEdit_textChanged(const QString &arg1);

		void onServerAnswer(const CommandPacket::ServerAnswer res);

		void onClientError(const Exception::Type extType);

		void onClientConnected();

		void onClientDisconnected();

		void onClientSocketError(const QAbstractSocket::SocketError err);

		void on_goToRegisterButton_clicked();

		void on_alreadyRegistered_stateChanged(int arg1);

		void on_lineEdit_textChanged(const QString &arg1);

		void on_lineEdit_2_textChanged(const QString &arg1);

		void on_confirmPasswordEdit_textChanged(const QString &arg1);

		void on_goToPathConfig_clicked();

		void on_changePathButton_clicked();

		void on_goToEndButton_clicked();

		void on_endButton_clicked();

	private:
		Ui::FirstConfigForm *ui;
		QMovie				*m_movie;
		Client				*m_client;
		bool				m_result;
		QString				m_workPath;
		QString				m_address;
		quint16				m_port;
		QString				m_email;
		QString				m_hash1;
		QString				m_hash2;
		QTabWidget			*m_tabWidget;
		QPushButton			*m_goToConnection;
		QPushButton			*m_goToRegister;
		QPushButton			*m_goToPathConfig;
		QPushButton			*m_goToEnd;
		QPushButton			*m_end;
		QLabel				*m_connectionLoader;
		QLabel				*m_connectionMessage;
		QLabel				*m_registerLoader;
		QLabel				*m_registerMessage;
		QLabel				*m_pathLoader;
		QLabel				*m_pathMessage;
		QLabel				*m_confirmPasswordLabel;
		QLineEdit			*m_confirmPasswordEdit;
		QCheckBox			*m_alreadyRegistered;
};

#endif // FIRSTCONFIGFORM_H
