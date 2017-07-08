#include "FileExplorer.h"
#include "System.h"
#include "CrossEditor.h"

#include <QHeaderView.h>

FileExplorer::FileExplorer(QWidget* parent) :
	QTreeView(parent)
{
	QString path = QDir::currentPath() + "/" + QString(sys->AssetsPath().c_str());
	file_system = new QFileSystemModel(this);
	file_system->setRootPath(path);
	setModel(file_system);
	setRootIndex(file_system->index(path));

	hideColumn(2);
	hideColumn(3);
	
	header()->setStretchLastSection(false);
	header()->setSectionResizeMode(0, QHeaderView::Stretch);
	header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

	connect(this, &QTreeView::doubleClicked, this, &FileExplorer::OnItemDoubleClick);
}

FileExplorer::~FileExplorer(){
	delete file_system;
	disconnect(this, &QTreeView::doubleClicked, this, &FileExplorer::OnItemDoubleClick);
}

void FileExplorer::OnItemDoubleClick(QModelIndex index){
	QFileInfo fileInfo = file_system->fileInfo(index);
	if(fileInfo.suffix() == "scn"){
		editor->LoadScene(fileInfo.absoluteFilePath());
	}
}