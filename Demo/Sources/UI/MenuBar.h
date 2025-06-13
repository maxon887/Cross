/*	Copyright © 2018 Maksim Lukyanov

	This file is part of Cross++ Game Engine.

	Cross++ Game Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Cross++ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/
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
	FilesView* GetFilesView() const;

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