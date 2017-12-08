#ifndef TRANSFORM_COMPONENT
#define TRANSFORM_COMPONENT

#include "Cross.h"
#include "ComponentView.h"
#include "Transform.h"

class QLineEdit;

using namespace cross;

class TransformComponent : public ComponentView<Transform>
{
	Q_OBJECT
public:
	TransformComponent(QWidget* parent = 0) : ComponentView(parent) { }
	~TransformComponent() override { }

	void Initialize() override;
	void Update(float sec) override;

private:
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