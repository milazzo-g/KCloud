#ifndef RESOURCE_H
#define RESOURCE_H

#include "resource_global.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QFileInfo>
#include <QByteArray>
#include <QDataStream>
#include <QTemporaryFile>

#include "../NetObject/NetObject.h"
#include "../ResourceHeader/ResourceHeader.h"
#include "../LIBS/quazip-0.7.1_build/include/JlCompress.h"

namespace KCloud{

	class RESOURCESHARED_EXPORT Resource : public NetObject{
			Q_OBJECT

		public:
				enum Mode{
					Download,
					Upload
				};

			explicit				Resource(QObject *parent = 0);

						QString		getResourcePath() const;
						QString		getZipName() const;
						QString		getZipPath() const;

						void		setResourcePath(const QString &path) throw(Exception);
						void		setZipName(const QString &name, const QString &ext = "");
						void		setZipDir(const QString &path) throw(Exception);

						QString		getZipDir() const;

						void		clear();
			virtual		void		prepareForSend() throw(Exception);
			virtual		void		prepareForRecv();
						void		compress() throw(Exception);
						void		decompress(const ResourceHeader &head, const bool &autoRemove = true) throw(Exception);
						bool		removeZipFile() throw(Exception);

		signals:
						void		transmissionRate(const qint64 total, const qint64 transmitted, Mode mod);

		protected slots:
			virtual		void		send(const qint64 block = 0);
			virtual		void		recv() throw(Exception);
			virtual		void		behaviorOnSend(const qint64 dim) throw(Exception);

		protected:
			virtual		qint64		calculateNetworkSize() throw(Exception);
						void		setCompressed();
						void		setNotCompressed();
						bool		isCompressed() const;

		private:
						void		checkResource() throw(Exception);
						void		checkDir() throw(Exception);
						void		checkZip() throw(Exception);

			bool		m_compressionFlag;
			QString		m_resourcePath;
			QString		m_zipDir;
			QString		m_zipName;
			QFile *		m_zipFile;
			qint64		m_total;
			qint64		m_transmitted;
	};

}


#endif // RESOURCE_H
