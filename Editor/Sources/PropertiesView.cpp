#include "PropertiesView.h"
#include "CrossEditor.h"
#include "Entity.h"
#include "File.h"
#include "ComponentFactory.h"

#include "ui_EntityView.h"
#include "ui_ShaderView.h"
#include "ui_MaterialView.h"

#include <QTreeView>
#include <QContextMenuEvent>

PropertiesView::PropertiesView(QWidget* parent) :
	QDockWidget(parent)
{ 
	editor->UIInitialized.Connect(this, &PropertiesView::OnUIInitialized);
	editor->ScreenChanged.Connect(this, &PropertiesView::OnScreenChanged);

	context_menu = new QMenu(this);
	QMenu* addComponent = new QMenu("Add Component", context_menu);
	context_menu->addMenu(addComponent);

	Array<String> names = game->GetComponentFactory()->GetRegisteredComponentsName();
	for(const String name : names) {
		QAction* compAction = new QAction(addComponent);
		compAction->setText(name.ToCStr());
		connect(compAction, &QAction::triggered, this, &PropertiesView::OnAddComponent);
		addComponent->addAction(compAction);
	}
}

PropertiesView::~PropertiesView() { 
}

void PropertiesView::OnUIInitialized(){
	QWidget* layoutWidget = findChild<QWidget*>("layout");
	layout = dynamic_cast<QVBoxLayout*>(layoutWidget->layout());
	CreateView<EntityView, Ui::EntityViewClass>("entityView");
	CreateView<ShaderView, Ui::ShaderViewClass>("shaderView");
	CreateView<MaterialView, Ui::MaterialViewClass>("materialView");

	for(PropertyView* v : views) {
		v->Initialize();
		v->hide();
	}
}

void PropertiesView::Update(float sec){
	if(selected_entity){
		for(PropertyView* v : views){
			v->Update(sec);
		}
	}
}

void PropertiesView::OnScreenChanged(Screen*){
	selected_entity = NULL;
	for(PropertyView* v : views) {
		v->hide();
	}
}

void PropertiesView::OnEntitySelected(Entity* entity) {
	show();
	for(PropertyView* v : views){
		v->OnEntitySelected(entity);
	}
	selected_entity = entity;
}

void PropertiesView::OnEntityChanged(Entity* entity){
	if(entity == selected_entity){
		for(PropertyView* v : views) {
			v->OnEntitySelected(entity);
		}
	}
}

void PropertiesView::OnFileSelected(QString filename){
	for(PropertyView* view : views){
		view->OnFileSelected(filename.toLatin1().data());
	}
}

void PropertiesView::contextMenuEvent(QContextMenuEvent *event) {
	if(selected_entity){
		context_menu->exec(event->globalPos());
	}
}

void PropertiesView::OnAddComponent() {
	QAction* action = dynamic_cast<QAction*>(sender());
	Component* component = game->GetComponentFactory()->Create(action->text().toLatin1().data());
	selected_entity->AddComponent(component);
	OnEntitySelected(selected_entity);
}