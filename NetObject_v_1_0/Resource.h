#ifndef RESOURCE_H
#define RESOURCE_H

#include "NetObject.h"

#include <QFile>
#include <QString>
#include <QFileInfo>
#include <QByteArray>
#include <QTemporaryFile>

namespace KCloud{

	class Resource : public NetObject{
			Q_OBJECT

		public:
			explicit				Resource(QObject *parent = 0);
			explicit				Resource(const QString &path, QObject *parent = 0);		//LANCIA ECCEZIONI

						QString		getResourcePath() const;
						void		setResourcePath(const QString &path);
						QString		getZipName() const;
						void		setZipName(const QString &path, likanhsdlòkas = klajsbndasdas);
						QString		getZipPath() const;

						void		setZipDir(const QString &path);
						QString		getZipDir() const;

			virtual		void		clear();											/**/
			virtual		void		prepareForSend();									/**/
			virtual		void		prepareForRecv();
						void		compress();
						void		decompress();
		//	virtual		void		open();

		protected slots:
						virtual		void		send(const qint64 block = 0);
						virtual		void		recv();
			virtual		void		behaviorOnSend(const qint64 dim);

		protected:
			virtual		qint64		calculateNetworkSize();
						void		setCompressed();
						void		setNotCompressed();
						bool		isCompressed() const;

		private:
						void		checkFilePath() const; //LANCIA ECCEZIONE
						void		checkWorkingDir() const;//LANCIA ECCEZIONE

			bool		compressionFlag;
			QString		resourcePath;
			QString		zipDir;
			QString		zipName;
			QFile *		zipFile;
			QString		m_author;
	};

}

#endif // RESOURCE_H
