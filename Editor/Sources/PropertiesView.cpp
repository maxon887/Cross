#include "PropertiesView.h"
#include "PropertiesViews/EntityComponent.h"
#include "PropertiesViews/MeshComponent.h"
#include "CrossEditor.h"
#include "Entity.h"
#include "File.h"

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
	entity_components.push_back(findChild<EntityComponent*>());
	entity_components.push_back(findChild<MeshComponent*>());
	for(ComponentView* v : entity_components) {
		v->Initialize();
		v->hide();
	}

	file_properties_widget = new QWidget(this);
	Ui::FilePropertiesClass ui;
	ui.setupUi(file_properties_widget);
	shader_properties = file_properties_widget->findChild<ShaderView*>();
	QWidget* parentWiget = findChild<QWidget*>("layout");
	shader_properties->setParent(parentWiget);
	QVBoxLayout* hBox = dynamic_cast<QVBoxLayout*>(parentWiget->layout());
	if(hBox) {
		hBox->insertWidget(0, shader_properties);
	} else {
		parentWiget->layout()->addWidget(shader_properties);
	}
	shader_properties->Initialize();
}

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
	shader_properties->hide();
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

void PropertiesView::OnFileSelected(string filename){
	for(ComponentView* view : entity_components){
		view->hide();
	}
	string ext = File::ExtensionFromFile(filename);
	if(ext == "she"){
		shader_properties->show();
	}else{
		shader_properties->hide();
	}
}