#include "CrossEditor.h"
#include "SceneView.h"

#include <QSettings.h>

CrossEditor::CrossEditor(QWidget *parent) :
	QMainWindow(parent),
	Game()
{
	ui.setupUi(this);
	game = this;

	connect(ui.actionLoad_Scene, &QAction::triggered, this, &CrossEditor::OnLoadSceneClick);
	connect(ui.actionFile_Explorer, &QAction::triggered, this, &CrossEditor::OnFileExplorerClick);
}

CrossEditor::~CrossEditor()
{ }

Screen* CrossEditor::GetStartScreen(){
	SceneView* sceneView = new SceneView();
	return sceneView;
}

void CrossEditor::closeEvent(QCloseEvent* eve){
	ui.openGLWidget->shutDown();
	QSettings settings("CrossEditor");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState", saveState());
	QMainWindow::closeEvent(eve);
}

void CrossEditor::RestoreSettings(){
	QSettings settings("CrossEditor");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());
}

void CrossEditor::OnLoadSceneClick(){
	
}

void CrossEditor::OnFileExplorerClick(){

}