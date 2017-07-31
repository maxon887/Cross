#include "PropertiesView.h"
#include "CrossEditor.h"
#include "Entity.h"
#include "File.h"

#include "ui_EntityComponent.h"

#include <QTreeView.h>

PropertiesView::PropertiesView(QWidget* parent) :
	QDockWidget(parent)
{ 
	editor->UIInitialized.Connect(this, &PropertiesView::OnUIInitialized);
	editor->ScreenChanged.Connect(this, &PropertiesView::OnScreenChanged);
}

PropertiesView::~PropertiesView() { 
	delete file_properties_widget;
}

void PropertiesView::OnUIInitialized(){
	entity_components.push_back(findChild<MeshComponent*>());
	
	QWidget* entityComponentClass = new QWidget(this);
	Ui::EntityComponentClass entityComponentUI;
	entityComponentUI.setupUi(entityComponentClass);
	QWidget* parentWiget = findChild<QWidget*>("layout");
	QVBoxLayout* hBox = dynamic_cast<QVBoxLayout*>(parentWiget->layout());
	hBox->insertWidget(0, entityComponentClass);
	EntityComponent* entityComponent = entityComponentClass->findChild<EntityComponent*>("entityComponent");
	entity_components.push_back(entityComponent);

	for(PropertyView* v : entity_components) {
		v->Initialize();
		v->hide();
	}
	
	file_properties_widget = new QWidget();
	Ui::FilePropertiesClass ui;
	ui.setupUi(file_properties_widget);
	shader_properties = file_properties_widget->findChild<ShaderView*>();
	parentWiget = findChild<QWidget*>("layout");
	shader_properties->setParent(parentWiget);
	hBox = dynamic_cast<QVBoxLayout*>(parentWiget->layout());
	hBox->insertWidget(0, shader_properties);

	shader_properties->Initialize();
	shader_properties->hide();
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
	shader_properties->hide();
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
		view->hide();
	}
	string ext = File::ExtensionFromFile(filename);
	if(ext == "she"){
		shader_properties->show();
		shader_properties->OnFileSelected(filename);
	}else{
		shader_properties->hide();
	}
}