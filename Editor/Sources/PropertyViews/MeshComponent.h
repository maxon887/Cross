#ifndef MESH_COMPONENT
#define MESH_COMPONENT

#include "Cross.h"
#include "ComponentView.h"

class QLineEdit;
class QLabel;

using namespace cross;

class MeshComponent : public ComponentView
{
	Q_OBJECT
public:
	MeshComponent(QWidget* parent = 0);
	~MeshComponent();

	void Initialize() override;
	void OnEntitySelected(Entity* e) override;

private:
	QLabel* shader_filename = NULL;
};

#endif