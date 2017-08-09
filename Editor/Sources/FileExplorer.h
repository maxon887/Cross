#ifndef FILE_EXPLORER
#define FILE_EXPLORER

#include "Cross.h"
#include "Event.h"

#include <QTreeView>

using namespace cross;

class QFileSystemModel;

class FileExplorer : public QTreeView
{
	Q_OBJECT
public:
	Event<string> FileSelected;

	FileExplorer(QWidget* parent = 0);
	~FileExplorer();

	void SetupProjectDirectory(QString dir);

protected:
	void contextMenuEvent(QContextMenuEvent *event) override;

private:
	QFileSystemModel* file_system	= NULL;
	QMenu* context_menu				= NULL;

	void OnItemSelected(QModelIndex index);
	void OnItemDoubleClick(QModelIndex index);

	void OnNewShaderClick();
};

#endif