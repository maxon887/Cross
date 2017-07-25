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
	file_properties_widget = new QWidget(this);
	Ui::FilePropertiesClass ui;
	ui.setupUi(file_properties_widget);
}

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

void PropertiesView::showEvent(QShowEvent *event) {
	editor->ScreenChanged.Connect(this, &PropertiesView::OnScreenChanged);
	entity_components.push_back(findChild<EntityComponent*>());
	entity_components.push_back(findChild<MeshComponent*>());
	for(ComponentView* v : entity_components) {
		v->hide();
	}

	QWidget* layout = findChild<QWidget*>("layout");
	shader_properties = file_properties_widget->findChild<ShaderView*>();
	shader_properties->setParent(layout);
}