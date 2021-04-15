#ifndef COMPONENT_VIEW
#define COMPONENT_VIEW

#include "Cross.h"
#include "Entity.h"
#include "PropertyView.h"

#include <QGroupBox>
#include <QMenu>
#include <QContextMenuEvent>

using namespace cross;

template<class T>
class ComponentView : public PropertyView
{
public:
	ComponentView(QWidget* parent = 0);

	virtual void Show(T* e) { }

	virtual bool OnComponentSelected(Component* c);

protected:
	T* component = nullptr;

	void contextMenuEvent(QContextMenuEvent *event) override;

private:
	QMenu* context_menu		= nullptr;

	void OnRemove();
};

template<class T>
ComponentView<T>::ComponentView(QWidget* parent) :
	PropertyView(parent)
{
	context_menu = new QMenu(this);
	QAction* remove = new QAction(context_menu);
	remove->setText("Remove Component");
	connect(remove, &QAction::triggered, this, &ComponentView::OnRemove);
	context_menu->addAction(remove);
}

template<class T>
bool ComponentView<T>::OnComponentSelected(Component* c) {
	T* comp = dynamic_cast<T*>(c);
	if(comp) {
		component = comp;
		Show(component);
		return true;
	} else {
		return false;
	}
}

template<class T>
void ComponentView<T>::contextMenuEvent(QContextMenuEvent *event) {
	context_menu->exec(event->globalPos());
}

template<class T>
void ComponentView<T>::OnRemove() {
	component->GetEntity()->RemoveComponent(component);
	hide();
}

#endif