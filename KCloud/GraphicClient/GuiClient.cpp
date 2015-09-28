#include "GuiClient.h"
#include "LoginForm.h"
#include "ui_GuiClient.h"
#include "FirstConfigForm.h"


GuiClient::GuiClient(QWidget *parent) : QMainWindow(parent), ui(new Ui::GuiClient){

	QSettings appSettings;
	ui->setupUi(this);

	m_client	= new Client(Client::AsGuiThread, this);
	m_tree		= ui->mainTreeWidget;
	m_scene		= new QGraphicsScene(this);
	m_loader	= new Loader(this);


	ui->graphicsView->setScene(m_scene);
	m_client->start();


	connect(m_client	, SIGNAL(serverAnswer(CommandPacket::ServerAnswer)		),
			this		, SLOT	(onServerAnswer(CommandPacket::ServerAnswer)	));

	connect(m_client	, SIGNAL(transmissionRate(qint64,qint64,Resource::Mode)	),
			m_loader	, SLOT	(updateStatus(qint64,qint64,Resource::Mode)		));

	connect(m_loader	, SIGNAL(trasmissionEnd()								),
			this		, SLOT	(finalize()										));



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
		QTimer::singleShot(0, this, SLOT(requestTree()));
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

		qDebug() << "head.getId() = " << head.getId();

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

void GuiClient::requestTree(){

	Waiter waiter(m_client, "Attendo l'albero delle risorse...", this);
	m_client->resourceTree();
	waiter.exec();
}

void GuiClient::finalize(){
	trace;
	Waiter(m_client, "Finalizzo", this).exec();
	this->setEnabled(true);
}

void GuiClient::stampacomeipazzi(const qint64 &total, const qint64 &transmitted){
	trace << " Total: " << total << " Transmitted: " << transmitted;
}

void GuiClient::onServerAnswer(const CommandPacket::ServerAnswer serv){

	trace << " serv =  " << (qint32)serv;

	if(serv == CommandPacket::ResourceTreeOk){
		refreshTree();
	}else if(serv == CommandPacket::ResourceDownOk){
		m_loader->show();
		this->setEnabled(false);
		m_loader->setEnabled(true);
	}
}

void GuiClient::on_mainTreeWidget_itemClicked(QTreeWidgetItem *item, int column){

	Q_UNUSED(column);
	GraphicResourceHeader * tmp = reinterpret_cast<GraphicResourceHeader *>(item);
	m_scene->clear();
	m_scene->addPixmap(tmp->getImage());


}


void GuiClient::on_mainTreeWidget_itemSelectionChanged(){
	on_mainTreeWidget_itemClicked(m_tree->currentItem(), 0);
}

void GuiClient::on_downloadButton_clicked(){

	if(m_tree->currentItem() == NULL){
		qDebug() << "ma che minchia ti hai messo in testa??";
		return;
	}
	quint64 id = reinterpret_cast<GraphicResourceHeader *>(m_tree->currentItem())->getId();
	m_client->newDownload(id);
	Waiter waiter(m_client, "Verifico se posso scaricare...", this);
	waiter.exec();
}
