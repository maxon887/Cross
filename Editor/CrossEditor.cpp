#include "CrossEditor.h"

cross::Game* cross::game = NULL;

CrossEditor::CrossEditor(QWidget *parent) :
	QMainWindow(parent),
	Game()
{
	ui.setupUi(this);
	game = this;
}

CrossEditor::~CrossEditor()
{ }

void CrossEditor::closeEvent(QCloseEvent* eve){
	ui.openGLWidget->shutDown();
}
