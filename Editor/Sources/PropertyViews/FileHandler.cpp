#include "FileHandler.h"
#include "Cross.h"

#include <QDragEnterEvent>
#include <QMimeData>
#include <QMessageBox>

FileHandler::FileHandler(QWidget* parent)
	: QLineEdit(parent)
{ }

void FileHandler::SetFileExtension(const QString& ext) {
	file_extension = ext;
}

void FileHandler::dragEnterEvent(QDragEnterEvent *event) {
	drop_accepted = false;
	const QMimeData* data = event->mimeData();
	QList<QUrl> urls = data->urls();
	if(file_extension == "") {
		event->ignore();

		QMessageBox* msgBox = new QMessageBox(this);
		msgBox->setText("Something goes wrong");
		msgBox->setInformativeText("File extension not specified for this handle");
		msgBox->setIcon(QMessageBox::Icon::Warning);
		msgBox->show();
		return;
	}
	if(urls.size() != 1) {
		event->ignore();

		QMessageBox* msgBox = new QMessageBox(this);
		msgBox->setText("Something goes wrong");
		msgBox->setInformativeText("File Handler can contain only one file");
		msgBox->setIcon(QMessageBox::Icon::Warning);
		msgBox->show();
		return;
	}
	QString filename = urls.at(0).fileName();
	if(filename.endsWith("." + file_extension)) {
		drop_accepted = true;
		event->accept();
	}
}

void FileHandler::dragMoveEvent(QDragMoveEvent *event) {
	if(drop_accepted) {
		event->accept();
	}
}

void FileHandler::dropEvent(QDropEvent *event) {

}