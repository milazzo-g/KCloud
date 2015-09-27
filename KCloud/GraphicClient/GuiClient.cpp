#include "GuiClient.h"
#include "ui_GuiClient.h"
#include "LoginForm.h"
#include "FirstConfigForm.h"
GuiClient::GuiClient(QWidget *parent) : QMainWindow(parent), ui(new Ui::GuiClient){

	ui->setupUi(this);

	FirstConfigForm f(this);

	f.exec();
}

GuiClient::~GuiClient()
{
	delete ui;
}
