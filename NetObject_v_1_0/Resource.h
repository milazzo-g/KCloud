#ifndef RESOURCE_H
#define RESOURCE_H

#include "NetObject.h"
#include "JlCompress.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QFileInfo>
#include <QByteArray>
#include <QDataStream>
#include <QTemporaryFile>

namespace KCloud{

	class Resource : public NetObject{
			Q_OBJECT

		public:
			explicit				Resource(QObject *parent = 0);
									Resource(const QString &path, QObject *parent = 0);		//LANCIA ECCEZIONI

						QString		getResourcePath() const;
						QString		getZipName() const;
						QString		getZipPath() const;
						void		setResourcePath(const QString &path);
						void		setZipName(const QString &path, const QString &ext = "");

						void		setZipDir(const QString &path);							// LANCIA ECCEZIONE
						QString		getZipDir() const;

			virtual		void		clear();
			virtual		void		prepareForSend();										// LANCIA ECCEZIONE
			virtual		void		prepareForRecv();										// LANCIA ECCEZIONE
						void		compress();												// LANCIA ECCEZIONE
						void		decompress(const bool autoRemove = true);											// LANCIA ECCEZIONE
		//	virtual		void		open();

		protected slots:
			virtual		void		send(const qint64 block = 0);
			virtual		void		recv();
			virtual		void		behaviorOnSend(const qint64 dim);						// LANCIA ECCEZIONE

		protected:
			virtual		qint64		calculateNetworkSize();									// LANCIA ECCEZIONE
						void		setCompressed();
						void		setNotCompressed();
						bool		isCompressed() const;

		private:
						void		checkResource();										// LANCIA ECCEZIONE
						void		checkDir();												// LANCIA ECCEZIONE
						void		checkZip();												// LANCIA ECCEZIONE

			bool		m_compressionFlag;
			QString		m_resourcePath;
			QString		m_zipDir;
			QString		m_zipName;
			QFile *		m_zipFile;
	};

}

#endif // RESOURCE_H
