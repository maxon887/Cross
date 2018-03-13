#ifndef FILE_HANDLER
#define FILE_HANDLER

#include <QLineEdit>

class FileHandler : public QLineEdit 
{
	Q_OBJECT
public:
	FileHandler(QWidget* parent);

	void SetFileExtension(const QString& ext);

protected:
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent* event) override;

private:
	QString file_extension = "";
	bool drop_accepted = false;
};

#endif