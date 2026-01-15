#pragma once
#include "Cross.h"
#include "Input.h"

class View;
class FilesView;

using namespace cross;

class MenuBar {
public:
	MenuBar();
	~MenuBar();

	void Update(float sec);

	void ShowMenu();
	void CloseAllViews();
	float GetHeight() const;

protected:
	float menu_height			= 0;
	bool show_style_editor		= false;
	bool fullscreen				= false;

	Array<View*> views;
	FilesView* files_view		= nullptr;
	View* log					= nullptr;
	View* stats					= nullptr;
	View* about					= nullptr;

	void UpdateDocking();
	void SaveScene();
	void KeyPressed(Key key);
};