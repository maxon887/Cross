#ifndef FILE_EXPLORER
#define FILE_EXPLORER

#include "Cross.h"

#include <QTreeView>
#include <QFileSystemModel>

using namespace cross;

class CrossEditor;

class FileExplorer : public QTreeView
{
	Q_OBJECT
public:
	FileExplorer(QWidget* parent = 0);
	~FileExplorer();

	void SetupProjectDirectory(QString dir);

private:
	CrossEditor* cross_editor;
	QFileSystemModel* file_system;

	void OnItemDoubleClick(QModelIndex index);
};

#endif