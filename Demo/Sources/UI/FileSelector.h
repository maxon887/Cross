#pragma once
#include "Cross.h"
#include "Base/Array.h"
#include "Base/Event.h"
#include "Base/String.h"

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
