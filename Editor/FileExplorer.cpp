#include "FileExplorer.h"
#include "Launcher.h"

FileExplorer::FileExplorer(QWidget* parent) :
	QTreeView(parent)
{
	/*
	QString path = "C:/";
	fileSystem = new QFileSystemModel(this);
	fileSystem->setRootPath(path);
	setModel(fileSystem);*/
}


FileExplorer::~FileExplorer(){

}
