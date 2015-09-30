#ifndef RENAMEFORM_H
#define RENAMEFORM_H

#include <QString>
#include <QRegExp>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QRegExpValidator>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QGraphicsScene>
#include <QPixmap>
#include <QLabel>

#include "GraphicResourceHeader.h"

namespace Ui {
	class RenameForm;
}

using namespace KCloud;

class RenameForm : public QDialog
{
		Q_OBJECT

	public:
		explicit				RenameForm(const GraphicResourceHeader * const res, QWidget *parent = 0);

								~RenameForm();
					bool		wasAccepted();
					QString		getNewName();

	private slots:
					void on_cancelButton_clicked();

					void on_confirmButton_clicked();

	private:
				Ui::RenameForm *			ui;
				QLineEdit *					m_name;
				QPushButton *				m_confirm;
				QPushButton *				m_cancel;
				QLabel *					m_renameLabel;
		const	GraphicResourceHeader *		m_header;
				QString						m_newName;
				bool						m_accepted;
				QGraphicsView *				m_view;
				QTableWidget *				m_table;
};

#endif // RENAMEFORM_H
