#include "CrossEditor.h"
#include "SceneView.h"

CrossEditor::CrossEditor(QWidget *parent) :
	QMainWindow(parent),
	Game()
{
	ui.setupUi(this);
	game = this;
}

CrossEditor::~CrossEditor()
{ }

Screen* CrossEditor::GetStartScreen(){
	SceneView* sceneView = new SceneView();
	return sceneView;
}

void CrossEditor::closeEvent(QCloseEvent* eve){
	ui.openGLWidget->shutDown();
}
