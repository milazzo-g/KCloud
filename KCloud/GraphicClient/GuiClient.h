#ifndef GUICLIENT_H
#define GUICLIENT_H

#include <QMainWindow>
#include <QTimer>
#include <QSettings>
#include <QCloseEvent>
#include <QMap>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "../Client/Client.h"

#include "Waiter.h"
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

		void	onServerAnswer(const CommandPacket::ServerAnswer serv);

		void	on_mainTreeWidget_itemClicked(QTreeWidgetItem *item, int column);

		void	on_mainTreeWidget_itemSelectionChanged();

	private:
		Ui::GuiClient							*ui;
		bool									m_status;
		Client									*m_client;
		User									*m_user;
		QTreeWidget								*m_tree;
		QMap<quint64, GraphicResourceHeader *>	m_resourceMap;
		QGraphicsScene							*m_scene;

};

#endif // GUICLIENT_H
