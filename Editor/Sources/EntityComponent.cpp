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

	rotX->setText(QString::number(entity->GetRotate().x));
	rotY->setText(QString::number(entity->GetRotate().y));
	rotZ->setText(QString::number(entity->GetRotate().z));

	scaleX->setText(QString::number(entity->GetScale().x));
	scaleY->setText(QString::number(entity->GetScale().y));
	scaleZ->setText(QString::number(entity->GetScale().z));
}

void EntityComponent::SetEntity(Entity* entity){
	this->entity = entity;
	setTitle(entity->GetName().c_str());
}

void EntityComponent::showEvent(QShowEvent *event){
	posX = findChild<QLineEdit*>("posX");
	posY = findChild<QLineEdit*>("posY");
	posZ = findChild<QLineEdit*>("posZ");
	rotX = findChild<QLineEdit*>("rotX");
	rotY = findChild<QLineEdit*>("rotY");
	rotZ = findChild<QLineEdit*>("rotZ");
	scaleX = findChild<QLineEdit*>("scaleX");
	scaleY = findChild<QLineEdit*>("scaleY");
	scaleZ = findChild<QLineEdit*>("scaleZ");
}