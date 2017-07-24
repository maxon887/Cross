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
		entity_component->Update(sec);
	}
}

void PropertiesView::OnScreenChanged(Screen*){
	selected_entity = NULL;
	entity_component->hide();
	mesh_component->hide();
}

void PropertiesView::OnEntitySelected(Entity* entity) {
	show();
	selected_entity = entity;
	if(entity){
		entity_component->show();
		entity_component->SetEntity(entity);

		Mesh* mesh = entity->GetComponent<Mesh>();
		if(mesh){
			mesh_component->show();
		}
	}else{
		entity_component->hide();
		mesh_component->hide();
	}
}

void PropertiesView::OnEntityChanged(Entity* entity){
	if(entity == selected_entity){
		entity_component->SetEntity(selected_entity);
	}
}

void PropertiesView::showEvent(QShowEvent *event) {
	editor->ScreenChanged.Connect(this, &PropertiesView::OnScreenChanged);
	entity_component = findChild<EntityComponent*>();
	entity_component->hide();
	mesh_component = findChild<MeshComponent*>();
	mesh_component->hide();
}