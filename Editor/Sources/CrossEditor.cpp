#include "CrossEditor.h"
#include "SceneView.h"

#include <QSettings.h>
#include <QFileDialog.h>

CrossEditor* editor = NULL;

CrossEditor::CrossEditor(QWidget *parent) :
	QMainWindow(parent),
	Game()
{
	editor = this;
	game = editor;
	ui.setupUi(this);
	connect(ui.actionFile_Explorer, &QAction::triggered, this, &CrossEditor::OnFileExplorerClick);
}

CrossEditor::~CrossEditor()
{ }

Screen* CrossEditor::GetStartScreen(){
	scene_view = new SceneView();
	return scene_view;
}

void CrossEditor::closeEvent(QCloseEvent* eve){
	ui.openGLWidget->shutDown();
	QSettings settings("CrossEditor");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState", saveState());
	QMainWindow::closeEvent(eve);
}

void CrossEditor::LoadScene(QString& file){
	scene_view->Load(file.toStdString(), true);
	ui.treeView_2->reset();
	SceneLoaded(scene_view);
}

void CrossEditor::RestoreSettings(){
	QSettings settings("CrossEditor");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());
}

void CrossEditor::OnFileExplorerClick(){
	ui.fileExplorer->show();
}