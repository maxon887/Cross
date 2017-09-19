#ifndef COMPONENT_VIEW
#define COMPONENT_VIEW

#include "Cross.h"
#include "System.h"
#include "PropertyView.h"
#include "Entity.h"

#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>

using namespace cross;

template<class ComponentType>
class ComponentView : public PropertyView 
{
public:
	ComponentView(QWidget* parent = 0);

	virtual void Show(Entity* entity) { }

	void OnEntitySelected(Entity* entity) override;

protected:
	Entity* selected_entity = NULL;

	void contextMenuEvent(QContextMenuEvent *event) override;

private:
	QMenu* context_menu		= NULL;

	void OnRemove();
};

template<class ComponentType>
ComponentView<ComponentType>::ComponentView(QWidget* parent) :
	PropertyView(parent)
{
	context_menu = new QMenu(this);
	QAction* remove = new QAction(context_menu);
	remove->setText("Remove Component");
	connect(remove, &QAction::triggered, this, &ComponentView::OnRemove);
	context_menu->addAction(remove);
}

template<class ComponentType>
void ComponentView<ComponentType>::OnEntitySelected(Entity* entity) {
	if(!entity) {
		PropertyView::OnEntitySelected(entity);
	} else {
		selected_entity = entity;

		ComponentType* component = entity->GetComponent<ComponentType>();
		if(!component) {
			PropertyView::OnEntitySelected(entity);
			return;
		}
		show();//qt

		Show(entity);//cross
	}
}

template<class ComponentType>
void ComponentView<ComponentType>::contextMenuEvent(QContextMenuEvent *event) {
	context_menu->exec(event->globalPos());
}

template<class ComponentType>
void ComponentView<ComponentType>::OnRemove() {
	Component* component = selected_entity->GetComponent<ComponentType>();
	CROSS_FAIL(component, "Selected entity does not have this component");
	selected_entity->RemoveComponent(component);
	hide();
}

#endif