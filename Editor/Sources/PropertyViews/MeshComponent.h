#ifndef MESH_COMPONENT
#define MESH_COMPONENT

#include "Cross.h"
#include "PropertyView.h"

class QLabel;

using namespace cross;

class MeshComponent : public PropertyView
{
	Q_OBJECT
public:
	MeshComponent(QWidget* parent = 0) : PropertyView(parent) { }
	~MeshComponent() override { }

	void Initialize() override;
	void OnEntitySelected(Entity* e) override;

private:
	QLabel* model_label		= NULL;
	QLabel* material_label	= NULL;
};

#endif