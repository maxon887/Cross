#ifndef CROSSEDITOR_H
#define CROSSEDITOR_H

#include "GraphicsGL.h"

#include <QtWidgets/QMainWindow>
#include "ui_CrossEditor.h"

class CrossEditor : public QMainWindow
{
	Q_OBJECT

public:
	CrossEditor(QWidget *parent = 0);
	~CrossEditor();

private:
	Ui::CrossEditorClass ui;
};

#endif // CROSSEDITOR_H
