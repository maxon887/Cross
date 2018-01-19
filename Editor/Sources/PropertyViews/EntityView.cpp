#include "EntityView.h"
#include "Entity.h"

#include "ui_TransformComponent.h"
#include "ui_MeshComponent.h"
//#include "ui_LightComponent.h"

EntityView::EntityView(QWidget* parent) : 
	PropertyView(parent) 
{ }

void EntityView::Initialize() {
	view_layout = dynamic_cast<QVBoxLayout*>(this->layout());
	AddComponentView<MeshComponent, Ui::MeshComponentClass>("meshComponent");
	AddComponentView<TransformComponent, Ui::TransformComponentClass>("transformComponent");

	for(PropertyView* v : component_views) {
		v->Initialize();
		v->hide(); 
	}
}

void EntityView::OnEntitySelected(Entity* e) {
	for(PropertyView* v : component_views) {
		v->hide();
	}
	for(QGroupBox* unknownBox : unknown_components) {
		view_layout->removeWidget(unknownBox);
		delete unknownBox;
	}
	unknown_components.clear();
	if(e) {
		setTitle(e->GetName().c_str());
		show();

		for(Component* component : e->GetComponents()) {
			bool viewFound = false;
			for(PropertyView* v : component_views) {
				viewFound = v->OnComponentSelected(component);
				if(viewFound) {
					v->show();
					break;
				}
			}
			if(!viewFound) {
				//CROSS_ASSERT(false, "Unknown component found");
				QGroupBox* unknownComponentBox = new QGroupBox(this);
				unknownComponentBox->setTitle("Unknown Component");
				view_layout->insertWidget(view_layout->count() - 1, unknownComponentBox);
				unknown_components.push_back(unknownComponentBox);
			}
		}
	} else {
		PropertyView::OnEntitySelected(e);
	}
}

void EntityView::Update(float sec) {
	if(isVisible()) {
		for(PropertyView* v : component_views) {
			v->Update(sec);
		}
	}
}