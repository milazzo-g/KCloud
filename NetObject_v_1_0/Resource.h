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
			explicit				Resource(const QString &path, QObject *parent = 0);		//LANCIA ECCEZIONI

						QString		getPath() const;
						void		setPath(const QString &path);							//LANCIA ECCEZIONI
			virtual		void		clear();											/**/
			virtual		void		prepareForSend();									/**/
			virtual		void		prepareForRecv();
		protected slots:
			virtual		void		send(const qint64 block = 0);
			virtual		void		recv();
			virtual		void		behaviorOnSend(const qint64 dim);

		protected:
			virtual		qint64		calculateNetworkSize();
		private:
			QString		filePath;
			QFile *		file;
	};

}

#endif // RESOURCE_H
