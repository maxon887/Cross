#pragma once

#include "GraphicsGL.h"

#include <QtWidgets/QMainWindow>
#include <Game.h>
#include "ui_CrossEditor.h"

class CrossEditor : public QMainWindow, public Game
{
	Q_OBJECT

public:
	CrossEditor(QWidget *parent = 0);
	~CrossEditor();

protected:
	void closeEvent(QCloseEvent* eve);
private:
	Ui::CrossEditorClass ui;
};