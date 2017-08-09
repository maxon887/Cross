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
	QAction* newShader = new QAction(context_menu);
	newShader->setText("New Shader");
	connect(newShader, &QAction::triggered, this, &FileExplorer::OnNewShaderClick);
	context_menu->addAction(newShader);
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

void FileExplorer::OnNewShaderClick(){
	QModelIndexList indexes = selectedIndexes();
	if(indexes.size() > 0){
		QFileInfo fileInfo = file_system->fileInfo(indexes[0]);
	}else{
		Shader* newShader = new Shader();
		newShader->Save("New Shader.she");
		QString absolutePath = file_system->rootDirectory().absolutePath() + "/New Shader.she";
		QModelIndex index = file_system->index(absolutePath);
		edit(index);
	}
}