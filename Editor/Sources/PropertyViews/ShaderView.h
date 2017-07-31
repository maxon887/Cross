#ifndef SHADER_VIEW
#define SHADER_VIEW

#include "Cross.h"
#include "PropertyView.h"

class QLineEdit;

class ShaderView : public PropertyView
{
	Q_OBJECT
public:
	ShaderView(QWidget* parent = 0) : PropertyView(parent) { }
	~ShaderView() override { }

	void Initialize();
	void OnFileSelected(const string& filename);
	
private:
	QGroupBox* macrosies_box		= NULL;
	QGroupBox* properties_box		= NULL;
	QLineEdit* vertex_file			= NULL;
	QLineEdit* fragment_file		= NULL;

	void Clear();

	void OnVertexFileClicked();
	void OnFragmentFileClicked();

	QWidget* OnAddMacroClicked();
	QWidget* OnAddPropertyClicked();
	void OnRemoveClicked();
};

#endif