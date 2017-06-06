#include "CrossEditor.h"
#include "SceneView.h"

#include <QSettings.h>
#include <QFileDialog.h>

CrossEditor::CrossEditor(QWidget *parent) :
	QMainWindow(parent),
	Game()
{
	ui.setupUi(this);
	game = this;

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
}

void CrossEditor::RestoreSettings(){
	QSettings settings("CrossEditor");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());
}

void CrossEditor::OnFileExplorerClick(){
	ui.fileExplorer->show();
}