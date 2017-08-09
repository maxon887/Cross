#ifndef MATERIAL_VIEW
#define MATERIAL_VIEW

#include "Cross.h"
#include "PropertyView.h"
#include "Shaders/Shader.h"

class QLabel;
class QColorDialog;
class QPushButton;

class MaterialView : public PropertyView
{
	Q_OBJECT
public:
	MaterialView(QWidget* parent = 0) : PropertyView(parent) { }
	~MaterialView() override;

	void Initialize();
	void OnEntitySelected(Entity* e) override;
	void OnFileSelected(const string& filename) override;
	
private:
	Material* material					= NULL;
	Shader::Property* current_property	= NULL;

	QLabel* shader_label				= NULL;
	QGroupBox* properties_box			= NULL;
	//color options
	QColorDialog* color_dialog			= NULL;
	QWidget* current_property_layout	= NULL;

	QPushButton* apply_btn				= NULL;
	QPushButton* revert_btn				= NULL;

	void Clear();
	void OnValueChanged();
	//color options
	void OnColorPickerClicked();
	void OnCurrentColorChanged(const QColor& color);
	void OnColorSelected(const QColor& color);
	void OnColorRejected();
	string GetColorStr(const Color& color);

	QWidget* CreateProperty(const string& name, Shader::Property::Type type);

	void OnApplyClick();
	void OnRevertClick();
};

#endif