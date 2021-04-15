#ifndef MATERIAL_VIEW
#define MATERIAL_VIEW

#include "Cross.h"
#include "PropertyView.h"
#include "Shaders/Shader.h"

class QLabel;
class FileHandler;
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
	void OnFileSelected(const String& filename) override;
	
private:
	Material* material					= nullptr;
	Material* original					= nullptr;
	Shader::Property* current_property	= nullptr;

	QLabel* shader_label				= nullptr;
	FileHandler* shader_handler			= nullptr;
	QGroupBox* properties_box			= nullptr;
	//color options
	QColorDialog* color_dialog			= nullptr;
	QWidget* current_property_layout	= nullptr;

	QPushButton* apply_btn				= nullptr;
	QPushButton* revert_btn				= nullptr;

	void Clear();
	void OnShaderChanged(QString filename);
	void RefreshProperties();
	void OnValueChanged();
	void OnSomethingChanged();

	QWidget* CreateProperty(const String& name, Shader::Property::Type type);

	void OnApplyClick();
	void OnRevertClick();
	//color options
	void OnColorPickerClicked();
	void OnCurrentColorChanged(const QColor& color);
	void OnColorSelected(const QColor& color);
	void OnColorRejected();
};

#endif