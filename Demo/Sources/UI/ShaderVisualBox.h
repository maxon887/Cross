#pragma once
#include "Cross.h"
#include "Base/Array.h"
#include "Base/String.h"

using namespace cross;

class FileSelector;
class FilesView;

class ShaderVisualBox {
public:
	ShaderVisualBox(FilesView* filesView);
	~ShaderVisualBox();

	void Update();

	void OnFileSelected(const String& filename);

private:
	Shader* shader = nullptr;
	String shader_filename;
	Array<String> type_names;

	FileSelector* vertex_file_selector = nullptr;
	FileSelector* fragment_file_selector = nullptr;
};
