#ifndef RESOURCE_H
#define RESOURCE_H

#include "NetObject.h"
#include "FolderCompressor.h"

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

						QString		getPath() const;
						void		setPath(const QString &path);
						void		from(const QString &path);							//LANCIA ECCEZIONI
						void		to(const QString &path);
						void		setWorkingDir(const QString &path);
						QString		getWorkingDir() const;
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
						void checkFilePath() const; //LANCIA ECCEZIONE
						void checkWorkingDir() const;//LANCIA ECCEZIONE

			bool		compressionFlag;
			QString		workingDir;
			QString		filePath;
			QFile *		file;
	};

}

#endif // RESOURCE_H
