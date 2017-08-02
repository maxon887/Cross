#ifndef MATERIAL_VIEW
#define MATERIAL_VIEW

#include "Cross.h"
#include "PropertyView.h"
#include "Shaders/Shader.h"

class QLabel;

class MaterialView : public PropertyView
{
	Q_OBJECT
public:
	MaterialView(QWidget* parent = 0) : PropertyView(parent) { }
	~MaterialView() override;

	void Initialize();
	void OnFileSelected(const string& filename);
	
private:
	Material* material			= NULL;

	QLabel* shaderLabel			= NULL;
	QGroupBox* properties_box	= NULL;

	void Clear();

	void OnValueChanged();

	QWidget* CreateProperty(const string& name, Shader::Property::Type type);
};

#endif