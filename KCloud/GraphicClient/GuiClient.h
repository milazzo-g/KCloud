#ifndef GUICLIENT_H
#define GUICLIENT_H

#include <QMap>
#include <QTimer>
#include <QThread>
#include <QSettings>
#include <QCloseEvent>
#include <QMainWindow>
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

		void	refreshTree();
		void	requestTree();
		void	finalize();

		void	stampacomeipazzi(const qint64 &total, const qint64 &transmitted);
		void	onServerAnswer(const CommandPacket::ServerAnswer serv);

		void	on_mainTreeWidget_itemClicked(QTreeWidgetItem *item, int column);

		void	on_mainTreeWidget_itemSelectionChanged();

		void	on_downloadButton_clicked();

	private:
		Ui::GuiClient							*ui;
		bool									m_status;
		Client									*m_client;
		User									*m_user;
		QTreeWidget								*m_tree;
		QMap<quint64, GraphicResourceHeader *>	m_resourceMap;
		QGraphicsScene							*m_scene;
		Loader *								m_loader;
};

#endif // GUICLIENT_H
