#include "FileExplorer.h"
#include "System.h"
#include "CrossEditor.h"
#include "Graphics3D.h"
#include "Scene.h"
#include "Material.h"
#include "File.h"

#include <QHeaderView>
#include <QFileSystemModel>
#include <QContextMenuEvent>

FileExplorer::FileExplorer(QWidget* parent) :
	QTreeView(parent)
{
	file_system = new QFileSystemModel(this);
	file_system->setReadOnly(false);
	setModel(file_system);

	SetupProjectDirectory(QDir::currentPath() + "/" + QString(sys->AssetsPath().c_str()));

	hideColumn(2);
	hideColumn(3);
	
	header()->setStretchLastSection(false);
	header()->setSectionResizeMode(0, QHeaderView::Stretch);
	header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

	connect(this, &QTreeView::clicked, this, &FileExplorer::OnItemSelected);
	connect(this, &QTreeView::doubleClicked, this, &FileExplorer::OnItemDoubleClick);

	//context menu creating
	context_menu = new QMenu(this);

	QAction* newFolder = new QAction(context_menu);
	newFolder->setText("New Folder");
	connect(newFolder, &QAction::triggered, this, &FileExplorer::OnNewFolderClick);
	context_menu->addAction(newFolder);
	QAction* newShader = new QAction(context_menu);
	newShader->setText("New Shader");
	connect(newShader, &QAction::triggered, this, &FileExplorer::OnNewShaderClick);
	context_menu->addAction(newShader);
	QAction* deleteAction = new QAction(context_menu);
	deleteAction->setText("Delete");
	connect(deleteAction, &QAction::triggered, this, &FileExplorer::OnDeleteClick);
	context_menu->addAction(deleteAction);
}

FileExplorer::~FileExplorer(){
	delete file_system;
	disconnect(this, &QTreeView::doubleClicked, this, &FileExplorer::OnItemDoubleClick);
}

void FileExplorer::SetupProjectDirectory(QString dir){
	file_system->setRootPath(dir);
	setRootIndex(file_system->index(dir));
}

void FileExplorer::contextMenuEvent(QContextMenuEvent *event) {
	context_menu->exec(event->globalPos());
}

void FileExplorer::OnItemSelected(QModelIndex index){
	QFileInfo fileInfo = file_system->fileInfo(index);
	QDir root = file_system->rootDirectory();
	QString filepath = root.relativeFilePath(fileInfo.absoluteFilePath());
	try{
		FileSelected(filepath.toStdString());
	}catch(Exception ex){
		editor->ExceptionMsgBox(ex.message);
	}
}

void FileExplorer::OnItemDoubleClick(QModelIndex index){
	QFileInfo fileInfo = file_system->fileInfo(index);
	QDir root = file_system->rootDirectory();
	QString filepath = root.relativeFilePath(fileInfo.absoluteFilePath());
	try{
		if(fileInfo.suffix() == "scn"){
			editor->LoadScene(filepath);
		}else if(fileInfo.suffix() == "obj" || fileInfo.suffix() == "fbx"){
			Model* model = gfx3D->LoadModel(filepath.toStdString());
			gfx3D->AdjustMaterial(model->hierarchy, gfx3D->GetDefaultMaterial()->Clone());
			editor->GetCurrentScene()->AddModel(model);
			editor->GetCurrentScene()->AddEntity(model->hierarchy);
		}
	}catch(Exception ex){
		editor->ExceptionMsgBox(ex.message);
	}
}

void FileExplorer::OnNewFolderClick(){
	QString selectedDir = GetSelectedDirectory();
	
	QString baseName = "/New Folder";
	QDir dir = selectedDir + baseName;
	int nameNumber = 0;
	while(dir.exists()){
		nameNumber++;
		dir = selectedDir + baseName + " " + QString::number(nameNumber);
	}
	dir.mkdir(dir.absolutePath());
	QModelIndex index = file_system->index(dir.absolutePath());
	edit(index);
}

void FileExplorer::OnNewShaderClick(){
	QString selectedDir = GetSelectedDirectory();

	QString baseName = "/New Shader";
	QString shaderName = baseName + ".she";
	QFile shaderFile = selectedDir + shaderName;
	int nameNumber = 0;
	while(shaderFile.exists()) {
		nameNumber++;
		shaderName = baseName + " " + QString::number(nameNumber) + ".she";
		shaderFile.setFileName(selectedDir + shaderName);
	}

	Shader* newShader = new Shader();
	newShader->Save(shaderFile.fileName().toStdString());
	QModelIndex index = file_system->index(shaderFile.fileName());
	edit(index);
}

void FileExplorer::OnDeleteClick(){

}

QString FileExplorer::GetSelectedDirectory(){
	QString root = file_system->rootDirectory().absolutePath();
	QModelIndexList indexes = selectedIndexes();
	if(indexes.size() > 0) {
		QFileInfo fileInfo = file_system->fileInfo(indexes.first());
		if(fileInfo.isFile()){
			root = fileInfo.absolutePath();
		}else{
			root = fileInfo.absoluteFilePath();
		}
	}
	return root;
}