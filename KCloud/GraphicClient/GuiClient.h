#ifndef GUICLIENT_H
#define GUICLIENT_H

#include <QMainWindow>
#include <QTimer>
#include <QSettings>

#include "../Client/Client.h"

namespace Ui {
	class GuiClient;
}

class GuiClient : public QMainWindow{
		Q_OBJECT

	public:
		explicit GuiClient(QWidget *parent = 0);
		~GuiClient();

	private slots:
		void on_pushButton_clicked();

	private:
		Ui::GuiClient	*ui;
		KCloud::Client	*client;
};

#endif // GUICLIENT_H
