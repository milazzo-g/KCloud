#include "GuiClient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName("KCloudClient");
	a.setApplicationVersion("1.0");
	a.setOrganizationName("Unikore");
	a.setOrganizationDomain("www.unikore.it");
	GuiClient w;
	w.show();

	return a.exec();
}

