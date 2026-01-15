#pragma once
#include "Cross.h"
#include "Base/String.h"
#include "Base/Map.h"

using namespace cross;

class FileSelector;
class FilesView;

class MaterialVisualBox {
public:
	MaterialVisualBox(FilesView* filesView);
	~MaterialVisualBox();

	void Update();

	void OnFileSelected(const String& filename);
	void OnShaderSelected(const String& filename);
	void OnScreenChanged(Screen* newScreen);

private:
	FilesView* files_view = nullptr;
	Material* mat = nullptr;
	FileSelector* shader_selector = nullptr;
	bool loaded_from_scene = false;
	
	//key property name 
	Map<String, FileSelector*> texture_selectors;

	void DeleteMaterialIfNeeded();
	void CreateTextureSelectors();
};
