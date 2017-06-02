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
	return new SceneView();
}

void CrossEditor::closeEvent(QCloseEvent* eve){
	ui.openGLWidget->shutDown();
}
