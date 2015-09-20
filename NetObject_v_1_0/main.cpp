#include <QCoreApplication>
#include <QDir>
#include "Resource.h"

using namespace KCloud;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	QDir dir;
	dir.mkdir("/home/giuseppe/Scrivania/pippo");
	Resource r("/home/giuseppe/Scrivania/UML/KCloud.vpp");
	r.setWorkingDir("/home/giuseppe/Scrivania/pippo");
	r.prepareForSend();
	FolderCompressor compress;
	compress.decompressFolder("/home/giuseppe/Scrivania/pippo/KCloud.vpp.kcomp", "/home/giuseppe/Scrivania/pippo/KCloud.vpp");

    return a.exec();
}

