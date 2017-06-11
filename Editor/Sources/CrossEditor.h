#ifndef CROSS_EDITOR
#define CROSS_EDITOR

#include "Game.h"
#include "ui_CrossEditor.h"

#include <QMainWindow>

class SceneView;

extern CrossEditor* editor;

class CrossEditor : public QMainWindow, 
					public Game
{
	Q_OBJECT
public:
	Event<Scene*> SceneLoaded;

	CrossEditor(QWidget *parent = 0);
	~CrossEditor();

	virtual Screen* GetStartScreen();

	void LoadScene(QString& path);
	void RestoreSettings();

protected:
	void closeEvent(QCloseEvent* eve);

private:
	Ui::CrossEditorClass ui;
	SceneView* scene_view;

private slots:
	void OnFileExplorerClick();
};

#endif // !CROSS_EDITOR