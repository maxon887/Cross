#ifndef FILE_HANDLER
#define FILE_HANDLER

#include "Cross.h"
#include "Event.h"

#include <QLineEdit>

using namespace cross;

class FileHandler : public QLineEdit 
{
	Q_OBJECT
public:
	Event<QString> FileChanged;

	FileHandler(QWidget* parent);

	QString GetFile();
	void SetFile(const QString& filename);
	void SetFileExtension(const QString& ext);

protected:
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent* event) override;

private:
	QString file_extension = "";
	QString file_path = "";
	bool drop_accepted = false;
};

#endif