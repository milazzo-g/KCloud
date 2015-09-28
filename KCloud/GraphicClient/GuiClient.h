#ifndef GUICLIENT_H
#define GUICLIENT_H

#include <QMainWindow>
#include <QTimer>
#include <QSettings>
#include <QCloseEvent>

#include "../Client/Client.h"

namespace Ui {
	class GuiClient;
}

using namespace KCloud;

class GuiClient : public QMainWindow{
		Q_OBJECT

	public:
		explicit GuiClient(QWidget *parent = 0);
		~GuiClient();

	protected:
		void		closeEvent(QCloseEvent *event);

	private slots:
		void on_pushButton_clicked();

	private:
		Ui::GuiClient	*ui;
		bool			m_status;
		Client			*m_client;
		User			*m_user;
};

#endif // GUICLIENT_H
