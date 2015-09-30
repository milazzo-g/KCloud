#ifndef LOGINFORM_H
#define LOGINFORM_H

#include "../Client/Client.h"
#include "../MainServer/defines.h"
#include "../Exceptions/Exceptions.h"
#include "../CommandPacket/CommandPacket.h"

#include <QFont>
#include <QMovie>
#include <QLabel>
#include <QString>
#include <QDialog>
#include <QPixmap>
#include <QPalette>
#include <QLineEdit>
#include <QCheckBox>
#include <QSettings>
#include <QByteArray>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QCryptographicHash>
#include <QCloseEvent>
#include <QMessageBox>
#include <QTimer>
namespace Ui{

	class LoginForm;
}

using namespace KCloud;

class LoginForm : public QDialog{
		Q_OBJECT

	public:
		explicit			LoginForm(Client * client, QWidget *parent = 0);
							~LoginForm();

					bool	getResult() const;

	private slots:

		void onServerAnswer(const CommandPacket::ServerAnswer res);

		void onClientError(const Exception::Type extType);

		void onClientConnected();

		void onClientSocketError(const QAbstractSocket::SocketError err);

		void on_cancelBtn_clicked();

		void on_loginBtn_clicked();

		void on_emailEdit_textChanged(const QString &arg1);

		void on_passwordEdit_textChanged(const QString &arg1);

	private:
		Ui::LoginForm	*ui;
		bool			m_result;
		QMovie			*m_movie;
		QLabel			*m_message;
		QLabel			*m_loader;
		QCheckBox		*m_remember;
		QLineEdit		*m_email;
		QLineEdit		*m_password;
		QPushButton		*m_login;
		QPushButton		*m_canel;
		QGraphicsView	*m_graphics;
		QGraphicsScene	*m_scene;
		QPixmap			*m_pixmap;
		Client			*m_client;
		QString			m_resultMail;
		QString			m_resultHash;
};

#endif // LOGINFORM_H
