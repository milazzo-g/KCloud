#ifndef WAITER_H
#define WAITER_H

#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QDialog>
#include <QCloseEvent>

#include "../Client/Client.h"


namespace Ui {
	class Waiter;
}

using namespace KCloud;

class Waiter : public QDialog{
		Q_OBJECT

	public:
		explicit Waiter(Client * client, const QString &message, QWidget *parent = 0);

		~Waiter();
	protected:
		void	closeEvent(QCloseEvent * event);

	private slots:
		void	restoreClose();
		void	startLoader();

	private:
		Ui::Waiter *				ui;
		Client *					m_client;
		QLabel *					m_loader;
		QLabel *					m_message;
		QMovie *					m_movie;
		bool						m_close;
};

#endif // WAITER_H
