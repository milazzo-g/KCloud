#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QRegExp>
#include <QSettings>
#include <QRegExpValidator>
#include <QFileDialog>

#include "../MainServer/defines.h"

namespace Ui {
	class Settings;
}

class Settings : public QDialog
{
		Q_OBJECT

	public:
		explicit				Settings(QWidget *parent = 0);
					bool		wasAccepted() const;
								~Settings();

	private slots:
		void on_changePath_clicked();

		void on_confirmBtn_clicked();

	private:
		Ui::Settings *	ui;
		bool			m_accepted;
};

#endif // SETTINGS_H
