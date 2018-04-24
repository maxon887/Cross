#ifndef CROSS_EDITOR
#define CROSS_EDITOR

#include "Game.h"

#include <QMainWindow>

class Action;
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

	void Start() override;
	void Update(float sec) override;

	SceneExplorer* GetSceneExplorer();
	PropertiesView* GetPropertyView();
	FileExplorer* GetFileExplorer();

	void LoadScene(QString& fileInfo);
	void RestoreSettings();
	void ExceptionMsgBox(const char* msg);

	void OnNewSceneClick();
	void OnSaveAsClick();
	void OnSetupProjectDirectoryClick();

	void OnScreenChanged(Screen* screen);
	void OnSomethingChanged(Action* a);

protected:
	void closeEvent(QCloseEvent* eve) override;
	void keyPressEvent(QKeyEvent* key) override;
	void keyReleaseEvent(QKeyEvent* key) override;

private:
	Ui::CrossEditorUI* ui;
	QString scene_file;
};

extern CrossEditor* editor;

#endif // !CROSS_EDITOR