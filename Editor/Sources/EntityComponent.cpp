#include "EntityComponent.h"
#include "Entity.h"

#include <QLineEdit.h>

EntityComponent::EntityComponent(QWidget* parent) :
	QGroupBox(parent)
{ }

EntityComponent::~EntityComponent() { }

void EntityComponent::Update(float sec){
	posX->setText(QString::number(entity->GetPosition().x));
	posY->setText(QString::number(entity->GetPosition().y));
	posZ->setText(QString::number(entity->GetPosition().z));
}

void EntityComponent::SetEntity(Entity* entity){
	this->entity = entity;
	setTitle(entity->GetName().c_str());
}

void EntityComponent::showEvent(QShowEvent *event){
	posX = findChild<QLineEdit*>("posX");
	posY = findChild<QLineEdit*>("posY");
	posZ = findChild<QLineEdit*>("posZ");
}