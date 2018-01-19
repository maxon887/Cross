#ifndef SHADER_VIEW
#define SHADER_VIEW

#include "Cross.h"
#include "PropertyView.h"

class QLineEdit;
class QPushButton;

class ShaderView : public PropertyView
{
	Q_OBJECT
public:
	ShaderView(QWidget* parent = 0) : PropertyView(parent) { }
	~ShaderView() override = default;

	void Initialize();
	void OnFileSelected(const string& filename);

protected:
	void showEvent(QShowEvent* event) override;
	
private:
	Shader* shader					= NULL;

	QGroupBox* macrosies_box		= NULL;
	QGroupBox* properties_box		= NULL;
	QLineEdit* vertex_file			= NULL;
	QLineEdit* fragment_file		= NULL;
	QPushButton* apply_btn			= NULL;
	QPushButton* revert_btn			= NULL;

	void Clear();

	void OnVertexFileClicked();
	void OnFragmentFileClicked();

	QWidget* OnAddMacroClicked();
	QWidget* OnAddPropertyClicked();
	void OnRemoveClicked();

	void OnSomethingChanged();

	void OnApplyClick();
	void OnRevertClick();
};

#endif