#ifndef GUICLIENT_H
#define GUICLIENT_H

#include <QMainWindow>
#include <QSettings>

namespace Ui {
	class GuiClient;
}

class GuiClient : public QMainWindow
{
		Q_OBJECT

	public:
		explicit GuiClient(QWidget *parent = 0);
		~GuiClient();

	private:
		Ui::GuiClient *ui;

};

#endif // GUICLIENT_H
