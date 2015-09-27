#ifndef GUICLIENT_H
#define GUICLIENT_H

#include <QMainWindow>
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

	private:
		Ui::GuiClient	*ui;
		KCloud::Client	*client;
};

#endif // GUICLIENT_H
