#ifndef LOGINFORM_H
#define LOGINFORM_H

#include "../MainServer/defines.h"

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

namespace Ui{

	class LoginForm;
}

class LoginForm : public QDialog{
		Q_OBJECT

	public:
		explicit LoginForm(QWidget *parent = 0);
		~LoginForm();


	private slots:

		void			updateMail(const QString &mail);
		void			updateHash(const QString &hash);
		void			connectHost();

	private:
		Ui::LoginForm	*ui;
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

		QString			m_resultMail;
		QString			m_resultHash;
};

#endif // LOGINFORM_H
