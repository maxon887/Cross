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
#include "Array.h"
#include "Event.h"

using namespace cross;

class FilesView;

class FileSelector {
public:
	Event<const String&> FileSelected;
	
	FileSelector(FilesView* filesView, const String& label, const String& fileExtension);
	
	bool Update();
	
	String GetSelectedFile() const;
	void SetSelectedFile(const String& filename);
	void SetText(const String& messageText);
	
private:
	Array<String> files_list;		//all potential raw file names that we are working with
	Array<String> all_names;		//all potential files short names without a path
	Array<String> suggested_names;	//selection from all_names that match our criteria

	FilesView* files_view = nullptr;
	String extension;
	String label;
	String selected_file;
	String current_input = String("", 0, 256);
	int suggested_index = 0;

	void ValueChanged();
	bool CheckFileExtension(const String& filename);
	void RefreshAllAvailableFiles();
};
