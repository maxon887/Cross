#ifndef MENU
#define MENU

#include <QMenuBar>

class Menu : public QMenuBar {
	Q_OBJECT
public:
	Menu(QWidget* parent);

private:
	void OnNewSceneClick();
	void OnSaveAsClick();
	void OnSetupProjectDirectoryClick();
};


#endif