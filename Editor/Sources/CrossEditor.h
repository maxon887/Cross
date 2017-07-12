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

	Screen* GetStartScreen() override;
	void Update(float sec) override;

	void LoadScene(QString& path);
	void RestoreSettings();
	void ExceptionMsgBox(const char* msg);

protected:
	void closeEvent(QCloseEvent* eve);

private:
	Ui::CrossEditorClass ui;
	SceneView* scene_view;

	void OnFileExplorerClick();
	void OnSceneExplorerClick();
};

#endif // !CROSS_EDITOR