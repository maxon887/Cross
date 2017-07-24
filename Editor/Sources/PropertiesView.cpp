#include "PropertiesView.h"
#include "EntityComponent.h"
#include "MeshComponent.h"
#include "CrossEditor.h"
#include "Entity.h"

#include <QTreeView.h>

PropertiesView::PropertiesView(QWidget* parent) :
	QDockWidget(parent)
{ }

PropertiesView::~PropertiesView() { }

void PropertiesView::Update(float sec){
	if(selected_entity){
		for(ComponentView* v : entity_components){
			v->Update(sec);
		}
	}
}

void PropertiesView::OnScreenChanged(Screen*){
	selected_entity = NULL;
	for(ComponentView* v : entity_components) {
		v->hide();
	}
}

void PropertiesView::OnEntitySelected(Entity* entity) {
	show();
	selected_entity = entity;
	for(ComponentView* v : entity_components){
		if(selected_entity){
			v->show();
			v->OnEntitySelected(entity);
		}else{
			v->hide();
		}
	}
}

void PropertiesView::OnEntityChanged(Entity* entity){
	if(entity == selected_entity){
		for(ComponentView* v : entity_components) {
			v->OnEntitySelected(entity);
		}
	}
}

void PropertiesView::showEvent(QShowEvent *event) {
	editor->ScreenChanged.Connect(this, &PropertiesView::OnScreenChanged);
	entity_components.push_back(findChild<EntityComponent*>());
	entity_components.push_back(findChild<MeshComponent*>());
	for(ComponentView* v : entity_components) {
		v->hide();
	}
}