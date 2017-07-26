#ifndef ENTITY_COMPONENT
#define ENTITY_COMPONENT

#include "Cross.h"
#include "ComponentView.h"

class QLineEdit;

using namespace cross;

class EntityComponent : public ComponentView
{
	Q_OBJECT
public:
	EntityComponent(QWidget* parent = 0);
	~EntityComponent();

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