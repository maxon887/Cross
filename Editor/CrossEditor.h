#ifndef CROSS_EDITOR
#define CROSS_EDITOR

#include "Game.h"
#include "ui_CrossEditor.h"

#include <QMainWindow>

class CrossEditor : public QMainWindow, 
					public Game
{
	Q_OBJECT
public:
	CrossEditor(QWidget *parent = 0);
	~CrossEditor();

	virtual Screen* GetStartScreen();

protected:
	void closeEvent(QCloseEvent* eve);

private:
	Ui::CrossEditorClass ui;
};

#endif // !CROSS_EDITOR