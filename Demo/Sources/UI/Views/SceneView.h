#pragma once
#include "Cross.h"
#include "UI/Views/View.h"
#include "Base/Event.h"

using namespace cross;

class SceneView : public View {
public:
	Event<Entity*> EntitySelected;

	SceneView();

	void Shown() override;
	void Hidden() override;
	void PreUpdate() override;
	void Update(float sec) override;
	void PostUpdate() override;

	DockPosition GetDefaultDockPosition() const override { return DockPosition::LEFT; }

	Entity* GetSelectedEntity();
	
private:
	Entity* selected_entity = nullptr;
	bool editing = false;
	bool clicked = false;

	void OnSceneChanged(Screen* newScreen);

	void LookAtObject();
	void BuildNode(Entity* entity);
	void ContextMenu();
	void DeleteEntity();
};
