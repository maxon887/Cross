#ifndef LIGHT_COMPONENT
#define LIGHT_COMPONENT

#include "Cross.h"
#include "ComponentView.h"
#include "Light.h"

using namespace cross;

class LightComponent : public ComponentView<Light>
{
	Q_OBJECT
public:
	LightComponent(QWidget* parent = 0) : ComponentView(parent) { }
	~LightComponent() override { }

	void Initialize() override;
	void Show(Entity* e) override;
};

#endif