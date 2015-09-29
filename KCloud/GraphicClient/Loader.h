#ifndef LOADER_H
#define LOADER_H

#include <QTimer>
#include <QLabel>
#include <QThread>
#include <QDialog>
#include <QCloseEvent>
#include <QProgressBar>
#include "../Client/Client.h"

namespace Ui {
	class Loader;
}

using namespace KCloud;

class Loader : public QDialog{
		Q_OBJECT

	public:

		explicit Loader(QWidget *parent = 0);
		~Loader();
	protected:

		void	closeEvent(QCloseEvent *event);
	signals:
		void	trasmissionEnd();

	public slots:
		void	updateStatus(const qint64 &total, const qint64 &transmitted, const Resource::Mode mode);
		void	quit();

	private:
		Ui::Loader *	ui;
		bool			m_close;
		QLabel *		m_transferred;
		QLabel *		m_remaining;
		QLabel *		m_message;
		QProgressBar *	m_progressBar;
};

#endif // LOADER_H
