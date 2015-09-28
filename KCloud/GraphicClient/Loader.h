#ifndef LOADER_H
#define LOADER_H

#include <QTimer>
#include <QLabel>
#include <QDialog>
#include <QCloseEvent>
#include <QProgressBar>

#include "../Client/Client.h"

namespace Ui {
	class Loader;
}

using namespace KCloud;

class Loader : public QDialog
{
		Q_OBJECT

	public:
		enum Mode{
			Download,
			Upload
		};

		explicit Loader(Client * client, const Mode = Download, QWidget *parent = 0);
		~Loader();
	protected:

		void	closeEvent(QCloseEvent *event);

	private slots:
		void	setMessage();

	private:
		Ui::Loader *	ui;
		QLabel *		m_toLeftBytes;
		QLabel *		m_transferredBytes;
		QLabel *		m_message;
		QProgressBar *	m_progressBar;
		Client *		m_client;
		Mode			m_mode;
		int				m_spacchio;
};

#endif // LOADER_H
