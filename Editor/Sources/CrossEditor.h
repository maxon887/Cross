#ifndef CROSS_EDITOR
#define CROSS_EDITOR

#include "Game.h"

#include <QMainWindow>

class Action;
class Menu;
class SceneExplorer;
class PropertiesView;
class FileExplorer;

namespace Ui {
class CrossEditorUI;
}

using namespace cross;

class CrossEditor : public QMainWindow, 
					public Game
{
	Q_OBJECT
public:
	Event<> UIInitialized;
	Event<Action*> SomethingChanged;

	CrossEditor(QWidget *parent = 0);
	~CrossEditor();

	void Update(float sec) override;

	SceneExplorer* GetSceneExplorer();
	PropertiesView* GetPropertyView();
	FileExplorer* GetFileExplorer();
	Ui::CrossEditorUI* GetUI();

	void LoadScene(QString& fileInfo);
	void RestoreSettings();

	void OnScreenChanged(Screen* screen);
	void OnSomethingChanged(Action* a);

protected:
	void closeEvent(QCloseEvent* eve) override;
	void keyPressEvent(QKeyEvent* key) override;
	void keyReleaseEvent(QKeyEvent* key) override;

private:
	Ui::CrossEditorUI* ui = nullptr;
};

extern CrossEditor* editor;

#endif // !CROSS_EDITOR