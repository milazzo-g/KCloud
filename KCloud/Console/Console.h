#ifndef CONSOLE_H
#define CONSOLE_H

#include "console_global.h"

#include <QFile>
#include <QObject>
#include <QThread>
#include <QString>
#include <QTextStream>
#include <QSocketNotifier>

namespace KCloud{

	class CONSOLESHARED_EXPORT Console : public QThread{
		Q_OBJECT
		public:
			explicit					Console(QObject *parent = 0);
										~Console();
		signals:
						void			input(QString str);
		public slots:
						void			output(const QString &str);
		private slots:
						void			processNewInput();

		private:
						QSocketNotifier	m_notitier;
	};

}

#endif // CONSOLE_H
