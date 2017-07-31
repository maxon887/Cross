#include "PropertiesView.h"
#include "CrossEditor.h"
#include "Entity.h"
#include "File.h"

#include "ui_EntityComponent.h"
#include "ui_ShaderView.h"

#include <QTreeView.h>

PropertiesView::PropertiesView(QWidget* parent) :
	QDockWidget(parent)
{ 
	editor->UIInitialized.Connect(this, &PropertiesView::OnUIInitialized);
	editor->ScreenChanged.Connect(this, &PropertiesView::OnScreenChanged);
}

PropertiesView::~PropertiesView() { 
}

void PropertiesView::OnUIInitialized(){
	QWidget* layoutWidget = findChild<QWidget*>("layout");
	QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(layoutWidget->layout());

	QWidget* entityComponentClass = new QWidget(this);
	Ui::EntityComponentClass entityComponentUI;
	entityComponentUI.setupUi(entityComponentClass);
	layout->insertWidget(0, entityComponentClass);
	EntityComponent* entityComponent = entityComponentClass->findChild<EntityComponent*>("entityComponent");
	entity_components.push_back(entityComponent);

	QWidget* shaderViewClass = new QWidget(this);
	Ui::ShaderViewClass shaderViewUI;
	shaderViewUI.setupUi(shaderViewClass);
	layout->insertWidget(layout->count() - 1, shaderViewClass);
	ShaderView* shaderView = shaderViewClass->findChild<ShaderView*>("shaderView");
	entity_components.push_back(shaderView);

	for(PropertyView* v : entity_components) {
		v->Initialize();
		v->hide();
	}
}

void PropertiesView::Update(float sec){
	if(selected_entity){
		for(PropertyView* v : entity_components){
			v->Update(sec);
		}
	}
}

void PropertiesView::OnScreenChanged(Screen*){
	selected_entity = NULL;
	for(PropertyView* v : entity_components) {
		v->hide();
	}
}

void PropertiesView::OnEntitySelected(Entity* entity) {
	show();
	selected_entity = entity;
	for(PropertyView* v : entity_components){
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
		for(PropertyView* v : entity_components) {
			v->OnEntitySelected(entity);
		}
	}
}

void PropertiesView::OnFileSelected(string filename){
	for(PropertyView* view : entity_components){
		view->OnFileSelected(filename);
	}
}