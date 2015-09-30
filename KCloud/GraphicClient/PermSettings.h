#ifndef PERMSETTINGS_H
#define PERMSETTINGS_H

#include <QMap>
#include <QIcon>
#include <QLabel>
#include <QDialog>
#include <QCheckBox>
#include <QMessageBox>
#include <QRadioButton>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "../User/User.h"
#include "../MainServer/defines.h"

#include "GraphicResourceHeader.h"

namespace Ui {

	class PermSettings;
}

using namespace KCloud;

class PermSettings : public QDialog{
		Q_OBJECT

	public:

		explicit PermSettings(const User * const user, const GraphicResourceHeader * const res, QWidget *parent = 0);
		~PermSettings();

		bool												wasAccepted() const;
		QMap<QString, GraphicResourceHeader::ResourcePerm>	getPermMap() const;
		GraphicResourceHeader::ResourcePerm					getPublicPerm() const;
		ResourceHeader										getNewHeader() const;


	private slots:

		void on_tableWidget_itemClicked(QTableWidgetItem *item);

		void on_publicChec_stateChanged(int arg1);

		void on_addUserBtn_clicked();

		void on_delUserBtn_clicked();

		void on_tableWidget_itemChanged(QTableWidgetItem *item);

		void on_confirmBtn_clicked();

		void on_readRadio_clicked();

		void on_writeRadio_clicked();

	private:
				Ui::PermSettings *									ui;
				QTableWidget *										m_table;
				QMap<QString, GraphicResourceHeader::ResourcePerm>	m_map;
				QCheckBox *											m_public;
				QRadioButton *										m_publicRead;
				QRadioButton *										m_publicWrite;
				GraphicResourceHeader::ResourcePerm					m_publicPerm;
		const	GraphicResourceHeader *								m_currentResource;
				bool												m_accepted;
				QString												m_user;
				bool												m_crafting;

};

#endif // PERMSETTINGS_H