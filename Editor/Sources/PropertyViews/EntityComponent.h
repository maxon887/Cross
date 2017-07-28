#ifndef ENTITY_COMPONENT
#define ENTITY_COMPONENT

#include "Cross.h"
#include "PropertyView.h"

class QLineEdit;

using namespace cross;

class EntityComponent : public PropertyView
{
	Q_OBJECT
public:
	EntityComponent(QWidget* parent = 0) : PropertyView(parent) { }
	~EntityComponent() override { }

	void Initialize() override;
	void OnEntitySelected(Entity* e) override;
	void Update(float sec) override;

private:
	Entity* entity		= NULL;
	QLineEdit* posX		= NULL;
	QLineEdit* posY		= NULL;
	QLineEdit* posZ		= NULL;
	QLineEdit* rotX		= NULL;
	QLineEdit* rotY		= NULL;
	QLineEdit* rotZ		= NULL;
	QLineEdit* angle	= NULL;
	QLineEdit* scaleX	= NULL;
	QLineEdit* scaleY	= NULL;
	QLineEdit* scaleZ	= NULL;

	void PositionChanged();
	void RotationChanged();
	void ScaleChanged();

	void UpdateRotatioon();
};

#endif