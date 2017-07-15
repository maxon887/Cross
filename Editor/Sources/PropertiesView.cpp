#include "PropertiesView.h"
#include "EntityComponent.h"
#include "CrossEditor.h"

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
	entity_component->SetEntity(NULL);
	entity_component->hide();
}

void PropertiesView::OnEntitySelected(const QModelIndex &index) {
	show();
	selected_entity = (Entity*)index.internalPointer();
	entity_component->show();
	entity_component->SetEntity(selected_entity);
}

void PropertiesView::showEvent(QShowEvent *event) {
	editor->ScreenChanged.Connect(this, &PropertiesView::OnScreenChanged);
	entity_component = findChild<EntityComponent*>();
	entity_component->hide();
}