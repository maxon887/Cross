#include "FileExplorer.h"
#include "System.h"

FileExplorer::FileExplorer(QWidget* parent) :
	QTreeView(parent)
{
	QString path = QDir::currentPath() + "/" + QString(sys->AssetsPath().c_str());
	fileSystem = new QFileSystemModel(this);
	fileSystem->setRootPath(path);
	setModel(fileSystem);
	setRootIndex(fileSystem->index(path));
}


FileExplorer::~FileExplorer(){

}
