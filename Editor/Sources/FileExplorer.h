#ifndef FILE_EXPLORER
#define FILE_EXPLORER

#include "Cross.h"
#include "Event.h"

#include <QTreeView>
#include <QFileSystemModel>

using namespace cross;

class CrossEditor;

class FileExplorer : public QTreeView
{
	Q_OBJECT
public:
	Event<string> FileSelected;

	FileExplorer(QWidget* parent = 0);
	~FileExplorer();

	void SetupProjectDirectory(QString dir);

private:
	CrossEditor* cross_editor;
	QFileSystemModel* file_system;

	void OnItemSelected(QModelIndex index);
	void OnItemDoubleClick(QModelIndex index);
};

#endif