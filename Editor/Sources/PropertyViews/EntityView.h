#ifndef ENTITY_VIEW
#define ENTITY_VIEW

#include "Cross.h"
#include "PropertyView.h"

#include <QVBoxLayout>

class EntityView : public PropertyView
{
	Q_OBJECT
public:
	EntityView(QWidget* parent = 0);

	void Initialize() override;
	void OnEntitySelected(Entity* e) override;
	void Update(float sec) override;

private:
	Array<PropertyView*> component_views;
	QVBoxLayout* view_layout = NULL;
	Array<QGroupBox*> unknown_components;

	template<class View, class UI>
	void AddComponentView(const QString& name);
};

template<class View, class UI>
void EntityView::AddComponentView(const QString& name) {
	QWidget* container = new QWidget(this);
	UI ui;
	ui.setupUi(container);
	view_layout->insertWidget(view_layout->count() - 1, container);
	View* view = container->findChild<View*>(name);
	component_views.push_back(view);
}

#endif