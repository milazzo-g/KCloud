#ifndef GUICLIENT_H
#define GUICLIENT_H

#include <QMap>
#include <QUrl>
#include <QFile>
#include <QTimer>
#include <QThread>
#include <QSettings>
#include <QListView>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QHeaderView>
#include <QTableWidget>
#include <QMediaPlayer>
#include <QProgressBar>
#include <QTemporaryDir>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "../Client/Client.h"

#include "Waiter.h"
#include "Loader.h"
#include "Settings.h"
#include "LoginForm.h"
#include "RenameForm.h"
#include "PermSettings.h"
#include "FirstConfigForm.h"
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

		void	waitForServer();

		void	refreshTree();
		void	requestTree();
		void	disableMain();
		void	restoreMain();
		void	restoreMainWithSound();
		void	showLoader();

		void onServerAnswer(const CommandPacket::ServerAnswer serv);

		void on_mainTreeWidget_itemClicked(QTreeWidgetItem *item, int column);

		void on_mainTreeWidget_itemSelectionChanged();

		void on_downloadButton_clicked();

		void on_uploadButton_clicked();

		void on_deleteButton_clicked();

		void on_refreshBtn_clicked();

		void on_modifyButton_clicked();

	private:
		Ui::GuiClient *							ui;
		bool									m_status;
		Client *								m_client;
		User *									m_user;
		QTreeWidget	*							m_tree;
		QMap<quint64, GraphicResourceHeader *>	m_resourceMap;
		QGraphicsScene *						m_scene;
		QGraphicsScene *						m_permScene;
		Loader *								m_loader;
		Waiter *								m_waiter;
		QMediaPlayer *							m_player;
		QTemporaryDir							m_dir;
		QTableWidget *							m_resourceInfoTable;
		QTableWidget *							m_permissionTable;
		QProgressBar *							m_userSpaceBar;
		QLabel *								m_freeSpace;
		QLabel *								m_busySpace;
		QLabel *								m_totalSpace;

		static const QString					PARSER;
};

#endif // GUICLIENT_H
