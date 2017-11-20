#include "EntityComponent.h"
#include "Entity.h"
#include "Transform.h"
#include "../CrossEditor.h"
#include "../History.h"

#include <QLineEdit>
#include <QDoubleValidator>

void EntityComponent::Initialize(){
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

void EntityComponent::OnEntitySelected(Entity* entity) {
	this->entity = entity;
	if(entity){
		show();
		setTitle(entity->GetName().c_str());
	}else{
		hide();
	}
}

void EntityComponent::Update(float sec) {
	if(!entity) {
		return;
	}
	if(!posX->hasFocus()){
		posX->setText(QString::number(entity->GetTransform()->GetPosition().x));
	}
	if(!posY->hasFocus()){
		posY->setText(QString::number(entity->GetTransform()->GetPosition().y));
	}
	if(!posZ->hasFocus()){
		posZ->setText(QString::number(entity->GetTransform()->GetPosition().z));
	}

	if(!rotX->hasFocus() && !rotY->hasFocus() && !rotZ->hasFocus() && !angle->hasFocus()){
		UpdateRotatioon();
	}

	if(!scaleX->hasFocus()){
		scaleX->setText(QString::number(entity->GetTransform()->GetScale().x));
	}
	if(!scaleY->hasFocus()){
		scaleY->setText(QString::number(entity->GetTransform()->GetScale().y));
	}
	if(!scaleZ->hasFocus()){
		scaleZ->setText(QString::number(entity->GetTransform()->GetScale().z));
	}
}

void EntityComponent::PositionChanged(){
	EntityChanged* action = new EntityChanged(entity);
	Vector3D pos;
	pos.x = posX->text().toFloat();
	pos.y = posY->text().toFloat();
	pos.z = posZ->text().toFloat();
	entity->GetTransform()->SetPosition(pos);
	editor->SomethingChanged(action);//trigger
}

void EntityComponent::RotationChanged(){
	EntityChanged* action = new EntityChanged(entity);
	Vector3D axis;
	axis.x = rotX->text().toFloat();
	axis.y = rotY->text().toFloat();
	axis.z = rotZ->text().toFloat();
	float a = angle->text().toFloat();
	Quaternion rotation(axis, a);
	entity->GetTransform()->SetRotate(rotation);
	UpdateRotatioon();
	editor->SomethingChanged(action);//trigger
}

void EntityComponent::ScaleChanged(){
	EntityChanged* action = new EntityChanged(entity);
	Vector3D scale;
	scale.x = scaleX->text().toFloat();
	scale.y = scaleY->text().toFloat();
	scale.z = scaleZ->text().toFloat();
	entity->GetTransform()->SetScale(scale);
	editor->SomethingChanged(action);//trigger
}

void EntityComponent::UpdateRotatioon(){
	Quaternion rotation = entity->GetTransform()->GetRotate().GetNormalized();
	Vector3D axis = rotation.GetAxis();
	rotX->setText(QString::number(axis.x));
	rotY->setText(QString::number(axis.y));
	rotZ->setText(QString::number(axis.z));
	angle->setText(QString::number(rotation.GetAngle()));
}