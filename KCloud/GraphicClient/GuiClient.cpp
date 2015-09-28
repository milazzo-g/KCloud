#include "GuiClient.h"
#include "LoginForm.h"
#include "ui_GuiClient.h"
#include "FirstConfigForm.h"

GuiClient::GuiClient(QWidget *parent) : QMainWindow(parent), ui(new Ui::GuiClient){

	QSettings appSettings;
	ui->setupUi(this);

	m_client	= new KCloud::Client(KCloud::Client::AsGuiThread, this);
	m_tree		= ui->mainTreeWidget;
	m_scene		= new QGraphicsScene(this);

	ui->graphicsView->setScene(m_scene);
	m_client->start();

	connect(m_client, SIGNAL(serverAnswer(CommandPacket::ServerAnswer)		),
			this	, SLOT	(onServerAnswer(CommandPacket::ServerAnswer)	));


	if(!appSettings.contains(T_STARTED)){
		FirstConfigForm f(m_client, this);
		f.exec();
		if(!f.getResult()){
			QTimer::singleShot(0, this, SLOT(close()));
		}
	}else{
		qint64 i = appSettings.value(T_STARTED).toLongLong();
		appSettings.setValue(T_STARTED, ++i);
	}

	LoginForm login(m_client, this);
	login.exec();
	if(!login.getResult()){
		QTimer::singleShot(0, this, SLOT(close()));
	}else{

		m_user = m_client->getSessionUser();
		m_client->resourceTree();

	}





}

GuiClient::~GuiClient(){
	delete ui;
}

void GuiClient::closeEvent(QCloseEvent *event){

	if(m_status){
		m_client->disconnectFromHost();
	}
	QMainWindow::closeEvent(event);
}

void GuiClient::copyResourceList(){

}

void GuiClient::refreshTree(){

	if(m_client->getResourceList().isEmpty()){
		return;
	}

	foreach (quint64 id, m_resourceMap.keys()) {
		delete m_resourceMap[id];
	}

	m_resourceMap.clear();

	bool flag = true;
	quint64 publicId;

	foreach (ResourceHeader head, m_client->getResourceList()) {

		if(m_resourceMap.contains(head.getParentId())){
			m_resourceMap.insert(head.getId(), new GraphicResourceHeader(head, m_resourceMap[head.getParentId()]));
		}else{
			if(head.getName() == m_user->getEmail()){
				m_resourceMap.insert(head.getId(), new GraphicResourceHeader(head, m_tree));
			}else{
				if(head.getPermission(m_user->getEmail()) != ResourceHeader::PermUndef){
					ResourceHeader tmp(head.getParentId(), this);
					tmp.setType(ResourceHeader::Dir);
					tmp.setName(head.getOwner());
					m_resourceMap.insert(tmp.getId(), new GraphicResourceHeader(tmp, m_tree, GraphicResourceHeader::OtherUser));
					m_resourceMap.insert(head.getId(), new GraphicResourceHeader(head, m_resourceMap[head.getParentId()]));
				}else{
					if(flag){
						ResourceHeader tmp(head.getParentId(), this);
						tmp.setType(ResourceHeader::Dir);
						tmp.setName("Pubblici");
						publicId = tmp.getId();
						flag = false;
						m_resourceMap.insert(tmp.getId(), new GraphicResourceHeader(tmp, m_tree, GraphicResourceHeader::Public));
					}
					m_resourceMap.insert(head.getId(), new GraphicResourceHeader(head, m_resourceMap[publicId]));
				}
			}
		}
	}
}

void GuiClient::onServerAnswer(const CommandPacket::ServerAnswer serv){

switch (serv) {
	case CommandPacket::ResourceTreeOk:

		refreshTree();
		break;
	default:
		break;
}

}

void GuiClient::on_pushButton_clicked(){

}

void GuiClient::on_mainTreeWidget_itemClicked(QTreeWidgetItem *item, int column){

	GraphicResourceHeader * tmp = reinterpret_cast<GraphicResourceHeader *>(item);
	m_scene->clear();
	m_scene->addPixmap(tmp->getImage());

}
