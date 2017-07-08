#include "PropertiesView.h"
#include "EntityComponent.h"

PropertiesView::PropertiesView(QWidget* parent) :
	QDockWidget(parent)
{ }

PropertiesView::~PropertiesView() { }

void PropertiesView::showEvent(QShowEvent *event){
	QWidget* entityComponent = findChild<EntityComponent*>();
	entityComponent->hide();
}