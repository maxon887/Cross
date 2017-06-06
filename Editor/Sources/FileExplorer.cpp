#include "FileExplorer.h"
#include "System.h"
#include "CrossEditor.h"

#include <QHeaderView.h>

FileExplorer::FileExplorer(QWidget* parent) :
	QTreeView(parent)
{
	QString path = QDir::currentPath() + "/" + QString(sys->AssetsPath().c_str());
	fileSystem = new QFileSystemModel(this);
	fileSystem->setRootPath(path);
	setModel(fileSystem);
	setRootIndex(fileSystem->index(path));

	hideColumn(2);
	hideColumn(3);
	
	header()->setStretchLastSection(false);
	header()->setSectionResizeMode(0, QHeaderView::Stretch);
	header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

	connect(this, &QTreeView::doubleClicked, this, &FileExplorer::OnItemDoubleClick);
}


FileExplorer::~FileExplorer(){

}

void FileExplorer::OnItemDoubleClick(QModelIndex index){
	QFileInfo fileInfo = fileSystem->fileInfo(index);
	if(fileInfo.suffix() == "scn"){
		((CrossEditor*)game)->LoadScene(fileInfo.absoluteFilePath());
	}
}