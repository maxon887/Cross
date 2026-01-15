#pragma once
#include "Cross.h"
#include "FileSelector.h"

using namespace cross;

class MeshVisualBox {
public:
	MeshVisualBox(FilesView* filesView);
	~MeshVisualBox();
	
	void Update();
	
	void EntitySelected(Entity* newEntity);
	void MaterialSelected(const String& materialFile);
	
private:
	Mesh* mesh = nullptr;
	FileSelector* material_file = nullptr;
};
