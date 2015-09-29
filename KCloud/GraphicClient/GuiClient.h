#ifndef GUICLIENT_H
#define GUICLIENT_H

#include <QMap>
#include <QUrl>
#include <QFile>
#include <QTimer>
#include <QThread>
#include <QSettings>
#include <QCloseEvent>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QTemporaryDir>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "../Client/Client.h"

#include "Waiter.h"
#include "Loader.h"
#include "GraphicResourceHeader.h"

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

		void	waitResponse(const QString &message);
		void	waitTransmission();
		void	refreshTree();
		void	requestTree();
		void	disableMain();
		void	restoreMain();
		void	restoreMainWithSound();

		void	onServerAnswer(const CommandPacket::ServerAnswer serv);

		void	on_mainTreeWidget_itemClicked(QTreeWidgetItem *item, int column);

		void	on_mainTreeWidget_itemSelectionChanged();

		void	on_downloadButton_clicked();

	private:
		Ui::GuiClient *							ui;
		bool									m_status;
		Client *								m_client;
		User *									m_user;
		QTreeWidget	*							m_tree;
		QMap<quint64, GraphicResourceHeader *>	m_resourceMap;
		QGraphicsScene *						m_scene;
		Loader *								m_loader;
		Waiter *								m_waiter;
		QMediaPlayer *							m_player;
		QTemporaryDir							m_dir;
};

#endif // GUICLIENT_H
