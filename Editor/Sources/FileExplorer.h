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

private:
	CrossEditor* cross_editor;
	QFileSystemModel* fileSystem;

private slots:
	void OnItemDoubleClick(QModelIndex index);
};

#endif