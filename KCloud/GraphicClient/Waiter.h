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
		explicit Waiter(QWidget *parent = 0);

		~Waiter();
	signals:
		void	waitComplete() const;

	public slots:
		void	waitForServer() const;
		void	quit();
		void	setMessage(const QString &message);
	protected:
		void	closeEvent(QCloseEvent * event);

	private slots:
		void	startLoader();

	private:
		Ui::Waiter *				ui;
		QLabel *					m_loader;
		QLabel *					m_message;
		QMovie *					m_movie;
		bool						m_close;
};

#endif // WAITER_H
