#include "FileExplorer.h"

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
