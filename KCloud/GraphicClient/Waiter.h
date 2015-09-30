#ifndef WAITER_H
#define WAITER_H

#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>

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
		void	serverTimeout() const;

	public slots:
		void	waitForServer() const;
		void	quit();
		void	setMessage(const QString &message);
	public slots:

		void	show();
	protected:
		void	closeEvent(QCloseEvent * event);

	private slots:
		void	startLoader();
		void	panic();

	private:
		Ui::Waiter *				ui;
		QLabel *					m_loader;
		QLabel *					m_message;
		QMovie *					m_movie;
		QTimer *					m_timer;
		bool						m_close;
};

#endif // WAITER_H
