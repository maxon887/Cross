#include "CrossEditor.h"
#include "SceneView.h"

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
	connect(ui.actionFile_Explorer, &QAction::triggered, this, &CrossEditor::OnFileExplorerClick);
}

CrossEditor::~CrossEditor()
{ }

Screen* CrossEditor::GetStartScreen(){
	scene_view = new SceneView();
	return scene_view;
}

void CrossEditor::closeEvent(QCloseEvent* eve){
	ui.sceneView->shutDown();
	QSettings settings("CrossEditor");
	settings.setValue("geometry", QVariant(geometry()));
	settings.setValue("windowState", saveState());
	QMainWindow::closeEvent(eve);
}

void CrossEditor::LoadScene(QString& file){
	try{
		scene_view->Load(file.toStdString(), true);
		ui.treeView_2->reset();
		SceneLoaded(scene_view);
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

void CrossEditor::OnFileExplorerClick(){
	ui.fileExplorer->show();
}