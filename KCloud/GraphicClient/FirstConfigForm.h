#ifndef FIRSTCONFIGFORM_H
#define FIRSTCONFIGFORM_H

#include <QFile>
#include <QMovie>
#include <QDebug>
#include <QRegExp>
#include <QString>
#include <QDialog>
#include <QSettings>
#include <QTabWidget>
#include <QValidator>
#include <QPushButton>
#include <QTextStream>

namespace Ui {
	class FirstConfigForm;
}

class FirstConfigForm : public QDialog{
		Q_OBJECT

	public:
		explicit FirstConfigForm(QWidget *parent = 0);
		~FirstConfigForm();

	private slots:

		void on_acceptCheckBox_stateChanged(int arg1);

		void on_nextPageBtn_clicked();

		void on_serverAddressEdit_textChanged(const QString &arg1);

		void on_serverPortEdit_textChanged(const QString &arg1);

	private:
		Ui::FirstConfigForm *ui;
		QMovie				*m_movie;
		QString				m_address;
		quint16				m_port;
		QString				m_email;
		QString				m_hash1;
		QString				m_hash2;
};

#endif // FIRSTCONFIGFORM_H
