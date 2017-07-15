#include "CrossEditor.h"
#include "SceneView.h"
#include "System.h"

#include <QSettings.h>
#include <QFileDialog.h>
#include <QMessageBox>

CrossEditor* editor = NULL;

CrossEditor::CrossEditor(QWidget *parent) :
	QMainWindow(parent),
	Game()
{
	editor = this;
	game = editor;
	ui.setupUi(this);
	connect(ui.actionNew_Scene, &QAction::triggered, this, &CrossEditor::OnNewSceneClick);
	connect(ui.actionSave_As, &QAction::triggered, this, &CrossEditor::OnSaveAsClick);
	connect(ui.actionFile_Explorer, &QAction::triggered, this, &CrossEditor::OnFileExplorerClick);
	connect(ui.actionScene_Explorer, &QAction::triggered, this, &CrossEditor::OnSceneExplorerClick);
	
	connect(ui.sceneExplorerTree, &QTreeView::clicked, ui.propertiesView, &PropertiesView::OnEntitySelected);
}

CrossEditor::~CrossEditor()
{ }

Screen* CrossEditor::GetStartScreen(){
	return new SceneView();
}

void CrossEditor::Update(float sec){
	ui.propertiesView->Update(sec);
}

void CrossEditor::closeEvent(QCloseEvent* eve){
	ui.sceneView->shutDown();
	QSettings settings("CrossEditor");
	settings.setValue("geometry", QVariant(geometry()));
	settings.setValue("windowState", saveState());
	QMainWindow::closeEvent(eve);
}

void CrossEditor::LoadScene(QString& scenePath){
	try{
		scene_file = scenePath;
		ui.sceneExplorerTree->clearSelection();
		SetScene(new SceneView(), scenePath.toStdString());
		QFile file(scenePath);
		setWindowTitle(QString("Cross Editor - ") + file.fileName());
	}catch(Exception exc){
		string msg = string(exc.message) +
			+"\nFile: " + string(exc.filename) +
			+"\nLine: " + to_string(exc.line);
		ExceptionMsgBox(msg.c_str());
	}
}

void CrossEditor::RestoreSettings(){
	QSettings settings("CrossEditor");
	if(settings.contains("geometry")){
		setGeometry(settings.value("geometry").value<QRect>());
	}
	restoreState(settings.value("windowState").toByteArray());
}

void CrossEditor::ExceptionMsgBox(const char* msg) {
	QMessageBox msgBox;
	msgBox.setText("Unhandled Exception");
	msgBox.setInformativeText(msg);
	msgBox.setIcon(QMessageBox::Icon::Critical);
	msgBox.exec();
}

void CrossEditor::OnNewSceneClick(){
	ui.sceneExplorerTree->reset();
	SetScene(new SceneView());
}

void CrossEditor::OnSaveAsClick(){
	QString path = QDir::currentPath() + "/" + QString(sys->AssetsPath().c_str()) + scene_file;
	QString filePath = QFileDialog::getSaveFileName(this, "Save Scene", path, "Scene File (*.scn)");
	if(filePath != ""){
		game->GetCurrentScene()->Save(filePath.toStdString());
	}
}

void CrossEditor::OnFileExplorerClick(){
	ui.fileExplorer->show();
}

void CrossEditor::OnSceneExplorerClick(){
	ui.sceneExplorer->show();
}