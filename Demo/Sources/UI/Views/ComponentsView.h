#pragma once
#include "UI/Views/View.h"
#include "Property.h"
#include "UI/MeshVisualBox.h"

class SceneView;

class ComponentsView : public View {
public:
	ComponentsView(SceneView* sceneView, FilesView* filesView);

	void Update(float sec) override;

	DockPosition GetDefaultDockPosition() const override { return DockPosition::RIGHT; }

protected:
	MeshVisualBox mesh_box;
	SceneView* scene_view = nullptr;

	void ShowProperty(BaseProperty* baseProperty) const;

	void ContextMenu(Entity* selectedEntity) const;
};
