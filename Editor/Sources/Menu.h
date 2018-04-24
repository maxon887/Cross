#ifndef MENU
#define MENU

#include <QMenuBar>

class Menu : public QMenuBar {
	Q_OBJECT
public:
	Menu(QWidget* parent);

private:
	void OnUIInitialized();

	void OnNewSceneClick();
	void OnSaveAsClick();
	void OnSetupProjectDirectoryClick();

	void OnFileExplorerTriggered(bool visibility);
	void OnFileExplorerVisibilityChanged(bool visibility);

	void OnSceneExplorerTriggered(bool visibility);
	void OnSceneExplorerVisibilityChanged(bool visibility);

	void OnPropertiesViewTriggered(bool visibility);
	void OnPropertiesViewVisibilityChanged(bool visibility);
};


#endif