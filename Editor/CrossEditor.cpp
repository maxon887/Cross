#include "CrossEditor.h"
#include "Cross.h"

cross::Game* cross::game = NULL;

CrossEditor::CrossEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

CrossEditor::~CrossEditor()
{

}
