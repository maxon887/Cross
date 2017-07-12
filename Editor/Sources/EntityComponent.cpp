#include "EntityComponent.h"
#include "Entity.h"

#include <QLineEdit>
#include <QDoubleValidator>

EntityComponent::EntityComponent(QWidget* parent) :
	QGroupBox(parent)
{ }

EntityComponent::~EntityComponent() { }

void EntityComponent::Update(float sec){
	if(!posX->hasFocus()){
		posX->setText(QString::number(entity->GetPosition().x));
	}
	if(!posY->hasFocus()){
		posY->setText(QString::number(entity->GetPosition().y));
	}
	if(!posZ->hasFocus()){
		posZ->setText(QString::number(entity->GetPosition().z));
	}

	if(!rotX->hasFocus() && !rotY->hasFocus() && !rotZ->hasFocus() && !angle->hasFocus()){
		UpdateRotatioon();
	}

	if(!scaleX->hasFocus()){
		scaleX->setText(QString::number(entity->GetScale().x));
	}
	if(!scaleY->hasFocus()){
		scaleY->setText(QString::number(entity->GetScale().y));
	}
	if(!scaleZ->hasFocus()){
		scaleZ->setText(QString::number(entity->GetScale().z));
	}
}

void EntityComponent::SetEntity(Entity* entity){
	this->entity = entity;
	setTitle(entity->GetName().c_str());
}

void EntityComponent::showEvent(QShowEvent *event){
	QValidator* validator = new QDoubleValidator(-1000.0, 1000.0, 10, this);
	posX = findChild<QLineEdit*>("posX");
	posX->setValidator(validator);
	posY = findChild<QLineEdit*>("posY");
	posY->setValidator(validator);
	posZ = findChild<QLineEdit*>("posZ");
	posZ->setValidator(validator);
	rotX = findChild<QLineEdit*>("rotX");
	rotX->setValidator(validator);
	rotY = findChild<QLineEdit*>("rotY");
	rotY->setValidator(validator);
	rotZ = findChild<QLineEdit*>("rotZ");
	rotZ->setValidator(validator);
	angle = findChild<QLineEdit*>("angle");
	angle->setValidator(validator);
	scaleX = findChild<QLineEdit*>("scaleX");
	scaleX->setValidator(validator);
	scaleY = findChild<QLineEdit*>("scaleY");
	scaleY->setValidator(validator);
	scaleZ = findChild<QLineEdit*>("scaleZ");
	scaleZ->setValidator(validator);

	connect(posX, &QLineEdit::returnPressed, this, &EntityComponent::PositionChanged);
	connect(posY, &QLineEdit::returnPressed, this, &EntityComponent::PositionChanged);
	connect(posZ, &QLineEdit::returnPressed, this, &EntityComponent::PositionChanged);

	connect(rotX, &QLineEdit::returnPressed, this, &EntityComponent::RotationChanged);
	connect(rotY, &QLineEdit::returnPressed, this, &EntityComponent::RotationChanged);
	connect(rotZ, &QLineEdit::returnPressed, this, &EntityComponent::RotationChanged);
	connect(angle, &QLineEdit::returnPressed, this, &EntityComponent::RotationChanged);

	connect(scaleX, &QLineEdit::returnPressed, this, &EntityComponent::ScaleChanged);
	connect(scaleX, &QLineEdit::returnPressed, this, &EntityComponent::ScaleChanged);
	connect(scaleX, &QLineEdit::returnPressed, this, &EntityComponent::ScaleChanged);
}

void EntityComponent::PositionChanged(){
	Vector3D pos;
	pos.x = posX->text().toFloat();
	pos.y = posY->text().toFloat();
	pos.z = posZ->text().toFloat();
	entity->SetPosition(pos);
}

void EntityComponent::RotationChanged(){
	Vector3D axis;
	axis.x = rotX->text().toFloat();
	axis.y = rotY->text().toFloat();
	axis.z = rotZ->text().toFloat();
	float a = angle->text().toFloat();
	Quaternion rotation(axis, a);
	entity->SetRotate(rotation);
	UpdateRotatioon();
}

void EntityComponent::ScaleChanged(){
	Vector3D scale;
	scale.x = scaleX->text().toFloat();
	scale.y = scaleY->text().toFloat();
	scale.z = scaleZ->text().toFloat();
	entity->SetScale(scale);
}

void EntityComponent::UpdateRotatioon(){
	Quaternion rotation = entity->GetRotate().GetNormalized();
	Vector3D axis = rotation.GetAxis();
	rotX->setText(QString::number(axis.x));
	rotY->setText(QString::number(axis.y));
	rotZ->setText(QString::number(axis.z));
	angle->setText(QString::number(rotation.GetAngle()));
}