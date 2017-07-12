#include "PropertiesView.h"
#include "EntityComponent.h"

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

void PropertiesView::OnEntitySelected(const QModelIndex &index) {
	show();
	selected_entity = (Entity*)index.internalPointer();
	entity_component->show();
	entity_component->SetEntity(selected_entity);
}

void PropertiesView::showEvent(QShowEvent *event) {
	entity_component = findChild<EntityComponent*>();
	entity_component->hide();
}